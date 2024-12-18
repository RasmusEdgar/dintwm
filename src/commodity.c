// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/commodity.h"

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";

int fact = TILE_FACT_DEF;
int gap_change_value = GAP_CHANGE_VALUE_DEF;
static short info_on = TRUE;

static unsigned long mainsig, wakeupsigs, subsig;
static struct Task *maintask = NULL, *subtask = NULL;
static unsigned char subactionchkname[] = "CXM_window_state_checker";
static short first_run = TRUE;
static struct Window *awin_comp;
static short running = TRUE;
static short autotile = FALSE;

static char KEY_TILE[] = KEY_TILE_TXT,
	KEY_HGRID[] = KEY_HGRID_TXT,
	KEY_SPIRAL[] = KEY_SPIRAL_TXT,
	KEY_DWINDLE[] = KEY_DWINDLE_TXT,
	KEY_SWITCHF[] = KEY_SWITCHF_TXT,
	KEY_SWITCHB[] = KEY_SWITCHB_TXT,
	KEY_INCTOPGAP[] = KEY_INCTOPGAP_TXT,
	KEY_INCBOTTOMGAP[] = KEY_INCBOTTOMGAP_TXT,
	KEY_INCLEFTGAP[] = KEY_INCLEFTGAP_TXT,
	KEY_INCRIGHTGAP[] = KEY_INCRIGHTGAP_TXT,
	KEY_DECTOPGAP[] = KEY_DECTOPGAP_TXT,
	KEY_DECBOTTOMGAP[] = KEY_DECBOTTOMGAP_TXT,
	KEY_DECLEFTGAP[] = KEY_DECLEFTGAP_TXT,
	KEY_DECRIGHTGAP[] = KEY_DECRIGHTGAP_TXT,
	KEY_INCALLGAPS[] = KEY_INCALLGAPS_TXT,
	KEY_DECALLGAPS[] = KEY_DECALLGAPS_TXT,
	KEY_TILE_OFF[] = KEY_TILE_OFF_TXT,
	KEY_CMD_0[] = KEY_CMD_0_TXT,
	KEY_CMD_1[] = KEY_CMD_1_TXT,
	KEY_CMD_2[] = KEY_CMD_2_TXT,
	KEY_CMD_3[] = KEY_CMD_3_TXT,
	KEY_CMD_4[] = KEY_CMD_4_TXT,
	KEY_CMD_5[] = KEY_CMD_5_TXT,
	KEY_CMD_6[] = KEY_CMD_6_TXT,
	KEY_CMD_7[] = KEY_CMD_7_TXT,
	KEY_CMD_8[] = KEY_CMD_8_TXT,
	KEY_CMD_9[] = KEY_CMD_9_TXT,
	KEY_WS_0[] = KEY_WS_0_TXT,
	KEY_WS_1[] = KEY_WS_1_TXT,
	KEY_WS_2[] = KEY_WS_2_TXT,
	KEY_WS_3[] = KEY_WS_3_TXT,
	KEY_WS_4[] = KEY_WS_4_TXT,
	KEY_WS_5[] = KEY_WS_5_TXT,
	KEY_CWS_0[] = KEY_CWS_0_TXT,
	KEY_CWS_1[] = KEY_CWS_1_TXT,
	KEY_CWS_2[] = KEY_CWS_2_TXT,
	KEY_CWS_3[] = KEY_CWS_3_TXT,
	KEY_CWS_4[] = KEY_CWS_4_TXT,
	KEY_CWS_5[] = KEY_CWS_5_TXT,
	KEY_CXM_EXIT[] = KEY_CXM_EXIT_TXT,
	KEY_TAB_NEXT[] = KEY_TAB_NEXT_TXT;

