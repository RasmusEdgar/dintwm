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
int commo(void);
