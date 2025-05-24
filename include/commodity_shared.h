// Copyright 2025 Rasmus Edgar

#define BDWARN 0
#define UQWARN 1
#define WIWARN 0

struct Cmo {
	struct MsgPort *mp;
	struct Library *iconbase;
	struct DiskObject *diskobj;
	CxObj *broker;
	long mainsignum;
	long subsignum;
	int failarr[8];
};


unsigned long mainsig, wakeupsigs, subsig;
struct Task *maintask, *subtask;
struct Window *awin_comp;
short running;

void init_cmo(struct Cmo *cmo);
void subactionchk(void);
void cleanup_cxm(struct Cmo *cmo);