Keys defkeys[] = {
	{ TYPE_TILE, KEY_TILE, KEYTYPE_ID, tile, {0} },
	{ TYPE_HGRID, KEY_HGRID, KEYTYPE_ID, hgrid, {0} },
	{ TYPE_SPIRAL, KEY_SPIRAL, KEYTYPE_ID, fibonacci, { .i = 0} },
	{ TYPE_DWINDLE, KEY_DWINDLE, KEYTYPE_ID, fibonacci, { .i = 1 } },
	{ TYPE_SWITCHF, KEY_SWITCHF, KEYTYPE_ID, switcher, { .i = 1 } },
	{ TYPE_SWITCHB, KEY_SWITCHB, KEYTYPE_ID, switcher, { .i = 0 } },
	{ TYPE_INCTOPGAP, KEY_INCTOPGAP, KEYTYPE_ID, changegaps, {.i = TOPGAP_ID} },
	{ TYPE_INCBOTTOMGAP, KEY_INCBOTTOMGAP, KEYTYPE_ID, changegaps, {.i = BOTTOMGAP_ID} },
	{ TYPE_INCLEFTGAP, KEY_INCLEFTGAP, KEYTYPE_ID, changegaps, {.i = LEFTGAP_ID} },
	{ TYPE_INCRIGHTGAP, KEY_INCRIGHTGAP, KEYTYPE_ID, changegaps, {.i = RIGHTGAP_ID} },
	{ TYPE_DECTOPGAP, KEY_DECTOPGAP, KEYTYPE_ID, changegaps, {.i = -TOPGAP_ID} },
	{ TYPE_DECBOTTOMGAP, KEY_DECBOTTOMGAP, KEYTYPE_ID, changegaps, {.i = -BOTTOMGAP_ID} },
	{ TYPE_DECLEFTGAP, KEY_DECLEFTGAP, KEYTYPE_ID, changegaps, {.i = -LEFTGAP_ID} },
	{ TYPE_DECRIGHTGAP, KEY_DECRIGHTGAP, KEYTYPE_ID, changegaps, {.i = -RIGHTGAP_ID} },
	{ TYPE_INCALLGAPS, KEY_INCALLGAPS, KEYTYPE_ID, changegaps, {.i = INCALLGAPS_ID} },
	{ TYPE_DECALLGAPS, KEY_DECALLGAPS, KEYTYPE_ID, changegaps, {.i = DECALLGAPS_ID} },
	{ TYPE_TILE_OFF, KEY_TILE_OFF, KEYTYPE_ID, tileoff, {0} },
	{ TYPE_KEY_CMD_0, KEY_CMD_0, KEYTYPE_ID, docmd, { .i = CMD_ID_0 } },
	{ TYPE_KEY_CMD_1, KEY_CMD_1, KEYTYPE_ID, docmd, { .i = CMD_ID_1 } },
	{ TYPE_KEY_CMD_2, KEY_CMD_2, KEYTYPE_ID, docmd, { .i = CMD_ID_2 } },
	{ TYPE_KEY_CMD_3, KEY_CMD_3, KEYTYPE_ID, docmd, { .i = CMD_ID_3 } },
	{ TYPE_KEY_CMD_4, KEY_CMD_4, KEYTYPE_ID, docmd, { .i = CMD_ID_4 } },
	{ TYPE_KEY_CMD_5, KEY_CMD_5, KEYTYPE_ID, docmd, { .i = CMD_ID_5 } },
	{ TYPE_KEY_CMD_6, KEY_CMD_6, KEYTYPE_ID, docmd, { .i = CMD_ID_7 } },
	{ TYPE_KEY_CMD_7, KEY_CMD_7, KEYTYPE_ID, docmd, { .i = CMD_ID_7 } },
	{ TYPE_KEY_CMD_8, KEY_CMD_8, KEYTYPE_ID, docmd, { .i = CMD_ID_8 } },
	{ TYPE_KEY_CMD_9, KEY_CMD_9, KEYTYPE_ID, docmd, { .i = CMD_ID_9 } },
	{ TYPE_KEY_WS_0, KEY_WS_0, KEYTYPE_ID, changews, { .u = WS_0 } },
	{ TYPE_KEY_WS_1, KEY_WS_1, KEYTYPE_ID, changews, { .u = WS_1 } },
	{ TYPE_KEY_WS_2, KEY_WS_2, KEYTYPE_ID, changews, { .u = WS_2 } },
	{ TYPE_KEY_WS_3, KEY_WS_3, KEYTYPE_ID, changews, { .u = WS_3 } },
	{ TYPE_KEY_WS_4, KEY_WS_4, KEYTYPE_ID, changews, { .u = WS_4 } },
	{ TYPE_KEY_WS_5, KEY_WS_5, KEYTYPE_ID, changews, { .u = WS_5 } },
	{ TYPE_KEY_CWS_0, KEY_CWS_0, KEYTYPE_ID, movetows, { .u = WS_0 } },
	{ TYPE_KEY_CWS_1, KEY_CWS_1, KEYTYPE_ID, movetows, { .u = WS_1 } },
	{ TYPE_KEY_CWS_2, KEY_CWS_2, KEYTYPE_ID, movetows, { .u = WS_2 } },
	{ TYPE_KEY_CWS_3, KEY_CWS_3, KEYTYPE_ID, movetows, { .u = WS_3 } },
	{ TYPE_KEY_CWS_4, KEY_CWS_4, KEYTYPE_ID, movetows, { .u = WS_4 } },
	{ TYPE_KEY_CWS_5, KEY_CWS_5, KEYTYPE_ID, movetows, { .u = WS_5 } },
	{ TYPE_KEY_TAB_NEXT, KEY_TAB_NEXT, KEYTYPE_ID, tabnextwin, { .i = 0 } },
	{ TYPE_KEY_CXM_EXIT, KEY_CXM_EXIT, KEYTYPE_ID, exit_cxm, {.s = FALSE} },
};

