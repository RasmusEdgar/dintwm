// Copyright 2021 Rasmus Edgar
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
#include <devices/timer.h>

#define MIN(A, B)               ((A) < (B) ? (A) : (B))

#define KEYTYPE_ID 1 // Keytype id
#define OPTTYPE_ID 2 // Opttype id
#define K_CGAP_ID 301 // longopts ketopts id
#define	TOPGAP_ID 400 // topgap type identifier
#define	DEFAULT_TOPGAP_ID 401 // default topgap identifier
#define BOTTOMGAP_ID 402 // bottomgap type identifier
#define	LEFTGAP_ID 403 // leftgap type iNewshell command dentifier
#define	RIGHTGAP_ID 404 // rightgap type identifier
#define INCALLGAPS_ID 405 // increase all gaps identifier
#define DECALLGAPS_ID 406 // decrease all gaps identifier
#define GAP_CHANGE_VALUE_ID 407 // decrease all gaps identifier
#define BAR_ID 408 // Bar identifier
#define BAR_BG_COL_ID 409 // Bar bg color identifier
#define BAR_FPW_COL_ID 410 // Bar current ws frontpen color identifier
#define BAR_BPW_COL_ID 411 // Bar current ws backpen color identifier
#define BAR_FPCURW_COL_ID 412 // Bar current ws frontpen color identifier
#define BAR_BPCURW_COL_ID 413 // Bar current ws backpen color identifier
#define BAR_FPTM_COL_ID 414 // Bar tilemode frontpen color identifier
#define BAR_BPTM_COL_ID 415 // Bar tilemode backpen color identifier
#define BAR_FPTI_COL_ID 416 // Bar title frontpen color identifier
#define BAR_BPTI_COL_ID 417 // Bar title backpen color identifier
#define BAR_FPSEP_ONE_COL_ID 418 // Bar seperator one frontpen color identifier
#define BAR_BPSEP_ONE_COL_ID 419 // Bar seperator one backpen color identifier
#define BAR_FPSEP_TWO_COL_ID 420 // Bar seperator two frontpen color identifier
#define BAR_BPSEP_TWO_COL_ID 421 // Bar seperator two backpen color identifier
#define BAR_HIDE_EMPTY_ID 422 // Hide bar on empty workspaces
#define BAR_TEXT_WS0_ID 423 // Bar text ws id
#define BAR_TEXT_WS1_ID 424 // Bar text ws id
#define BAR_TEXT_WS2_ID 425 // Bar text ws id
#define BAR_TEXT_WS3_ID 426 // Bar text ws id
#define BAR_TEXT_WS4_ID 427 // Bar text ws id
#define BAR_TEXT_WS5_ID 428 // Bar text ws id
#define BAR_TEXT_TILE_ID 429 // Bar text tile field id
#define BAR_TEXT_GRID_ID 430 // Bar text tile field id
#define BAR_TEXT_DWINDLE_ID 431 // Bar text tile field id
#define BAR_TEXT_SPIRAL_ID 432 // Bar text tile field id
#define BAR_TEXT_SEP_1_ID 433 // Bar text tile field id
#define BAR_TEXT_SEP_2_ID 434 // Bar text tile field id
#define BAR_TEXT_SPACE_ID 435 // Bar text tile field id
#define	EXCL_WTYPE_ID_0 600 // exclude window type identifier
#define	EXCL_WTYPE_ID_1 601 // exclude window type identifier
#define	EXCL_WTYPE_ID_2 602 // exclude window type identifier
#define	EXCL_WTYPE_ID_3 603 // exclude window type identifier
#define	EXCL_WTYPE_ID_4 604 // exclude window type identifier
#define	EXCL_WTYPE_ID_5 605 // exclude window type identifier
#define	EXCL_WTYPE_ID_6 606 // exclude window type identifier
#define	EXCL_WTYPE_ID_7 607 // exclude window type identifier
#define	EXCL_WTYPE_ID_8 608 // exclude window type identifier
#define	EXCL_WTYPE_ID_9 609 // exclude window type identifier
#define	INCL_WTYPE_ID_0 700 // include window type identifier
#define	INCL_WTYPE_ID_1 701 // include window type identifier
#define	INCL_WTYPE_ID_2 702 // include window type identifier
#define	INCL_WTYPE_ID_3 703 // include window type identifier
#define	INCL_WTYPE_ID_4 704 // include window type identifier
#define	INCL_WTYPE_ID_5 705 // include window type identifier
#define	INCL_WTYPE_ID_6 706 // include window type identifier
#define	INCL_WTYPE_ID_7 707 // include window type identifier
#define	INCL_WTYPE_ID_8 708 // include window type identifier
#define	INCL_WTYPE_ID_9 709 // include window type identifier
#define	CONLINE_ID_0 800 // conline identifier
#define	CONLINE_ID_1 801 // conline identifier
#define	CONLINE_ID_2 802 // conline identifier
#define	CONLINE_ID_3 803 // conline identifier
#define	CONLINE_ID_4 804 // conline identifier
#define	CONLINE_ID_5 805 // conline identifier
#define	CONLINE_ID_6 806 // conline identifier
#define	CONLINE_ID_7 807 // conline identifier
#define	CONLINE_ID_8 808 // conline identifier
#define	CONLINE_ID_9 809 // conline identifier
#define	CMD_ID_0 900 // cmd spawn identifier
#define	CMD_ID_1 901 // cmd spawn identifier
#define	CMD_ID_2 902 // cmd spawn identifier
#define	CMD_ID_3 903 // cmd spawn identifier
#define	CMD_ID_4 904 // cmd spawn identifier
#define	CMD_ID_5 905 // cmd spawn identifier
#define	CMD_ID_6 906 // cmd spawn identifier
#define	CMD_ID_7 907 // cmd spawn identifier
#define	CMD_ID_8 908 // cmd spawn identifier
#define	CMD_ID_9 909 // cmd spawn identifier
#define AUTO_ID 1000 // AUTO TILE ID
#define AUTO_INTERVAL_DELAY_ID 1001 // AUTO TILE MICRO ID
#define INFO_OFF_ID 1002 // info off identifier
#define VWS_ON_ID 1003 // turn on virtual workspaces
#define TILE_FACT_ID 1200 // mfact opt id

