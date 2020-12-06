//-V::677
#include "dintwm.h"

#define DEFAULT_TOPGAP 0
#define KEYTYPE 1
#define OPTTYPE 2
#define STORE 1
#define RESTORE 2
#define FREE 3

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager for AmigaOS";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";
static char TYPE_TILE[] = "POPKEY_TILE";
static char TYPE_HGRID[] = "POPKEY_HGRID";
static char TYPE_SPIRAL[] = "POPKEY_SPIRAL";
static char TYPE_DWINDLE[] = "POPKEY_DWINDLE";
static char TYPE_RESTORE[] = "POPKEY_RESTORE";
static char TYPE_SWITCHF[] = "POPKEY_SWITCHF";
static char TYPE_SWITCHB[] = "POPKEY_SWITCHB";
static char TYPE_CLEANSNAPSHOT[] = "POPKEY_CLEANSNAPSHOT";
static char TYPE_TAKESNAPSHOT[] = "POPKEY_TAKESNAPSHOT";
static char TYPE_SHELL_1[] = "POPKEY_SHELL_1";
static char TYPE_SHELL_2[] = "POPKEY_SHELL_2";
static char TYPE_SHELL_3[] = "POPKEY_SHELL_3";
static char TYPE_TOPGAP[] = "TOPGAP";
static char TYPE_DEFAULT_TOPGAP[] = "DEFAULT_TOPGAP";
static char TYPE_BOTTOMGAP[] = "BOTTOMGAP";
static char TYPE_LEFTGAP[] = "LEFTGAP";
static char TYPE_RIGHTGAP[] = "RIGHTGAP";
static char TYPE_EXCL_WTYPE[] = "EXCL_WTYPE";
static char TYPE_INCL_WTYPE[] = "INCL_WTYPE";
static char TYPE_AUTO[] = "AUTO";
static char TYPE_CONLINE_0[] = "CONLINE_0";
static char TYPE_SHELLCMD_0[] = "SHELLCMD_0";
static char TYPE_CONLINE_1[] = "CONLINE_1";
static char TYPE_SHELLCMD_1[] = "SHELLCMD_1";
static char TYPE_CONLINE_2[] = "CONLINE_2";
static char TYPE_SHELLCMD_2[] = "SHELLCMD_2";
static char TYPE_CONLINE_3[] = "CONLINE_3";
static char TYPE_SHELLCMD_3[] = "SHELLCMD_3";
static char KEY_TILE[] = "rawkey control lcommand t";
static char KEY_HGRID[] = "rawkey control lcommand g";
static char KEY_SPIRAL[] = "rawkey control lcommand f";
static char KEY_DWINDLE[] = "rawkey control lcommand d";
static char KEY_RESTORE[] = "rawkey control lcommand r";
static char KEY_SWITCHF[] = "rawkey control lcommand s";
static char KEY_SWITCHB[] = "rawkey control lcommand x";
static char KEY_CLEANSNAPSHOT[] = "rawkey control lcommand c";
static char KEY_TAKESNAPSHOT[] = "rawkey control lcommand p";
static char KEY_SHELL[] = "rawkey control lcommand return";
static char NA[] = "0";

static short autotile = FALSE;

static BOOL attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);

static struct Library *iconbase;
static Optdefnew defkeys[] = {
	{ TYPE_TILE, FUNC_TILE, KEY_TILE, KEYTYPE, tile, {0} },
	{ TYPE_HGRID, FUNC_HGRID, KEY_HGRID, KEYTYPE, hgrid, {0} },
	{ TYPE_SPIRAL, FUNC_SPIRAL, KEY_SPIRAL, KEYTYPE, spiral, {0} },
	{ TYPE_DWINDLE, FUNC_DWINDLE, KEY_DWINDLE, KEYTYPE, dwindle, {0} },
	{ TYPE_RESTORE, FUNC_RESTORE, KEY_RESTORE, KEYTYPE, restore, {0} },
	{ TYPE_SWITCHF, FUNC_SWITCHF, KEY_SWITCHF, KEYTYPE, switcher, {.i = 1} },
	{ TYPE_SWITCHB, FUNC_SWITCHB, KEY_SWITCHB, KEYTYPE, switcher, {.i = 0} },
	{ TYPE_CLEANSNAPSHOT, FUNC_CLEANSNAPSHOT, KEY_CLEANSNAPSHOT, KEYTYPE, cleansnapshot, {0} },
	{ TYPE_TAKESNAPSHOT, FUNC_TAKESNAPSHOT, KEY_TAKESNAPSHOT, KEYTYPE, takesnapshot, {0} },
	{ TYPE_SHELL_1, FUNC_SHELL, KEY_SHELL, KEYTYPE, doshell, { .i = 1 } },
	{ TYPE_SHELL_2, FUNC_SHELL, KEY_SHELL, KEYTYPE, doshell, { .i = 2 } },
	{ TYPE_SHELL_3, FUNC_SHELL, KEY_SHELL, KEYTYPE, doshell, { .i = 3 } },
};

