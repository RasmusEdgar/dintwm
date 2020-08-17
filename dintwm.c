#include "dintwm.h"
#include "ketopt.h"
#include <dos/dostags.h>

static void fibonacci(int);
static void switcher(int);
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
short rc;
static int layout_start = LAYOUT_START;
static int *layout_number = &layout_start;
int main(int argc, char **argv)
{
	ketopt_t opt = KETOPT_INIT;
	static int c;
	static int dint_opt_state = COMMODITIZE;
	static int dint_exit_state = EXIT_SUCCESS;


	while ((c = ketopt(&opt, argc, argv, 1, "aA:B:Cdghst", 0)) >= 0) {
		switch (c) {
			case 'a':
				topgap = calcgap();
				break;
			case 'A':
				topgap = atoi(opt.arg);
				if (topgap > screen->Height || topgap < 0) {
					dint_opt_state = GAP_ERR;
				}
				break;
			case 'B':
				bottomgap = atoi(opt.arg);
				if (bottomgap > screen->Height || bottomgap < 0) {
					dint_opt_state = GAP_ERR;
				}
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
			case '?':
				printf("unknown opt: -%c\n", opt.opt? opt.opt : ':');
				break;
			case ':':
				printf("missing arg: -%c\n", opt.opt? opt.opt : ':');
				break;
			default:
				break;
		}
			
	}

	switch (dint_opt_state) {
		case DOUBLE_OPTION_ERR:
			printf("Do not use two tile functions at the same time.\n");
			dint_exit_state = EXIT_FAILURE;
			break;
		case GAP_ERR:
			printf("Gap is larger or smaller than screen.\n");
			dint_exit_state = EXIT_FAILURE;
			break;
		case COMMODITIZE:
			if ((rc = commo()) != 0) {
				dint_exit_state = EXIT_FAILURE;
			}
			if (windowliststore != NULL) {
				free(windowliststore);
			}
			break;
		default:
			defkeyfuncs[dint_opt_state].func();
			break;
	}

	exit(dint_exit_state);

}