#define DEF_BAR_TEXT_WS_ZERO "W0"
#define DEF_BAR_TEXT_WS_ONE "W1"
#define DEF_BAR_TEXT_WS_TWO "W2"
#define DEF_BAR_TEXT_WS_THREE "W3"
#define DEF_BAR_TEXT_WS_FOUR "W4"
#define DEF_BAR_TEXT_WS_FIVE "W5"

#define DEF_BAR_TEXT_MODE_TILE "Tile"
#define DEF_BAR_TEXT_MODE_GRID "Grid"
#define DEF_BAR_TEXT_MODE_DWINDLE "FibD"
#define DEF_BAR_TEXT_MODE_SPIRAL "FibS"

#define DEF_BAR_TEXT_SEP_ONE ":"
#define DEF_BAR_TEXT_SEP_TWO ":"
#define DEF_BAR_TEXT_SPACE " "
#define DEF_BAR_TEXT_ERR "Fail"

#define DEFCON "CON:0/0/550/350/Dintwm Shell/AUTO/CLOSE/WAIT"
#define DEFCMD "NewShell"
#define COMMODITIZE -1 // Commoditize on/off
#define LAYOUT_START -1 // switcher function - determines if current_layot should be set
#define DOUBLE_OPTION_ERR -2 // Can not call two tile functions from cli
#define	GAP_ERR -3 // Gaps are too small or too big
#define	UNKNOWN -4 // Unknown option
#define	MISSING -5 // Missing argument
#define NOTSET -6 // Notset var
#define DEFAULT_TOPGAP 0 // Default topgap on/off
#define NOSKIP 0 // Noskip window on/off
#define FRONT 0 // Front on/off
#define	SKIP 1 // Skip window on/off
#define BACK 1 // Back on/off
#define AUTO_INTERVAL_DELAY_DEF 50000UL // default AUTO_TILE Delay INTERVAL in microseconds
#define	FUNC_TILE 0 // tile function identifier
#define	FUNC_HGRID 1 // hgrid function identifier
#define	FUNC_SPIRAL 2 // spiral function identifier
#define	FUNC_DWINDLE 3 // dwindle function identifier
#define	TILE_FUNC_LIMIT 3 // switcher function - maximum limit of tiling layouts to switch through
#define	BAR_COLOR_LENGTH 3 // Color Max Length
#define WTYPE_MAX 9 // Number excluded/included window titles
#define CMD_MAX 9 // Number of custom cmds
#define GAP_CHANGE_VALUE_DEF 20 // default gap increment/decrement value
#define	FUNC_PRINTUSAGE 42 // printusage function identifier
#define	PRINTVERSION 43 // printusage function identifier
#define TT_MAX_LENGTH 128 // Tooltype Max Length
#define GAP_INC_OFFSET 300 // substract from screen width or height using dynamic gaps
#define TILE_FACT_DEF 550 // Default mfact value
/*#define WS_0 (1U << 0)
#define WS_1 (1U << 1)
#define WS_2 (1U << 2)
#define WS_3 (1U << 3)
#define WS_4 (1U << 4)
#define WS_5 (1U << 5)
#define WBAR (1U << 6)
#define WBENCH (1U << 7)*/
#define WBAR_HEIGHT 20
#define BAR_HIDE_ON (1U << 0)
#define BAR_HIDE_TOGGLE (1U << 1)
#define DEF_BAR_FP_WS_COL 1U // Default bar workspace frontpen color
#define DEF_BAR_BP_WS_COL 3U // Default bar workspace backpen color
#define DEF_BAR_FP_CUR_COL 6U // Default bar active workspace frontpen color
#define DEF_BAR_BP_CUR_COL 3U // Default bar workspace backpen color
#define DEF_BAR_FP_TM_COL 1U // Default bar tilemode frontpen color
#define DEF_BAR_BP_TM_COL 3U // Default bar tilemode backpen color
#define DEF_BAR_FP_TI_COL 1U // Default bar title frontpen color
#define DEF_BAR_BP_TI_COL 3U // Default bar title backpen color
#define DEF_BAR_FP_SEP_ONE_COL 1U // Default bar title frontpen color
#define DEF_BAR_BP_SEP_ONE_COL 3U // Default bar title frontpen color
#define DEF_BAR_FP_SEP_TWO_COL 1U // Default bar title backpen color
#define DEF_BAR_BP_SEP_TWO_COL 3U // Default bar title backpen color
#define DEF_BAR_BG_COL 3U // Default bar bg color

