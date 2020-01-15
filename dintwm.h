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
//#include <workbench/startup.h>

#define MIN(A, B)               ((A) < (B) ? (A) : (B))

// Commodity info

#define COMMODITY_NAME "DintWM commodity"
#define COMMODITY_TITLE "Sets up hotkeys for DintWM"
#define COMMODITY_DESC "To change hotkeys edit source"
#define KEY_TILE "rawkey control lshift t"
#define KEY_HGRID "rawkey control lshift g"
#define KEY_SPIRAL "rawkey control lshift f"
#define KEY_DWINDLE "rawkey control lshift d"
//#define KEY_ARRAY_SIZE 4
#define DEFAULT_TOPGAP 0
#define KEYTYPE 1
#define OPTTYPE 2


// dintwm main functions
void tile(void);
void hgrid(void);
void spiral(void);
void dwindle(void);
int topgap;
char * exclude_wtype;

// commodity headers
int commo(void);

struct Optdef {
	const char* optname;
        unsigned long cxint;
	const char* defaultval;
	int tt_type;
};

struct Popkeys {
        char * restrict rawcombo;
};

struct Keyfuncdef {
        void (*func)(void);
};
	
struct Popkeys* keys;
struct Opts* opts;

