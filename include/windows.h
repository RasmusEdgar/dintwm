// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

#define WS_SET 0
#define WS_GET 1

typedef struct {
        short wbwin;
        enum ws_num workspace;
        short skip;
        struct Window *wptr;
} Wlut;

static Wlut *wlut;
