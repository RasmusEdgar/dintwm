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
int topgap;
long int *current_layout;
char exclude_wtype[TT_MAX_LENGTH];
char include_wtype[TT_MAX_LENGTH];

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
	GAP_ERR = -3, // Can not call two tile functions from cli
	LAYOUT_START = -1, // switcher function - determines if current_layot should be set 
	TILE_FUNC_LIMIT = 3, // switcher funcetion - maximum limit of tiling layouts to switch through
	FUNC_TILE = 0, // tile function identifier
	FUNC_HGRID = 1, // hgrid function identifier
	FUNC_SPIRAL = 2, // spiral function identifier
	FUNC_DWINDLE = 3, // dwindle function identifier
	FUNC_RESTORE = 4, // restore function identifier
	FUNC_SWITCHF = 5, // switchf function identifier
	FUNC_SWITCHB = 6, // switchb function identifier
	FUNC_CLEANSNAPSHOT = 7,
	FUNC_TAKESNAPSHOT = 8,
	FUNC_PRINTUSAGE = 9, // printusage function identifier
	K_CGAP_ID = 301, // longopts ketopts id
	TOPGAP_ID = 900, // topgap type identifier
	DEFAULT_TOPGAP_ID = 901, // default topgap identifier
	EXCL_WTYPE_ID = 902, // exclude window type identifier
	INCL_WTYPE_ID = 903 // exclude window type identifier
};
