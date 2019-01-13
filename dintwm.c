#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/dos.h>

#define MAX(A, B)               ((A) > (B) ? (A) : (B))
#define MIN(A, B)               ((A) < (B) ? (A) : (B))

void tile(void);
void something(void);
void grid(void);
void lockbasescreen(unsigned long *ilock, struct Screen **screen);
void unlockbasescreen(unsigned long *ilock, struct Screen **screen);

struct Screen *screen;
unsigned long ilock;
struct Window *window;
static const int nmaster = 1;
static const int fact = 530;

int main(void)
{
	grid();

	return 0;
}

void tile(void)
{
	int wincount = 0, wnr = 0, mwinwidth = 0, winheight = 0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;
        lockbasescreen(&ilock, &screen);
        // count windows
        for (wincount = 0, window = screen->FirstWindow; window;
             window = window->NextWindow, wincount++) ;
        if (wincount == 0) {
                return;
        }
        // remove count for workbench window
        wincount = wincount - 1;
	//winwidth = screen->Width;
	//winheight = screen->Height;
	printf("Wincount: %d\n",wincount);
        if (wincount > nmaster)
                mwinwidth = nmaster ? (screen->Width * fact) / 1000 : 0;
        else
                mwinwidth = screen->Width;
        
        for (wnr = 0, window = screen->FirstWindow; window;
             window = window->NextWindow, wnr++) {
                if (strcmp("Workbench", window->Title) == 0) {
                        //wnr--;
                        continue;
                }
                if(wnr < nmaster) {
                        winheight = (screen->Height - mwiny) / (MIN(wincount, nmaster) - wnr);
			BeginRefresh(window);
			ChangeWindowBox(window, winx, winy + mwiny,
						mwinwidth +
						/*((screen->WBorRight - 1) * wincount) -
						((screen->WBorLeft - 1) * wincount), */
						((screen->WBorRight - 1) -
						(screen->WBorLeft - 1)),
						winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
       	        	mwiny += winheight;
                } else {
                        winheight = (screen->Height - nwiny) / (wincount - wnr);
			BeginRefresh(window);
			ChangeWindowBox(window, winx + mwinwidth, winy + nwiny,
						screen->Width - mwinwidth +
						/* ((screen->WBorRight - 1) * wincount) -
						((screen->WBorLeft - 1) * wincount), */
						((screen->WBorRight - 1) -
						(screen->WBorLeft - 1)),
						winheight);
			EndRefresh(window, TRUE);
			RefreshWindowFrame(window);
			nwiny += winheight;
		}
	}
        unlockbasescreen(&ilock, &screen);
	wincount = 0, wnr = 0, mwinwidth = 0, winheight = 0, winx = 0, winy = 0, nwiny = 0, mwiny = 0;
}

void grid(void)
{       
        unsigned int n, i;
        int w = 0;
        int ntop, nbottom, winx = 0, winy = 0;
	struct Window *c;

	lockbasescreen(&ilock, &screen);
        for (n = 0, window = screen->FirstWindow; window;
             window = window->NextWindow){
                if (strcmp("Workbench", window->Title) == 0) {
                        continue;
                }
		n++;
	}

        if(n == 0)
                return;
        else if(n == 1) { /* Just fill the whole screen */
		c = window->NextWindow;
                BeginRefresh(c);
                ChangeWindowBox(c, winx, winy,
                                        screen->Width +
                                        ((screen->WBorRight - 1) -
                                        (screen->WBorLeft - 1)),
                                        screen->Height);
                EndRefresh(c, TRUE);
                RefreshWindowFrame(c);
        } else if(n == 2) { /* Split vertically */
                w = screen->Width / 2;
                c = window->NextWindow;
                ChangeWindowBox(c, winx, winy,
                                        w +
                                        ((screen->WBorRight - 1) -
                                        (screen->WBorLeft - 1)),
                                        screen->Height);
                EndRefresh(c, TRUE);
                RefreshWindowFrame(c);
                c = window->NextWindow;
                BeginRefresh(c);
                ChangeWindowBox(c, winx + w, winy,
                                        w +
                                        ((screen->WBorRight - 1) -
                                        (screen->WBorLeft - 1)),
                                        screen->Height);
                EndRefresh(c, TRUE);
                RefreshWindowFrame(c);
        } else {
                ntop = n / 2;
                nbottom = n - ntop;
        	for (i = 0, c = screen->FirstWindow; c;
             		c = c->NextWindow, i++) {
                        if(i < ntop) {
                		BeginRefresh(c);
                		ChangeWindowBox(c, winx + i * screen->Width / ntop, winy,
                                		        screen->Width / ntop +
                                        		((screen->WBorRight - 1) -
                                        		(screen->WBorLeft - 1)), 
                                        		screen->Height / 2);
                		EndRefresh(c, TRUE);
               			RefreshWindowFrame(c);
			} else {
                		BeginRefresh(c);
                		ChangeWindowBox(c, winx + (i - ntop) * screen->Width / nbottom, winy + screen->Height / 2,
                                		        screen->Width / ntop +
                                        		((screen->WBorRight - 1) -
                                        		(screen->WBorLeft - 1)), 
                                        		screen->Height / 2);
                		EndRefresh(c, TRUE);
               			RefreshWindowFrame(c);
			}
		}
        }
        unlockbasescreen(&ilock, &screen);

}

void something(void)
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
