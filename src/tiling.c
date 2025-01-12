// Copyright 2024 Rasmus Edgar
#include "../include/tiling.h"

int tiling_layout(int action, int layout)
{
	static int layout_number = 0;

	if (action == TL_GET) {
		if (layout_number > TILE_FUNC_LIMIT) {
			layout_number = 0;
		}
		return layout_number;
	}
	if (action == TL_SET) {
		return layout_number = layout;
	}
	if (action == TL_INC) {
		if (layout_number > TILE_FUNC_LIMIT) {
			layout_number = LAYOUT_START;
		}
		return layout_number++;
	}
	if (action == TL_DEC) {
		layout_number--;
		if (layout_number < 0) {
			layout_number = TILE_FUNC_LIMIT;
		}
		return layout_number;
	}
	// Should never be reached
	return layout_number;
}

struct Screen * tiling_lock(int action, struct Screen *s)
{
	static unsigned long il = 0;
	static int lock_state = 0;

	if (action == TLOCK && lock_state == 0 && s == NULL) {
		il = LockIBase(0L);
		s = LockPubScreen(NULL);
		lock_state = 1;
	}
	if (action == TUNLOCK && lock_state == 1) {
		UnlockPubScreen(NULL, s);
		UnlockIBase(il);
		lock_state = 0;
		il = 0;
	}
	return s;
}

int tiling_screen_info(int action, int size)
{
	static int sh = 0;
	static int sw = 0;

	switch (action) {
	case SH_GET:
		return sh;
		break;
	case SH_SET:
		sh = size;
		return sh;
		break;
	case SW_GET:
		return sw;
		break;
	case SW_SET:
		sw = size;
		return sw;
		break;
	default:
		// Never reached
		break;
	}
	return -1;
}

int tiling_screen_width(void)
{
	static int sw = 0;

	if (sw == 0) {
		struct Screen *scr = tiling_lock(TLOCK, NULL);
		sw = (int)scr->Width;
		(void)tiling_lock(TUNLOCK, scr);
	}
	return sw;
}

int tiling_screen_height(void)
{
	static int sh = 0;

	if (sh == 0) {
		struct Screen *scr = tiling_lock(TLOCK, NULL);
		sh = (int)scr->Height;
		(void)tiling_lock(TUNLOCK, scr);
	}
	return sh;
}

int tiling_calc_menugap(void)
{
	static int bheight = 0;

	if (bheight == 0) {
		struct Screen *scr = tiling_lock(TLOCK, NULL);
		bheight = (int)scr->BarHeight + 1;
		(void)tiling_lock(TUNLOCK, scr);
	}

	return bheight;
}

int tiling_gaps(int action, int amount)
{
	static int sh = 0;
	static int tgap = DEFAULT_TOPGAP;
	static int bgap = 0;
	static int lgap = 0;
	static int rgap = 0;
	static int gcv = GAP_CHANGE_VALUE_DEF;
	static int sw = 0;
	sw = sw == 0 ? tiling_screen_width() : sw;
	sh = sh == 0 ? tiling_screen_height() : sh;

	switch(action) {
	case TOPGAP_ID:
		tgap += tgap <= ((sh - GAP_INC_OFFSET) / 2) ? gcv : 0;
		return tgap;
		break;
	case -TOPGAP_ID:
		tgap = (tgap - gcv) > 0 ? tgap - gcv : 0;
		return tgap;
		break;
	case TOPGAP_SET:
		tgap = amount;
		return tgap;
		break;
	case TOPGAP_GET:
		return tgap;
		break;
	case BOTTOMGAP_ID:
		bgap += bgap <= ((sh - GAP_INC_OFFSET) / 2) ? gcv : 0;
		return bgap;
		break;
	case -BOTTOMGAP_ID:
		bgap = (bgap - gcv) > 0 ? bgap - gcv : 0;
		return bgap;
		break;
	case BOTTOMGAP_SET:
		bgap = amount;
		return bgap;
		break;
	case BOTTOMGAP_GET:
		return bgap;
		break;
	case LEFTGAP_ID:
		lgap += lgap <= ((sw - GAP_INC_OFFSET) / 2) ? gcv : 0;
		return lgap;
		break;
	case -LEFTGAP_ID:
		lgap = (lgap - gcv) > 0 ? lgap - gcv : 0;
		return lgap;
		break;
	case LEFTGAP_SET:
		lgap = amount;
		return lgap;
		break;
	case LEFTGAP_GET:
		return lgap;
		break;
	case RIGHTGAP_ID:
		rgap += rgap <= ((sw - GAP_INC_OFFSET) / 2) ? gcv : 0;
		return rgap;
		break;
	case -RIGHTGAP_ID:
		rgap = (rgap - gcv) > 0 ? rgap - gcv : 0;
		return rgap;
		break;
	case RIGHTGAP_SET:
		rgap = amount;
		return rgap;
		break;
	case RIGHTGAP_GET:
		return rgap;
		break;
	case GAP_CHANGE_VAL_SET:
		gcv = amount;
		return gcv;
		break;
	case GAP_CHANGE_VAL_GET:
		return gcv;
		break;
	default:
		// Never reached
		break;
	}

	return -1;
}
