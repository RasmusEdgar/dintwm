//-V::677
#include "dintwm.h"
#include "ketopt.h"
#include <dos/dostags.h>

static void cwb(struct Window *w, int wx, int wy, int ww, int wh);
static void lockbasescreen(unsigned long *il, struct Screen **s);
static void unlockbasescreen(unsigned long *il, struct Screen **s);
static struct Window *copywindowlist(void);
static int skipper(struct Window *w);
static unsigned long ilock;
static int skip = 0;
static struct Window *window;
static struct Window *windowliststore = NULL;
static const int nmaster = 1;
static const int fact = 550;
static unsigned char restoretag = 'r';
static int layout_start = LAYOUT_START;
static int *layout_number = &layout_start;
static int nolock = 0;
int main(int argc, char **argv)
{
	ketopt_t opt = KETOPT_INIT;
	static int c;
	static int dint_opt_state = COMMODITIZE;
	static int dint_exit_state = EXIT_SUCCESS;
	static int dint_fail_state = EXIT_SUCCESS;
	static short rc;


	while ((c = ketopt(&opt, argc, argv, 1, "uU:B:L:R:Cdghst", 0)) >= 0) {
		switch (c) {
			case 'u':
				topgap = calcgap();
				break;
			case 'U':
				if(*opt.arg == '-') {
					dint_fail_state = MISSING;
					break;
				}
				topgap = atoi(opt.arg);
				lockbasescreen(&ilock, &screen);
				if (topgap > screen->Height || topgap < 0) {
					dint_fail_state = GAP_ERR;
				}
				unlockbasescreen(&ilock, &screen);
				break;
			case 'B':
				if(*opt.arg == '-') {
					dint_fail_state = MISSING;
					break;
				}
				bottomgap = atoi(opt.arg);
				lockbasescreen(&ilock, &screen);
				if (bottomgap > screen->Height || bottomgap < 0) {
					dint_fail_state = GAP_ERR;
				}
				unlockbasescreen(&ilock, &screen);
				break;
			case 'L':
				if(*opt.arg == '-') {
					dint_fail_state = MISSING;
					break;
				}
				leftgap = atoi(opt.arg);
				lockbasescreen(&ilock, &screen);
				if (leftgap > screen->Width || leftgap < 0) {
					dint_fail_state = GAP_ERR;
				}
				unlockbasescreen(&ilock, &screen);
				break;
			case 'R':
				if(*opt.arg == '-') {
					dint_fail_state = MISSING;
					break;
				}
				rightgap = atoi(opt.arg);
				lockbasescreen(&ilock, &screen);
				if (rightgap > screen->Width || rightgap < 0) {
					dint_fail_state = GAP_ERR;
				}
				unlockbasescreen(&ilock, &screen);
				break;
			case 'C':
				dint_opt_state = dint_opt_state == COMMODITIZE ? COMMODITIZE : DOUBLE_OPTION_ERR;
				break;
			case 'd': 
				dint_opt_state = dint_opt_state == COMMODITIZE ? FUNC_DWINDLE : DOUBLE_OPTION_ERR;
				break;
			case 'g':
				dint_opt_state = dint_opt_state == COMMODITIZE ? FUNC_HGRID : DOUBLE_OPTION_ERR;
				break;
			case 'h':
				dint_opt_state = dint_opt_state == COMMODITIZE ? FUNC_PRINTUSAGE : DOUBLE_OPTION_ERR;
				break;
			case 's':
				dint_opt_state = dint_opt_state == COMMODITIZE ? FUNC_SPIRAL : DOUBLE_OPTION_ERR;
				break;
			case 't':
				dint_opt_state = dint_opt_state == COMMODITIZE ? FUNC_TILE : DOUBLE_OPTION_ERR;
				break;
			case ':':
				dint_fail_state = MISSING;
				break;
			case '?':
				dint_fail_state = UNKNOWN;
				break;
			default:
				break;
		}
	}

	switch (dint_fail_state) {
		case DOUBLE_OPTION_ERR:
			printf("Do not use two tile functions at the same time.\n");
			dint_exit_state = EXIT_FAILURE;
			goto exit_state;
			break;
		case GAP_ERR:
			printf("Gap is larger or smaller than screen.\n");
			dint_exit_state = EXIT_FAILURE;
			goto exit_state;
			break;
		case UNKNOWN:
			printf("unknown opt: -%c\n", opt.opt? opt.opt : ':');
			dint_exit_state = EXIT_FAILURE;
			goto exit_state;
			break;
		case MISSING:
			printf("missing arg: -%c\n", opt.opt? opt.opt : ':');
			dint_exit_state = EXIT_FAILURE;
			goto exit_state;
			break;
		default:
			break;
	}

	
	switch (dint_opt_state) {
		case COMMODITIZE:
			if ((rc = commo()) != 0) {
				dint_exit_state = EXIT_FAILURE;
				goto exit_state;
			}
			if (windowliststore != NULL) {
				free(windowliststore);
			}
			break;
		case FUNC_PRINTUSAGE:
			printusage();
			break;
		default:
			defkeys[dint_opt_state].func(&defkeys[dint_opt_state].arg);
			break;
	}

	exit_state:
		exit(dint_exit_state);

}

