#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>

void tile(void);
void lockbasescreen(unsigned long *ilock, struct Screen **screen);
void unlockbasescreen(unsigned long *ilock, struct Screen **screen);

struct Screen *screen;
unsigned long ilock;
struct Window *window;

int main(void)
{
	tile();

	return 0;
}

void tile(void)
{
	int wincount = 0, wnr = 0, winwidth = 0, winheight = 0, winx = 0, winy = 0, maxncols = 0, colcount = 0;
	int masterwindowwidth = 0, masterwindowheight = 0, masterwindowx = 0, masterwindowy = 0;
	int slavewindowwidth = 0, slavewindowheight = 0, slavewindowx = 0, slavewindowy = 0;
	lockbasescreen(&ilock, &screen);
	// count windows
	for (wincount = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wincount++) ;
	if (wincount == 0) {
		return;
	}
	// remove count for workbench window
	wincount--;
	printf("wincount: %d\n", wincount);

	if (wincount <= 2) {
		winwidth = screen->Width / wincount;
		winheight = screen->Height;
	}

	if (wincount > 2 && wincount % 2 == 0 || wincount >= 8) {
		printf("jdsakdj\n");
		if (wincount % 2 == 1) {
			maxncols = wincount / 3;
			winwidth = screen->Width / maxncols;
			winheight = screen->Height / maxncols;
		} 
		if (wincount % 3 == 0) {
			maxncols = wincount / 3;
			winwidth = screen->Width / maxncols;
			winheight = screen->Height / 3;
		} else {
			maxncols = wincount / 2;
			winwidth = screen->Width / maxncols;
			winheight = screen->Height / 2;
		}
	}

	if ((wincount % 2 == 1 && wincount <= 7) && (wincount > 2)) {
		masterwindowwidth = screen->Width;
		masterwindowheight = screen->Height / 2;
		masterwindowy = screen->Height / 2;
		masterwindowx = screen->Height / 2;
		slavewindowwidth = screen->Width / (wincount - 1);
		slavewindowheight = screen->Height / 2;
		slavewindowy = 0;
		slavewindowx = 0;
		printf("Number is odd\n");
	}

	for (wnr = 0, window = screen->FirstWindow; window;
	     window = window->NextWindow, wnr++) {
		if (strcmp("Workbench", window->Title) == 0) {
			wnr--;
			continue;
		}
		//printf("Masterwindow status: %d\n", masterwindowon);
		printf("Masterwindow width: %d\n", masterwindowwidth);

		if (masterwindowwidth) {
			if (wnr < (wincount - 1)) {
				BeginRefresh(window);
				ChangeWindowBox(window, slavewindowx,
						slavewindowy,
						slavewindowwidth +
						((screen->WBorRight -
						  1) * wincount) -
						((screen->WBorLeft -
						  1) * wincount),
						slavewindowheight);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
				slavewindowx += slavewindowwidth;
				//masterwindowon = 1;
				printf("Slave window part\n");
			} else {
				printf("Inside masterwindow if: %d\n",
				       masterwindowy);
				BeginRefresh(window);
				ChangeWindowBox(window, masterwindowx,
						masterwindowy,
						masterwindowwidth +
						((screen->WBorRight -
						  1) * wincount) -
						((screen->WBorLeft -
						  1) * wincount),
						masterwindowheight);
				EndRefresh(window, TRUE);
				RefreshWindowFrame(window);
				masterwindowwidth = 0;
				masterwindowheight = 0;
				printf("Master window part\n");
			}
		}
		if ((wincount >= 2 && wincount % 2 == 0) || (wincount == 1) || (wincount >= 8)) {
			printf("colcount %d\n maxncols %d\n", colcount,
			       maxncols);
			if (colcount == maxncols && wincount > 2) {
				winy += winheight;
				colcount = 0;
				winx = 0;
				printf("winy: %d\n", winy);
			}
			colcount++;
			BeginRefresh(window);
			ChangeWindowBox(window, winx, winy,
					winwidth +
					((screen->WBorRight - 1) * wincount) -
					((screen->WBorLeft - 1) * wincount),
					winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			winx += winwidth;
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
