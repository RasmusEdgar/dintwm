// Copyright 2024 Rasmus Edgar
#include "../include/windows.h"
#define DIVISOR 1025

int window_alloc_lut(void)
{
	size_t wlut_size = DIVISOR - 1;
        if ((wlut = (Wlut *) calloc(wlut_size, sizeof(Wlut))) == NULL) {
                return EXIT_FAILURE;
        }
	return 0;
}

void window_free_lut(void)
{
	free(wlut);
}

static int window_index(struct Window *w)
{
	unsigned long uw = (unsigned long)w;
        return (int)uw % DIVISOR;
}

void window_set_wptr(struct Window *w)
{
	int windex = window_index(w);
	if (wlut[windex].wptr == NULL) {
		wlut[windex].wptr = w;
	} else {
		exit(EXIT_FAILURE);
	}
}

short window_check_wptr(struct Window *w)
{
	int windex = window_index(w);
	if (wlut[windex].wptr == NULL) {
		return FALSE;
	}
	return TRUE;
}

void window_set_wbwin(struct Window *w, short toggle)
{
	wlut[window_index(w)].wbwin = toggle;
}

short window_get_wbwin(struct Window *w)
{
	if (wlut[window_index(w)].wbwin) {
		return TRUE;
	}
	return FALSE;
}

void window_set_ws_num(struct Window *w, int ws)
{
	wlut[window_index(w)].workspace = ws;
}

int window_get_ws_num(struct Window *w)
{
	return wlut[window_index(w)].workspace;
}

int window_current_ws(int action, int ws)
{
	static int current_ws = WS_0;

	if (action == WS_SET) {
		current_ws = ws;
	}
	return current_ws;
}

void window_set_skip(struct Window *w)
{
	wlut[window_index(w)].skip = TRUE;
}

short window_get_skip(struct Window *w)
{
	return wlut[window_index(w)].skip;
}

struct Window * window_active(int action, struct Window *w)
{
	static struct Window *active_window = 0;
	if (action == AW_SET) {
		active_window = w;
	}
	return wlut[window_index(active_window)].wptr;
}

struct Window * window_wbar(struct Window *w)
{
// TODO(blah): Fix
	static int windex = 0;

	if (windex == 0 && w != NULL) {
		window_set_wptr(w);
		windex = window_index(w);
	}

	return wlut[windex].wptr;
}
