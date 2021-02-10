// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "./dintwm.h"
#include <dos/dostags.h>
#include "./ketopt.h"

static void cwb(struct Window *w, int wx, int wy, int ww, int wh);
static void lockbasescreen(unsigned long *il, struct Screen **s);
static void unlockbasescreen(unsigned long *il, struct Screen **s);
static unsigned long ilock;
static int skip = 0;
static struct Window *window;
static const int nmaster = 1;
static int layout_start = LAYOUT_START;
static int *layout_number = &layout_start;
static int nolock = 0;
static struct Screen *screen;
static short printusage(void);
static short skipper(struct Window *w);
static void moveallwin(int m);
static void clearextdata(void);

int main(int argc, char **argv)
{
	ketopt_t opt = KETOPT_INIT;
	static int c;
	static int dint_opt_state = NOTSET;
	static int dint_exit_state = EXIT_SUCCESS;
	static int dint_fail_state = EXIT_SUCCESS;
	static int not_known;

	fact = TILE_FACT_DEF;
	current_ws = 0U;
	current_ws |= WS_0;
	backdropped = FALSE;

	while ((c = ketopt(&opt, argc, argv, 1, "uU:B:L:R:Cdghst", 0)) >= 0) {
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
			lockbasescreen(&ilock, &screen);
			if (topgap > screen->Height || topgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			unlockbasescreen(&ilock, &screen);
			break;
		case 'B':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			bottomgap = (int)strtol(opt.arg, (char **)NULL, 10);
			lockbasescreen(&ilock, &screen);
			if (bottomgap > screen->Height || bottomgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			unlockbasescreen(&ilock, &screen);
			break;
		case 'L':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			leftgap = (int)strtol(opt.arg, (char **)NULL, 10);
			lockbasescreen(&ilock, &screen);
			if (leftgap > screen->Width || leftgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			unlockbasescreen(&ilock, &screen);
			break;
		case 'R':
			if (*opt.arg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			rightgap = (int)strtol(opt.arg, (char **)NULL, 10);
			lockbasescreen(&ilock, &screen);
			if (rightgap > screen->Width || rightgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			unlockbasescreen(&ilock, &screen);
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
		printf("Do not use two tile functions at the same time.\n");
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
		default:
			dint_exit_state =
		    		defkeys[dint_opt_state].func(&defkeys[dint_opt_state].arg);
			break;
		}
	}

	current_ws &= ~(WS_0|WS_1|WS_2|WS_3|WS_4|WS_5);
	clearextdata();
	return dint_exit_state;
}

static short printusage(void)
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
	       "Options:",
	       "-d: Fibonacci dwindle",
	       "-g: Horizontal grid",
	       "-t: Tile with left master",
	       "-s: Fibonacci spiral",
	       "<other arg> -u: Add workbench bar gap",
	       "<other arg> -U<int>: Add custom top gap",
	       "<other arg> -B<int>: Add custom bottom gap",
	       "<other arg> -L<int>: Add custom left gap",
	       "<other arg> -R<int>: Add custom right gap", "-h: This message");

	return TRUE;
}

static short skipper(struct Window *w)
{
	if (w->Flags & (unsigned long)WFLG_BACKDROP) {
		backdropped = TRUE;
		return SKIP;
	} else {
		backdropped = FALSE;
	}

	if (w->Flags & (unsigned long)WFLG_GIMMEZEROZERO) {
		return SKIP;
	}

	if (strcmp("Workbench", (const char *)w->Title) == 0) {
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

	sh = screen->Height - (bottomgap + topgap);
	sw = screen->Width - (rightgap + leftgap);

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

	sh = screen->Height - (bottomgap + topgap);
	sw = screen->Width - (rightgap + leftgap);

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

	sh = screen->Height - (bottomgap + topgap);
	sw = screen->Width - (rightgap + leftgap);

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

static void clearextdata(void)
{
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window;
	     window = window->NextWindow) {
		if (window->ExtData) {
			window->ExtData = NULL;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

int countwindows(int l)
{
	int wincount;
	if (l) {
		lockbasescreen(&ilock, &screen);
	}
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
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

	if ((file = Open(conline, MODE_OLDFILE))) {
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
		(void)SystemTagList(cmd, stags);

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
		topgap += topgap <= ((screen->Height - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case BOTTOMGAP_ID:
		bottomgap += bottomgap <= ((screen->Height - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case LEFTGAP_ID:
		leftgap += leftgap <= ((screen->Width - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		break;
	case RIGHTGAP_ID:
		rightgap += rightgap <= ((screen->Width - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
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
		topgap += topgap <= ((screen->Height - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		bottomgap += bottomgap <= ((screen->Height - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		leftgap += leftgap <= ((screen->Width - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
		rightgap += rightgap <= ((screen->Width - GAP_INC_OFFSET) / 2) ? gap_change_value : 0;
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
	lockbasescreen(&ilock, &screen);
	for (window = screen->FirstWindow; window;
		window = window->NextWindow) {
		if ((skip = skipper(window)) == SKIP) {
			continue;
		}
		if (m == FRONT) {
			WindowToFront(window);
		} else {
			WindowToBack(window);
		}
	}
	unlockbasescreen(&ilock, &screen);
}


short changews(const Arg * arg) {
	if (current_ws & arg->u) {
		return TRUE;
	}

	if (backdropped) {
		return TRUE;
	}

	moveallwin(BACK);
	current_ws = arg->u;
	moveallwin(FRONT);

	return(defkeys[*current_layout].func(&defkeys[*current_layout].arg));
}

short movetows(const Arg * arg) {
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
		}
	}
	unlockbasescreen(&ilock, &screen);
	return(defkeys[*current_layout].func(&defkeys[*current_layout].arg));
}
