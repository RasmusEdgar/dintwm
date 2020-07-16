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
void fibonacci(int);
void tile(void);
void hgrid(void);
void spiral(void);
void dwindle(void);
void restore(void);
void switcher(int);
void switchf(void);
void switchb(void);
int topgap;
int *current_layout;
char exclude_wtype[TT_MAX_LENGTH];

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
enum tile_funcs {
	LAYOUT_START = -1,
	TILE_FUNC_LIMIT = 4
};
