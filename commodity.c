// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "./dintwm.h"
#include "./config.h"

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager for AmigaOS";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";

_Bool attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);
static short alloc_opts(char *tt_optvalue, Ostore *s, size_t i, int subtract);
static void free_opts(void);

static struct Library *iconbase;
int fact = TILE_FACT_DEF;
int gap_change_value = GAP_CHANGE_VALUE_DEF;

Keys defkeys[] = {
	{ TYPE_TILE, KEY_TILE, KEYTYPE, tile, {0} },
	{ TYPE_HGRID, KEY_HGRID, KEYTYPE, hgrid, {0} },
	{ TYPE_SPIRAL, KEY_SPIRAL, KEYTYPE, fibonacci, {.i = 0} },
	{ TYPE_DWINDLE, KEY_DWINDLE, KEYTYPE, fibonacci, {.i = 1} },
	{ TYPE_RESTORE, KEY_RESTORE, KEYTYPE, restore, {0} },
	{ TYPE_SWITCHF, KEY_SWITCHF, KEYTYPE, switcher, {.i = 1} },
	{ TYPE_SWITCHB, KEY_SWITCHB, KEYTYPE, switcher, {.i = 0} },
	{ TYPE_CLEANSNAPSHOT, KEY_CLEANSNAPSHOT, KEYTYPE, cleansnapshot, {0} },
	{ TYPE_TAKESNAPSHOT, KEY_TAKESNAPSHOT, KEYTYPE, takesnapshot, {0} },
	{ TYPE_INCTOPGAP, KEY_INCTOPGAP, KEYTYPE, changegaps, {.i = TOPGAP_ID} },
	{ TYPE_INCBOTTOMGAP, KEY_INCBOTTOMGAP, KEYTYPE, changegaps, {.i = BOTTOMGAP_ID} },
	{ TYPE_INCLEFTGAP, KEY_INCLEFTGAP, KEYTYPE, changegaps, {.i = LEFTGAP_ID} },
	{ TYPE_INCRIGHTGAP, KEY_INCRIGHTGAP, KEYTYPE, changegaps, {.i = RIGHTGAP_ID} },
	{ TYPE_DECTOPGAP, KEY_DECTOPGAP, KEYTYPE, changegaps, {.i = -TOPGAP_ID} },
	{ TYPE_DECBOTTOMGAP, KEY_DECBOTTOMGAP, KEYTYPE, changegaps, {.i = -BOTTOMGAP_ID} },
	{ TYPE_DECLEFTGAP, KEY_DECLEFTGAP, KEYTYPE, changegaps, {.i = -LEFTGAP_ID} },
	{ TYPE_DECRIGHTGAP, KEY_DECRIGHTGAP, KEYTYPE, changegaps, {.i = -RIGHTGAP_ID} },
	{ TYPE_INCALLGAPS, KEY_INCALLGAPS, KEYTYPE, changegaps, {.i = INCALLGAPS_ID} },
	{ TYPE_DECALLGAPS, KEY_DECALLGAPS, KEYTYPE, changegaps, {.i = DECALLGAPS_ID} },
	{ TYPE_KEY_CMD_0, KEY_CMD_0, KEYTYPE, docmd, { .i = CMD_ID_0 } },
	{ TYPE_KEY_CMD_1, KEY_CMD_1, KEYTYPE, docmd, { .i = CMD_ID_1 } },
	{ TYPE_KEY_CMD_2, KEY_CMD_2, KEYTYPE, docmd, { .i = CMD_ID_2 } },
	{ TYPE_KEY_CMD_3, KEY_CMD_3, KEYTYPE, docmd, { .i = CMD_ID_3 } },
	{ TYPE_KEY_CMD_4, KEY_CMD_4, KEYTYPE, docmd, { .i = CMD_ID_4 } },
	{ TYPE_KEY_CMD_5, KEY_CMD_5, KEYTYPE, docmd, { .i = CMD_ID_5 } },
	{ TYPE_KEY_CMD_6, KEY_CMD_6, KEYTYPE, docmd, { .i = CMD_ID_7 } },
	{ TYPE_KEY_CMD_7, KEY_CMD_7, KEYTYPE, docmd, { .i = CMD_ID_7 } },
	{ TYPE_KEY_CMD_8, KEY_CMD_8, KEYTYPE, docmd, { .i = CMD_ID_8 } },
	{ TYPE_KEY_CMD_9, KEY_CMD_9, KEYTYPE, docmd, { .i = CMD_ID_9 } },
	{ TYPE_KEY_WS_0, KEY_WS_0, KEYTYPE, changews, { .u = WS_0 } },
	{ TYPE_KEY_WS_1, KEY_WS_1, KEYTYPE, changews, { .u = WS_1 } },
	{ TYPE_KEY_WS_2, KEY_WS_2, KEYTYPE, changews, { .u = WS_2 } },
	{ TYPE_KEY_WS_3, KEY_WS_3, KEYTYPE, changews, { .u = WS_3 } },
	{ TYPE_KEY_WS_4, KEY_WS_4, KEYTYPE, changews, { .u = WS_4 } },
	{ TYPE_KEY_WS_5, KEY_WS_5, KEYTYPE, changews, { .u = WS_5 } },
	{ TYPE_KEY_CXM_EXIT, KEY_CXM_EXIT, KEYTYPE, exit_cxm, {0} },
};

