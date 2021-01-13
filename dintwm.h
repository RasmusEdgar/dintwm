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
#include <clib/alib_protos.h>
#include <stdio.h>
#include <devices/timer.h>

#define MIN(A, B)               ((A) < (B) ? (A) : (B))
#define DEFCON "CON:0/40/640/150/dintwm/AUTO/CLOSE/WAIT"
#define DEFCMD "NewShell"

enum dintwm_identifiers {
	DEFAULT_TOPGAP = 0,
	KEYTYPE = 1,
	OPTTYPE = 2,
	STORE = 1,
	NOSKIP = 0,
	SKIP = 1,
	RESTORE = 2,
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
	K_CGAP_ID = 301, // longopts ketopts id
	TOPGAP_ID = 400, // topgap type identifier
	DEFAULT_TOPGAP_ID = 401, // default topgap identifier
	BOTTOMGAP_ID = 402, // bottomgap type identifier
	LEFTGAP_ID = 403, // leftgap type iNewshell command dentifier
	RIGHTGAP_ID = 404, // rightgap type identifier
	WTYPE_MAX = 9, // Number excluded/included window titles
	EXCL_WTYPE_ID_0 = 600, // exclude window type identifier
	EXCL_WTYPE_ID_1 = 601, // exclude window type identifier
	EXCL_WTYPE_ID_2 = 602, // exclude window type identifier
	EXCL_WTYPE_ID_3 = 603, // exclude window type identifier
	EXCL_WTYPE_ID_4 = 604, // exclude window type identifier
	EXCL_WTYPE_ID_5 = 605, // exclude window type identifier
	EXCL_WTYPE_ID_6 = 606, // exclude window type identifier
	EXCL_WTYPE_ID_7 = 607, // exclude window type identifier
	EXCL_WTYPE_ID_8 = 608, // exclude window type identifier
	EXCL_WTYPE_ID_9 = 609, // exclude window type identifier
	INCL_WTYPE_ID_0 = 700, // include window type identifier
	INCL_WTYPE_ID_1 = 701, // include window type identifier
	INCL_WTYPE_ID_2 = 702, // include window type identifier
	INCL_WTYPE_ID_3 = 703, // include window type identifier
	INCL_WTYPE_ID_4 = 704, // include window type identifier
	INCL_WTYPE_ID_5 = 705, // include window type identifier
	INCL_WTYPE_ID_6 = 706, // include window type identifier
	INCL_WTYPE_ID_7 = 707, // include window type identifier
	INCL_WTYPE_ID_8 = 708, // include window type identifier
	INCL_WTYPE_ID_9 = 709, // include window type identifier
	CMD_MAX = 9, // Number of custom cmds
	CONLINE_ID_0 = 800, // conline identifier
	CONLINE_ID_1 = 801, // conline identifier
	CONLINE_ID_2 = 802, // conline identifier
	CONLINE_ID_3 = 803, // conline identifier
	CONLINE_ID_4 = 804, // conline identifier
	CONLINE_ID_5 = 805, // conline identifier
	CONLINE_ID_6 = 806, // conline identifier
	CONLINE_ID_7 = 807, // conline identifier
	CONLINE_ID_8 = 808, // conline identifier
	CONLINE_ID_9 = 809, // conline identifier
	CMD_ID_0 = 900, // cmd spawn identifier
	CMD_ID_1 = 901, // cmd spawn identifier
	CMD_ID_2 = 902, // cmd spawn identifier
	CMD_ID_3 = 903, // cmd spawn identifier
	CMD_ID_4 = 904, // cmd spawn identifier
	CMD_ID_5 = 905, // cmd spawn identifier
	CMD_ID_6 = 906, // cmd spawn identifier
	CMD_ID_7 = 907, // cmd spawn identifier
	CMD_ID_8 = 908, // cmd spawn identifier
	CMD_ID_9 = 909, // cmd spawn identifier
	AUTO_ID = 1000, // AUTO TILE ID
	AUTO_INTERVAL_MICRO_ID = 1001, // AUTO TILE MICRO ID
	AUTO_INTERVAL_MICRO_DEF = 15000, // AUTO TILE INTERVAL
	TILE_FACT_DEF = 550, // Default mfact value
	TILE_FACT_ID = 1200 // mfact opt id
};

typedef union {
	int i;
	const void *v;
} Arg;

// dintwm main functions shared
short tile(const Arg *arg);
short hgrid(const Arg *arg);
short fibonacci(const Arg *arg);
short spiral(const Arg *arg);
short dwindle(const Arg *arg);
short restore(const Arg *arg);
short switcher(const Arg *arg);
short takesnapshot(const Arg *arg);
short cleansnapshot(const Arg *arg);
int countwindows(int l);
int cstring_cmp(const void *a, const void *b);
short docmd(const Arg *arg);
int calcgap(void);
short exit_cxm(const Arg *arg);
size_t strnlen(const char *s, size_t maxlen);
int fact;

int topgap;
int bottomgap;
int leftgap;
int rightgap;
long int *current_layout;
int exclude_wtype;
int include_wtype;
long unsigned int auto_interval;

// timer stuff
void delete_timer(struct timerequest *);
struct timerequest *create_timer(ULONG);
void wait_for_timer(struct timerequest *, struct timeval *);
LONG time_delay(struct timeval *, ULONG);


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

/*	unsigned char *con_strings[CMD_MAX];
	unsigned char *excl_strings[WTYPE_MAX];
	unsigned char *incl_strings[WTYPE_MAX];*/

extern Ostore cmds[];
extern Ostore cons[];
extern Ostore incls[];
extern Ostore excls[];

extern Keys defkeys[];
extern Opts defopts[];
