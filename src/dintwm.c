// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/dintwm.h"
//#include "../include/wbar_config.h"

int main(int argc, char **argv)
{
	static int dint_exit_state = EXIT_SUCCESS;

#ifdef FORTIFY
	Fortify_EnterScope();
#endif

	initdefaults();

	dint_exit_state = dintwmrun(argc, argv);

	window_free_lut();

#ifdef FORTIFY
	Fortify_LeaveScope();
	Fortify_OutputStatistics();
#endif

	return dint_exit_state;
}

static int dintwmrun(int argc, char **argv)
{
	int c = 0;
	int dint_opt_state = NOTSET;
	static int dint_fail_state = EXIT_SUCCESS;
	static int dint_exit_state = EXIT_SUCCESS;
	int not_known = 0;
	int sh = tiling_screen_height();
	int sw = tiling_screen_height();

	while ((c = getopt(argc, argv, "uU:B:L:R:CdghstV")) >= 0) {	/* Flawfinder: ignore */
		switch (c) {
		case 'u':
			(void)tiling_gaps(TOPGAP_SET, tiling_calc_menugap());
			break;
		case 'U':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int tgap = tiling_gaps(TOPGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (tgap > sh || tgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'B':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int bgap = tiling_gaps(BOTTOMGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (bgap > sh || bgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'L':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int lgap = tiling_gaps(LEFTGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (lgap > sw || lgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'R':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int rgap = tiling_gaps(RIGHTGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (rgap > sw || rgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'C':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = COMMODITIZE;
			}
			break;
		case 'd':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_DWINDLE;
			}
			break;
		case 'g':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_HGRID;
			}
			break;
		case 'h':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_PRINTUSAGE;
			}
			break;
		case 's':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_SPIRAL;
			}
			break;
		case 't':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_TILE;
			}
			break;
		case 'V':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = PRINTVERSION;
			}
			break;
		case ':':
			dint_fail_state = MISSING;
			break;
		case '?':
			dint_fail_state = UNKNOWN;
			not_known = optopt;
			break;
		default:
			// Do nothing
			break;
		}
	}

	switch (dint_fail_state) {
	case DOUBLE_OPTION_ERR:
		printf("Do not use -%c with other option.\n", optopt);
		dint_exit_state = EXIT_FAILURE;
		break;
	case GAP_ERR:
		printf("Gap is larger or smaller than screen.\n");
		dint_exit_state = EXIT_FAILURE;
		break;
	case UNKNOWN:
		if (optopt != 0) {
			printf("unknown opt: -%c\n", not_known);
		} else {
			printf("unknown opt: -%c\n", ':');
		}
		dint_exit_state = EXIT_FAILURE;
		break;
	case MISSING:
		if (optopt != 0) {
			printf("missing arg: -%c\n", optopt);
		} else {
			printf("missing arg: -%c\n", ':');
		}
		printf("missing arg: -%c\n", optopt != 0 ? optopt : (int)':');
		dint_exit_state = EXIT_FAILURE;
		break;
	default:
		// Do nothing
		break;
	}

	// If no arg is used, set to commoditize
	if (dint_opt_state != NOTSET && dint_exit_state != EXIT_FAILURE) {
		dint_fail_state = DOUBLE_OPTION_ERR;
	} else {
		dint_opt_state = COMMODITIZE;
	}

	if (dint_exit_state != EXIT_FAILURE) {
		switch (dint_opt_state) {
		case COMMODITIZE:
			dint_exit_state = commo();
			break;
		case FUNC_PRINTUSAGE:
			dint_exit_state = printusage();
			break;
		case PRINTVERSION:
			printf("Dintwm version: %s\n", DINTWM_VERSION);
			dint_exit_state = TRUE;
			break;
		default:
			dint_exit_state = defkeys[dint_opt_state].func(&defkeys[dint_opt_state].arg);
			break;
		}
	}

	return dint_exit_state;
}

static void initdefaults(void)
{
	unsigned char nil = '\0';
	(void)option(TILE_FACT_GET, 0);
	(void)option_bool(BACKDROP_SET, FALSE);
	struct Window *window = NULL;

	if ((window_alloc_lut()) != 0) {
		printf("allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BAR_LAST_COLOR; ++i) {
		bar_color[i].color[0] = nil;
	}

	// Set reusable screen pointer for later usage
	(void)tiling_screen_light();

	struct Screen *scr = tiling_lock(TLOCK, NULL);
	(void)tiling_screen_info(SH_SET, scr->Height);
	(void)tiling_screen_info(SW_SET, scr->Width);

	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		window_set_wptr(window);
		if (strcmp("Workbench", (const char *)window->Title) == 0) {
			window_set_wbwin(window, TRUE);
			window_set_ws_num(window, WS_0);
			continue;
		} else {
			window_set_wbwin(window, FALSE);
		}
		 window_set_ws_num(window, window_current_ws(WS_GET, 0));
	}
	(void)tiling_lock(TUNLOCK, scr);
}

static short printusage(void)
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
	       "Options:",
	       "-d: Fibonacci dwindle",
	       "-g: Horizontal grid",
	       "-t: Tile with left master",
	       "-s: Fibonacci spiral",
	       "-V: Show version",
	       "<other arg> -u: Add workbench bar gap",
	       "<other arg> -U<int>: Add custom top gap",
	       "<other arg> -B<int>: Add custom bottom gap", "<other arg> -L<int>: Add custom left gap", "<other arg> -R<int>: Add custom right gap",
	       "-h: This message");

	return EXIT_SUCCESS;
}