Opts defopts[] = {
	{ TYPE_TOPGAP, TOPGAP_ID, OPTTYPE },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, OPTTYPE },
	{ TYPE_BOTTOMGAP, BOTTOMGAP_ID, OPTTYPE },
	{ TYPE_LEFTGAP, LEFTGAP_ID, OPTTYPE },
	{ TYPE_RIGHTGAP, RIGHTGAP_ID, OPTTYPE },
	{ TYPE_AUTO, AUTO_ID, OPTTYPE },
	{ TYPE_AUTO_INTERVAL_MICRO, AUTO_INTERVAL_MICRO_ID, OPTTYPE },
	{ TYPE_TILE_FACT, TILE_FACT_ID, OPTTYPE },
	{ TYPE_GAP_CHANGE_VALUE, GAP_CHANGE_VALUE_ID, OPTTYPE },
	{ TYPE_EXCL_WTYPE_0, EXCL_WTYPE_ID_0, OPTTYPE },
	{ TYPE_EXCL_WTYPE_1, EXCL_WTYPE_ID_1, OPTTYPE },
	{ TYPE_EXCL_WTYPE_2, EXCL_WTYPE_ID_2, OPTTYPE },
	{ TYPE_EXCL_WTYPE_3, EXCL_WTYPE_ID_3, OPTTYPE },
	{ TYPE_EXCL_WTYPE_4, EXCL_WTYPE_ID_4, OPTTYPE },
	{ TYPE_EXCL_WTYPE_5, EXCL_WTYPE_ID_5, OPTTYPE },
	{ TYPE_EXCL_WTYPE_6, EXCL_WTYPE_ID_6, OPTTYPE },
	{ TYPE_EXCL_WTYPE_7, EXCL_WTYPE_ID_7, OPTTYPE },
	{ TYPE_EXCL_WTYPE_8, EXCL_WTYPE_ID_8, OPTTYPE },
	{ TYPE_EXCL_WTYPE_9, EXCL_WTYPE_ID_9, OPTTYPE },
	{ TYPE_INCL_WTYPE_0, INCL_WTYPE_ID_0, OPTTYPE },
	{ TYPE_INCL_WTYPE_1, INCL_WTYPE_ID_1, OPTTYPE },
	{ TYPE_INCL_WTYPE_2, INCL_WTYPE_ID_2, OPTTYPE },
	{ TYPE_INCL_WTYPE_3, INCL_WTYPE_ID_3, OPTTYPE },
	{ TYPE_INCL_WTYPE_4, INCL_WTYPE_ID_4, OPTTYPE },
	{ TYPE_INCL_WTYPE_5, INCL_WTYPE_ID_5, OPTTYPE },
	{ TYPE_INCL_WTYPE_6, INCL_WTYPE_ID_6, OPTTYPE },
	{ TYPE_INCL_WTYPE_7, INCL_WTYPE_ID_7, OPTTYPE },
	{ TYPE_INCL_WTYPE_8, INCL_WTYPE_ID_8, OPTTYPE },
	{ TYPE_INCL_WTYPE_9, INCL_WTYPE_ID_9, OPTTYPE },
	{ TYPE_CONLINE_0, CONLINE_ID_0, OPTTYPE },
	{ TYPE_CONLINE_1, CONLINE_ID_1, OPTTYPE },
	{ TYPE_CONLINE_2, CONLINE_ID_2, OPTTYPE },
	{ TYPE_CONLINE_3, CONLINE_ID_3, OPTTYPE },
	{ TYPE_CONLINE_4, CONLINE_ID_4, OPTTYPE },
	{ TYPE_CONLINE_5, CONLINE_ID_5, OPTTYPE },
	{ TYPE_CONLINE_6, CONLINE_ID_6, OPTTYPE },
	{ TYPE_CONLINE_7, CONLINE_ID_7, OPTTYPE },
	{ TYPE_CONLINE_8, CONLINE_ID_8, OPTTYPE },
	{ TYPE_CONLINE_9, CONLINE_ID_9, OPTTYPE },
	{ TYPE_CMD_0, CMD_ID_0, OPTTYPE },
	{ TYPE_CMD_1, CMD_ID_1, OPTTYPE },
	{ TYPE_CMD_2, CMD_ID_2, OPTTYPE },
	{ TYPE_CMD_3, CMD_ID_3, OPTTYPE },
	{ TYPE_CMD_4, CMD_ID_4, OPTTYPE },
	{ TYPE_CMD_5, CMD_ID_5, OPTTYPE },
	{ TYPE_CMD_6, CMD_ID_6, OPTTYPE },
	{ TYPE_CMD_7, CMD_ID_7, OPTTYPE },
	{ TYPE_CMD_8, CMD_ID_8, OPTTYPE },
	{ TYPE_CMD_9, CMD_ID_9, OPTTYPE },
};

