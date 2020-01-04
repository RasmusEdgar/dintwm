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
#define KEY_TILE "rawkey control lshift t"
#define KEY_HGRID "rawkey control lshift g"
#define KEY_SPIRAL "rawkey control lshift f"
#define KEY_DWINDLE "rawkey control lshift d"

// Commodity info

#define COMMODITY_NAME "DintWM commodity"
#define COMMODITY_TITLE "Sets up hotkeys for DintWM"
#define COMMODITY_DESC "To change hotkeys edit source"

// dintwm main functions
void tile(void);
void hgrid(void);
void fibonacci(int);
void spiral(void);
void dwindle(void);
void printusage(int, int);
void lockbasescreen(unsigned long *ilock, struct Screen **screen);
void unlockbasescreen(unsigned long *ilock, struct Screen **screen);
int skipper(struct Window *window);

// commodity headers
int commo(unsigned char **ttypes);
//struct Library      *WorkbenchBase;