Opts defopts[] = {
	{ TYPE_TOPGAP, TOPGAP_ID, OPTTYPE_ID },
	{ TYPE_DEFAULT_TOPGAP, DEFAULT_TOPGAP_ID, OPTTYPE_ID },
	{ TYPE_BOTTOMGAP, BOTTOMGAP_ID, OPTTYPE_ID },
	{ TYPE_LEFTGAP, LEFTGAP_ID, OPTTYPE_ID },
	{ TYPE_RIGHTGAP, RIGHTGAP_ID, OPTTYPE_ID },
	{ TYPE_AUTO, AUTO_ID, OPTTYPE_ID },
	{ TYPE_AUTO_INTERVAL_DELAY, AUTO_INTERVAL_DELAY_ID, OPTTYPE_ID },
	{ TYPE_TILE_FACT, TILE_FACT_ID, OPTTYPE_ID },
	{ TYPE_GAP_CHANGE_VALUE, GAP_CHANGE_VALUE_ID, OPTTYPE_ID },
	{ TYPE_BAR, BAR_ID, OPTTYPE_ID },
	{ TYPE_BAR_HIDE_EMPTY, BAR_HIDE_EMPTY_ID, OPTTYPE_ID },
	{ TYPE_BAR_BG_COL, BAR_BG_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPW_COL, BAR_FPW_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPW_COL, BAR_BPW_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPCURW_COL, BAR_FPCURW_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPCURW_COL, BAR_BPCURW_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPTM_COL, BAR_FPTM_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPTM_COL, BAR_BPTM_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPTI_COL, BAR_FPTI_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPTI_COL, BAR_BPTI_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPSEP_ONE_COL, BAR_FPSEP_ONE_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPSEP_ONE_COL, BAR_BPSEP_ONE_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_FPSEP_TWO_COL, BAR_FPSEP_TWO_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_BPSEP_TWO_COL, BAR_BPSEP_TWO_COL_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS0, BAR_TEXT_WS0_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS1, BAR_TEXT_WS1_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS2, BAR_TEXT_WS2_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS3, BAR_TEXT_WS3_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS4, BAR_TEXT_WS4_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_WS5, BAR_TEXT_WS5_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_TILE, BAR_TEXT_TILE_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_GRID, BAR_TEXT_GRID_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_DWINDLE, BAR_TEXT_DWINDLE_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_SPIRAL, BAR_TEXT_SPIRAL_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_SEP_1, BAR_TEXT_SEP_1_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_SEP_2, BAR_TEXT_SEP_2_ID, OPTTYPE_ID },
	{ TYPE_BAR_TEXT_SPACE, BAR_TEXT_SPACE_ID, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_0, EXCL_WTYPE_ID_0, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_1, EXCL_WTYPE_ID_1, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_2, EXCL_WTYPE_ID_2, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_3, EXCL_WTYPE_ID_3, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_4, EXCL_WTYPE_ID_4, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_5, EXCL_WTYPE_ID_5, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_6, EXCL_WTYPE_ID_6, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_7, EXCL_WTYPE_ID_7, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_8, EXCL_WTYPE_ID_8, OPTTYPE_ID },
	{ TYPE_EXCL_WTYPE_9, EXCL_WTYPE_ID_9, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_0, INCL_WTYPE_ID_0, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_1, INCL_WTYPE_ID_1, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_2, INCL_WTYPE_ID_2, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_3, INCL_WTYPE_ID_3, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_4, INCL_WTYPE_ID_4, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_5, INCL_WTYPE_ID_5, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_6, INCL_WTYPE_ID_6, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_7, INCL_WTYPE_ID_7, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_8, INCL_WTYPE_ID_8, OPTTYPE_ID },
	{ TYPE_INCL_WTYPE_9, INCL_WTYPE_ID_9, OPTTYPE_ID },
	{ TYPE_CONLINE_0, CONLINE_ID_0, OPTTYPE_ID },
	{ TYPE_CONLINE_1, CONLINE_ID_1, OPTTYPE_ID },
	{ TYPE_CONLINE_2, CONLINE_ID_2, OPTTYPE_ID },
	{ TYPE_CONLINE_3, CONLINE_ID_3, OPTTYPE_ID },
	{ TYPE_CONLINE_4, CONLINE_ID_4, OPTTYPE_ID },
	{ TYPE_CONLINE_5, CONLINE_ID_5, OPTTYPE_ID },
	{ TYPE_CONLINE_6, CONLINE_ID_6, OPTTYPE_ID },
	{ TYPE_CONLINE_7, CONLINE_ID_7, OPTTYPE_ID },
	{ TYPE_CONLINE_8, CONLINE_ID_8, OPTTYPE_ID },
	{ TYPE_CONLINE_9, CONLINE_ID_9, OPTTYPE_ID },
	{ TYPE_CMD_0, CMD_ID_0, OPTTYPE_ID },
	{ TYPE_CMD_1, CMD_ID_1, OPTTYPE_ID },
	{ TYPE_CMD_2, CMD_ID_2, OPTTYPE_ID },
	{ TYPE_CMD_3, CMD_ID_3, OPTTYPE_ID },
	{ TYPE_CMD_4, CMD_ID_4, OPTTYPE_ID },
	{ TYPE_CMD_5, CMD_ID_5, OPTTYPE_ID },
	{ TYPE_CMD_6, CMD_ID_6, OPTTYPE_ID },
	{ TYPE_CMD_7, CMD_ID_7, OPTTYPE_ID },
	{ TYPE_CMD_8, CMD_ID_8, OPTTYPE_ID },
	{ TYPE_CMD_9, CMD_ID_9, OPTTYPE_ID },
	{ TYPE_INFO_OFF, INFO_OFF_ID, OPTTYPE_ID },
	{ TYPE_VWS_ON, VWS_ON_ID, OPTTYPE_ID },
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
        NBU_UNIQUE | NBU_NOTIFY,
        0,
        0,
        0,
        0
};

