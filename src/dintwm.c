// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "../include/dintwm.h"
#include <dos/dostags.h>
#include "../include/ketopt.h"
#include "../include/wbar_config.h"

static void cwb(struct Window *w, int wx, int wy, int ww, int wh);
static unsigned long ilock;
static int skip = 0;
static struct Window *window;
static const int nmaster = 1;
static int layout_start = LAYOUT_START;
static int *layout_number = &layout_start;
static int nolock = 0;
static short printusage(void);
static short skipper(struct Window *w);
static void moveallwin(int m);
static void lockbasescreen(unsigned long *il, struct Screen **s);
static void unlockbasescreen(unsigned long *il, struct Screen **s);
static inline void mapws(void);
static inline unsigned char * maptm(void);
static inline unsigned char * padwbartext(unsigned char * s);
static short int wbartextwidth(int lei, unsigned char * it);
static int dintwmrun(int argc, char **argv);
static void initdefaults(void);
static struct Window * findfirstwin(void);

int main(int argc, char **argv)
{
	static int dint_exit_state = EXIT_SUCCESS;

	initdefaults();

	dint_exit_state = dintwmrun(argc, argv);

	return dint_exit_state;
}

static int dintwmrun(int argc, char **argv)
{
	ketopt_t opt = KETOPT_INIT;
	static int c;
	static int dint_opt_state = NOTSET;
	static int dint_fail_state = EXIT_SUCCESS;
	static int dint_exit_state = EXIT_SUCCESS;
	static int not_known;

	while ((c = ketopt(&opt, argc, argv, 1, "uU:B:L:R:CdghstV", 0)) >= 0) {
		switch (c) {
		case 'u':
			topgap = calcgap();
			break;
		case 'U':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			topgap = (int)strtol(opt.arg, (char **)NULL, 10);
			if (topgap > sheight || topgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'B':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			bottomgap = (int)strtol(opt.arg, (char **)NULL, 10);
			if (bottomgap > sheight || bottomgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'L':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			leftgap = (int)strtol(opt.arg, (char **)NULL, 10);
			if (leftgap > swidth || leftgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'R':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			rightgap = (int)strtol(opt.arg, (char **)NULL, 10);
			if (rightgap > swidth || rightgap < 0) {
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
			not_known = opt.opt;
			break;
		default:
			// Do nothing
			break;
		}
	}

	switch (dint_fail_state) {
	case DOUBLE_OPTION_ERR:
		printf("Do not use -%c with other option.\n", opt.opt);
		dint_exit_state = EXIT_FAILURE;
		break;
	case GAP_ERR:
		printf("Gap is larger or smaller than screen.\n");
		dint_exit_state = EXIT_FAILURE;
		break;
	case UNKNOWN:
		if (opt.opt != 0) {
			printf("unknown opt: -%c\n", not_known);
		} else {
			printf("unknown opt: -%c\n", ':');
		}
		dint_exit_state = EXIT_FAILURE;
		break;
	case MISSING:
		if (opt.opt != 0) {
			printf("missing arg: -%c\n", opt.opt);
		} else {
			printf("missing arg: -%c\n", ':');
		}
		printf("missing arg: -%c\n", opt.opt != 0 ? opt.opt : (int)':');
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
			dint_exit_state =
		    		defkeys[dint_opt_state].func(&defkeys[dint_opt_state].arg);
			break;
		}
	}

	return dint_exit_state;
}

static void initdefaults(void)
{
	nil = (unsigned char)'\0';
	fact = TILE_FACT_DEF;
	hidewbar = 0U;
	current_ws = 0U;
	current_ws |= WS_0;
	backdropped = FALSE;
	tile_off = FALSE;

	(void)memset(bar_text, nil, sizeof(bar_text));
	for (int i = 0; i < BAR_LAST_COLOR; ++i) {
		bar_color[i].color[0] = nil;
	}

	bar_on = FALSE;
	vws_on = FALSE;
	wbarheight = 0;

	lockbasescreen(&ilock, &screen);
	sheight = screen->Height;
	swidth = screen->Width;
	for (window = screen->FirstWindow; window;
		window = window->NextWindow) {
		if (strcmp("Workbench", (const char *)window->Title) == 0) {
			window->ExtData = (unsigned char *)WBENCH;
			continue;
		}
		if (!window->ExtData) {
			window->ExtData = (unsigned char *)current_ws;
		}
	}
	unlockbasescreen(&ilock, &screen);
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
		"<other arg> -B<int>: Add custom bottom gap",
		"<other arg> -L<int>: Add custom left gap",
		"<other arg> -R<int>: Add custom right gap",
		"-h: This message");

	return EXIT_SUCCESS;
}

static short skipper(struct Window *w)
{
	if (w->Flags & (unsigned long)WFLG_BACKDROP) {
		backdropped = TRUE;
		return SKIP;
	}

	if (w->Flags & (unsigned long)WFLG_GIMMEZEROZERO) {
		return SKIP;
	}

	if (w->Flags & (unsigned long)WFLG_BORDERLESS) {
		return SKIP;
	}

	if ((unsigned int)window->ExtData & WBENCH) {
		return SKIP;
	}

	if (!w->ExtData) {
		w->ExtData = (unsigned char *)current_ws;
	}


	if ((unsigned int)w->ExtData & current_ws) {
		if (include_wtype != 0 && exclude_wtype == 0) {
			if (bsearch
		    	(&w->Title, incls->strings, WTYPE_MAX, sizeof(char *),
		     	cstring_cmp)) {
				return NOSKIP;
			} else {
				return SKIP;
			}
		}

		if (exclude_wtype != 0 && include_wtype == 0) {
			if (bsearch
		    	(&w->Title, excls->strings, WTYPE_MAX, sizeof(char *),
		     	cstring_cmp)) {
				return SKIP;
			}
		}
	} else {
		return SKIP;
	}

	return NOSKIP;
}

void cwb(struct Window *w, int wx, int wy, int ww, int wh)
{
	ChangeWindowBox(w, (short int)wx, (short int)wy, (short int)ww,
			(short int)wh);
	WindowToFront(w);
}

short tile(const Arg * arg)
{
	(void)arg;

	int wincount = 0, wnr = 0, mwinwidth = 0, nwiny = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	lockbasescreen(&ilock, &screen);

	sh = sheight - (bottomgap + topgap);
	sw = swidth - (rightgap + leftgap);

	wincount = countwindows(nolock);

	if (wincount == 0) {
		unlockbasescreen(&ilock, &screen);
		return TRUE;
	}

	if (wincount > nmaster) {
		mwinwidth = nmaster != 0 ? (sw * fact) / 1000 : 0;
	} else {
		mwinwidth = sw;
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == SKIP) {
			wnr--;
			continue;
		}
		if (wnr < nmaster) {
			wx = leftgap;
			wy = topgap;
			ww = mwinwidth;
			wh = sh / (MIN(wincount, nmaster) - wnr);
		} else {
			wx = mwinwidth + leftgap;
			wy = nwiny + topgap;
			ww = sw - mwinwidth;
			wh = (sh - nwiny) / (wincount - wnr);
			nwiny += wh;
		}
		cwb(window, wx, wy, ww, wh);
	}
	unlockbasescreen(&ilock, &screen);

	return TRUE;
}

short hgrid(const Arg * arg)
{
	(void)arg;

	int wincount, wnr, ntop = 0, nbottom = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	lockbasescreen(&ilock, &screen);

	sh = sheight - (bottomgap + topgap);
	sw = swidth - (rightgap + leftgap);

	wincount = countwindows(nolock);

	if (wincount == 0) {
		unlockbasescreen(&ilock, &screen);
		return TRUE;
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == SKIP) {
			wnr--;
			continue;
		}
		if (wincount <= 1) {
			wx = leftgap;
			wy = topgap;
			ww = sw / wincount;
			wh = sh;
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				wx = wnr * (sw / ntop) + leftgap;
				wy = topgap;
				ww = sw / ntop;
				wh = sh / 2;

			} else {
				wx = (wnr - ntop) * (sw / nbottom) + leftgap;
				wy = (sh / 2) + topgap;
				ww = sw / nbottom;
				wh = sh / 2;
			}
		}
		cwb(window, wx, wy, ww, wh);
	}
	unlockbasescreen(&ilock, &screen);

	return TRUE;
}

