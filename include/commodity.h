// Copyright 2021 Rasmus Edgar
#include <devices/timer.h>

#define BDWARN 0
#define UQWARN 1
#define WIWARN 2

const char *warn_messages[] = {
	"Virtual WSs are incompatible with backdropped WB. Unbackdrop WB or remove TT VWS_ON. Quitting.",
	"Commodity is already running. Quitting.",
	"Dintwm only supports up to 1023 windows. Dintwm applauds the effort!"
};

struct Cmo {
	struct MsgPort *mp;
	struct Library *iconbase;
	struct DiskObject *diskobj;
	CxObj *broker;
	long mainsignum;
	long subsignum;
	int failarr[8];
};

static void init_cmo(struct Cmo *cmo);

const char *cmo_fail_msgs[] = {
	"Message port allocation failed. Exiting.",
	"Iconbase allocation failed. Exiting.",
	"Diskobject allocatoin failed. Exiting.",
	"Broker allocation failed. Exitng.",
	"Main signal allocation failed. Exiting.",
	"Sub signal allocation failed. Exiting.",
	"Activating CxObj failed. Exiting.",
	"Attaching tooltypes failed. Exiting."
};

static short attachtooltypes(CxObj * broker, struct MsgPort *port, struct DiskObject *diskobj);
//static short alloc_opts(const char *tt_optvalue, Ostore * s, size_t i, int subtract);
static void free_opts(void);
//static short assign_bar_item(Bar_Text * b, enum bar_texts x, const char *c);
static void cleanup(struct Cmo *cmo);
//static short apply_options(Opts const *dopts, const char *tt_optvalue, size_t i);

static void subactionchk(void);
static int winnum_start;

static unsigned long mainsig, wakeupsigs, subsig;
static struct Task *maintask = NULL, *subtask = NULL;
static struct Window *awin_comp;
static short running = TRUE;

Ostore cmds[] = { 0 };
Ostore cons[] = { 0 };
Ostore incls[] = { 0 };
Ostore excls[] = { 0 };

// timer stuff
static void delete_timer(struct timerequest *tr);
static struct timerequest *create_timer(unsigned long unit);
static void time_delay(struct timerequest *tr, const struct timeval *tv);
