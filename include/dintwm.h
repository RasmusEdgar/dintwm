// Copyright 2021 Rasmus Edgar
#include <dos/dostags.h>
#include <getopt.h>
#include "../include/wbar_config.h"
#ifdef FORTIFY
#include "../fortify.h"
#endif

//VERCUT
#define DINTWM_VERSION "VERGIT"
//VERCUT

static void cwb(struct Window *w, int wx, int wy, int ww, int wh);
static short printusage(void);
static short skipper(struct Window *w);
static void moveallwin(int m);
static int dintwmrun(int argc, char **argv);
static void initdefaults(void);
static struct Window *findfirstwin(struct Screen const *scr);

// Bar declarations
static inline void mapws(void);
static inline unsigned char *maptm(void);
static unsigned char *padwbartext(Bar_Text * b, enum bar_texts x);
static short int wbartextwidth(int lei, unsigned char *it);

// Global tiling variables declaration
int exclude_wtype;
int include_wtype;
short backdropped;
