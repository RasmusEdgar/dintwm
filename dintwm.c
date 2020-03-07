#include "dintwm.h"

static void fibonacci(int);
static void printusage(int, int);
static void cwb(struct Window *w, int wx, int wy, int ww, int wh);
static void lockbasescreen(unsigned long *il, struct Screen **s);
static void unlockbasescreen(unsigned long *il, struct Screen **s);
static struct Window *copywindowlist(struct Screen *s);
static void printList(struct Window *w);
static int skipper(struct Window *w);
static struct Screen *screen;
static unsigned long ilock;
static int skip = 0;
static struct Window *window;
static struct Window *windowliststore;
static const int nmaster = 1;
static const int fact = 550;
static signed char restoretag[] = "r";
short rc;

int main(int argc, char **argv)
{
	int i = 0, optnum = 0, optargerr = 0;
	char margopt = 'a';
	
	if ((argc == 0 ) || ((argv[1][0] == '-') && (argv[1][1] == 'C'))) {
		lockbasescreen(&ilock, &screen);
		windowliststore = copywindowlist(screen);
		printList(windowliststore);
		unlockbasescreen(&ilock, &screen);
		if((rc = commo()) == 0) {
			free(windowliststore);
			exit(EXIT_SUCCESS);
		}
		printf("Commo failed\n");
		free(windowliststore);
		exit(EXIT_FAILURE);
	}

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'b':
				if (optargerr == 0 && topgap == 0) {
					topgap = screen->BarHeight * 2 - 2;
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

int skipper(struct Window *w)
{
	if (w->Flags & BACKDROP) {
		return 1;
	}

	if (strcmp("Workbench", (const char *)w->Title) == 0) {
		return 1;
	}

	if (strcmp(exclude_wtype, (const char *)w->Title) == 0) {
		return 1;
	}

	return 0;

}

void cwb(struct Window *w, int wx, int wy, int ww, int wh) {
	BeginRefresh(window);
	ChangeWindowBox(w, (short int)wx, (short int)wy, (short int)ww, (short int)wh);
	EndRefresh(window, TRUE);
	RefreshWindowFrame(window);
}

void tile(void)
{
	int wincount = 0, wnr = 0, mwinwidth = 0, winheight =
	    0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;

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
			cwb(window, winx, (topgap - winy + mwiny), mwinwidth, winheight);
			mwiny += winheight;
		} else {
			winheight =
			    (screen->Height - nwiny -
			     topgap) / (wincount - wnr);
			cwb(window, (winx + mwinwidth), (topgap - winy + nwiny), (screen->Width - mwinwidth), winheight);
			nwiny += winheight;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void hgrid(void)
{
	int wincount, wnr, ntop = 0, nbottom = 0;
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
			cwb(window, winx, (topgap - winy), winwidth, (screen->Height - topgap));
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				cwb(window, (winx + wnr * screen->Width / ntop), (topgap - winy), (screen->Width / ntop), ((screen->Height - topgap) / 2));
			} else {
				cwb(window, (winx + (wnr - ntop) * screen->Width / nbottom), (topgap + winy + screen->Height / 2), (screen->Width / nbottom), ((screen->Height - topgap) / 2));
			}
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void fibonacci(int s)
{
	int wnr, wincount, winx, winwidth, winheight, winy;

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

struct Window *copywindowlist(struct Screen *s) {
	struct Window *dst = NULL, **next = &dst, *w = s->FirstWindow;
	while (w)
	{
		/*if ((skip = skipper(w)) == 1) {
			continue;
		}*/
        	// allocate new node
		*next = malloc(sizeof(**next));
		if (*next) {
			(*next)->Title = w->Title;
			(*next)->LeftEdge = w->LeftEdge;
			(*next)->TopEdge = w->TopEdge;
			(*next)->Width = w->Width;
			(*next)->Height = w->Height;
			(*next)->Flags = w->Flags;
			(*next)->UserData = restoretag;
			w->UserData = restoretag;

			// reposition our next-link to the address of ptr->next
			//  of the node we just added.
			next = &(*next)->NextWindow;

			// and finally, advance the source pointer
			w = w->NextWindow;
		} else {
			perror("Failed to allocate node.");
			exit(EXIT_FAILURE);
		}
	}
	*next = NULL;
	return dst;
}

void printList(struct Window *w) {

	struct Window *ptr;
	ptr = w;
	while (ptr)
	{
		printf("Title %s\n", ptr->Title);
		printf("LeftEdge %hu\n", ptr->LeftEdge);
		printf("TopEdge %hu\n", ptr->TopEdge);
		printf("Width %hu\n", ptr->Width);
		printf("Height %hu\n", ptr->Height);
		ptr = ptr->NextWindow;
	}

	printf("done\n");
}

void restore(void)
{
	lockbasescreen(&ilock, &screen);
	struct Window *storehead = windowliststore;
	for (window = screen->FirstWindow; window;
	     window = window->NextWindow) {
		if ((skip = skipper(window)) == 1) {
			windowliststore = windowliststore->NextWindow;
			continue;
		}
		if (windowliststore->UserData == window->UserData) {
			printf("UserData val store round one %s\n", windowliststore->UserData);
			cwb(window, windowliststore->LeftEdge, windowliststore->TopEdge, windowliststore->Width, windowliststore->Height);
			//window->UserData = windowliststore->UserData;
		} else {
			printf("No UserData match!\n");
			printf("UserData val store %s\n", windowliststore->UserData);
			printf("UserData val window %s\n", window->UserData);
		}
		windowliststore = windowliststore->NextWindow;
	}
	windowliststore = storehead;
	unlockbasescreen(&ilock, &screen);
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
