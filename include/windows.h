// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

//static int tiling_screen_width(void);
static int window_index(struct Window *w);
static struct Window *findfirstwin(struct Screen const *scr);
static void moveallwin(int m);
static void window_set_skip(struct Window *w);
static short window_get_skip(struct Window *w);
static short window_check_wptr(struct Window *w);
static int window_get_ws_num(struct Window *w);

#define WS_SET 0
#define WS_GET 1

typedef struct {
        struct Window *wptr;
        enum ws_num workspace;
        short wbwin;
        short skip;
} Wlut;

static Wlut *wlut;
