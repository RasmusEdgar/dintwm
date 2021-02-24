// Copyright 2021 Rasmus Edgar
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define DEFCON "CON:0/40/640/150/dintwm/AUTO/CLOSE/WAIT"
#define DEFCMD "NewShell"
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
#define WTYPE_MAX 9 // Number excluded/included window titles
#define CMD_MAX 9 // Number of custom cmds
#define	TOPGAP_ID 400 // topgap type identifier
#define	DEFAULT_TOPGAP_ID 401 // default topgap identifier
#define BOTTOMGAP_ID 402 // bottomgap type identifier
#define	LEFTGAP_ID 403 // leftgap type iNewshell command dentifier
#define	RIGHTGAP_ID 404 // rightgap type identifier
#define INCALLGAPS_ID 405 // increase all gaps identifier
#define DECALLGAPS_ID 406 // decrease all gaps identifier
#define GAP_CHANGE_VALUE_ID 407 // decrease all gaps identifier
#define GAP_CHANGE_VALUE_DEF 20 // default gap increment/decrement value
#define GAP_INC_OFFSET 300 // substract from screen width or height using dynamic gaps
#define AUTO_INTERVAL_DELAY_DEF 1 // default AUTO_TILE Delay INTERVAL in ticks
#define WS_0 (1U << 0)
#define WS_1 (1U << 1)
#define WS_2 (1U << 2)
#define WS_3 (1U << 3)
#define WS_4 (1U << 4)
#define WS_5 (1U << 5)
#define WBAR (1U << 6)
#define WBAR_HEIGHT 20
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
#define DEF_BAR_BG_COL 3U // Default bar bg color
#define DEF_BAR_FPWS_COL 1U // Default bar workspace frontpen color
#define DEF_BAR_BPWS_COL 3U // Default bar workspace backpen color
#define DEF_BAR_FPCURW_COL 139U // Default bar active workspace frontpen color
#define DEF_BAR_BPCURW_COL 3U // Default bar workspace backpen color
#define DEF_BAR_FPTM_COL 1U // Default bar tilemode frontpen color
#define DEF_BAR_BPTM_COL 3U // Default bar tilemode backpen color
#define DEF_BAR_FPTI_COL 1U // Default bar title frontpen color
#define DEF_BAR_BPTI_COL 3U // Default bar title backpen color
#define DEF_BAR_FPSEP_ONE_COL 1U // Default bar title frontpen color
#define DEF_BAR_BPSEP_ONE_COL 3U // Default bar title frontpen color
#define DEF_BAR_FPSEP_TWO_COL 1U // Default bar title backpen color
#define DEF_BAR_BPSEP_TWO_COL 3U // Default bar title backpen color

enum dintwm_identifiers {
	DEFAULT_TOPGAP = 0,
	NOSKIP = 0,
	FRONT = 0,
	BACK = 1,
	KEYTYPE = 1,
	STORE = 1,
	SKIP = 1,
	OPTTYPE = 2,
	FREE = 3,
	COMMODITIZE = -1,
	DOUBLE_OPTION_ERR = -2, // Can not call two tile functions from cli
	GAP_ERR = -3, // Gaps are too small or too big
	UNKNOWN = -4, // Unknown option
	MISSING = -5, // Missing argument
	NOTSET = -6, // Notset var
	LAYOUT_START = -1, // switcher function - determines if current_layot should be set
	TILE_FUNC_LIMIT = 3, // switcher function - maximum limit of tiling layouts to switch through
	FUNC_TILE = 0, // tile function identifier
	FUNC_HGRID = 1, // hgrid function identifier
	FUNC_SPIRAL = 2, // spiral function identifier
	FUNC_DWINDLE = 3, // dwindle function identifier
	FUNC_PRINTUSAGE = 42, // printusage function identifier
	TT_MAX_LENGTH = 128, // Tooltype Max Length
	BAR_COLOR_LENGTH = 3, // Color Max Length
	K_CGAP_ID = 301, // longopts ketopts id
	AUTO_ID = 1000, // AUTO TILE ID
	AUTO_INTERVAL_DELAY_ID = 1001, // AUTO TILE MICRO ID
	TILE_FACT_DEF = 550, // Default mfact value
	TILE_FACT_ID = 1200 // mfact opt id
};

typedef union {
	int i;
	unsigned int u;
	short s;
	const void *v;
} Arg;

// dintwm main functions shared
short tile(const Arg *arg);
short hgrid(const Arg *arg);
short fibonacci(const Arg *arg);
short spiral(const Arg *arg);
short dwindle(const Arg *arg);
short switcher(const Arg *arg);
short changegaps(const Arg *arg);
int countwindows(int l);
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

int topgap;
int bottomgap;
int leftgap;
int rightgap;
long int *current_layout;
int exclude_wtype;
int include_wtype;
long unsigned int auto_interval;
unsigned int current_ws;
short backdropped;
struct Window *wbw;
int sheight;
int swidth;
struct Screen *screen;
struct Window *active_win;

// Wbar specific vars
short bar_on;
int wbarheight;
unsigned char wbarbgcolor[3];
unsigned char wbarfpws[3];
unsigned char wbarbpws[3];
unsigned char wbarfpwscur[3];
unsigned char wbarbpwscur[3];
unsigned char wbarfptm[3];
unsigned char wbarbptm[3];
unsigned char wbarfpti[3];
unsigned char wbarbpti[3];
unsigned char wbarfpsepone[3];
unsigned char wbarbpsepone[3];
unsigned char wbarfpseptwo[3];
unsigned char wbarbpseptwo[3];

// commodity headers
short int commo(void);

struct Popkeys {
        char *rawcombo;
};

typedef struct {
	char *optname;
        long cxint;
	int tt_type;
} Opts;


typedef struct {
	char *optname;
	char *defaultval;
	int tt_type;
	short (*func)(const Arg *);
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
