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
	CONLINE_0_ID = 800, // include window type identifier
	CMD_0_ID = 900, // include window type identifier
	CONLINE_1_ID = 801, // include window type identifier
	CMD_1_ID = 901, // include window type identifier
	CONLINE_2_ID = 802, // include window type identifier
	CMD_2_ID = 902, // include window type identifier
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
unsigned char conline[TT_MAX_LENGTH];
unsigned char shellcmd[TT_MAX_LENGTH];

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
	//long cxint;
	char *defaultval;
	int tt_type;
	void (*func)(const Arg *);
	const Arg arg;
} Keys;

extern Keys defkeys[];
extern Opts defopts[];
