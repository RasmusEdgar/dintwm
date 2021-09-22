// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
// Copyright 2021 Rasmus Edgar
#include "./dintwm.h"
#include "./cxm_config.h"

static unsigned char COMMODITY_NAME[] = "DintWM commodity";
static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager";
static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";

static short attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj);
static short alloc_opts(char *tt_optvalue, Ostore *s, size_t i, int subtract);
static void free_opts(void);
static short alloc_bar_item(unsigned char **b, const char * s);

static struct Library *iconbase;
int fact = TILE_FACT_DEF;
int gap_change_value = GAP_CHANGE_VALUE_DEF;
short info_on = TRUE;
void subactionchk(void);
static inline __attribute__((always_inline)) unsigned long hash(unsigned char *str);
unsigned long winhashes(void);
unsigned long whash_start;
static inline __attribute__((always_inline)) unsigned char* twocat(unsigned char* dest, unsigned char* src);
short alloc_wtstring(void);

long mainsignum = -1;
long subsignum = -1;
unsigned long mainsig, wakeupsigs, subsig, subwake;
struct Task *maintask = NULL, *subtask = NULL;
unsigned char subactionchkname[] = "CXM_window_state_checker";
static short first_run = TRUE;
static struct Window *awin_comp;
short running = TRUE;
unsigned char *wtstring;
unsigned int wtincrementer = 1;
static void cleanup(void);

