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
	size_t i;
	BOOL rc = FALSE;
	size_t arrsizes;
	struct Popkeys* keys;

	keys = malloc(sizeof(*defopts) * sizeof(*keys));
	arrsizes = sizeof(defopts) / sizeof(*defopts);

	for (i = 0; i < arrsizes ; ++i) {
       		const char * tt_optvalue = (const char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);

		if(tt_optvalue == NULL) {
			if(defopts[i].tt_type == KEYTYPE ) {
				keys[i].rawcombo = defopts[i].defaultval;
			} 
		} else {
			if (defopts[i].tt_type == KEYTYPE) {
				keys[i].rawcombo = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);
			} else if(defopts[i].tt_type == OPTTYPE ) {
				switch (defopts[i].cxint) {
					case 900: 	
						topgap = atoi((const char *)tt_optvalue);
						break;
					case 901:
						strncpy(exclude_wtype,tt_optvalue,(strlen(tt_optvalue))+1);
						break;
				}
			}
		}
	
		
		if(keys[i].rawcombo != NULL) {
			CxObj *filter;
			if((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, defopts[i].cxint)))
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
					(void)WaitPort(mp);
 
					while ((msg = (APTR)GetMsg(mp)))
					{
						long id = CxMsgID(msg);
						unsigned long type = CxMsgType(msg);
 
						ReplyMsg((struct Message *)msg);
 
						if ((int)type == CXM_COMMAND)
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
						else if ((int)type == CXM_IEVENT)
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
	}
 
	return 0;
}