static short attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj)
{
	short rc = TRUE;
	size_t keyarrsize;
	size_t optarrsize;
	struct Popkeys* keys;
	const char *bar_default_text[BAR_LAST_TEXT];
        bar_default_text[ws_zero] = DEF_BAR_TEXT_WS_ZERO;
        bar_default_text[ws_one] = DEF_BAR_TEXT_WS_ONE;
        bar_default_text[ws_two] = DEF_BAR_TEXT_WS_TWO;
        bar_default_text[ws_three] = DEF_BAR_TEXT_WS_THREE;
        bar_default_text[ws_four] = DEF_BAR_TEXT_WS_FOUR;
        bar_default_text[ws_five] = DEF_BAR_TEXT_WS_FIVE;
        bar_default_text[ws_wb] = DEF_BAR_TEXT_WS_WB;
        bar_default_text[mode_tile] = DEF_BAR_TEXT_MODE_TILE;
        bar_default_text[mode_grid] = DEF_BAR_TEXT_MODE_GRID;
        bar_default_text[mode_dwindle] = DEF_BAR_TEXT_MODE_DWINDLE;
        bar_default_text[mode_spiral]= DEF_BAR_TEXT_MODE_SPIRAL;
        bar_default_text[sep_one]= DEF_BAR_TEXT_SEP_ONE;
        bar_default_text[sep_two]= DEF_BAR_TEXT_SEP_TWO;
        bar_default_text[space]= DEF_BAR_TEXT_SPACE;
        bar_default_text[err]= DEF_BAR_TEXT_ERR;

	exclude_wtype = 0;
	include_wtype = 0;

	optarrsize = sizeof(defopts) / sizeof(*defopts);
        keyarrsize = sizeof(defkeys) / sizeof(*defkeys);

        bar_text = (Bar_Text *)malloc(BAR_LAST_TEXT * sizeof(*bar_text));
        for (int i = 0; i < BAR_LAST_TEXT; i++) {
                bar_text[i].text = malloc(TT_MAX_LENGTH * 2);
		bar_text[i].text[0] = nil;
        }

	for (size_t i = 0; i < optarrsize ; ++i) {
       		const char *tt_optvalue = (char *)FindToolType((const unsigned char **)diskobj->do_ToolTypes, (const unsigned char *)defopts[i].optname);

		if ((tt_optvalue) && ((strnlen(tt_optvalue, TT_MAX_LENGTH) < (size_t)TT_MAX_LENGTH))) {
			if (defopts[i].cxint >= EXCL_WTYPE_ID_0 && defopts[i].cxint <= (WTYPE_MAX + EXCL_WTYPE_ID_0)) {
				rc = alloc_opts(tt_optvalue, excls, i, EXCL_WTYPE_ID_0);
				if (exclude_wtype == 0) {
					exclude_wtype = 1;
				}
			}

			if (defopts[i].cxint >= INCL_WTYPE_ID_0 && defopts[i].cxint <= (WTYPE_MAX + INCL_WTYPE_ID_0)) {
				rc = alloc_opts(tt_optvalue, incls, i, INCL_WTYPE_ID_0);
				if (include_wtype == 0) {
					include_wtype = 1;
				}
			}

			if (defopts[i].cxint >= CONLINE_ID_0 && defopts[i].cxint <= (CMD_MAX + CONLINE_ID_0)) {
				rc = alloc_opts(tt_optvalue, cons, i, CONLINE_ID_0);
			}

			if (defopts[i].cxint >= CMD_ID_0 && defopts[i].cxint <= (CMD_MAX + CMD_ID_0)) {
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
				case BAR_ID:
					bar_on = TRUE;
					break;
				case BAR_BG_COL_ID:
					bar_color[bg].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPW_COL_ID:
					bar_color[fp_ws].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPW_COL_ID:
					bar_color[bp_ws].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPCURW_COL_ID:
					bar_color[fp_cur].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPCURW_COL_ID:
					bar_color[bp_cur].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPTM_COL_ID:
					bar_color[fp_tm].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPTM_COL_ID:
					bar_color[bp_tm].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPTI_COL_ID:
					bar_color[fp_ti].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPTI_COL_ID:
					bar_color[bp_ti].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPSEP_ONE_COL_ID:
					bar_color[fp_sep_one].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPSEP_ONE_COL_ID:
					bar_color[bp_sep_one].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_FPSEP_TWO_COL_ID:
					bar_color[fp_sep_two].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_BPSEP_TWO_COL_ID:
					bar_color[bp_sep_two].color[0] = (unsigned char)strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case BAR_HIDE_EMPTY_ID:
					hidewbar |= BAR_HIDE_ON;
					break;
				case BAR_TEXT_WS0_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_zero, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS1_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_one, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS2_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_two, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS3_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_three, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS4_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_four, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS5_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, ws_five, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_TILE_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, mode_tile, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_GRID_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, mode_grid, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_DWINDLE_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, mode_dwindle, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SPIRAL_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, mode_spiral, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SEP_1_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, sep_one, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SEP_2_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, sep_two, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SPACE_ID:
					rc = rc == TRUE ? assign_bar_item(bar_text, space, tt_optvalue) : FALSE;
					break;
				case AUTO_ID:
					autotile = TRUE;
					break;
				case AUTO_INTERVAL_DELAY_ID:
					auto_interval = strtoul((const char *)tt_optvalue, (char **)NULL, 10);
					break;
				case TILE_FACT_ID:
					fact = (int)strtol((const char*)tt_optvalue, (char **)NULL, 10);
					break;
				case INFO_OFF_ID:
					info_on = FALSE;
					break;
				case VWS_ON_ID:
					vws_on = TRUE;
					break;
				default:
					// Do nothing
					break;
			}
		}
	}

	if (rc == TRUE && bar_on == TRUE) {
                for (int i = 0; i < BAR_LAST_TEXT; i++) {
			if (bar_text[i].text[0] != nil) {
				continue;
			}
                        if ((assign_bar_item(bar_text, i, bar_default_text[i]) == FALSE)) {
                                return FALSE;
                        }
                }

		// Bar color assignment
		if (*bar_color[bg].color == nil) { bar_color[bg].color[0] = DEF_BAR_BG_COL; }
		if (*bar_color[fp_ws].color == nil) { bar_color[fp_ws].color[0] = DEF_BAR_FP_WS_COL; }
		if (*bar_color[bp_ws].color == nil) { bar_color[bp_ws].color[0] = DEF_BAR_BP_WS_COL; }
		if (*bar_color[fp_cur].color == nil) { bar_color[fp_cur].color[0] = DEF_BAR_FP_CUR_COL; }
		if (*bar_color[bp_cur].color == nil) { bar_color[bp_cur].color[0] = DEF_BAR_BP_CUR_COL; }
		if (*bar_color[fp_tm].color == nil) { bar_color[fp_tm].color[0] = DEF_BAR_FP_TM_COL; }
		if (*bar_color[bp_tm].color == nil) { bar_color[bp_tm].color[0] = DEF_BAR_BP_TM_COL; }
		if (*bar_color[fp_ti].color == nil) { bar_color[fp_ti].color[0] = DEF_BAR_FP_TI_COL; }
		if (*bar_color[bp_ti].color == nil) { bar_color[bp_ti].color[0] = DEF_BAR_BP_TI_COL; }
		if (*bar_color[fp_sep_one].color == nil) { bar_color[fp_sep_one].color[0]  = DEF_BAR_FP_SEP_ONE_COL; }
		if (*bar_color[bp_sep_one].color == nil) { bar_color[bp_sep_one].color[0] = DEF_BAR_BP_SEP_ONE_COL; }
		if (*bar_color[fp_sep_two].color == nil) { bar_color[fp_sep_two].color[0] = DEF_BAR_FP_SEP_TWO_COL; }
		if (*bar_color[bp_sep_two].color == nil) { bar_color[bp_sep_two].color[0] = DEF_BAR_BP_SEP_TWO_COL; }
	}

	if (rc == FALSE) {
		return rc;
	}

	if (exclude_wtype != 0) {
		qsort(excls->strings, WTYPE_MAX, sizeof(*excls->strings), cstring_cmp);
	}

	if (include_wtype != 0) {
		qsort(incls->strings, WTYPE_MAX, sizeof(*incls->strings), cstring_cmp);
	}

	keys = malloc(sizeof(*keys) * keyarrsize);

	if (keys != NULL) {
		for (size_t i = 0; i < keyarrsize ; ++i) {
			keys[i].rawcombo = (char *)FindToolType((const unsigned char **)diskobj->do_ToolTypes, (const unsigned char *)defkeys[i].optname);

			if (keys[i].rawcombo == NULL) {
				keys[i].rawcombo = defkeys[i].defaultval;
			}

			if (keys[i].rawcombo != NULL) {
				CxObj *filter;
				if ((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, (long int)i)) != NULL)
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

void init_cmo(struct Cmo *cmo)
{
	unsigned char iconlib[] = "icon.library";
	unsigned char diskobjname[] = "PROGDIR:dintwm";
	enum {
		mpfail,
		iconfail,
		dobjfail,
		brokerfail,
		msigfail,
		ssigfail,
		attfail,
		acxofail
	};
	cmo->mp = NULL;
	cmo->mp = CreateMsgPort();
	if (cmo->mp == NULL) {
		cmo->failarr[mpfail] = -1;
	}
	cmo->iconbase = NULL;
	cmo->iconbase = OpenLibrary(iconlib, 37);
	if (cmo->iconbase == NULL) {
		cmo->failarr[iconfail] = -1;
	}
	cmo->diskobj = NULL;
	cmo->diskobj = GetDiskObject(diskobjname);
	if (cmo->diskobj == NULL) {
		cmo->failarr[dobjfail] = -1;
	}
	MyBroker.nb_Port = cmo->mp;
	cmo->broker = NULL;
	cmo->broker = CxBroker(&MyBroker, NULL);
	if (cmo->broker == NULL) {
		cmo->failarr[brokerfail] = -1;
	}
	cmo->mainsignum = -1;
	cmo->mainsignum = AllocSignal(-1);
	if (cmo->mainsignum == -1L) {
		cmo->failarr[msigfail] = -1;
	}
	cmo->subsignum = -1;
	cmo->subsignum = AllocSignal(-1);
	if (cmo->subsignum == -1L) {
		cmo->failarr[ssigfail] = -1;
	}
	if ((attachtooltypes(cmo->broker, cmo->mp, cmo->diskobj)) != TRUE) {
		cmo->failarr[attfail] = -1;
	}
	if ((ActivateCxObj(cmo->broker, 1)) != 0) {
		cmo->failarr[acxofail] = -1;
	}

	return;
}

short int commo(void)
{
	struct Cmo cmo;
	init_cmo(&cmo);
	auto_interval = (unsigned long)AUTO_INTERVAL_DELAY_DEF;
	size_t cmo_arr_length = sizeof(cmo.failarr) / sizeof(cmo.failarr[0]);
	//printf("%s\n", cmo_fail_msgs[7]);

	for (size_t i = 0; i < cmo_arr_length; i++) {
		if (cmo.failarr[i] == -1) {
			info_window(cmo_fail_msgs[i]);
			cleanup(&cmo);
			return EXIT_FAILURE;
		}
	}

	mainsig = 1UL << (unsigned long)cmo.mainsignum;
	subsig = 1UL << (unsigned long)cmo.subsignum;
	maintask = FindTask(NULL);

	if (running == TRUE)
	{
		CxMsg *msg;
		CloseLibrary(cmo.iconbase);
		cmo.iconbase = NULL;
		FreeDiskObject(cmo.diskobj);
		cmo.diskobj = NULL;

		/*if (ActivateCxObj(cmo.broker, 1) != 0) {
			cleanup(&cmo);
			return EXIT_FAILURE;
		}*/

		// Muting GCC warning here. Following official Amiga CreateTask example
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"
		subtask = CreateTask(subactionchkname, 0L, (void *)subactionchk, 2048L);
		#pragma GCC diagnostic pop
		if(subtask == NULL) {
			running = FALSE;
		}

		if (vws_on == TRUE) {
			(void)countwindows(LOCK);
			getactive();
			if (backdropped == TRUE) {
				if (info_on == TRUE) {
					info_window(warn_messages[BDWARN]);
				}
				bar_on = FALSE;
				cleanup(&cmo);
				return EXIT_FAILURE;
			}
		}

		if (bar_on == TRUE && running == TRUE) {
			if (wbarheight == 0) {
				wbarheight = WBAR_HEIGHT;
			}
			sheight = sheight - wbarheight;

			if (!wbw) {
				running = init_wbar();
			}

			if (running == TRUE && wbw) {
				getactive();
				awin_comp = active_win;
				update_wbar();
			}
		}

		// Initial tile
		running = defkeys[*current_layout].func(&defkeys[*current_layout].arg);

		//Main Loop
		while (running == TRUE)
		{
			winnum_start = countwindows(NOLOCK);

			wakeupsigs = Wait((mainsig) | (1UL << cmo.mp->mp_SigBit));

			if((wakeupsigs & mainsig) != 0UL) {
				if (winfo[awin_index].wbwin == TRUE) {
					Signal(subtask, subsig);
					continue;
				}
				if (tile_off == FALSE) {
					running = defkeys[*current_layout].func(&defkeys[*current_layout].arg);
					update_wbar();
					winnum_start = countwindows(NOLOCK);
				}
				Signal(subtask, subsig);
			}

			while ((msg = (void *)GetMsg(cmo.mp)) != NULL)
			{
				long id = CxMsgID(msg);
				unsigned long type = CxMsgType(msg);

				ReplyMsg((struct Message *)msg);

				if (type == (unsigned long)CXM_COMMAND)
				{
					switch (id)
					{
						case CXCMD_UNIQUE:
							if(info_on == TRUE) {
								info_window(warn_messages[UQWARN]);
							}
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
				} else if (type == (unsigned long)CXM_IEVENT) {
					if (id <= (TILE_FUNC_LIMIT)) {
						*current_layout = id;
					}
					running = defkeys[id].func(&defkeys[id].arg);
					if (running == FALSE) {
						Signal(subtask, subsig);
						(void)Wait((mainsig) | (1UL << cmo.mp->mp_SigBit));
					}
					if (bar_on == TRUE && (hidewbar & BAR_HIDE_TOGGLE) == 0U) {
						wbarcwb();
						update_wbar();
					}
				}
			}
		}
	}

	cleanup(&cmo);

	return EXIT_SUCCESS;
}

static short alloc_opts(const char *t, Ostore *s, size_t i, int subtract)
{
	int cxint = defopts[i].cxint - subtract;

	s->strings[cxint] = malloc((strnlen(t, TT_MAX_LENGTH)+1U) * sizeof(unsigned char));

	if (s->strings[cxint] != NULL) {
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
		if (cons->strings[i] != NULL) {
			free(cons->strings[i]);
			cons->strings[i] = NULL;
		}
		if (cmds->strings[i] != NULL) {
			free(cmds->strings[i]);
			cmds->strings[i] = NULL;
		}
	}

	for (i = 0; i < WTYPE_MAX; ++i) {
		if (excls->strings[i] != NULL) {
			free(excls->strings[i]);
			excls->strings[i] = NULL;
		}
		if (incls->strings[i] != NULL) {
			free(incls->strings[i]);
			incls->strings[i] = NULL;
		}
	}
}

static short assign_bar_item(Bar_Text *b, enum bar_texts x, const char *c)
{
        int res = snprintf((char *)b[x].text, TT_MAX_LENGTH, "%s", c);
        if (res < 0) {
                return FALSE;
        }
        return TRUE;
}


static void subactionchk(void)
{
	struct timeval currentval;
	struct timerequest *tr;

	if (!(tr = create_timer(UNIT_VBLANK))) {
		running = FALSE;
	}

	currentval.tv_secs = 0UL;
	currentval.tv_micro = auto_interval;

	if (first_run == TRUE) {
		Signal(maintask, mainsig);
		first_run = FALSE;
		(void)Wait((subsig));
	}

	while(running == TRUE) {
		time_delay(tr, &currentval);
		int wincnt = countwindows(NOLOCK);
		if (wincnt > (DIVISOR - 2)) {
			running = FALSE;
			info_window(warn_messages[WIWARN]);
			continue;
		}
		if (tile_off == FALSE && autotile == TRUE) {
			if (winnum_start != wincnt) {
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			if (awin_comp == NULL ||
				awin_comp != active_win ||
				(awin_comp->Flags & (unsigned long)WFLG_WINDOWACTIVE) == 0U) {
				getactive();
				awin_comp = active_win;
				update_wbar();
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			// If previous active window is no longer active, refresh bar
			if ((awin_comp->Flags & (unsigned long)WFLG_WINDOWACTIVE) == 0U) {
				getactive();
				// Don't retile if workbench screen
				if (winfo[awin_index].wbwin == TRUE) {
					awin_comp = active_win;
					update_wbar();
					Signal(maintask, mainsig);
					(void)Wait((subsig));
				}
			}

			if (running == FALSE) {
				delete_timer(tr);
				Signal(maintask, mainsig);
				(void)Wait(0L);
			}
		}
	}
}

struct timerequest *create_timer(unsigned long unit)
{
	/* return a pointer to a timer request.  If any problem, return NULL */
	struct MsgPort *timerport;
	signed char error;
	struct timerequest *TimerIO;
	unsigned char tdevice[] = "timer.device";

	timerport = CreatePort(0, 0);

	if (timerport == NULL) {
		return NULL;
	}

	TimerIO =
	    (struct timerequest *)CreateExtIO(timerport,
					      sizeof(struct timerequest));
	if (TimerIO == NULL) {
		DeletePort(timerport);	/* Delete message port */
		return NULL;
	}

	error = OpenDevice(tdevice, unit, (struct IORequest *)TimerIO, 0L);

	if ((int)error != 0) {
		delete_timer(TimerIO);
		return NULL;
	}

	return TimerIO;
}

void time_delay(struct timerequest *tr, const struct timeval *tv) {
	unsigned short traddreq = TR_ADDREQUEST;
	tr->tr_node.io_Command = traddreq;	/* add a new timer request */

	/* structure assignment */
	tr->tr_time = *tv;

	/* post request to the timer -- will go to sleep till done */
	(void)DoIO((struct IORequest *)tr);
}

void delete_timer(struct timerequest *tr) {
	if (tr != 0) {
		struct MsgPort *tp;
		tp = tr->tr_node.io_Message.mn_ReplyPort;

		if (tp != 0) {
			DeletePort(tp);
		}

		if (!(CheckIO((struct IORequest *)tr))) {
			AbortIO((struct IORequest *)tr);    /* Ask device to abort any pending requests */
		}
		(void)WaitIO((struct IORequest *)tr);         /* Clean up */
		CloseDevice((struct IORequest *)tr);
		DeleteExtIO((struct IORequest *)tr);
	}
}

static void cleanup(struct Cmo *cmo)
{
	if (cmo->mp != NULL) {
		CxMsg *msg;
		while ((msg = (void *)GetMsg(cmo->mp)) != NULL) {
			ReplyMsg((struct Message *)msg);
		}
		DeleteMsgPort(cmo->mp);
	}

	if (subtask != NULL) {
		Forbid();
		DeleteTask(subtask);
		Permit();
	}

	if (cmo->mainsignum != -1L) {
		FreeSignal(cmo->mainsignum);
	}
	if (cmo->subsignum != -1L) {
		FreeSignal(cmo->subsignum);
	}
	if (cmo->iconbase != NULL) {
		CloseLibrary(cmo->iconbase);
	}
	if (cmo->diskobj != NULL) {
		FreeDiskObject(cmo->diskobj);
	}
	if (cmo->broker != NULL) {
		DeleteCxObjAll(cmo->broker);
	}

	free_opts();

	if (bar_on == TRUE) {
		if (wbw != NULL) {
			CloseWindow(wbw);
		}
        	for (int i = 0; i < BAR_LAST_TEXT; i++) {
                	free(bar_text[i].text);
        	}
        	free(bar_text);
	}
}
