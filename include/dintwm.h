// Copyright 2021 Rasmus Edgar
#include <dos/dostags.h>
#include <getopt.h>
#ifdef FORTIFY
#include "../fortify.h"
#endif

//VERCUT
#define DINTWM_VERSION "VERGIT"
//VERCUT

static short printusage(void);
static short skipper(struct Window *w);
static void moveallwin(int m);
static int dintwmrun(int argc, char **argv);
static void initdefaults(void);
static struct Window *findfirstwin(struct Screen const *scr);

// Global tiling variables declaration
int exclude_wtype;
int include_wtype;
