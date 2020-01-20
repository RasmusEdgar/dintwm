#include "dintwm.h"

static void fibonacci(int);
static void printusage(int, int);
static void lockbasescreen(unsigned long *ilock, struct Screen **screen);
static void unlockbasescreen(unsigned long *ilock, struct Screen **screen);
static int skipper(struct Window *window);
static struct Screen *screen;
static unsigned long ilock;
static int skip = 0;
static struct Window *window;
static const int nmaster = 1;
static const int fact = 550;
short rc;

int main(int argc, char **argv)
{
	int i = 0, optnum = 0, optargerr = 0;
	char margopt = 'a';
	
	if ((argc == 0 ) || ((argv[1][0] == '-') && (argv[1][1] == 'C'))) {
		if((rc = commo()) == 0) {
			exit(EXIT_SUCCESS);
		}
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'b':
				if (optargerr == 0 && topgap == 0) {
					topgap = screen->BarHeight -1;
				} else {
					optargerr = 1;
				}
				break;
			case 'B':
				if (optargerr == 0 && topgap == 0) {
					topgap = argv[i][2] != '\0' ? atoi(&argv[i][2]) : atoi(&argv[i][3]);
					if (topgap > screen->Height || topgap < 0) {
						optargerr = 2;
					}
				} else {
					optargerr = 1;
				}
				break;
			default:
                               	margopt = margopt == 'a' ? argv[i][1] : 'x';
				break;
			}
		} else {
			if (topgap == 0) {
				optnum = i;
				margopt = 'x';
				break;
			}
		}
		if (optargerr >= 1 && margopt != 'x') {
			margopt = 'x';		
		}
	}

	switch (margopt) {
		case 'd':
			dwindle();
			break;
		case 'g':
			hgrid();
			break;
		case 'h':
			printusage(0, optnum);
			break;
		case 't':
			tile();
			break;
		case 's':
			spiral();
			break;
		default:
			if(optargerr < 0) {
				printusage(optargerr, optnum);
			}
	}

	exit(EXIT_SUCCESS);
}

void printusage(int err, int optnum)
{

	switch (err) {
		case 1:
			printf("%s\n","Too many optional, main arguments or unknown option");
			break;
		case 2:
			printf("%s\n","Custom topgap is either too big or too small.");
			break;
		case 3:
			printf("%s %d\n","Unknown option in position", optnum);
			break;
		default:
			printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
				"Options:",
				"-d: Fibonacci dwindle",
				"-g: Horizontal grid",
				"-t: Tile with left master",
				"-s: Fibonacci spiral",
				"<other arg> -b: Add workbench bar gap",
				"<other arg> -B<int>: Add custom top gap",
				"-h: This message");
	}
}

int skipper(struct Window *window)
{
	if (window->Flags & BACKDROP) {
		return 1;
	}

	if (strcmp("Workbench", (const char *)window->Title) == 0) {
		return 1;
	}

	printf("wtype: %s\n", exclude_wtype);
	if (strcmp(exclude_wtype, (const char *)window->Title) == 0) {
		return 1;
	}

	return 0;

}

void tile(void)
{
	int wincount = 0, wnr = 0, mwinwidth = 0, winheight =
	    0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;

	lockbasescreen(&ilock, &screen);
	// count windows
	/*for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++);*/
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
		if ((skip = skipper(window)) == 1) {
			wincount--;
			continue;
		}
	}

	if (wincount == 0) {
		unlockbasescreen(&ilock, &screen);
		return;
	}

	// remove count for workbench window
	//wincount--;

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
			BeginRefresh(window);
			ChangeWindowBox(window, winx, topgap - winy + mwiny,
					mwinwidth, winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			mwiny += winheight;
		} else {
			winheight =
			    (screen->Height - nwiny -
			     topgap) / (wincount - wnr);
			BeginRefresh(window);
			ChangeWindowBox(window, winx + mwinwidth,
					topgap - winy + nwiny,
					screen->Width - mwinwidth, winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			nwiny += winheight;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void hgrid(void)
{
	unsigned int wincount, wnr, ntop = 0, nbottom = 0;
	int winwidth = 0;
	int winx = 0, winy = 0;

	lockbasescreen(&ilock, &screen);
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
		if ((skip = skipper(window)) == 1) {
			wincount--;
			continue;
		}
	}

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
			BeginRefresh(window);
			ChangeWindowBox(window, winx, topgap - winy,
					winwidth, screen->Height - topgap);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				BeginRefresh(window);
				ChangeWindowBox(window,
						winx +
						wnr * screen->Width / ntop,
						topgap - winy,
						screen->Width / ntop,
						(screen->Height - topgap) / 2);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			} else {
				BeginRefresh(window);
				ChangeWindowBox(window,
						winx + (wnr -
							ntop) * screen->Width /
						nbottom,
						topgap + winy + screen->Height / 2,
						screen->Width / nbottom,
						(screen->Height - topgap) / 2);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			}
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void fibonacci(int s)
{
	int wnr, wincount, winx, winwidth, winheight;
	int winy;

	lockbasescreen(&ilock, &screen);
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
		if ((skip = skipper(window)) == 1) {
			wincount--;
			continue;
		}
	}

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
				}
			} else if (wnr == 1) {
				winwidth = screen->Width - winwidth;
			}
			wnr++;
		}
		BeginRefresh(window);
		ChangeWindowBox(window, winx, winy, winwidth, winheight);
		EndRefresh(window, TRUE);
		RefreshWindowFrame(window);
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

void lockbasescreen(unsigned long *ilock, struct Screen **screen)
{
	*ilock = LockIBase(0L);
	*screen = LockPubScreen(NULL);
}

void unlockbasescreen(unsigned long *ilock, struct Screen **screen)
{
	UnlockPubScreen(NULL, *screen);
	UnlockIBase(*ilock);
}
