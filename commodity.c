#include "dintwm.h"

#define DEFAULT_TOPGAP 0
#define KEYTYPE 1
#define OPTTYPE 2

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "Sets up hotkeys for DintWM";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit source";
static char TYPE_TILE[] = "POPKEY_TILE";
static char TYPE_HGRID[] = "POPKEY_HGRID";
static char TYPE_SPIRAL[] = "POPKEY_SPIRAL";
static char TYPE_DWINDLE[] = "POPKEY_DWINDLE";
static char TYPE_TOPGAP[] = "TOPGAP";
static char TYPE_EXCL_WTYPE[] = "EXCL_WTYPE";
static char KEY_TILE[] = "rawkey control lshift t";
static char KEY_HGRID[] = "rawkey control lshift g";
static char KEY_SPIRAL[] = "rawkey control lshift f";
static char KEY_DWINDLE[] = "rawkey control lshift d";
static char NA[] = "0";

static struct Library *iconbase;
static struct Optdef defopts[] = {
	{ TYPE_TILE, 1, KEY_TILE, KEYTYPE },
	{ TYPE_HGRID, 2, KEY_HGRID, KEYTYPE },
	{ TYPE_SPIRAL, 3, KEY_SPIRAL, KEYTYPE },
	{ TYPE_DWINDLE, 4, KEY_DWINDLE, KEYTYPE },
	{ TYPE_TOPGAP, 900, NA, OPTTYPE },
	{ TYPE_EXCL_WTYPE, 901, NA, OPTTYPE }
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
        COMMODITY_NAME,
        COMMODITY_TITLE,
        COMMODITY_DESC,
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
       		char *tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);

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
					default:
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
 
short int commo(void)
{
	struct MsgPort *mp = CreateMsgPort();
	static struct DiskObject *diskobj;
	static unsigned char iconlib[] = "icon.library";
	static unsigned char diskobjname[] = "dintwm";

	if(!(iconbase = OpenLibrary(iconlib,37))) {
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
 
					//while ((msg = (APTR)GetMsg(mp)))
					while ((msg = (void *)GetMsg(mp)))
					{
						long id = CxMsgID(msg);
						uint32_t type = CxMsgType(msg);
 
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
								default:
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
 
			while ((msg = (void *)GetMsg(mp)))
				ReplyMsg((struct Message *)msg);
		}
		DeleteMsgPort(mp);
	}
 
	return 0;
}
