// Copyright 2024 Rasmus Edgar
#include "../include/windows.h"
#define DIVISOR 1025

int window_alloc_lut(void)
{
	size_t wlut_size = DIVISOR - 1;
        if ((wlut = (Wlut *) calloc(wlut_size, sizeof(Wlut))) == NULL) {
                return EXIT_FAILURE;
        }
	return 0;
}

void window_free_lut(void)
{
	free(wlut);
}

static int window_index(struct Window *w)
{
	unsigned long uw = (unsigned long)w;
        return (int)uw % DIVISOR;
}

void window_set_wptr(struct Window *w)
{
	int windex = window_index(w);
	if (wlut[windex].wptr == NULL) {
		wlut[windex].wptr = w;
	} else {
		exit(EXIT_FAILURE);
	}
}

static short window_check_wptr(struct Window *w)
{
	int windex = window_index(w);
	if (wlut[windex].wptr == NULL) {
		return FALSE;
	}
	return TRUE;
}

void window_set_wbwin(struct Window *w, short toggle)
{
	wlut[window_index(w)].wbwin = toggle;
}

short window_get_wbwin(struct Window *w)
{
	if (wlut[window_index(w)].wbwin) {
		return TRUE;
	}
	return FALSE;
}

void window_set_ws_num(struct Window *w, int ws)
{
	wlut[window_index(w)].workspace = ws;
}

static int window_get_ws_num(struct Window *w)
{
	return wlut[window_index(w)].workspace;
}

int window_current_ws(int action, int ws)
{
	static int current_ws = WS_0;

	if (action == WS_SET) {
		current_ws = ws;
	}
	return current_ws;
}

static void window_set_skip(struct Window *w)
{
	wlut[window_index(w)].skip = TRUE;
}

static short window_get_skip(struct Window *w)
{
	return wlut[window_index(w)].skip;
}

struct Window * window_active(int action, struct Window *w)
{
	static struct Window *active_window = 0;
	if (action == AW_SET) {
		active_window = w;
	}
	return wlut[window_index(active_window)].wptr;
}

struct Window * window_wbar(struct Window *w)
{
// TODO(blah): Fix
	static int windex = 0;

	if (windex == 0 && w != NULL) {
		window_set_wptr(w);
		windex = window_index(w);
	}

	return wlut[windex].wptr;
}

short info_window(const char *info_text)
{
	struct TagItem tagitem[9];
	struct Window *iwin, *twin;
	short closewin = FALSE;
	short info_text_length;
	unsigned long l, t, w, h, tleft;
	int sheight = tiling_screen_info(SH_GET, 0);
	int swidth = tiling_screen_info(SH_GET, 0);
	struct IntuiText iitext = {
		.TopEdge = 0,
		.LeftEdge = 0,
		.ITextFont = NULL,
		.DrawMode = JAM2,
		.FrontPen = 1,
		.BackPen = 2,
		.IText = (unsigned char *)info_text,
		.NextText = NULL
	};

	tagitem[0].ti_Tag = WA_Width;
	tagitem[0].ti_Data = 100;
	tagitem[1].ti_Tag = WA_Height;
	tagitem[1].ti_Data = 50;
	tagitem[2].ti_Tag = WA_Top;
	tagitem[2].ti_Data = (unsigned long)((unsigned long)sheight / 2UL);
	tagitem[3].ti_Tag = WA_SimpleRefresh;
	tagitem[3].ti_Data = 1;
	tagitem[4].ti_Tag = WA_IDCMP;
	tagitem[4].ti_Data = IDCMP_CLOSEWINDOW;
	tagitem[5].ti_Tag = WA_Flags;
	tagitem[5].ti_Data = WFLG_SIZEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_CLOSEGADGET | WFLG_ACTIVATE;
	tagitem[6].ti_Tag = WA_Title;
	tagitem[6].ti_Data = (unsigned long)"Dintwm Info";
	tagitem[7].ti_Tag = WA_Left;
	tagitem[7].ti_Data = (unsigned long)(((unsigned long)swidth / 2UL) - 150UL);
	tagitem[8].ti_Tag = TAG_DONE;

	struct Screen *scr = tiling_lock(TLOCK, NULL);

	// Hack to precalculate needed window width
	twin = OpenWindowTagList(NULL, tagitem);
	if (!twin) {
		(void)tiling_lock(TUNLOCK, scr);
		return FALSE;
	}
	info_text_length = TextLength(twin->RPort, (unsigned char *)info_text, strnlen((const char *)info_text, TT_MAX_LENGTH));
	tleft = (unsigned long)info_text_length + (unsigned long)twin->BorderLeft + (unsigned long)twin->BorderRight;
	tagitem[0].ti_Tag = WA_Width;
	tagitem[0].ti_Data = tleft;
	tagitem[7].ti_Tag = WA_Left;
	tagitem[7].ti_Data = (unsigned long)((unsigned long)swidth / 2UL) - (unsigned long)((unsigned long)tleft / 2UL);
	CloseWindow(twin);
	// End hack

	iwin = OpenWindowTagList(NULL, tagitem);
	if (!iwin) {
		(void)tiling_lock(TUNLOCK, scr);
		return FALSE;
	}

	(void)tiling_lock(TUNLOCK, scr);

	l = (unsigned long)iwin->BorderLeft;
	t = (unsigned long)iwin->BorderTop;
	w = (unsigned long)iwin->Width - (unsigned long)iwin->BorderLeft - (unsigned long)iwin->BorderRight;
	h = (unsigned long)iwin->Height - (unsigned long)iwin->BorderTop - (unsigned long)iwin->BorderBottom;

	SetAPen(iwin->RPort, 2);
	RectFill(iwin->RPort, l, t, l + w - 1UL, t + h - 1UL);
	PrintIText(iwin->RPort, &iitext, 4, 25);

	while (closewin == FALSE) {
		struct IntuiMessage *msg;
		(void)Wait(1UL << iwin->UserPort->mp_SigBit);
		msg = (struct IntuiMessage *)GetMsg(iwin->UserPort);
		ReplyMsg((struct Message *)msg);
		if (msg->Class == (unsigned long)IDCMP_CLOSEWINDOW) {
			CloseWindow(iwin);
			closewin = TRUE;
		}
	}
	return TRUE;
}