static short skipper(struct Window *w)
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
		//if (include_wtype != 0 && exclude_wtype == 0) {
		if (option_bool(INCLUDE_WTYPE_GET, TRUE) && !option_bool(EXCLUDE_WTYPE_GET, TRUE)) {
			if (bsearch(&w->Title, incls->strings, WTYPE_MAX, sizeof(char *), cstring_cmp) != NULL) {
				return NOSKIP;
			} else {
				window_set_skip(w);
				return SKIP;
			}
		}

		//if (exclude_wtype != 0 && include_wtype == 0) {
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

void cwb(struct Window *w, int wx, int wy, int ww, int wh)
{
	ChangeWindowBox(w, (short int)wx, (short int)wy, (short int)ww, (short int)wh);
	WindowToFront(w);
}

short tile(const Arg *arg)
{
	(void)arg;
	static const int nmaster = 1;
	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int fact = option(TILE_FACT_GET, 0);

	int wincount = 0, wnr = 0, mwinwidth = 0, nwiny = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	if (wincount == 0) {
		(void)tiling_lock(TUNLOCK, scr);
		return TRUE;
	}

	if (wincount > nmaster) {
		mwinwidth = (sw * fact) / 1000;
	} else {
		mwinwidth = sw;
	}

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
			wnr--;
			continue;
		}
		if (wnr < nmaster) {
			wx = lgap;
			wy = tgap;
			ww = mwinwidth;
			wh = sh / (MIN(wincount, nmaster) - wnr);
		} else {
			wx = mwinwidth + lgap;
			wy = nwiny + tgap;
			ww = sw - mwinwidth;
			wh = (sh - nwiny) / (wincount - wnr);
			nwiny += wh;
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short hgrid(const Arg *arg)
{
	(void)arg;

	int wincount, wnr, ntop = 0, nbottom = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	if (wincount == 0) {
		(void)tiling_lock(TUNLOCK, scr);
		return TRUE;
	}

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
			wnr--;
			continue;
		}
		if (wincount <= 1) {
			wx = lgap;
			wy = tgap;
			ww = sw / wincount;
			wh = sh;
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				wx = wnr * (sw / ntop) + lgap;
				wy = tgap;
				ww = sw / ntop;
				wh = sh / 2;

			} else {
				wx = (wnr - ntop) * (sw / nbottom) + lgap;
				wy = (sh / 2) + tgap;
				ww = sw / nbottom;
				wh = sh / 2;
			}
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short fibonacci(const Arg *arg)
{
	int wnr, wincount;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int fact = option(TILE_FACT_GET, 0);

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	wy = tgap;
	wx = lgap;
	ww = sw;
	wh = sh;

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
			wnr--;
			continue;
		}
		if (wnr < wincount - 1) {
			if (wnr % 2 != 0) {
				wh /= 2;
			} else {
				ww /= 2;
			}
			if ((wnr % 4) == 2 && arg->i == 0) {
				wx += ww;
			} else if ((wnr % 4) == 3 && arg->i == 0) {
				wy += wh;
			}
		}
		if (wincount == 1) {
			wh = sh;
		} else if ((wnr % 4) == 0) {
			if (arg->i != 0) {
				wy += wh;
			} else {
				wy -= wh;
			}
		} else if ((wnr % 4) == 1) {
			wx += ww;
		} else if ((wnr % 4) == 2) {
			wy += wh;
		} else if ((wnr % 4) == 3) {
			if (arg->i != 0) {
				wx += ww;
			} else {
				wx -= ww;
			}
		}
		if (wnr < wincount && wnr == 0) {
			if (wincount != 1) {
				ww = (sw * fact) / 1000;
				wy = tgap;
			}
		} else if (wnr == 1) {
			ww = sw - ww;
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short switcher(const Arg *arg)
{
	short rc;

	if (arg->i != 0) {
		int t_layout = tiling_layout(TL_INC, 0);
		rc = defkeys[t_layout].func(&defkeys[t_layout].arg);
	} else {
		int t_layout = tiling_layout(TL_DEC, 0);
		rc = defkeys[t_layout].func(&defkeys[t_layout].arg);
	}

	return rc;
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

short docmd(const Arg *arg)
{
	int cmdid = arg->i - (int)CMD_ID_0;
	struct TagItem stags[5];
	long int file;
	unsigned char conline[TT_MAX_LENGTH];
	unsigned char cmd[TT_MAX_LENGTH];

	size_t conlen = strnlen((const char *)cons->strings[cmdid], TT_MAX_LENGTH);
	size_t cmdlen = strnlen((const char *)cmds->strings[cmdid], TT_MAX_LENGTH);

	if (conlen != 0U) {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s", cons->strings[cmdid]);
	} else {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s", DEFCON);
	}

	if (cmdlen != 0U) {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s", cmds->strings[cmdid]);
	} else {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s", DEFCMD);
	}

	if ((file = Open(conline, MODE_NEWFILE)) != 0L) {
		stags[0].ti_Tag = SYS_Input;
		stags[0].ti_Data = (long unsigned int)file;
		stags[1].ti_Tag = SYS_Output;
		stags[1].ti_Data = 0;
		stags[2].ti_Tag = SYS_Asynch;
		stags[2].ti_Data = TRUE;
		stags[3].ti_Tag = SYS_UserShell;
		stags[3].ti_Data = TRUE;
		stags[4].ti_Tag = TAG_DONE;

		if ((SystemTagList(cmd, stags)) == -1) {
			const char dcwarn[] = "Custom CMD/CONLINE is not correct. Quitting";
			info_window(dcwarn);
			return FALSE;
		}

		return TRUE;
	} else {
		return FALSE;
	}
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

short exit_cxm(const Arg *arg)
{
	return arg->s;
}

int cstring_cmp(const void *a, const void *b)
{
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t len;

	for (len = (size_t)0; len < maxlen; len++, s++) {
		if (*s == '\0') {
			break;
		}
	}
	return len;
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

short tileoff(const Arg *arg)
{
	(void)arg;
	option_bool(TILE_OFF_GET, TRUE) == FALSE ?
		(void)option_bool(TILE_OFF_SET, TRUE) :
		(void)option_bool(TILE_OFF_SET, FALSE);
	return TRUE;
}
