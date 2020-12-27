//-V::677
#include "dintwm.h"
#include "config.h"

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager for AmigaOS";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";

static BOOL attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);

static struct Library *iconbase;

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
	{ TYPE_KEY_CMD_0, KEY_CMD_0, KEYTYPE, docmd, { .i = CMD_0_ID } },
	{ TYPE_KEY_CMD_1, KEY_CMD_1, KEYTYPE, docmd, { .i = CMD_1_ID } },
	{ TYPE_KEY_CMD_2, KEY_CMD_2, KEYTYPE, docmd, { .i = CMD_2_ID } },
	{ TYPE_KEY_CMD_3, KEY_CMD_3, KEYTYPE, docmd, { .i = CMD_3_ID } },
	{ TYPE_KEY_CMD_4, KEY_CMD_4, KEYTYPE, docmd, { .i = CMD_4_ID } },
	{ TYPE_KEY_CMD_5, KEY_CMD_5, KEYTYPE, docmd, { .i = CMD_5_ID } },
	{ TYPE_KEY_CMD_6, KEY_CMD_6, KEYTYPE, docmd, { .i = CMD_6_ID } },
	{ TYPE_KEY_CMD_7, KEY_CMD_7, KEYTYPE, docmd, { .i = CMD_7_ID } },
	{ TYPE_KEY_CMD_8, KEY_CMD_8, KEYTYPE, docmd, { .i = CMD_8_ID } },
	{ TYPE_KEY_CMD_9, KEY_CMD_9, KEYTYPE, docmd, { .i = CMD_9_ID } },
};

Opts defopts[] = {
	{ TYPE_TOPGAP, TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, NA, OPTTYPE },
	{ TYPE_BOTTOMGAP, BOTTOMGAP_ID, NA, OPTTYPE },
	{ TYPE_LEFTGAP, LEFTGAP_ID, NA, OPTTYPE },
	{ TYPE_RIGHTGAP, RIGHTGAP_ID, NA, OPTTYPE },
	{ TYPE_EXCL_WTYPE, EXCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_INCL_WTYPE, INCL_WTYPE_ID, NA, OPTTYPE },
	{ TYPE_AUTO, AUTO_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_0, CONLINE_0_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_1, CONLINE_1_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_2, CONLINE_2_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_3, CONLINE_3_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_4, CONLINE_4_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_5, CONLINE_5_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_6, CONLINE_6_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_7, CONLINE_7_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_8, CONLINE_8_ID, NA, OPTTYPE },
	{ TYPE_CONLINE_9, CONLINE_9_ID, NA, OPTTYPE },
	{ TYPE_CMD_0, CMD_0_ID, NA, OPTTYPE },
	{ TYPE_CMD_1, CMD_1_ID, NA, OPTTYPE },
	{ TYPE_CMD_2, CMD_2_ID, NA, OPTTYPE },
	{ TYPE_CMD_3, CMD_3_ID, NA, OPTTYPE },
	{ TYPE_CMD_4, CMD_4_ID, NA, OPTTYPE },
	{ TYPE_CMD_5, CMD_5_ID, NA, OPTTYPE },
	{ TYPE_CMD_6, CMD_6_ID, NA, OPTTYPE },
	{ TYPE_CMD_7, CMD_7_ID, NA, OPTTYPE },
	{ TYPE_CMD_8, CMD_8_ID, NA, OPTTYPE },
	{ TYPE_CMD_9, CMD_9_ID, NA, OPTTYPE },
};