short tabnextwin(const Arg *arg)
{
	(void)arg;
	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);
	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		if ((window->Flags & (unsigned long)WINDOWACTIVE) != 0UL) {
			if ((window->NextWindow->Flags & (unsigned long)WFLG_BORDERLESS) != 0UL) {
				window = window->NextWindow;
			}
			if ((window_get_ws_num(window)) == WS_WB) {
				ActivateWindow(findfirstwin(scr));
				(void)tiling_lock(TUNLOCK, scr);
				return TRUE;
			}
			if ((window_get_ws_num(window)) == window_current_ws(WS_GET, 0)) {
				ActivateWindow(window->NextWindow);
				(void)tiling_lock(TUNLOCK, scr);
				return TRUE;
			}
		}
	}
	ActivateWindow(findfirstwin(scr));
	(void)tiling_lock(TUNLOCK, scr);
	return TRUE;
}

static struct Window *findfirstwin(struct Screen const *scr)
{
	struct Window *window = NULL;
	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		if (skipper(window) == SKIP) {
			continue;
		}
		if (window_get_ws_num(window) == window_current_ws(WS_GET, 0)) {
			return window;
		}
	}
	return window;
}

static void moveallwin(int m)
{
	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		if (skipper(window) == SKIP) {
			continue;
		}
		if (m == FRONT) {
			WindowToFront(window);
		}
		if (m == BACK) {
			WindowToBack(window);
		}
	}

	(void)tiling_lock(TUNLOCK, scr);
}

short changews(const Arg *arg)
{
	int t_layout = tiling_layout(TL_GET, 0);

	if ((option_bool(VWS_ON_GET, TRUE)) == FALSE) {
		return TRUE;
	}

	if ((window_current_ws(WS_GET, 0)) == (int)arg->u) {
		return TRUE;
	}

	moveallwin(BACK);
	window_current_ws(WS_SET, (int)arg->u);
	moveallwin(FRONT);
	struct Screen *scr = tiling_lock(TLOCK, NULL);
	ActivateWindow(findfirstwin(scr));
	(void)tiling_lock(TUNLOCK, scr);

	return (defkeys[t_layout].func(&defkeys[t_layout].arg));
}

short movetows(const Arg *arg)
{
	if ((option_bool(VWS_ON_GET, TRUE)) == FALSE) {
		return TRUE;
	}

	int t_layout = tiling_layout(TL_GET, 0);

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);
	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		if (skipper(window) == SKIP) {
			continue;
		}
		if ((window->Flags & (unsigned long)WINDOWACTIVE) != 0UL) {
			window_set_ws_num(window, (int)arg->u);
			WindowToBack(window);
		}
	}
	ActivateWindow(findfirstwin(scr));
	(void)tiling_lock(TUNLOCK, scr);

	return (defkeys[t_layout].func(&defkeys[t_layout].arg));
}

