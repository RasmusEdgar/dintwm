#include "dintwm.h"
static struct Library *iconbase;
static struct Optdef defopts[] = {
	{ "POPKEY_TILE", 1, KEY_TILE, KEYTYPE },
	{ "POPKEY_HGRID", 2, KEY_HGRID, KEYTYPE },
	{ "POPKEY_SPIRAL", 3, KEY_SPIRAL, KEYTYPE },
	{ "POPKEY_DWINDLE", 4, KEY_DWINDLE, KEYTYPE },
	{ "TOPGAP", 900, "0", OPTTYPE },
	{ "EXCL_WTYPE", 901, "0", OPTTYPE }
};

/*struct Optdef defopts[] = {
	{ .optname = "POPKEY_TILE", .cxint = 1, .defaultval = KEY_TILE, .tt_type = KEYTYPE },
	{ .optname = "POPKEY_HGRID", .cxint = 2, .defaultval = KEY_HGRID, .tt_type = KEYTYPE },
	{ .optname = "POPKEY_SPIRAL", .cxint = 3, .defaultval = KEY_SPIRAL, .tt_type = KEYTYPE },
	{ .optname = "POPKEY_DWINDLE", .cxint = 4, .defaultval = KEY_DWINDLE, .tt_type = KEYTYPE },
	{ .optname = "TOPGAP", .cxint = 900, .tt_type = OPTTYPE },
	{ .optname = "INC_WTYPE", .cxint = 901, .tt_type = KEYTYPE }
};*/

static struct Keyfuncdef defkeyfuncs[] = {
	{ tile },
	{ hgrid },
	{ spiral },
	{ dwindle }
};

static BOOL attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);

static struct NewBroker MyBroker =
{
        NB_VERSION,
        (unsigned char *)COMMODITY_NAME,
        (unsigned char *)COMMODITY_TITLE,
        (unsigned char *)COMMODITY_DESC,
        NBU_UNIQUE | NBU_NOTIFY,
        0,
        0,
        0,
        0
};

static BOOL attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj)
{
	int i;
	BOOL rc = FALSE;
	size_t tt_length = 0;
	size_t arrsizes;
	struct Popkeys* keys;

	keys = malloc(sizeof(*defopts) * sizeof(*keys));

	arrsizes = sizeof(defopts) / sizeof(*defopts);

	exclude_wtype = calloc(TT_MAX_LENGTH, sizeof(char));

	for (i = 0; i < arrsizes ; ++i) {
        	char * tt_optvalue;
		CxObj *filter;

		if((tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname)) == NULL) {
			if(defopts[i].tt_type == KEYTYPE ) {
				tt_length = strlen(defopts[i].defaultval)+1;
				keys[i].rawcombo = malloc(TT_MAX_LENGTH * sizeof keys[i].rawcombo);
				strncpy(keys[i].rawcombo, defopts[i].defaultval, tt_length);
				printf("Default keys: %s\n", keys[i].rawcombo);
			} 
		} else if (defopts[i].tt_type == KEYTYPE) {
			keys[i].rawcombo = malloc(TT_MAX_LENGTH * sizeof keys[i].rawcombo);
			strncpy(keys[i].rawcombo, (const char *)tt_optvalue, strlen(tt_optvalue)+1);
			printf("Custom keys: %s\n", keys[i].rawcombo);
		} else if(defopts[i].tt_type == OPTTYPE ) {
			switch (defopts[i].cxint) {
				case 900: 	
					topgap = atoi((const char *)tt_optvalue);
					printf("topgap: %d\n", topgap);
					break;
				case 901:
					strncpy(exclude_wtype, (const char *)tt_optvalue, strlen(tt_optvalue)+1);
					printf("inc_wtype: %s\n", exclude_wtype);
					break;
			}
		}
	
		
		if((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, defopts[i].cxint)))
		{
			/* Attach hotkey object to broker and initialized hotkey detection.
		 	*/
			AttachCxObj(broker, filter);
 
			if (CxObjError(filter) != 0) {
				printf("CxObjError %lu\n", CxObjError(filter));
				rc = FALSE;
				break;
			} else {
				rc = TRUE;
			}
		}
		//free(keys[i].rawcombo);
	}
	free(keys);
 
	return rc;
}
 
int commo(void)
{
	struct MsgPort *mp = CreateMsgPort();
	static struct DiskObject *diskobj;

	if(!(iconbase = OpenLibrary((unsigned char *)"icon.library",37))) {
		DeleteMsgPort(mp);
		return 1;
	}

	if((diskobj = GetDiskObject((unsigned char *)"dintwm")) == NULL) {
		DeleteMsgPort(mp);
		return 1;
	}
 
	if (mp)
	{
		CxObj *broker;
 
		MyBroker.nb_Port = mp;
		broker = CxBroker(&MyBroker, NULL);
 
		if (broker)
		{
			CxMsg *msg;
 
			if (attachtooltypes(broker, mp, diskobj))
			{
				CloseLibrary(iconbase);
				FreeDiskObject(diskobj);

				BOOL running = TRUE;
 
				if(ActivateCxObj(broker, 1) != 0) {
					DeleteMsgPort(mp);
					return 1;
				}
 
				while (running)
				{
					WaitPort(mp);
 
					while ((msg = (APTR)GetMsg(mp)))
					{
						ULONG id = CxMsgID(msg);
						ULONG type = CxMsgType(msg);
 
						ReplyMsg((struct Message *)msg);
 
						if (type == CXM_COMMAND)
						{
							switch (id)
							{
								case CXCMD_UNIQUE:
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
							}
						}
						else if (type == CXM_IEVENT)
						{
							if (id == defopts[(id-1)].cxint) {
								defkeyfuncs[(id-1)].func();
							}
						}
					}
				}
			}
 
			DeleteCxObjAll(broker);
 
			while ((msg = (APTR)GetMsg(mp)))
				ReplyMsg((struct Message *)msg);
		}
		DeleteMsgPort(mp);
		free(exclude_wtype);
	}
 
	return 0;
}
