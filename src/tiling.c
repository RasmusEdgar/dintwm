// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

short tile(const Arg *arg)
{
	(void)arg;
	static const int nmaster = 1;
	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int fact = option(TILE_FACT_GET, 0);

	int wincount = 0, wnr = 0, mwinwidth = 0, nwiny = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	if (wincount == 0) {
		(void)tiling_lock(TUNLOCK, scr);
		return TRUE;
	}

	if (wincount > nmaster) {
		mwinwidth = (sw * fact) / 1000;
	} else {
		mwinwidth = sw;
	}

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
			wnr--;
			continue;
		}
		if (wnr < nmaster) {
			wx = lgap;
			wy = tgap;
			ww = mwinwidth;
			wh = sh / (MIN(wincount, nmaster) - wnr);
		} else {
			wx = mwinwidth + lgap;
			wy = nwiny + tgap;
			ww = sw - mwinwidth;
			wh = (sh - nwiny) / (wincount - wnr);
			nwiny += wh;
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short hgrid(const Arg *arg)
{
	(void)arg;

	int wincount, wnr, ntop = 0, nbottom = 0;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	if (wincount == 0) {
		(void)tiling_lock(TUNLOCK, scr);
		return TRUE;
	}

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
			wnr--;
			continue;
		}
		if (wincount <= 1) {
			wx = lgap;
			wy = tgap;
			ww = sw / wincount;
			wh = sh;
		} else {
			ntop = wincount / 2;
			nbottom = wincount - ntop;
			if (wnr < ntop) {
				wx = wnr * (sw / ntop) + lgap;
				wy = tgap;
				ww = sw / ntop;
				wh = sh / 2;

			} else {
				wx = (wnr - ntop) * (sw / nbottom) + lgap;
				wy = (sh / 2) + tgap;
				ww = sw / nbottom;
				wh = sh / 2;
			}
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short fibonacci(const Arg *arg)
{
	int wnr, wincount;
	int wx = 0, wy = 0, ww = 0, wh = 0, sh = 0, sw = 0;

	int tgap = tiling_gaps(TOPGAP_GET, 0);
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int fact = option(TILE_FACT_GET, 0);

	struct Window *window = NULL;
	struct Screen *scr = tiling_lock(TLOCK, NULL);

	sh = scr->Height - (bgap + tgap);
	sw = scr->Width - (rgap + lgap);

	wincount = countwindows(scr);

	wy = tgap;
	wx = lgap;
	ww = sw;
	wh = sh;

	for (wnr = 0, window = scr->FirstWindow; window != NULL; window = window->NextWindow, wnr++) {
		if (skipper(window) == SKIP) {
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
		if (wnr < wincount && wnr == 0) {
			if (wincount != 1) {
				ww = (sw * fact) / 1000;
				wy = tgap;
			}
		} else if (wnr == 1) {
			ww = sw - ww;
		}
		cwb(window, wx, wy, ww, wh);
	}
	(void)tiling_lock(TUNLOCK, scr);

	return TRUE;
}

short switcher(const Arg *arg)
{
	short rc;

	if (arg->i != 0) {
		int t_layout = tiling_layout(TL_INC, 0);
		rc = defkeys[t_layout].func(&defkeys[t_layout].arg);
	} else {
		int t_layout = tiling_layout(TL_DEC, 0);
		rc = defkeys[t_layout].func(&defkeys[t_layout].arg);
	}

	return rc;
}