short skipper(struct Window *w)
{
	if (!window_check_wptr(w)) {
		window_set_wptr(w);
		window_set_ws_num(w, window_current_ws(WS_GET, 0));
	}

	if ((window_get_skip(w)) == TRUE) {
		return SKIP;
	}

	if ((w->Flags & (unsigned long)WFLG_BACKDROP) != 0UL) {
		(void)option_bool(BACKDROP_SET, TRUE);
		window_set_skip(w);
		return SKIP;
	}

	if ((w->Flags & (unsigned long)WFLG_GIMMEZEROZERO) != 0UL) {
		window_set_skip(w);
		return SKIP;
	}

	if ((w->Flags & (unsigned long)WFLG_BORDERLESS) != 0UL) {
		window_set_skip(w);
		return SKIP;
	}

	if (window_get_wbwin(w)) {
		window_set_skip(w);
		return SKIP;
	}

	if (window_get_ws_num(w) == window_current_ws(WS_GET, 0)) {
		if (option_bool(INCLUDE_WTYPE_GET, TRUE) && !option_bool(EXCLUDE_WTYPE_GET, TRUE)) {
			if (bsearch(&w->Title, incls->strings, WTYPE_MAX, sizeof(char *), cstring_cmp) != NULL) {
				return NOSKIP;
			} else {
				window_set_skip(w);
				return SKIP;
			}
		}

		if (option_bool(EXCLUDE_WTYPE_GET, TRUE) && !option_bool(INCLUDE_WTYPE_GET, TRUE)) {
			if (bsearch(&w->Title, excls->strings, WTYPE_MAX, sizeof(char *), cstring_cmp) != NULL) {
				window_set_skip(w);
				return SKIP;
			}
		}
	} else {
		// If not on current workspace skip the window
		return SKIP;
	}

	return NOSKIP;
}

short changegaps(const Arg *arg)
{
	int t_layout = tiling_layout(TL_GET, 0);

	switch (arg->i) {
	case TOPGAP_ID:
		(void)tiling_gaps(TOPGAP_ID, 0);
		break;
	case BOTTOMGAP_ID:
		(void)tiling_gaps(BOTTOMGAP_ID, 0);
		break;
	case LEFTGAP_ID:
		(void)tiling_gaps(LEFTGAP_ID, 0);
		break;
	case RIGHTGAP_ID:
		(void)tiling_gaps(RIGHTGAP_ID, 0);
		break;
	case -TOPGAP_ID:
		(void)tiling_gaps(-TOPGAP_ID, 0);
		break;
	case -BOTTOMGAP_ID:
		(void)tiling_gaps(-BOTTOMGAP_ID, 0);
		break;
	case -LEFTGAP_ID:
		(void)tiling_gaps(-LEFTGAP_ID, 0);
		break;
	case -RIGHTGAP_ID:
		tiling_gaps(-RIGHTGAP_ID, 0);
		break;
	case INCALLGAPS_ID:
		(void)tiling_gaps(TOPGAP_ID, 0);
		(void)tiling_gaps(BOTTOMGAP_ID, 0);
		(void)tiling_gaps(LEFTGAP_ID, 0);
		(void)tiling_gaps(RIGHTGAP_ID, 0);
		break;
	case DECALLGAPS_ID:
		(void)tiling_gaps(-TOPGAP_ID, 0);
		(void)tiling_gaps(-BOTTOMGAP_ID, 0);
		(void)tiling_gaps(-LEFTGAP_ID, 0);
		(void)tiling_gaps(-RIGHTGAP_ID, 0);
		break;
	default:
		// Do nothing
		break;
	}

	return (defkeys[t_layout].func(&defkeys[t_layout].arg));
}

void cwb(struct Window *w, int wx, int wy, int ww, int wh)
{
	ChangeWindowBox(w, (short int)wx, (short int)wy, (short int)ww, (short int)wh);
	WindowToFront(w);
}

void getactive(void)
{
	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);
	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		if ((window->Flags & (unsigned long)WFLG_WINDOWACTIVE) != 0UL) {
			(void)window_active(AW_SET, window);
			break;
		}
	}
	(void)tiling_lock(TUNLOCK, scr);
}

int countwindows(struct Screen const *scr)
{
	int wincount;
	struct Window *window = NULL;

	for (wincount = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wincount++) {
		if ((window->Flags & (unsigned long)WFLG_WINDOWACTIVE) != 0UL) {
			(void)window_active(AW_SET, window);
		}
		if (skipper(window) == SKIP) {
			wincount--;
			continue;
		}
	}

	return wincount;
}
