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
static unsigned long ilock;
static short printusage(void);
static short skipper(struct Window *w);
static void moveallwin(int m);
static void lockbasescreen(unsigned long *il, struct Screen **s);
static void unlockbasescreen(unsigned long *il, struct Screen **s);
static int dintwmrun(int argc, char **argv);
static void initdefaults(void);
static struct Window *findfirstwin(void);

// Screen
struct Screen *screen;
int sheight;
int swidth;

// Window
static struct Window *window;

// Bar declarations
static inline void mapws(void);
static inline unsigned char *maptm(void);
static unsigned char *padwbartext(Bar_Text * b, enum bar_texts x);
static short int wbartextwidth(int lei, unsigned char *it);
struct Window *wbw = NULL;

// Global tiling variables declaration
int topgap;
int bottomgap;
int leftgap;
int rightgap;
int exclude_wtype;
int include_wtype;
short backdropped;
