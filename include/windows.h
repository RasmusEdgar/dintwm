// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

#define WS_SET 0
#define WS_GET 1

typedef struct {
        struct Window *wptr;
        enum ws_num workspace;
        short wbwin;
        short skip;
} Wlut;

static Wlut *wlut;
