// Copyright 2025 Rasmus Edgar
#include "../include/dintwm_shared.h"

static short alloc_opts(const char *t, Ostore *s, size_t i, int subtract);

short option_bool(int action, short b)
{
	static short bdropped = FALSE;
	static short tiling_off = FALSE;
	static short baron = FALSE;
	static short vwson = FALSE;
	static short infoon = TRUE;
	static short firstrun = TRUE;
	static short auto_tile = FALSE;
	static short exc_wtype = FALSE;
	static short inc_wtype = FALSE;

	switch(action) {
	case BACKDROP_SET:
		return bdropped = b;
		break;
	case BACKDROP_GET:
		return bdropped;
		break;
	case TILE_OFF_SET:
		return tiling_off = b;
		break;
	case TILE_OFF_GET:
		return tiling_off;
		break;
	case BAR_ON_SET:
		return baron = b;
		break;
	case BAR_ON_GET:
		return baron;
		break;
	case VWS_ON_SET:
		return vwson = b;
		break;
	case VWS_ON_GET:
		return vwson;
		break;
	case INFO_ON_SET:
		return infoon = b;
		break;
	case INFO_ON_GET:
		return infoon;
		break;
	case FIRST_RUN_GET:
		return firstrun = b;
		break;
	case FIRST_RUN_SET:
		return firstrun;
		break;
	case AUTOTILE_SET:
		return auto_tile = b;
		break;
	case AUTOTILE_GET:
		return auto_tile;
		break;
	case EXCLUDE_WTYPE_SET:
		return exc_wtype = b;
		break;
	case EXCLUDE_WTYPE_GET:
		return exc_wtype;
		break;
	case INCLUDE_WTYPE_SET:
		return inc_wtype = b;
		break;
	case INCLUDE_WTYPE_GET:
		return inc_wtype;
		break;
	default:
		// Never reached
		break;
	}
	return -1;
}

unsigned long option_ul(int action, unsigned long amount)
{
	static unsigned long int autointerval = AUTO_INTERVAL_DELAY_DEF;

	switch(action) {
	case AUTO_INTERVAL_DELAY_ID:
		return  autointerval = (unsigned long)amount;
		break;
	case AUTO_INTERVAL_DELAY_GET:
		return autointerval;
		break;
	default:
		// Never reached
		break;
	}
	return 0UL;
}

int option(int action, int amount)
{
	static int wbar_height = WBAR_HEIGHT;
	static int factor = TILE_FACT_DEF;

	switch(action) {
	case WBAR_HEIGHT_SET:
		return wbar_height = amount;
		break;
	case WBAR_HEIGHT_GET:
		return wbar_height;
		break;
	case TILE_FACT_ID:
		return factor = amount;
		break;
	case TILE_FACT_GET:
		return factor;
		break;
	default:
		// Never reached
		break;
	}
	return -1;
}

static short alloc_opts(const char *t, Ostore *s, size_t i, int subtract)
{
        int cxint = defopts[i].cxint - subtract;

        s->strings[cxint] = malloc((strnlen(t, TT_MAX_LENGTH) + 1U) * sizeof(unsigned char));

        if (s->strings[cxint] != NULL) {
                (void)snprintf((char *)s->strings[cxint], TT_MAX_LENGTH, "%s", t);
                return (TRUE);
        } else {
                return (FALSE);
        }
}

short apply_options(Opts const *dopts, const char *tt_optvalue, size_t i)
{
        short rc = TRUE;

        if (dopts[i].cxint >= EXCL_WTYPE_ID_0 && dopts[i].cxint <= (WTYPE_MAX + EXCL_WTYPE_ID_0)) {
                rc = alloc_opts(tt_optvalue, excls, i, EXCL_WTYPE_ID_0);
                if (!option_bool(EXCLUDE_WTYPE_GET, TRUE)) {
                        (void)option_bool(EXCLUDE_WTYPE_SET, TRUE);
                }
        }

        if (dopts[i].cxint >= INCL_WTYPE_ID_0 && dopts[i].cxint <= (WTYPE_MAX + INCL_WTYPE_ID_0)) {
                rc = alloc_opts(tt_optvalue, incls, i, INCL_WTYPE_ID_0);
                if (!option_bool(INCLUDE_WTYPE_GET, TRUE)) {
                        (void)option_bool(INCLUDE_WTYPE_SET, TRUE);
                }
        }

        if (dopts[i].cxint >= CONLINE_ID_0 && dopts[i].cxint <= (CMD_MAX + CONLINE_ID_0)) {
                rc = alloc_opts(tt_optvalue, cons, i, CONLINE_ID_0);
        }

        if (dopts[i].cxint >= CMD_ID_0 && dopts[i].cxint <= (CMD_MAX + CMD_ID_0)) {
                rc = alloc_opts(tt_optvalue, cmds, i, CMD_ID_0);
        }

        switch (dopts[i].cxint) {
        case TOPGAP_ID:
                (void)tiling_gaps(TOPGAP_SET, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case DEFAULT_TOPGAP_ID:
                (void)tiling_gaps(TOPGAP_SET, tiling_calc_menugap());
                break;
        case BOTTOMGAP_ID:
                (void)tiling_gaps(BOTTOMGAP_SET, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case LEFTGAP_ID:
                (void)tiling_gaps(LEFTGAP_SET, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case RIGHTGAP_ID:
                (void)tiling_gaps(RIGHTGAP_SET, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case GAP_CHANGE_VALUE_ID:
                (void)tiling_gaps(GAP_CHANGE_VAL_SET, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case BAR_ID:
                (void)option_bool(BAR_ON_SET, TRUE);
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
                (void)option_bool(AUTOTILE_SET, TRUE);
                break;
        case AUTO_INTERVAL_DELAY_ID:
                (void)option_ul(AUTO_INTERVAL_DELAY_ID, strtoul((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case TILE_FACT_ID:
                (void)option(TILE_FACT_ID, (int)strtol((const char *)tt_optvalue, (char **)NULL, 10));
                break;
        case INFO_OFF_ID:
                (void)option_bool(INFO_ON_SET, TRUE);
                break;
        case VWS_ON_ID:
                (void)option_bool(VWS_ON_SET, TRUE);
                break;
        default:
                // Do nothing
                break;
        }
        return rc;
}
