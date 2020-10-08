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
//#define CALCGAP			(screen->BarHeight + screen->WBorBottom - 3)

#define TT_MAX_LENGTH 128

// dintwm main functions shared
void tile(void);
void hgrid(void);
void spiral(void);
void dwindle(void);
void restore(void);
void switchf(void);
void switchb(void);
void takesnapshot(void);
void cleansnapshot(void);
void printusage(void);
int countwindows(int l);
void doshell(void);
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

struct Optdef {
	char *optname;
        long cxint;
	char *defaultval;
	int tt_type;
};

struct Popkeys {
        char *rawcombo;
};

struct Keyfuncdef {
        void (*func)(void);
};
	
struct Opts* opts;

// tile function headers;
extern struct Keyfuncdef defkeyfuncs[]; 
enum dintwm_identifiers {
	COMMODITIZE = -1,
	DOUBLE_OPTION_ERR = -2, // Can not call two tile functions from cli
	GAP_ERR = -3, // Gaps are too small or too big
	LAYOUT_START = -1, // switcher function - determines if current_layot should be set 
	TILE_FUNC_LIMIT = 3, // switcher function - maximum limit of tiling layouts to switch through
	FUNC_TILE = 0, // tile function identifier
	FUNC_HGRID = 1, // hgrid function identifier
	FUNC_SPIRAL = 2, // spiral function identifier
	FUNC_DWINDLE = 3, // dwindle function identifier
	FUNC_RESTORE = 4, // restore function identifier
	FUNC_SWITCHF = 5, // switchf function identifier
	FUNC_SWITCHB = 6, // switchb function identifier
	FUNC_CLEANSNAPSHOT = 7, // Clean snapshot
	FUNC_TAKESNAPSHOT = 8, // Take snapshot
	FUNC_SHELL = 9, // Shell command identifier
	FUNC_PRINTUSAGE = 10, // printusage function identifier
	K_CGAP_ID = 301, // longopts ketopts id
	TOPGAP_ID = 700, // topgap type identifier
	DEFAULT_TOPGAP_ID = 701, // default topgap identifier
	BOTTOMGAP_ID = 702, // bottomgap type identifier
	LEFTGAP_ID = 703, // leftgap type iNewshell command dentifier
	RIGHTGAP_ID = 704, // rightgap type identifier
	EXCL_WTYPE_ID = 705, // exclude window type identifier
	INCL_WTYPE_ID = 706, // include window type identifier
	CONLINE_0_ID = 800, // include window type identifier
	SHELLCMD_0_ID = 900, // include window type identifier
	CONLINE_1_ID = 801, // include window type identifier
	SHELLCMD_1_ID = 901, // include window type identifier
	AUTO_ID = 1000 // AUTO TILE
};

typedef union {
	const void *v;
} Arg;