void printusage(void)
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
		"<other arg> -R<int>: Add custom right gap",
		"-h: This message");
}

int skipper(struct Window *w)
{
	if (w->Flags & BACKDROP) {
		goto exit_skip;
	}

	if (w->Flags & GIMMEZEROZERO) {
		goto exit_skip;
	}

	if (strcmp("Workbench", (const char *)w->Title) == 0) {
		goto exit_skip;
	}

	if (strcmp(exclude_wtype, (const char *)w->Title) == 0) {
		goto exit_skip;
	}

	if (strlen(include_wtype) == 0) {
		goto exit_noskip;
	}

	if (strcmp(include_wtype, (const char *)w->Title) == 0) {
		goto exit_noskip;
	}

	exit_skip:
		return 1;

	exit_noskip:
		return 0;

}

void cwb(struct Window *w, int wx, int wy, int ww, int wh) {
	ChangeWindowBox(w, (short int)wx, (short int)wy, (short int)ww, (short int)wh);
	WindowToFront(w);
}

void tile(const Arg *arg)
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
		return;
	}

	if (wincount > nmaster) {
		mwinwidth = nmaster != 0 ? (sw * fact) / 1000 : 0;
	} else {
		mwinwidth = sw;
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == 1) {
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
}

void hgrid(const Arg *arg)
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
		return;
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == 1) {
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
}

