// Copyright 2025 Rasmus Edgar
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/icon.h>
#include <proto/commodities.h>
#include <proto/dos.h>
#include <proto/exec.h>
#include <exec/types.h>
#include <exec/lists.h>
#include <clib/alib_protos.h>
#include <stdio.h>
#include "../include/shared_defs.h"
#ifdef FORTIFY
#include "../fortify.h"
#endif

typedef union {
	int i;
	unsigned int u;
	short s;
	const void *v;
} Arg;

enum ws_num {
	WS_0,
	WS_1,
	WS_2,
	WS_3,
	WS_4,
	WS_5,
	WS_WB = 80
};

// commodity headers
short int commo(void);

// dintwm main functions shared
short tile(const Arg * arg);
short hgrid(const Arg * arg);
short fibonacci(const Arg * arg);
short spiral(const Arg * arg);
short dwindle(const Arg * arg);
short switcher(const Arg * arg);
short changegaps(const Arg * arg);
int countwindows(struct Screen const *scr);
void getactive(void);
int cstring_cmp(const void *a, const void *b);
short docmd(const Arg * arg);
short exit_cxm(const Arg * arg);
size_t strnlen(const char *s, size_t maxlen);
short changews(const Arg * arg);
short movetows(const Arg * arg);
short init_wbar(void);
short update_wbar(void);
void wbarcwb(void);
short info_window(const char *info_text);
short tileoff(const Arg * arg);
short tabnextwin(const Arg * arg);
short skipper(struct Window *w);

struct Popkeys {
	char *rawcombo;
};

typedef struct {
	const char *optname;
	long cxint;
	int tt_type;
} Opts;

typedef struct {
	const char *optname;
	char *defaultval;
	int tt_type;
	short (*func)(const Arg * arg);
	const Arg arg;
} Keys;

typedef struct {
	unsigned char *strings[CMD_MAX];
} Ostore;

extern Ostore cmds[];
extern Ostore cons[];
extern Ostore incls[];
extern Ostore excls[];

extern Keys defkeys[];
extern Opts defopts[];

enum bar_texts {
	// text items
	ws_zero,
	ws_one,
	ws_two,
	ws_three,
	ws_four,
	ws_five,
	ws_wb,
	mode_tile,
	mode_grid,
	mode_dwindle,
	mode_spiral,
	sep_one,
	sep_two,
	space,
	err,
	BAR_LAST_TEXT
};

enum bar_colors {
	bg,
	fp_ws,
	bp_ws,
	fp_cur,
	bp_cur,
	fp_tm,
	bp_tm,
	fp_ti,
	bp_ti,
	fp_sep_one,
	bp_sep_one,
	fp_sep_two,
	bp_sep_two,
	BAR_LAST_COLOR
};

unsigned char awintitle[TT_MAX_LENGTH];

typedef struct {
	unsigned char *text;
} Bar_Text;

typedef struct {
	unsigned char color[2];
} Bar_Color;

Bar_Text *bar_text;
Bar_Color bar_color[BAR_LAST_COLOR];

// bar
short assign_bar_item(Bar_Text *b, enum bar_texts x, const char *c);

typedef struct {
	short wbwin;
	enum ws_num workspace;
	short skip;
	struct Window *wptr;
} Winfo;

Winfo *winfo;

// Dintwm run functions
int dintwmrun(int argc, char **argv);
void initdefaults(void);
void cleanup_dintwm(void);
void free_opts(void);

// Window functions
int window_alloc_lut(void);
void window_free_lut(void);
void window_set_wptr(struct Window *w);
void window_set_wbwin(struct Window *w, short toggle);
short window_get_wbwin(struct Window *w);
void window_set_ws_num(struct Window *w, int ws);
int window_current_ws(int action, int ws);
struct Window * window_active(int action, struct Window *w);
struct Window * window_wbar(struct Window *w);
void cwb(struct Window *w, int wx, int wy, int ww, int wh);
int win_start(int action, const struct Screen *scr);

// Tiling functions
int tiling_layout(int action, int layout);
struct Screen * tiling_lock(int action, struct Screen *s);
struct Screen * tiling_screen_light(void);
int tiling_screen_height(void);
int tiling_screen_info(int action, int size);
int tiling_calc_menugap(void);
int tiling_gaps(int action, int amount);

// Options functions
short option_bool(int action, short b);
unsigned long option_ul(int action, unsigned long amount);
int option(int action, int amount);
short apply_options(Opts const *dopts, const char *tt_optvalue, size_t i);
short init_misc_opts(void);
void free_misc_opts(void);