short fibonacci(const Arg * arg)
{
	int wnr, wincount;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	lockbasescreen(&ilock, &screen);

	sh = sheight - (bottomgap + topgap);
	sw = swidth - (rightgap + leftgap);

	wincount = countwindows(nolock);

	wy = topgap;
	wx = leftgap;
	ww = sw;
	wh = sh;

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == SKIP) {
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
		if (wnr == 0) {
			if (wincount != 1) {
				ww = (sw * fact) / 1000;
				wy = topgap;
			}
		} else if (wnr == 1) {
			ww = sw - ww;
		}
		cwb(window, wx, wy, ww, wh);
	}
	unlockbasescreen(&ilock, &screen);

	return TRUE;
}

short switcher(const Arg * arg)
{
	short rc;
	if (*current_layout < TILE_FUNC_LIMIT && *layout_number == LAYOUT_START) {
		*layout_number = *current_layout;
	}

	if (arg->i) {
		(*layout_number)++;
		if (*layout_number > TILE_FUNC_LIMIT) {
			*layout_number = 0;
		}
		rc = defkeys[(*layout_number)].func(&defkeys[(*layout_number)].
						    arg);
	} else {
		(*layout_number)--;
		if (*layout_number < 0) {
			*layout_number = TILE_FUNC_LIMIT;
		}
		rc = defkeys[(*layout_number)].func(&defkeys[(*layout_number)].
						    arg);
	}
	*current_layout = *layout_number;

	return rc;
}

