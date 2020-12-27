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

enum dintwm_identifiers {
	DEFAULT_TOPGAP = 0,
	KEYTYPE = 1,
	OPTTYPE = 2,
	STORE = 1,
	RESTORE = 2,
	FREE = 3,
	COMMODITIZE = -1,
	DOUBLE_OPTION_ERR = -2, // Can not call two tile functions from cli
	GAP_ERR = -3, // Gaps are too small or too big
	UNKNOWN = -4, // Unknown option
	MISSING = -5, // Missing argument
	LAYOUT_START = -1, // switcher function - determines if current_layot should be set 
	TILE_FUNC_LIMIT = 3, // switcher function - maximum limit of tiling layouts to switch through
	FUNC_TILE = 0, // tile function identifier
	FUNC_HGRID = 1, // hgrid function identifier
	FUNC_SPIRAL = 2, // spiral function identifier
	FUNC_DWINDLE = 3, // dwindle function identifier
	FUNC_PRINTUSAGE = 42, // printusage function identifier
	TT_MAX_LENGTH = 128, // Tooltype Max Length
	K_CGAP_ID = 301, // longopts ketopts id
	TOPGAP_ID = 700, // topgap type identifier
	DEFAULT_TOPGAP_ID = 701, // default topgap identifier
	BOTTOMGAP_ID = 702, // bottomgap type identifier
	LEFTGAP_ID = 703, // leftgap type iNewshell command dentifier
	RIGHTGAP_ID = 704, // rightgap type identifier
	EXCL_WTYPE_ID = 705, // exclude window type identifier
	INCL_WTYPE_ID = 706, // include window type identifier
	CMD_MAX = 9, // Number of custom cmds
	CONLINE_0_ID = 800, // conline identifier
	CONLINE_1_ID = 801, // conline identifier
	CONLINE_2_ID = 802, // conline identifier
	CONLINE_3_ID = 803, // conline identifier
	CONLINE_4_ID = 804, // conline identifier
	CONLINE_5_ID = 805, // conline identifier
	CONLINE_6_ID = 806, // conline identifier
	CONLINE_7_ID = 807, // conline identifier
	CONLINE_8_ID = 808, // conline identifier
	CONLINE_9_ID = 809, // conline identifier
	CMD_0_ID = 900, // cmd spawn identifier
	CMD_1_ID = 901, // cmd spawn identifier
	CMD_2_ID = 902, // cmd spawn identifier
	CMD_3_ID = 903, // cmd spawn identifier
	CMD_4_ID = 904, // cmd spawn identifier
	CMD_5_ID = 905, // cmd spawn identifier
	CMD_6_ID = 906, // cmd spawn identifier
	CMD_7_ID = 907, // cmd spawn identifier
	CMD_8_ID = 908, // cmd spawn identifier
	CMD_9_ID = 909, // cmd spawn identifier
	AUTO_ID = 1000 // AUTO TILE
};

typedef union {
	int i;
	const void *v;
} Arg;

// dintwm main functions shared
void tile(const Arg *arg);
void hgrid(const Arg *arg);
void fibonacci(const Arg *arg);
void spiral(const Arg *arg);
void dwindle(const Arg *arg);
void restore(const Arg *arg);
void switcher(const Arg *arg);
void takesnapshot(const Arg *arg);
void cleansnapshot(const Arg *arg);
void printusage(void);
int countwindows(int l);
void docmd(const Arg *arg);
int calcgap(void);
int topgap;
int bottomgap;
int leftgap;
int rightgap;
long int *current_layout;
char exclude_wtype[TT_MAX_LENGTH];
char include_wtype[TT_MAX_LENGTH];

// timer stuff
void delete_timer (struct timerequest *);
struct timerequest *create_timer( ULONG );
void wait_for_timer(struct timerequest *, struct timeval *);
LONG time_delay    ( struct timeval *, ULONG );

// Screen shared struct
struct Screen *screen;

// commodity headers
short int commo(void);

struct Popkeys {
        char *rawcombo;
};

typedef struct {
	char *optname;
        long cxint;
	char *defaultval;
	int tt_type;
} Opts;


typedef struct {
	char *optname;
	char *defaultval;
	int tt_type;
	void (*func)(const Arg *);
	const Arg arg;
} Keys;

typedef struct {
	unsigned char *cmd_strings[CMD_MAX];
	unsigned char *con_strings[CMD_MAX];
} Cmdstore;

extern Cmdstore cmds[];
extern Keys defkeys[];
extern Opts defopts[];
