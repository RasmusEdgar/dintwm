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
void lockbasescreen(unsigned long *ilock, struct Screen **screen);
void unlockbasescreen(unsigned long *ilock, struct Screen **screen);
int skipper(struct Window *window);

struct Screen *screen;
unsigned long ilock;
struct Window *window;
static const int nmaster = 1;
static const int fact = 530;

struct Flagarray {
	unsigned long hexflag;
	char flagname[14];
};

static struct Flagarray flagarray[] = {
	{0x00000100, "BACKDROP"},
	{0x00000400, "GIMMEZEROZERO"},
	{0x00000800, "BORDERLESS"},
	{0x00002000, "WINDOWACTIVE"},
	{0x02000000, "WBENCHWINDOW"},
	{0x10000000, "ZOOMED"}
};

int main(void)
{
	tile();

	return 0;
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
	int skip = 0, wincount = 0, wnr = 0, mwinwidth = 0, winheight =
	    0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;
	lockbasescreen(&ilock, &screen);
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
		if ((wnr < nmaster) && (window->Title)) {
			winheight =
			    (screen->Height - mwiny) / (MIN(wincount, nmaster) -
							wnr);
			BeginRefresh(window);
			ChangeWindowBox(window, winx, winy + mwiny,
					mwinwidth +
					((screen->WBorRight - 1) -
					 (screen->WBorLeft - 1)), winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			mwiny += winheight;
		} else {
			winheight = (screen->Height - nwiny) / (wincount - wnr);
			BeginRefresh(window);
			ChangeWindowBox(window, winx + mwinwidth, winy + nwiny,
					screen->Width - mwinwidth +
					((screen->WBorRight - 1) -
					 (screen->WBorLeft - 1)), winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			nwiny += winheight;
		}
	}
	unlockbasescreen(&ilock, &screen);
}

void hgrid(void)
{
	unsigned int wincount, wnr, skip = 0;
	int winwidth = 0;
	int ntop = 0, nbottom = 0, winx = 0, winy = 0;

	lockbasescreen(&ilock, &screen);
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
			ChangeWindowBox(window, winx, winy,
					winwidth +
					((screen->WBorRight - 1) -
					 (screen->WBorLeft - 1)),
					screen->Height);
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
						winy,
						screen->Width / ntop +
						((screen->WBorRight - 1) -
						 (screen->WBorLeft - 1)),
						screen->Height / 2);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			} else {
				BeginRefresh(window);
				ChangeWindowBox(window,
						winx + (wnr -
							ntop) * screen->Width /
						nbottom,
						winy + screen->Height / 2,
						screen->Width / nbottom +
						((screen->WBorRight - 1) -
						 (screen->WBorLeft - 1)),
						screen->Height / 2);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
			}
		}
	}
	unlockbasescreen(&ilock, &screen);
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