int countwindows(int l)
{
	int wincount;
	if (l) {
		lockbasescreen(&ilock, &screen);
	}
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
		if (window->Flags & (unsigned long)WFLG_WINDOWACTIVE) {
			active_win = window;
		}
		if ((skip = skipper(window)) == SKIP) {
			wincount--;
			continue;
		}
	}
	if (l) {
		unlockbasescreen(&ilock, &screen);
	}
	return wincount;
}

void getactive(void)
{
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window;
	     window = window->NextWindow) {
		if (window->Flags & (unsigned long)WFLG_WINDOWACTIVE) {
			active_win = window;
			break;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

short docmd(const Arg * arg)
{
	int cmdid = arg->i - (int)CMD_ID_0;
	struct TagItem stags[5];
	long int file;
	unsigned char conline[TT_MAX_LENGTH];
	unsigned char cmd[TT_MAX_LENGTH];

	size_t conlen =
	    strnlen((const char *)cons->strings[cmdid], TT_MAX_LENGTH);
	size_t cmdlen =
	    strnlen((const char *)cmds->strings[cmdid], TT_MAX_LENGTH);

	if (conlen) {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s",
			       cons->strings[cmdid]);
	} else {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s", DEFCON);
	}

	if (cmdlen) {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s",
			       cmds->strings[cmdid]);
	} else {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s", DEFCMD);
	}

	if ((file = Open(conline, MODE_NEWFILE))) {
		// Will not fix MISRA warnings from amiga NDK
		stags[0].ti_Tag = SYS_Input; //-V2544 //-V2568
		stags[0].ti_Data = (long unsigned int)file;
		stags[1].ti_Tag = SYS_Output; //-V2544 //-V2568
		stags[1].ti_Data = 0; //-V2568
		stags[2].ti_Tag = SYS_Asynch; //-V2544 //-V2568
		stags[2].ti_Data = TRUE; //-V2568
		stags[3].ti_Tag = SYS_UserShell; //-V2544 //-V2568
		stags[3].ti_Data = TRUE; //-V2568
		stags[4].ti_Tag = TAG_DONE; //-V2568

		if ((SystemTagList(cmd, stags)) == -1) {
			unsigned char dcwarn[] = "Custom CMD/CONLINE is not correct. Quitting";
			info_window(dcwarn);
			return FALSE;
		}

		return TRUE;
	} else {
		return FALSE;
	}
}

