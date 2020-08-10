#include "dintwm.h"

#define DEFAULT_TOPGAP 0
#define KEYTYPE 1
#define OPTTYPE 2
#define STORE 1
#define RESTORE 2
#define FREE 3

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "Sets up hotkeys for DintWM";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit source";
static char TYPE_TILE[] = "POPKEY_TILE";
static char TYPE_HGRID[] = "POPKEY_HGRID";
static char TYPE_SPIRAL[] = "POPKEY_SPIRAL";
static char TYPE_DWINDLE[] = "POPKEY_DWINDLE";
static char TYPE_RESTORE[] = "POPKEY_RESTORE";
static char TYPE_SWITCHF[] = "POPKEY_SWITCHF";
static char TYPE_SWITCHB[] = "POPKEY_SWITCHB";
static char TYPE_CLEANSNAPSHOT[] = "POPKEY_CLEANSNAPSHOT";
static char TYPE_TAKESNAPSHOT[] = "POPKEY_TAKESNAPSHOT";
static char TYPE_TOPGAP[] = "TOPGAP";
static char TYPE_DEFAULT_TOPGAP[] = "DEFAULT_TOPGAP";
static char TYPE_EXCL_WTYPE[] = "EXCL_WTYPE";
static char TYPE_INCL_WTYPE[] = "INCL_WTYPE";
static char TYPE_AUTO[] = "AUTO";
static char TYPE_SHELL[] = "SHELL";
static char TYPE_CLOSEWIN[] = "CLOSEWIN";
static char KEY_TILE[] = "rawkey control lcommand t";
static char KEY_HGRID[] = "rawkey control lcommand g";
static char KEY_SPIRAL[] = "rawkey control lcommand f";
static char KEY_DWINDLE[] = "rawkey control lcommand d";
static char KEY_RESTORE[] = "rawkey control lcommand r";
static char KEY_SWITCHL[] = "rawkey control lcommand s";
static char KEY_SWITCHB[] = "rawkey control lcommand x";
static char KEY_CLEANSNAPSHOT[] = "rawkey control lcommand c";
static char KEY_TAKESNAPSHOT[] = "rawkey control lcommand p";
static char KEY_SHELL[] = "rawkey control lcommand return";
static char KEY_CLOSEWIN[] = "rawkey control lcommand delete";
static char NA[] = "0";

static short autotile = FALSE;

static BOOL attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);

static struct Library *iconbase;
static struct Optdef defopts[] = {
	{ TYPE_TILE, FUNC_TILE, KEY_TILE, KEYTYPE },
	{ TYPE_HGRID, FUNC_HGRID, KEY_HGRID, KEYTYPE },
	{ TYPE_SPIRAL, FUNC_SPIRAL, KEY_SPIRAL, KEYTYPE },
	{ TYPE_DWINDLE, FUNC_DWINDLE, KEY_DWINDLE, KEYTYPE },
	{ TYPE_RESTORE, FUNC_RESTORE, KEY_RESTORE, KEYTYPE },
	{ TYPE_SWITCHF, FUNC_SWITCHF, KEY_SWITCHL, KEYTYPE },
	{ TYPE_SWITCHB, FUNC_SWITCHB, KEY_SWITCHB, KEYTYPE },
	{ TYPE_CLEANSNAPSHOT, FUNC_CLEANSNAPSHOT, KEY_CLEANSNAPSHOT, KEYTYPE },
	{ TYPE_TAKESNAPSHOT, FUNC_TAKESNAPSHOT, KEY_TAKESNAPSHOT, KEYTYPE },
	{ TYPE_SHELL, FUNC_SHELL, KEY_SHELL, KEYTYPE },
	{ TYPE_CLOSEWIN, FUNC_CLOSEWIN, KEY_CLOSEWIN, KEYTYPE },
	{ TYPE_TOPGAP, TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_EXCL_WTYPE, EXCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_INCL_WTYPE, INCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_AUTO, AUTO_ID, NA, OPTTYPE }
};

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
					case TOPGAP_ID: 	
						topgap = atoi((const char *)tt_optvalue);
						break;
					case DEFAULT_TOPGAP_ID: 	
						topgap = calcgap();
						break;
					case EXCL_WTYPE_ID:
						strncpy(exclude_wtype,tt_optvalue,(strlen(tt_optvalue))+1);
						break;
					case INCL_WTYPE_ID:
						strncpy(include_wtype,tt_optvalue,(strlen(tt_optvalue))+1);
						break;
					case AUTO_ID:
						autotile = TRUE;
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
	static int wincount = 0;
	current_layout = 0;

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
					if(autotile) {
						wincount = countwindows();
						WaitTOF();	
						if(wincount < (countwindows())) {
							defkeyfuncs[*current_layout].func();
						}
					} else {
						(void)WaitPort(mp);
					}
 
					while ((msg = (void *)GetMsg(mp)))
					{
						long id = CxMsgID(msg);
						unsigned long type = CxMsgType(msg);
 
						ReplyMsg((struct Message *)msg);
 
						if (type == CXM_COMMAND)
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
									break;
							}
						}
						else if (type == CXM_IEVENT)
						{
							if (id == defopts[id].cxint) {
								if(id <= (TILE_FUNC_LIMIT)) {
									*current_layout = id;
								}
								defkeyfuncs[id].func();
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
