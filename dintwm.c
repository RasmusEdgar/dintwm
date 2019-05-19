#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>

#define MIN(A, B)               ((A) < (B) ? (A) : (B))

void tile(void);
void hgrid(void);
void fibonacci(int);
void spiral(void);
void dwindle(void);
void printusage(void);
void lockbasescreen(unsigned long *ilock, struct Screen **screen);
void unlockbasescreen(unsigned long *ilock, struct Screen **screen);
int skipper(struct Window *window);

struct Screen *screen;
unsigned long ilock;
int skip = 0;
int topgap = 0;
struct Window *window;
static const int nmaster = 1;
static const int fact = 530;

int main(int argc, char **argv)
{
	int i = 0;

	lockbasescreen(&ilock, &screen);

	if (argv[1] == 0)
		printusage();

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			switch (argv[i][1]) {
			case 'b':
				topgap = screen->BarHeight - 1;
			case 'd':
				dwindle();
				break;
			case 'g':
				hgrid();
				break;
			case 'h':
				printusage();
				break;
			case 't':
				tile();
				break;
			case 's':
				spiral();
				break;
			default:
				printusage();
		} else
			printusage();
	}

	unlockbasescreen(&ilock, &screen);
	exit(0);
}

void printusage(void)
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
	       "Options:",
	       "-d: Fibonacci dwindle",
	       "-g: Horizontal grid",
	       "-t: Tile with left master",
	       "-s: Fibonacci spiral",
	       "<other arg> -b: Add workbench bar gap", "-h: This message");
}

int skipper(struct Window *window)
{
	if (window->Flags & BACKDROP) {
		return 1;
	}

	if (strcmp("Workbench", window->Title) == 0) {
		return 1;
	}

	return 0;

}

void tile(void)
{
	int wincount = 0, wnr = 0, mwinwidth = 0, winheight =
	    0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;
	// count windows
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) ;
	if (wincount == 0) {
		return;
	}
	// remove count for workbench window
	wincount--;
	if (wincount > nmaster)
		mwinwidth = nmaster ? (screen->Width * fact) / 1000 : 0;
	else
		mwinwidth = screen->Width;

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
}

void hgrid(void)
{
	unsigned int wincount, wnr;
	int winwidth = 0;
	int ntop = 0, nbottom = 0, winx = 0, winy = 0;

	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) {
		if ((skip = skipper(window)) == 1) {
			wincount--;
			continue;
		}
	}

	if (wincount == 0)
		return;

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
					winwidth, topgap - screen->Height);
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
						(screen->Height / 2) - topgap);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			} else {
				BeginRefresh(window);
				ChangeWindowBox(window,
						winx + (wnr -
							ntop) * screen->Width /
						nbottom,
						winy + screen->Height / 2,
						screen->Width / nbottom,
						screen->Height / 2);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			}
		}
	}
}

void fibonacci(int s)
{
	unsigned int wnr, wincount, winx, winy, winwidth, winheight;

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

		if ((wnr % 2
		     && winheight / 2 >
		     ((screen->WBorRight - 1) - (screen->WBorLeft - 1)))
		    || (!(wnr % 2)
			&& winwidth / 2 >
			((screen->WBorRight - 1) - (screen->WBorLeft - 1)))) {
			if (wnr < wincount - 1) {
				if (wnr % 2)
					winheight /= 2;
				else
					winwidth /= 2;
				if ((wnr % 4) == 2 && !s)
					winx += winwidth;
				else if ((wnr % 4) == 3 && !s)
					winy += winheight;
			}
			if ((wnr % 4) == 0) {
				if (s)
					winy += winheight;
				else
					winy -= winheight;
			} else if ((wnr % 4) == 1)
				winx += winwidth;
			else if ((wnr % 4) == 2)
				winy += winheight;
			else if ((wnr % 4) == 3) {
				if (s)
					winx += winwidth;
				else
					winx -= winwidth;
			}
			if (wnr == 0) {
				if (wincount != 1)
					winwidth =
					    (screen->Width * fact) / 1000;
				winy = topgap;
			} else if (wnr == 1)
				winwidth = screen->Width - winwidth;
			wnr++;
		}
		BeginRefresh(window);
		ChangeWindowBox(window, winx, winy, winwidth, winheight);
		EndRefresh(window, TRUE);
		RefreshWindowFrame(window);
	}
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