int calcgap(void)
{
	int bheight;
	lockbasescreen(&ilock, &screen);
	bheight = (int)screen->BarHeight + 1;
	unlockbasescreen(&ilock, &screen);
	return bheight;
}

short changegaps(const Arg *arg)
{
	switch(arg->i) {
	case TOPGAP_ID:
		topgap += topgap <= ((sheight - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case BOTTOMGAP_ID:
		bottomgap += bottomgap <= ((sheight - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case LEFTGAP_ID:
		leftgap += leftgap <= ((swidth - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case RIGHTGAP_ID:
		rightgap += rightgap <= ((swidth - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case -TOPGAP_ID:
		topgap = (topgap - gap_change_value) > 0 ? topgap - gap_change_value : 0;
		break;
	case -BOTTOMGAP_ID:
		bottomgap = (bottomgap - gap_change_value) > 0 ? bottomgap - gap_change_value : 0;
		break;
	case -LEFTGAP_ID:
		leftgap = (leftgap - gap_change_value) > 0 ? leftgap - gap_change_value : 0;
		break;
	case -RIGHTGAP_ID:
		rightgap = (rightgap - gap_change_value) > 0 ? rightgap - gap_change_value : 0;
		break;
	case INCALLGAPS_ID:
		topgap += topgap <= ((sheight - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		bottomgap += bottomgap <= ((sheight - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		leftgap += leftgap <= ((swidth - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		rightgap += rightgap <= ((swidth - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case DECALLGAPS_ID:
		topgap = (topgap - gap_change_value) > 0 ? topgap - gap_change_value : 0;
		bottomgap = (bottomgap - gap_change_value) > 0 ? bottomgap - gap_change_value : 0;
		leftgap = (leftgap - gap_change_value) > 0 ? leftgap - gap_change_value : 0;
		rightgap = (rightgap - gap_change_value) > 0 ? rightgap - gap_change_value : 0;
		break;
	default:
		// Do nothing
		break;
	}

	return(defkeys[*current_layout].func(&defkeys[*current_layout].arg));
}

static void lockbasescreen(unsigned long *il, struct Screen **s)
{
	*il = LockIBase(0L);
	*s = LockPubScreen(NULL);
}

static void unlockbasescreen(unsigned long *il, struct Screen **s)
{
	UnlockPubScreen(NULL, *s);
	UnlockIBase(*il);
}

short exit_cxm(const Arg * arg)
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

static void moveallwin(int m) {
	int countw = 0;
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window;
		window = window->NextWindow) {
		if ((skip = skipper(window)) == SKIP) {
			continue;
		}
		if (m == FRONT) {
			WindowToFront(window);
		}
		if (m == BACK) {
			WindowToBack(window);
		}
		countw++;
	}
	if (m == FRONT && hidewbar != 0U) {
		if (countw == 0) {
			WindowToBack(wbw);
			hidewbar |= BAR_HIDE_TOGGLE;
		}
		if (countw > 0) {
			WindowToFront(wbw);
			hidewbar &= ~(BAR_HIDE_TOGGLE);
		}
	}
	unlockbasescreen(&ilock, &screen);
}


short changews(const Arg * arg) {
	if (vws_on == FALSE) {
		return TRUE;
	}

	if (current_ws & arg->u) {
		return TRUE;
	}

	moveallwin(BACK);
	current_ws = arg->u;
	moveallwin(FRONT);
	lockbasescreen(&ilock, &screen);
	ActivateWindow(findfirstwin());
	unlockbasescreen(&ilock, &screen);

	return(defkeys[*current_layout].func(&defkeys[*current_layout].arg));
}

struct Window * findfirstwin(void)
{
	for (window = screen->FirstWindow; window;
		window = window->NextWindow) {
		if ((skip = skipper(window)) == SKIP) {
			continue;
		}
		if ((unsigned int)window->ExtData & current_ws) {
			return window;
		}
	}
	return window;
}

short movetows(const Arg * arg) {
	if (vws_on == FALSE) {
		return TRUE;
	}

	unsigned int setws = 0U;
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window;
		window = window->NextWindow) {
		if ((skip = skipper(window)) == SKIP) {
			continue;
		}
		if (window->Flags & (unsigned long)WINDOWACTIVE) {
			setws |= arg->u;
			window->ExtData = (unsigned char *)setws;
			WindowToBack(window);
		}
	}
	unlockbasescreen(&ilock, &screen);
	ActivateWindow(findfirstwin());
	return(defkeys[*current_layout].func(&defkeys[*current_layout].arg));
}

short tabnextwin(const Arg * arg) {
	(void)arg;
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window; window = window->NextWindow) {
		if (window->Flags & (unsigned long)WINDOWACTIVE) {
			if (window->NextWindow->Flags & (unsigned long)WFLG_BORDERLESS) {
				window = window->NextWindow;
			}
			if ((unsigned int)window->NextWindow->ExtData & current_ws) {
				ActivateWindow(window->NextWindow);
				unlockbasescreen(&ilock, &screen);
				return TRUE;
			}
		}
	}
	ActivateWindow(findfirstwin());
	unlockbasescreen(&ilock, &screen);
	return TRUE;
}

static inline unsigned char * padwbartext(unsigned char * s)
{
	unsigned char * tmp = s;
	(void)snprintf((char *)s, TT_MAX_LENGTH * 2, "%s%s", tmp, bar_text[space].text);
	return s;
}


short init_wbar(void) {
	struct TagItem tagitem[7];

	tagitem[0].ti_Tag = WA_Width; //-V2544 //-V2568
	tagitem[0].ti_Data = (unsigned long)(swidth - (leftgap + rightgap));
	tagitem[1].ti_Tag = WA_Height; //-V2544 //-V2568
	tagitem[1].ti_Data = (unsigned long)wbarheight;
	tagitem[2].ti_Tag = WA_Top; //-V2544 //-V2568
	tagitem[2].ti_Data = (unsigned long)(sheight - bottomgap);
	tagitem[3].ti_Tag = WA_Borderless; //-V2544 //-V2568
	tagitem[3].ti_Data = 1; //-V2568
	tagitem[4].ti_Tag = WA_SmartRefresh; //-V2544 //-V2568
	tagitem[4].ti_Data = 1; //-V2568
	tagitem[5].ti_Tag = WA_IDCMP; //-V2544 //-V2568
	tagitem[5].ti_Data = IDCMP_REFRESHWINDOW|IDCMP_CHANGEWINDOW; //-V2544 //-V2568
	tagitem[6].ti_Tag = TAG_DONE; //-V2544 //-V2568

	lockbasescreen(&ilock, &screen);
	wbw = OpenWindowTagList(NULL, tagitem);

	if (!wbw) {
		unlockbasescreen(&ilock, &screen);
		return FALSE;
	}
	unlockbasescreen(&ilock, &screen);

	WindowToFront(wbw);

	if (vws_on) {
		wstext_five = wbartext;
		wstext_five.IText = padwbartext(bar_text[ws_five].text);

		wstext_four = wbartext;
		wstext_four.IText = padwbartext(bar_text[ws_four].text);

		wstext_three = wbartext;
		wstext_three.IText = padwbartext(bar_text[ws_three].text);

		wstext_two = wbartext;
		wstext_two.IText = padwbartext(bar_text[ws_two].text);

		wstext_one = wbartext;
		wstext_one.IText = padwbartext(bar_text[ws_one].text);

		wstext_zero = wbartext;
		wstext_zero.IText = padwbartext(bar_text[ws_zero].text);
	} else {
		wstext_zero = wbartext;
		wstext_zero.IText = padwbartext(bar_text[ws_wb].text);
	}

	wbarsep_one = wbartext;
	wbarsep_one.IText = padwbartext(bar_text[sep_one].text);
	wbarsep_one.FrontPen = *bar_color[fp_sep_one].color;
	wbarsep_one.BackPen = *bar_color[bp_sep_one].color;

	wbarsep_two = wbartext;
	wbarsep_two.IText = padwbartext(bar_text[sep_two].text);
	wbarsep_two.FrontPen = *bar_color[fp_sep_two].color;
	wbarsep_two.BackPen = *bar_color[bp_sep_two].color;

	bar_text[mode_tile].text = padwbartext(bar_text[mode_tile].text);
	bar_text[mode_grid].text = padwbartext(bar_text[mode_grid].text);
	bar_text[mode_dwindle].text = padwbartext(bar_text[mode_dwindle].text);
	bar_text[mode_spiral].text = padwbartext(bar_text[mode_spiral].text);

	wbarmodetext = wbartext;
	wbarmodetext.IText = bar_text[mode_tile].text;
	wbarmodetext.FrontPen = *bar_color[fp_tm].color;
	wbarmodetext.BackPen = *bar_color[bp_tm].color;

	wbarwtitle = wbartext;
	wbarwtitle.FrontPen = *bar_color[fp_ti].color;
	wbarwtitle.BackPen = *bar_color[bp_ti].color;

	if (vws_on) {
		wstext_zero.NextText = &wstext_one;
		wstext_one.NextText = &wstext_two;
		wstext_two.NextText = &wstext_three;
		wstext_three.NextText = &wstext_four;
		wstext_four.NextText = &wstext_five;
		wstext_five.NextText = &wbarsep_one;
		wbarsep_one.NextText = &wbarmodetext;
		wbarmodetext.NextText = &wbarsep_two;
		wbarsep_two.NextText = &wbarwtitle;
	} else {
		wstext_zero.NextText = &wbarsep_one;
		wbarsep_one.NextText = &wbarmodetext;
		wbarmodetext.NextText = &wbarsep_two;
		wbarsep_two.NextText = &wbarwtitle;
	}

	if (vws_on) {
		wstext_one.LeftEdge = wbartextwidth(0, wstext_zero.IText);
		wstext_two.LeftEdge = wbartextwidth(wstext_one.LeftEdge, wstext_one.IText);
		wstext_three.LeftEdge = wbartextwidth(wstext_two.LeftEdge, wstext_two.IText);
		wstext_four.LeftEdge = wbartextwidth(wstext_three.LeftEdge, wstext_three.IText);
		wstext_five.LeftEdge = wbartextwidth(wstext_four.LeftEdge, wstext_four.IText);
		wbarsep_one.LeftEdge = wbartextwidth(wstext_five.LeftEdge, wstext_five.IText);
		wbarmodetext.LeftEdge = wbartextwidth(wbarsep_one.LeftEdge, wbarsep_one.IText);
		wbarsep_two.LeftEdge = wbartextwidth(wbarmodetext.LeftEdge, wbarmodetext.IText);
		wbarwtitle.LeftEdge = wbartextwidth(wbarsep_two.LeftEdge, wbarsep_two.IText);
	} else {
		wbarsep_one.LeftEdge = wbartextwidth(0, wstext_zero.IText);
		wbarmodetext.LeftEdge = wbartextwidth(wbarsep_one.LeftEdge, wbarsep_one.IText);
		wbarsep_two.LeftEdge = wbartextwidth(wbarmodetext.LeftEdge, wbarmodetext.IText);
		wbarwtitle.LeftEdge = wbartextwidth(wbarsep_two.LeftEdge, wbarsep_two.IText);
	}

	wbarcwb();
	return TRUE;
}

static short int wbartextwidth(int lei, unsigned char * it)
{
	struct TextExtent *barte = &teinit;
	short int le = (short int)lei;

	if (TextExtent(wbw->RPort, it, strnlen((const char *)it, TT_MAX_LENGTH), barte)) {
		if (barte == NULL) {
			return FALSE;
		}
	}
	return (short int)(le+(short int)barte->te_Width);
}

short update_wbar(void) {
	if (bar_on == FALSE) {
		return TRUE;
	}
	barbdata[0] = 1;
	barbdata[1] = 1;
	barbdata[2] = (short)((swidth - (leftgap + rightgap)) - 1);
	barbdata[3] = 1;
	barbdata[4] = (short)((swidth - (leftgap + rightgap)) -  1);
	barbdata[5] = (short)(wbarheight - 1);
	barbdata[6] = 1;
	barbdata[7] = (short)(wbarheight - 1);
	barbdata[8] = 1;
	barbdata[9] = 1;

	mapws();
	wbarmodetext.IText = maptm();

	(void)snprintf((char *)awintitle, TT_MAX_LENGTH, "%s", active_win->Title);

	wbarwtitle.IText = awintitle;

	barb.XY = barbdata;

	SetRast(wbw->RPort, *bar_color[bg].color);
	DrawBorder(wbw->RPort, &barb, 0, 0);
	PrintIText(wbw->RPort, &wstext_zero, 4, 0);

	return TRUE;
}

void wbarcwb(void) {
	if (bar_on == FALSE) {
		return;
	}
	struct IntuiMessage *msg;
	lockbasescreen(&ilock, &screen);
	cwb(wbw, leftgap, sheight - bottomgap, swidth - (leftgap + rightgap), wbarheight);
	unlockbasescreen(&ilock, &screen);
	(void)WaitPort(wbw->UserPort);
	while ((msg = (struct IntuiMessage *)GetMsg(wbw->UserPort))) {
		if (msg->Class == (unsigned long)IDCMP_SIZEVERIFY) {
			ReplyMsg((struct Message *)msg);  //-V2545
		}
	}
}

static inline void mapws(void)
{
	if (vws_on == FALSE) {
		wstext_zero.FrontPen = *bar_color[fp_ws].color;
		wstext_zero.BackPen = *bar_color[bp_ws].color;
		return;
	}

	if (current_ws & WS_0) {
		wstext_one.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen =
		wstext_four.FrontPen = wstext_five.FrontPen = *bar_color[fp_ws].color;

		wstext_one.BackPen = wstext_two.BackPen = wstext_three.BackPen =
		wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_zero.FrontPen = *bar_color[fp_cur].color;
		wstext_zero.BackPen = *bar_color[bp_cur].color;
	}
	if (current_ws & WS_1) {
		wstext_zero.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen =
		wstext_four.FrontPen = wstext_five.FrontPen = *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_two.BackPen = wstext_three.BackPen =
		wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;;

		wstext_one.FrontPen = *bar_color[fp_cur].color;
		wstext_one.BackPen = *bar_color[bp_cur].color;
	}
	if (current_ws & WS_2) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_three.FrontPen =
		wstext_four.FrontPen = wstext_five.FrontPen = *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_three.BackPen =
		wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_two.FrontPen = *bar_color[fp_cur].color;
		wstext_two.BackPen = *bar_color[bp_cur].color;
	}
	if (current_ws & WS_3) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen =
		wstext_four.FrontPen = wstext_five.FrontPen = *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen =
		wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_three.FrontPen = *bar_color[fp_cur].color;
		wstext_three.BackPen = *bar_color[bp_cur].color;
	}
	if (current_ws & WS_4) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen =
		wstext_three.FrontPen = wstext_five.FrontPen = *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen =
		wstext_three.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_four.FrontPen = *bar_color[fp_cur].color;
		wstext_four.BackPen = *bar_color[bp_cur].color;
	}
	if (current_ws & WS_5) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen =
		wstext_three.FrontPen = wstext_four.FrontPen = *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen =
		wstext_three.BackPen = wstext_four.BackPen = *bar_color[bp_ws].color;

		wstext_five.FrontPen = *bar_color[fp_cur].color;
		wstext_five.BackPen = *bar_color[bp_cur].color;
	}
}

static inline unsigned char * maptm(void)
{
	if (*current_layout == 0) {
		return bar_text[mode_tile].text;
	}
	if (*current_layout == 1) {
		return (bar_text[mode_grid].text);
	}
	if (*current_layout == 2) {
		return bar_text[mode_dwindle].text;
	}
	if (*current_layout == 3) {
		return bar_text[mode_spiral].text;
	}

	return bar_text[err].text;
}

short info_window(unsigned char * info_text)
{
	struct TagItem tagitem[9];
	struct Window *iwin, *twin;
	short closewin = FALSE;
	short info_text_length;
	unsigned long l, t, w, h, tleft;
	struct IntuiText iitext =
	{
		.TopEdge = 0,
		.LeftEdge = 0,
		.ITextFont = NULL,
		.DrawMode = JAM2, //-V2568
		.FrontPen = 1, //-V2568
		.BackPen = 2, //-V2568
		.IText = info_text,
		.NextText = NULL
	};

	tagitem[0].ti_Tag = WA_Width; //-V2544 //-V2568
	tagitem[0].ti_Data = 100; //-V2544 //-V2568
	tagitem[1].ti_Tag = WA_Height; //-V2544 //-V2568
	tagitem[1].ti_Data = 50; //-V2544 //-V2568
	tagitem[2].ti_Tag = WA_Top; //-V2544 //-V2568
	tagitem[2].ti_Data = (unsigned long)(sheight / 2);
	tagitem[3].ti_Tag = WA_SimpleRefresh; //-V2544 //-V2568
	tagitem[3].ti_Data = 1; //-V2568
	tagitem[4].ti_Tag = WA_IDCMP; //-V2544 //-V2568
	tagitem[4].ti_Data = IDCMP_CLOSEWINDOW; //-V2568
	tagitem[5].ti_Tag = WA_Flags; //-V2544 //-V2568
	tagitem[5].ti_Data = WFLG_SIZEGADGET|WFLG_DRAGBAR|WFLG_DEPTHGADGET|WFLG_CLOSEGADGET|WFLG_ACTIVATE; //-V2544 //-V2568
	tagitem[6].ti_Tag = WA_Title; //-V2544 //-V2568
	tagitem[6].ti_Data = (unsigned long)"Dintwm Info"; //-V2568
	tagitem[7].ti_Tag = WA_Left; //-V2544 //-V2568
	tagitem[7].ti_Data = (unsigned long)((swidth / 2) - 150); //-V2568
	tagitem[8].ti_Tag = TAG_DONE; //-V2544 //-V2568

	lockbasescreen(&ilock, &screen);

	// Hack to precalculate needed window width
	twin = OpenWindowTagList(NULL, tagitem);
	if (!twin) {
		unlockbasescreen(&ilock, &screen);
		return FALSE;
	}
	info_text_length = TextLength(twin->RPort, info_text, strnlen((const char *)info_text, TT_MAX_LENGTH));
	tleft = (unsigned long)info_text_length + (unsigned long)twin->BorderLeft + (unsigned long)twin->BorderRight;
	tagitem[0].ti_Tag = WA_Width; //-V2544 //-V2568
	tagitem[0].ti_Data = tleft; //-V2544 //-V2568
	tagitem[7].ti_Tag = WA_Left; //-V2544 //-V2568
	tagitem[7].ti_Data = (unsigned long)(swidth / 2) - (unsigned long)(tleft / 2); //-V2568
	CloseWindow(twin);
	// End hack

	iwin = OpenWindowTagList(NULL, tagitem);
	if (!iwin) {
		unlockbasescreen(&ilock, &screen);
		return FALSE;
	}

	unlockbasescreen(&ilock, &screen);

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
		ReplyMsg((struct Message *)msg); //-V2545
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
	tile_off = tile_off == FALSE ? TRUE : FALSE;
	return TRUE;
}
