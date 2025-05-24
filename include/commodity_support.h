// Copyright 2021 Rasmus Edgar
#include <devices/timer.h>

Ostore cmds[] = { 0 };
Ostore cons[] = { 0 };
Ostore incls[] = { 0 };
Ostore excls[] = { 0 };

static struct timerequest *create_timer(unsigned long unit);
static void time_delay(struct timerequest *tr, const struct timeval *tv);
static void delete_timer(struct timerequest *tr);
static short attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);