Ostore cmds[] = {0};
Ostore cons[] = {0};
Ostore incls[] = {0};
Ostore excls[] = {0};

static struct NewBroker MyBroker =
{
        NB_VERSION,
        COMMODITY_NAME,
        COMMODITY_TITLE,
        COMMODITY_DESC,
        NBU_UNIQUE | NBU_NOTIFY, //-V2544
        0,
        0,
        0,
        0
};

_Bool attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj)
{
	short rc = FALSE;
	size_t keyarrsize;
	size_t optarrsize;
	struct Popkeys* keys;

	exclude_wtype = 0;
	include_wtype = 0;

	optarrsize = sizeof(defopts) / sizeof(*defopts);
        keyarrsize = sizeof(defkeys) / sizeof(*defkeys);

	for (size_t i = 0; i < optarrsize ; ++i) {
       		char *tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);

		if((tt_optvalue) && ((strnlen(tt_optvalue, TT_MAX_LENGTH) < (size_t)TT_MAX_LENGTH))) {
			if(defopts[i].cxint >= EXCL_WTYPE_ID_0 && defopts[i].cxint <= (WTYPE_MAX + EXCL_WTYPE_ID_0)) {
				rc = alloc_opts(tt_optvalue, excls, i, EXCL_WTYPE_ID_0);
				if(exclude_wtype == 0) {
					exclude_wtype = 1;
				}
			}

			if(defopts[i].cxint >= INCL_WTYPE_ID_0 && defopts[i].cxint <= (WTYPE_MAX + INCL_WTYPE_ID_0)) {
				rc = alloc_opts(tt_optvalue, incls, i, INCL_WTYPE_ID_0);
				if(include_wtype == 0) {
					include_wtype = 1;
				}
			}

			if(defopts[i].cxint >= CONLINE_ID_0 && defopts[i].cxint <= (CMD_MAX + CONLINE_ID_0)) {
				rc = alloc_opts(tt_optvalue, cons, i, CONLINE_ID_0);
			}

			if(defopts[i].cxint >= CMD_ID_0 && defopts[i].cxint <= (CMD_MAX + CMD_ID_0)) {
				rc = alloc_opts(tt_optvalue, cmds, i, CMD_ID_0);
			}

			switch (defopts[i].cxint) {
				case TOPGAP_ID:
					topgap = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case DEFAULT_TOPGAP_ID:
					topgap = calcgap();
					break;
				case BOTTOMGAP_ID:
					bottomgap = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case LEFTGAP_ID:
					leftgap = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case RIGHTGAP_ID:
					rightgap = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case GAP_CHANGE_VALUE_ID:
					gap_change_value = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case AUTO_ID:
					autotile = TRUE;
					break;
				case AUTO_INTERVAL_MICRO_ID:
					auto_interval = strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case TILE_FACT_ID:
					fact = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				default:
					// Do nothing
					break;
			}
		}
	}

	if(exclude_wtype) {
		qsort(excls->strings, WTYPE_MAX, sizeof(*excls->strings), cstring_cmp);
	}

	if(include_wtype) {
		qsort(incls->strings, WTYPE_MAX, sizeof(*incls->strings), cstring_cmp);
	}


	keys = malloc(sizeof(*keys) * keyarrsize);

	if(keys != NULL) {
		for (size_t i = 0; i < keyarrsize ; ++i) {
			keys[i].rawcombo = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defkeys[i].optname);

			if(keys[i].rawcombo == NULL) {
				keys[i].rawcombo = defkeys[i].defaultval;
			}

			if(keys[i].rawcombo != NULL) {
				CxObj *filter;
				if((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, (long int)i)))
				{
					AttachCxObj(broker, filter);

					if (CxObjError(filter) != 0) {
						rc = FALSE;
						break;
					} else {
						rc = TRUE;
					}
				}
			}
		}
	} else {
		rc = FALSE;
	}

	free(keys);
	return rc;
}