static struct Optdef defoptsnew[] = {
	{ TYPE_TOPGAP, TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_BOTTOMGAP, BOTTOMGAP_ID, NA, OPTTYPE },
	{ TYPE_LEFTGAP, LEFTGAP_ID, NA, OPTTYPE },
	{ TYPE_RIGHTGAP, RIGHTGAP_ID, NA, OPTTYPE },
	{ TYPE_EXCL_WTYPE, EXCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_INCL_WTYPE, INCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_AUTO, AUTO_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_0, CONLINE_0_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_0, SHELLCMD_0_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_1, CONLINE_1_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_1, SHELLCMD_1_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_2, CONLINE_2_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_2, SHELLCMD_2_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_3, CONLINE_3_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_3, SHELLCMD_3_ID, NA, OPTTYPE },
};


static struct Optdef defopts[] = {
	{ TYPE_TILE, FUNC_TILE, KEY_TILE, KEYTYPE },
	{ TYPE_HGRID, FUNC_HGRID, KEY_HGRID, KEYTYPE },
	{ TYPE_SPIRAL, FUNC_SPIRAL, KEY_SPIRAL, KEYTYPE },
	{ TYPE_DWINDLE, FUNC_DWINDLE, KEY_DWINDLE, KEYTYPE },
	{ TYPE_RESTORE, FUNC_RESTORE, KEY_RESTORE, KEYTYPE },
	{ TYPE_SWITCHF, FUNC_SWITCHF, KEY_SWITCHF, KEYTYPE },
	{ TYPE_SWITCHB, FUNC_SWITCHB, KEY_SWITCHB, KEYTYPE },
	{ TYPE_CLEANSNAPSHOT, FUNC_CLEANSNAPSHOT, KEY_CLEANSNAPSHOT, KEYTYPE },
	{ TYPE_TAKESNAPSHOT, FUNC_TAKESNAPSHOT, KEY_TAKESNAPSHOT, KEYTYPE },
	{ TYPE_SHELL_1, FUNC_SHELL, KEY_SHELL, KEYTYPE },
	{ TYPE_TOPGAP, TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_BOTTOMGAP, BOTTOMGAP_ID, NA, OPTTYPE },
	{ TYPE_LEFTGAP, LEFTGAP_ID, NA, OPTTYPE },
	{ TYPE_RIGHTGAP, RIGHTGAP_ID, NA, OPTTYPE },
	{ TYPE_EXCL_WTYPE, EXCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_INCL_WTYPE, INCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_AUTO, AUTO_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_0, CONLINE_0_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_0, SHELLCMD_0_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_1, CONLINE_1_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_1, SHELLCMD_1_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_2, CONLINE_2_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_2, SHELLCMD_2_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_3, CONLINE_3_ID, NA, OPTTYPE },
	{ TYPE_SHELLCMD_3, SHELLCMD_3_ID, NA, OPTTYPE },
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
	size_t keyarrsize;
	size_t optarrsize;
	struct Popkeys* keys;

	conline[0] = 0;
	shellcmd[0] = 0;

	optarrsize = sizeof(defoptsnew) / sizeof(*defoptsnew);
        keyarrsize = sizeof(defkeys) / sizeof(*defkeys);
	keys = malloc(sizeof(*keys) * keyarrsize);


	for (i = 0; i < keyarrsize ; ++i) {
       		char *tt_keyvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defkeys[i].optname);

		if(tt_keyvalue == NULL) {
			if(defkeys[i].tt_type == KEYTYPE ) {
				keys[i].rawcombo = defkeys[i].defaultval;
			} 
		} else {
			keys[i].rawcombo = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defkeys[i].optname);
		}
	}

	for (i = 0; i < optarrsize ; ++i) {
       		char *tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defoptsnew[i].optname);

		if(defoptsnew[i].tt_type == OPTTYPE ) {
				switch (defoptsnew[i].cxint) {
					case TOPGAP_ID: 	
						topgap = atoi((const char *)tt_optvalue);
						break;
					case DEFAULT_TOPGAP_ID: 	
						topgap = calcgap();
						break;
					case BOTTOMGAP_ID: 	
						bottomgap = atoi((const char *)tt_optvalue);
						break;
					case LEFTGAP_ID: 	
						leftgap = atoi((const char *)tt_optvalue);
						break;
					case RIGHTGAP_ID: 	
						rightgap = atoi((const char *)tt_optvalue);
						break;
					case EXCL_WTYPE_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy(exclude_wtype,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case INCL_WTYPE_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy(include_wtype,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case CONLINE_0_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)conline,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case SHELLCMD_0_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)shellcmd,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case CONLINE_1_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)conline,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case SHELLCMD_1_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)shellcmd,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case CONLINE_2_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)conline,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case SHELLCMD_2_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)shellcmd,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case CONLINE_3_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)conline,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case SHELLCMD_3_ID:
						if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
							strncpy((char *)shellcmd,tt_optvalue,(strlen(tt_optvalue))+1);
						}
						break;
					case AUTO_ID:
						autotile = TRUE;
						break;
					default:
						break;
				}
			}
		
		if(keys[i].rawcombo != NULL) {
			CxObj *filter;
			if((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, defkeys[i].cxint)))
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
	static short first_run = TRUE;
	int lock = 1;

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
						wincount = countwindows(lock);
						WaitTOF();	
						if(wincount != (countwindows(lock)) || first_run == TRUE) {
							defkeyfuncs[*current_layout].func();
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
							if (id == defkeys[id].cxint) {
								if(id <= (TILE_FUNC_LIMIT)) {
									*current_layout = id;
								}
								if(id == 9) {
									defkeys[id].func(&defkeys[id].arg);
								}
								defkeys[id].func(&defkeys[id].arg);
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
