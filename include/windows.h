// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

//static int tiling_screen_width(void);
static int window_index(struct Window *w);

#define WS_SET 0
#define WS_GET 1

typedef struct {
        struct Window *wptr;
        enum ws_num workspace;
        short wbwin;
        short skip;
} Wlut;

static Wlut *wlut;