void fibonacci(const Arg *arg)
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
	     window = window->NextWindow) {
		if ((skip = skipper(window)) == 1) {
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
		if ((wnr % 4) == 0) {
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
		wnr++;
		cwb(window, wx, wy, ww, wh);
	}
	unlockbasescreen(&ilock, &screen);
}

void switcher(const Arg *arg)
{
	if(*current_layout < TILE_FUNC_LIMIT && *layout_number == LAYOUT_START) {
		*layout_number = *current_layout;
	}
		
	if(arg->i) {
		(*layout_number)++;
		if(*layout_number > TILE_FUNC_LIMIT) {
			*layout_number = 0;
		} 
		defkeys[(*layout_number)].func(&defkeys[(*layout_number)].arg);
	} else {
		(*layout_number)--;
		if(*layout_number < 0) {
			*layout_number = TILE_FUNC_LIMIT;
		} 
		defkeys[(*layout_number)].func(&defkeys[(*layout_number)].arg);
	}
	*current_layout = *layout_number;

}

struct Window *copywindowlist(void) {
	lockbasescreen(&ilock, &screen);
	struct Window *dst = NULL, **next = &dst, *w = screen->FirstWindow;

	while (w)
	{
		if ((skip = skipper(w)) == 1) {
			w = w->NextWindow;
			continue;
		}

		*next = malloc(sizeof(**next));
		if (*next == NULL) {
			unlockbasescreen(&ilock, &screen);
			exit(EXIT_FAILURE);
		}

		(*next)->Title = w->Title;
		(*next)->LeftEdge = w->LeftEdge;
		(*next)->TopEdge = w->TopEdge;
		(*next)->Width = w->Width;
		(*next)->Height = w->Height;
		(*next)->Flags = w->Flags;
		(*next)->ExtData = &restoretag;
		w->ExtData = &restoretag;

		next = &(*next)->NextWindow;

		w = w->NextWindow;
	}
	*next = NULL;

	unlockbasescreen(&ilock, &screen);
	return dst;
}

void takesnapshot(const Arg *arg) {
	(void)arg;
	cleansnapshot(0);
	windowliststore = copywindowlist();	
}

void cleansnapshot(const Arg *arg) {
	(void)arg;
	if(windowliststore != NULL) {
		free(windowliststore);
		windowliststore = NULL;
	}
}

void restore(const Arg *arg) {
	(void)arg;
	if(windowliststore != NULL) {
		lockbasescreen(&ilock, &screen);
		struct Window *storehead = windowliststore;
		for (window = screen->FirstWindow; window;
			window = window->NextWindow) {
			if ((skip = skipper(window)) == 1) {
				windowliststore = windowliststore->NextWindow;
				continue;
			}

			if (windowliststore->ExtData == window->ExtData) {
				cwb(window, windowliststore->LeftEdge, windowliststore->TopEdge, windowliststore->Width, windowliststore->Height);
				WindowToFront(window);
			} else {
				WindowToBack(window);
				continue;
			}
			windowliststore = windowliststore->NextWindow;
		}
		windowliststore = storehead;
		unlockbasescreen(&ilock, &screen);
	}
}

int countwindows(int l) {
	int wincount;
	if(l) {
		lockbasescreen(&ilock, &screen);	
	}
        for (wincount = 0, window = screen->FirstWindow; window;
             window = window->NextWindow, wincount++) {
                if ((skip = skipper(window)) == 1) {
                        wincount--;
                        continue;
                }
        }
	if(l) {
		unlockbasescreen(&ilock, &screen);
	}
	return wincount;
}

void docmd(const Arg *arg) {
	(void)arg;
	static unsigned char autocon[] ="CON:0/40/640/150/dintwm/AUTO/CLOSE/WAIT";
	static unsigned char cmd[] = "NewShell";
	struct TagItem stags[5];
    	long int file;

	if(conline[0] == 0) {
		strncpy((char *)conline,(char *)autocon,(strlen((const char *)autocon))+1);
	}

	if(shellcmd[0] == 0) {
		strncpy((char *)shellcmd,(char *)cmd,(strlen((const char *)cmd))+1);
	}

	if((file = Open(conline, MODE_OLDFILE))) {
        	stags[0].ti_Tag = SYS_Input;
        	stags[0].ti_Data = (long unsigned int)file;
        	stags[1].ti_Tag = SYS_Output;
        	stags[1].ti_Data = 0;
        	stags[2].ti_Tag = SYS_Asynch;
        	stags[2].ti_Data = TRUE;
        	stags[3].ti_Tag = SYS_UserShell;
        	stags[3].ti_Data = TRUE;
        	stags[4].ti_Tag = TAG_DONE;
		(void)SystemTagList(shellcmd,stags);
	} else {
		return;
	}

}

int calcgap(void) {
	int bheight;
	lockbasescreen(&ilock, &screen);
	bheight = screen->BarHeight + 1;
	unlockbasescreen(&ilock, &screen);
	return(bheight);
}

void lockbasescreen(unsigned long *il, struct Screen **s)
{
	*il = LockIBase(0L);
	*s = LockPubScreen(NULL);
}

void unlockbasescreen(unsigned long *il, struct Screen **s)
{
	UnlockPubScreen(NULL, *s);
	UnlockIBase(*il);
}