#define WTSTRING_INIT_SIZE 256U // Allow 128 windows before realloc of wtstring

#define MOD1 "control"
#define MOD2 "lcommand"
#define MOD3 "shift"

#define NOLOCK 0
#define LOCK 1
#define NOASSIGN 0
#define ASSIGN 1
#define DIVISOR 1025

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
short tile(const Arg *arg);
short hgrid(const Arg *arg);
short fibonacci(const Arg *arg);
short spiral(const Arg *arg);
short dwindle(const Arg *arg);
short switcher(const Arg *arg);
short changegaps(const Arg *arg);
int countwindows(int lock, int assign_winfo);
void getactive(void);
int cstring_cmp(const void *a, const void *b);
short docmd(const Arg *arg);
int calcgap(void);
short exit_cxm(const Arg *arg);
size_t strnlen(const char *s, size_t maxlen);
short changews(const Arg *arg);
short movetows(const Arg *arg);
short init_wbar(void);
short update_wbar(void);
void wbarcwb(void);
short info_window(unsigned char * info_text);
short tileoff(const Arg *arg);
short tabnextwin(const Arg * arg);
int modululator(unsigned long w);

extern int topgap;
extern int bottomgap;
extern int leftgap;
extern int rightgap;
extern long int *current_layout;
extern int exclude_wtype;
extern int include_wtype;
extern long unsigned int auto_interval;
//extern unsigned int current_ws;
extern short backdropped;
extern short tile_off;
extern struct Window *wbw;
extern int sheight;
extern int swidth;
extern struct Screen *screen;
extern struct Window *active_win;
int awin_index;
extern unsigned char nil;

// Wbar specific vars
extern short bar_on;
extern short vws_on;
extern unsigned int hidewbar;
extern int wbarheight;

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
	short (*func)(const Arg *arg);
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

extern int fact;
extern int gap_change_value;

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

typedef struct {
	unsigned char * text;
} Bar_Text;

typedef struct {
	unsigned char color[2];
} Bar_Color;

extern Bar_Text bar_text[BAR_LAST_TEXT];
extern Bar_Color bar_color[BAR_LAST_COLOR];

// timer stuff
void delete_timer(struct timerequest *tr);
struct timerequest *create_timer(unsigned long unit);
void time_delay(struct timerequest *tr, const struct timeval *tv);

typedef struct {
	short wbwin;
        //unsigned int workspace;
        enum ws_num workspace;
        unsigned int skip;
        struct Window *wptr;
} Winfo;

Winfo *winfo;