Keys defkeys[] = {
	{ TYPE_TILE, KEY_TILE, KEYTYPE_ID, tile, {0} },
	{ TYPE_HGRID, KEY_HGRID, KEYTYPE_ID, hgrid, {0} },
	{ TYPE_SPIRAL, KEY_SPIRAL, KEYTYPE_ID, fibonacci, {.i = 0} },
	{ TYPE_DWINDLE, KEY_DWINDLE, KEYTYPE_ID, fibonacci, {.i = 1} },
	{ TYPE_SWITCHF, KEY_SWITCHF, KEYTYPE_ID, switcher, {.i = 1} },
	{ TYPE_SWITCHB, KEY_SWITCHB, KEYTYPE_ID, switcher, {.i = 0} },
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
        NBU_UNIQUE | NBU_NOTIFY, //-V2544
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

	exclude_wtype = 0;
	include_wtype = 0;

	optarrsize = sizeof(defopts) / sizeof(*defopts);
        keyarrsize = sizeof(defkeys) / sizeof(*defkeys);

	for (size_t i = 0; i < optarrsize ; ++i) {
       		char *tt_optvalue = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defopts[i].optname);

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
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_zero].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS1_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_one].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS2_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_two].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS3_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_three].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS4_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_four].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_WS5_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_five].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_TILE_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_tile].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_GRID_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_grid].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_DWINDLE_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_dwindle].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SPIRAL_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_spiral].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SEP_1_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[sep_one].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SEP_2_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[sep_two].text, tt_optvalue) : FALSE;
					break;
				case BAR_TEXT_SPACE_ID:
					rc = rc == TRUE ? alloc_bar_item(&bar_text[space].text, tt_optvalue) : FALSE;
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
		// Bar text allocation
		rc = alloc_bar_item(&bar_text[ws_zero].text, DEF_BAR_TEXT_WS_ZERO);
		rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_one].text, DEF_BAR_TEXT_WS_ONE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_two].text, DEF_BAR_TEXT_WS_TWO) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_three].text, DEF_BAR_TEXT_WS_THREE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_four].text, DEF_BAR_TEXT_WS_FOUR) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[ws_five].text, DEF_BAR_TEXT_WS_FIVE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_tile].text, DEF_BAR_TEXT_MODE_TILE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_grid].text, DEF_BAR_TEXT_MODE_GRID) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_dwindle].text, DEF_BAR_TEXT_MODE_DWINDLE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[mode_spiral].text, DEF_BAR_TEXT_MODE_SPIRAL) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[sep_one].text, DEF_BAR_TEXT_SEP_ONE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[sep_two].text, DEF_BAR_TEXT_SEP_TWO) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[space].text, DEF_BAR_TEXT_SPACE) : FALSE;
		rc = rc == TRUE ? alloc_bar_item(&bar_text[err].text, DEF_BAR_TEXT_ERR) : FALSE;

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

	if (exclude_wtype) {
		qsort(excls->strings, WTYPE_MAX, sizeof(*excls->strings), cstring_cmp);
	}

	if (include_wtype) {
		qsort(incls->strings, WTYPE_MAX, sizeof(*incls->strings), cstring_cmp);
	}

	keys = malloc(sizeof(*keys) * keyarrsize);

	if (keys != NULL) {
		for (size_t i = 0; i < keyarrsize ; ++i) {
			keys[i].rawcombo = (char *)FindToolType(diskobj->do_ToolTypes, (unsigned char *)defkeys[i].optname);

			if (keys[i].rawcombo == NULL) {
				keys[i].rawcombo = defkeys[i].defaultval;
			}

			if (keys[i].rawcombo != NULL) {
				CxObj *filter;
				if ((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, (long int)i)))
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
	struct MsgPort *mp = CreateMsgPort();
	static struct DiskObject *diskobj;
	static unsigned char iconlib[] = "icon.library";
	static unsigned char diskobjname[] = "dintwm";

	auto_interval = (unsigned long)AUTO_INTERVAL_DELAY_DEF;

	if (!(iconbase = OpenLibrary(iconlib, 37))) {
		DeleteMsgPort(mp);
		return EXIT_FAILURE;
	}

	if ((diskobj = GetDiskObject(diskobjname)) == NULL) {
		DeleteMsgPort(mp);
		return EXIT_FAILURE;
	}

	if ((mainsignum = AllocSignal(-1)) == -1L) {
		DeleteMsgPort(mp);
		return EXIT_FAILURE;
	}

	if ((subsignum = AllocSignal(-1)) == -1L) {
		DeleteMsgPort(mp);
		return EXIT_FAILURE;
	}

	mainsig = 1UL << (unsigned long)mainsignum;
	subsig = 1UL << (unsigned long)subsignum;
	maintask = FindTask(NULL);

	if (mp)
	{
		CxObj *broker;

		MyBroker.nb_Port = mp;
		broker = CxBroker(&MyBroker, NULL);

		if (broker == NULL) {
			running = FALSE;
		}

		if (running == TRUE && (attachtooltypes(broker, mp, diskobj) == TRUE))
		{
			CxMsg *msg;
			CloseLibrary(iconbase);
			FreeDiskObject(diskobj);


			if (ActivateCxObj(broker, 1) != 0) {
				DeleteMsgPort(mp);
				return 1;
			}


			if (bar_on) {
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

			if (vws_on == TRUE) {
				(void)countwindows(1);
				getactive();
				if (backdropped == TRUE) {
					if (info_on) {
						info_window(bdwarn);
					}
					running = FALSE;
				}
			}

			// Muting GCC warning here. Following official Amiga CreateTask example
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wpedantic"
			subtask = CreateTask(subactionchkname, 0L, (void *)subactionchk, 2048L);
			#pragma GCC diagnostic pop
			if(!subtask) {
				running = FALSE;
			}

			if (running) {
				running = alloc_wtstring();
			}
			//Main Loop
			while (running)
			{
				running = alloc_wtstring();
				whash_start = winhashes();

				wakeupsigs = Wait((mainsig) | (1UL << mp->mp_SigBit));

				if(wakeupsigs & mainsig) {
					if (tile_off == FALSE) {
						running = defkeys[*current_layout].func(&defkeys[*current_layout].arg);
						update_wbar();
					}
					Signal(subtask, subsig);
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
								if(info_on) {
									info_window(uqwarn);
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
					} else if (type == (unsigned long)CXM_IEVENT) { //-V2544
						if (id <= (TILE_FUNC_LIMIT)) {
							*current_layout = id;
						}
						running = defkeys[id].func(&defkeys[id].arg);
						if (bar_on == TRUE && (hidewbar & BAR_HIDE_TOGGLE) == 0U) {
							wbarcwb();
							update_wbar();
						}
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

	cleanup();

	return EXIT_SUCCESS;
}

static short alloc_opts(char *t, Ostore *s, size_t i, int subtract)
{
	int cxint = defopts[i].cxint - subtract;

	s->strings[cxint] = malloc((strnlen(t, TT_MAX_LENGTH)) * sizeof(unsigned char));

	if (s->strings[cxint]) {
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
		if (cons->strings[i]) {
			free(cons->strings[i]);
		}
		if (cmds->strings[i]) {
			free(cmds->strings[i]);
		}
	}

	for (i = 0; i < WTYPE_MAX; ++i) {
		if (excls->strings[i]) {
			free(excls->strings[i]);
		}
		if (incls->strings[i]) {
			free(incls->strings[i]);
		}
	}
}

static short alloc_bar_item(unsigned char **b, const char * s)
{
	if (**b == nil) {
		if ((*b = malloc((strnlen(s, TT_MAX_LENGTH)+1U))) == NULL) {
			return FALSE;
		}
		(void)snprintf((char *)*b, TT_MAX_LENGTH, "%s", s);
	}

	return TRUE;
}

void subactionchk(void)
{
	struct timeval currentval;
	struct timerequest *tr;

	if (!(tr = create_timer(UNIT_VBLANK))) {
		running = FALSE;
	}

	currentval.tv_secs = 0UL;
	currentval.tv_micro = auto_interval;

	if (first_run) {
		Signal(maintask, mainsig);
		first_run = FALSE;
		(void)Wait((subsig));
	}

	while(running) {
		time_delay(tr, &currentval);
		if (tile_off == FALSE && autotile == TRUE) {
			if (awin_comp == NULL || awin_comp != active_win) {
				getactive();
				awin_comp = active_win;
				update_wbar();
			}

			// If previous active window is no longer active, refresh bar
			if ((awin_comp->Flags & (unsigned long)WFLG_WINDOWACTIVE) == 0U) {
				getactive();
				awin_comp = active_win;
				update_wbar();
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			if (whash_start != (winhashes())) {
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			if (running == FALSE) {
				delete_timer(tr);
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
		return (NULL);
	}

	TimerIO =
	    (struct timerequest *)CreateExtIO(timerport, //-V2545
					      sizeof(struct timerequest));

	if (TimerIO == NULL) {
		DeletePort(timerport);	/* Delete message port */
		return (NULL);
	}

	error = OpenDevice(tdevice, unit, (struct IORequest *)TimerIO, 0L); //-V2545

	if ((int)error != 0) {
		delete_timer(TimerIO);
		return (NULL);
	}

	return (TimerIO);
}

void time_delay(struct timerequest *tr, struct timeval *tv) {
	unsigned short traddreq = TR_ADDREQUEST;
	tr->tr_node.io_Command = traddreq;	/* add a new timer request */

	/* structure assignment */
	tr->tr_time = *tv;

	/* post request to the timer -- will go to sleep till done */
	(void)DoIO((struct IORequest *)tr); //-V2545
}

void delete_timer(struct timerequest *tr) {
	if (tr != 0) {
		struct MsgPort *tp;
		tp = tr->tr_node.io_Message.mn_ReplyPort;

		if (tp != 0) {
			DeletePort(tp);
		}

		CloseDevice((struct IORequest *)tr); //-V2545
		DeleteExtIO((struct IORequest *)tr); //-V2545
	}
}

short alloc_wtstring(void)
{
	if (wtstring == NULL) {
		// Use calloc to skip nul terminator later
		if ((wtstring = (unsigned char *)calloc(WTSTRING_INIT_SIZE, sizeof(unsigned char))) == NULL) {
			return FALSE;
		} else {
			return TRUE;
		}
	}

	// Check if string is about to overflow (use 4 bytes offset to be extra safe)
	// realloc window string with WTSTRING_INIT_SIZE times wtincrementer
	if ((strnlen((const char *)wtstring, (WTSTRING_INIT_SIZE * wtincrementer))) > ((WTSTRING_INIT_SIZE * wtincrementer) - 4U)) {
		unsigned char * tmp  = (unsigned char *)realloc(wtstring, WTSTRING_INIT_SIZE * wtincrementer);
		if (tmp == NULL) {
			return FALSE;
		} else {
			wtstring = tmp;
			wtincrementer++;
			return TRUE;
		}
	}

	return TRUE;
}

unsigned long winhashes(void)
{
	unsigned char *d = wtstring;
	struct Window *w;

	w = screen->FirstWindow;
	while (w->NextWindow != NULL) {
		d = twocat(d, w->Title);
		w = w->NextWindow;
	}
	*d++ = nil;

	return hash(wtstring);
}

static inline __attribute__((always_inline)) unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = (int)*str++)) {
		hash = ((hash << 5UL) + hash) + (unsigned long)c; /* hash * 33 + c */
	}

	return hash;
}

static inline __attribute__((always_inline)) unsigned char* twocat(unsigned char* dest, unsigned char* src)
{
	if (src) {
		*dest++ = *src++;
		*dest++ = *src++;
		return dest;
	} else {
		unsigned char n = 'n';
		unsigned char t = 't';
		*dest++ = n;
		*dest++ = t;
		return dest;
	}
}

static void cleanup(void)
{
	Forbid();
	DeleteTask(subtask);
	Permit();
	FreeSignal(mainsignum);
	FreeSignal(subsignum);

	free_opts();
	free(wtstring);

	current_ws &= ~(WS_0|WS_1|WS_2|WS_3|WS_4|WS_5);

	//lockbasescreen(&ilock, &screen);
	for (struct Window *window = screen->FirstWindow; window;
	     window = window->NextWindow) {
		if (window->ExtData) {
			window->ExtData = NULL;
		}
	}
	//unlockbasescreen(&ilock, &screen);

	if (bar_on) {
		CloseWindow(wbw);
		for (int i = 0; i < BAR_LAST_TEXT; ++i) {
			free(&bar_text[i].text);
		}
	}
}