Cmdstore cmds[] = {0};

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

	optarrsize = sizeof(defopts) / sizeof(*defopts);
        keyarrsize = sizeof(defkeys) / sizeof(*defkeys);

	printf("optarrsize %d\n",optarrsize);
	printf("keyarrsize %d\n",keyarrsize);

	for (i = 0; i < optarrsize ; ++i) {
       		char *tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);

		if(tt_optvalue) {

			if(defopts[i].cxint >= CONLINE_0_ID && defopts[i].cxint <= (CMD_MAX + CONLINE_0_ID)) {
				if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
					cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
					if(cmds->con_strings[defopts[i].cxint - CONLINE_0_ID]) {
						strcpy((char *)cmds->con_strings[defopts[i].cxint - CONLINE_0_ID],tt_optvalue);
					}
				}
			}

			if(defopts[i].cxint >= CMD_0_ID && defopts[i].cxint <= (CMD_MAX + CMD_0_ID)) {
				if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
					cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
					if(cmds->cmd_strings[defopts[i].cxint - CMD_0_ID]) {
						strcpy((char *)cmds->cmd_strings[defopts[i].cxint - CMD_0_ID],tt_optvalue);
					}
				}
			}

			switch (defopts[i].cxint) {
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
				/*case CONLINE_0_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[0],tt_optvalue);
					}
					break;
				case CONLINE_1_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[1],tt_optvalue);
					}
					break;
				case CONLINE_2_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[2],tt_optvalue);
					}
					break;
				case CONLINE_3_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[3],tt_optvalue);
					}
					break;
				case CONLINE_4_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[4],tt_optvalue);
					}
					break;
				case CONLINE_5_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[5],tt_optvalue);
					}
					break;
				case CONLINE_6_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[6],tt_optvalue);
					}
					break;
				case CONLINE_7_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[7],tt_optvalue);
					}
					break;
				case CONLINE_8_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[8],tt_optvalue);
					}
					break;
				case CONLINE_9_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->con_strings[defopts[i].cxint - CONLINE_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->con_strings[9],tt_optvalue);
					}
					break;
				case CMD_0_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[0],tt_optvalue);
					}
					break;
				case CMD_1_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[1],tt_optvalue);
					}
					break;
				case CMD_2_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[2],tt_optvalue);
					}
					break;
				case CMD_3_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[3],tt_optvalue);
					}
					break;
				case CMD_4_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[4],tt_optvalue);
					}
					break;
				case CMD_5_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[5],tt_optvalue);
					}
					break;
				case CMD_6_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[6],tt_optvalue);
					}
					break;
				case CMD_7_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[7],tt_optvalue);
					}
					break;
				case CMD_8_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[8],tt_optvalue);
					}
					break;
				case CMD_9_ID:
					if(((strlen(tt_optvalue))+1) < TT_MAX_LENGTH ) {
						cmds->cmd_strings[defopts[i].cxint - CMD_0_ID] = malloc((strlen(tt_optvalue)+1) * sizeof(unsigned char));
						strcpy((char *)cmds->cmd_strings[9],tt_optvalue);
					}
					break;*/
				case AUTO_ID:
					autotile = TRUE;
					break;
				default:
					break;
			}
		}
	}

	keys = malloc(sizeof(*keys) * keyarrsize);

	for (i = 0; i < keyarrsize ; ++i) {
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
	static int wincount = 0;
	current_layout = 0;
	static short first_run = TRUE;
	int i;
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
						/*WaitTOF();	
						WaitTOF();	
						WaitTOF();*/
						currentval.tv_secs = 0;
						currentval.tv_micro = 20000;
						time_delay( &currentval, UNIT_MICROHZ );
						if(wincount != (countwindows(lock)) || first_run == TRUE) {
							defkeys[*current_layout].func(&defkeys[*current_layout].arg);
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
							if(id <= (TILE_FUNC_LIMIT)) {
								*current_layout = id;
							}
							printf("id %lu\n", id);
							defkeys[id].func(&defkeys[id].arg);
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

	for (i = 0; i < CMD_MAX; ++i) {
		if(cmds->con_strings[i]) {
			free(cmds->con_strings[i]);
		}
		if(cmds->cmd_strings[i]) {
			free(cmds->cmd_strings[i]);
		}
	}
 
	return 0;
}