short int commo(void)
{
	struct timeval currentval;

	struct MsgPort *mp = CreateMsgPort();
	static struct DiskObject *diskobj;
	static unsigned char iconlib[] = "icon.library";
	static unsigned char diskobjname[] = "dintwm";

	auto_interval = (unsigned long)AUTO_INTERVAL_MICRO_DEF;

	if(!(iconbase = OpenLibrary(iconlib, 37))) {
		DeleteMsgPort(mp);
		return 1;
	}

	if((diskobj = GetDiskObject(diskobjname)) == NULL) {
		DeleteMsgPort(mp);
		return 1;
	}

	if (mp)
	{
		CxObj *broker;

		MyBroker.nb_Port = mp;
		broker = CxBroker(&MyBroker, NULL);


		if ((attachtooltypes(broker, mp, diskobj)) && (broker != NULL))
		{
			CxMsg *msg;
			CloseLibrary(iconbase);
			FreeDiskObject(diskobj);

			short running = TRUE;

			if(ActivateCxObj(broker, 1) != 0) {
				DeleteMsgPort(mp);
				return 1;
			}

			static int wincount = 0;
			static short first_run = TRUE;
			static int lock = 1;

			while (running)
			{
				if(autotile) {
					wincount = countwindows(lock);
					currentval.tv_secs = 0UL;
					currentval.tv_micro = auto_interval;
					(void)time_delay(&currentval, UNIT_MICROHZ);
					if(wincount != (countwindows(lock)) || first_run == TRUE) {
						running = defkeys[*current_layout].func(&defkeys[*current_layout].arg);
						if(first_run == TRUE) {
							first_run = FALSE;
						}
					}
				} else {
					(void)WaitPort(mp);
				}

				while ((msg = (void *)GetMsg(mp)))
				{
					long id = CxMsgID(msg);
					unsigned long type = CxMsgType(msg);

					ReplyMsg((struct Message *)msg); //-V2545

					if (type == (unsigned long)CXM_COMMAND) //-V2544
					{
						switch (id)
						{
							case CXCMD_UNIQUE:
								printf("Commodity is already running. Quitting.\n");
								running = FALSE;
								break;
							case CXCMD_DISABLE:
							case CXCMD_ENABLE:
								break;
							case CXCMD_KILL:
								running = FALSE;
								break;
							case CXCMD_APPEAR:
								break;
							case CXCMD_DISAPPEAR:
								break;
							default:
								// Do nothing
								break;
						}
					} else if (type == (unsigned long)CXM_IEVENT) { //-V2544
						if(id <= (TILE_FUNC_LIMIT)) {
							*current_layout = id;
						}
						running = defkeys[id].func(&defkeys[id].arg);
					}
				}
			}

			DeleteCxObjAll(broker);

			while ((msg = (void *)GetMsg(mp))) {
				ReplyMsg((struct Message *)msg); //-V2545
			}
		}
		DeleteMsgPort(mp);
	}

	free_opts();

	return 0;
}

static short alloc_opts(char *t, Ostore *s, size_t i, int subtract)
{
	int cxint = defopts[i].cxint - subtract;

	s->strings[cxint] = malloc((strnlen(t, TT_MAX_LENGTH)) * sizeof(unsigned char));

	if(s->strings[cxint]) {
		(void)snprintf((char *)s->strings[cxint], TT_MAX_LENGTH, "%s", t);
		return(TRUE);
	} else {
		return(FALSE);
	}
}

static void free_opts(void)
{
	int i;

	for (i = 0; i < CMD_MAX; ++i) {
		if(cons->strings[i]) {
			free(cons->strings[i]);
		}
		if(cmds->strings[i]) {
			free(cmds->strings[i]);
		}
	}

	for (i = 0; i < WTYPE_MAX; ++i) {
		if(excls->strings[i]) {
			free(excls->strings[i]);
		}
		if(incls->strings[i]) {
			free(incls->strings[i]);
		}
	}
}