void printusage(void)
{

	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
		"Options:",
		"-d: Fibonacci dwindle",
		"-g: Horizontal grid",
		"-t: Tile with left master",
		"-s: Fibonacci spiral",
		"<other arg> -a: Add workbench bar gap",
		"<other arg> -A<int>: Add custom top gap",
		"<other arg> -B<int>: Add custom bottom gap",
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

void tile(void)
{
	int wincount = 0, wnr = 0, mwinwidth = 0, winheight =
	    0, nwiny = 0, mwiny = 0;

	lockbasescreen(&ilock, &screen);
	wincount = countwindows();

	if (wincount == 0) {
		unlockbasescreen(&ilock, &screen);
		return;
	}

	if (wincount > nmaster) {
		mwinwidth = nmaster != 0 ? (screen->Width * fact) / 1000 : 0;
	} else {
		mwinwidth = screen->Width;
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if ((skip = skipper(window)) == 1) {
			wnr--;
			continue;
		}
		if (wnr < nmaster) {
			winheight =
			    (screen->Height - mwiny -
			     topgap) / (MIN(wincount, nmaster) - wnr);
			winheight -= bottomgap;
			cwb(window, 0, (topgap - mwiny), mwinwidth, winheight);
			mwiny += winheight;
		} else {
			winheight =
			    ((screen->Height - bottomgap) - nwiny -
			    topgap) / (wincount - wnr);
			cwb(window, mwinwidth, (topgap + nwiny), (screen->Width - mwinwidth), winheight);
			nwiny += winheight;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void hgrid(void)
{
	int wincount, wnr, ntop = 0, nbottom = 0;
	int winwidth = 0;
	int winx = 0;

	lockbasescreen(&ilock, &screen);
	wincount = countwindows();

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
			winwidth = screen->Width / wincount;
			winx = wnr == 1 ? screen->Width / wincount : 0;
			cwb(window, winx, topgap, winwidth, (screen->Height - topgap) - bottomgap);
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				cwb(window, (winx + wnr * screen->Width / ntop), topgap, (screen->Width / ntop), ((screen->Height - topgap) / 2));
			} else {
				cwb(window, (winx + (wnr - ntop) * screen->Width / nbottom), (topgap + (screen->Height - bottomgap) / 2), (screen->Width / nbottom), ((screen->Height - topgap) / 2) - bottomgap);
			}
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void fibonacci(int s)
{
	int wnr, wincount, winx, winwidth, winheight, winy;

	lockbasescreen(&ilock, &screen);
	wincount = countwindows();

	winx = 0;
	winy = topgap;
	winwidth = screen->Width;
	winheight = screen->Height - topgap;

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow) {
		if ((skip = skipper(window)) == 1) {
			wnr--;
			continue;
		}

		if ((wnr % 2 != 0
		     && winheight / 2 >
		     ((screen->WBorRight - 1) - (screen->WBorLeft - 1)))
		    || (wnr % 2 == 0
			&& winwidth / 2 >
			((screen->WBorRight - 1) - (screen->WBorLeft - 1)))) {
			if (wnr < wincount - 1) {
				if (wnr % 2 != 0) {
					winheight /= 2;
				} else {
					winwidth /= 2;
				}
				if ((wnr % 4) == 2 && s == 0) {
					winx += winwidth;
				} else if ((wnr % 4) == 3 && s == 0) {
					winy += winheight;
				}
			}
			if ((wnr % 4) == 0) {
				if (s != 0) {
					winy += winheight;
				} else {
					winy -= winheight;
				}
			} else if ((wnr % 4) == 1) {
				winx += winwidth;
			} else if ((wnr % 4) == 2) {
				winy += winheight;
			} else if ((wnr % 4) == 3) {
				if (s != 0) {
					winx += winwidth;
				} else {
					winx -= winwidth;
				}
			}
			if (wnr == 0) {
				if (wincount != 1) {
					winwidth =
					    (screen->Width * fact) / 1000;
				winy = topgap;
				winheight -= bottomgap;
				}
			} else if (wnr == 1) {
				winwidth = screen->Width - winwidth;
			}
			wnr++;
		}
		cwb(window, winx, winy, winwidth, winheight);
	}
	unlockbasescreen(&ilock, &screen);
}

void dwindle(void)
{
	fibonacci(1);
}

void spiral(void)
{
	fibonacci(0);
}

void switcher(int d)
{
	if(*current_layout < TILE_FUNC_LIMIT && *layout_number == LAYOUT_START) {
		*layout_number = *current_layout;
	}
		
	if(d) {
		(*layout_number)++;
		if(*layout_number > TILE_FUNC_LIMIT) {
			*layout_number = 0;
		} 
		defkeyfuncs[(*layout_number)].func();
	} else {
		(*layout_number)--;
		if(*layout_number < 0) {
			*layout_number = TILE_FUNC_LIMIT;
		} 
		defkeyfuncs[(*layout_number)].func();
	}
	*current_layout = *layout_number;

}

void switchf(void)
{
	switcher(1);
}

void switchb(void)
{
	switcher(0);
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
        	// allocate new node
		*next = malloc(sizeof(**next));
		if (*next) {
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
		} else {
			unlockbasescreen(&ilock, &screen);
			dst = NULL;
			return dst;
		}
	}
	*next = NULL;
	unlockbasescreen(&ilock, &screen);
	return dst;
}

void takesnapshot(void) {
	cleansnapshot();
	windowliststore = copywindowlist();	
}

void cleansnapshot(void) {
	if(windowliststore != NULL) {
		free(windowliststore);
		windowliststore = NULL;
	}
}

void restore(void)
{
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

int countwindows(void) {
	int wincount;
        for (wincount = 0, window = screen->FirstWindow; window;
             window = window->NextWindow, wincount++) {
                if ((skip = skipper(window)) == 1) {
                        wincount--;
                        continue;
                }
        }
	return wincount;
}

void doshell(void) {
	static unsigned char autocon[] ="CON:0/40/640/150/dintwm/AUTO/CLOSE/WAIT";
	static unsigned char cmd[] = "NewShell";
	struct TagItem stags[5];
    	long int file;

	if((file = Open(autocon, MODE_OLDFILE))) {
        	stags[0].ti_Tag = SYS_Input;
        	stags[0].ti_Data = (long unsigned int)file;
        	stags[1].ti_Tag = SYS_Output;
        	stags[1].ti_Data = 0;
        	stags[2].ti_Tag = SYS_Asynch;
        	stags[2].ti_Data = TRUE;
        	stags[3].ti_Tag = SYS_UserShell;
        	stags[3].ti_Data = TRUE;
        	stags[4].ti_Tag = TAG_DONE;
		(void)SystemTagList(cmd,stags);
	} else {
		return;
	}

}

int calcgap(void) {
	return(screen->BarHeight * 2);
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
