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

struct Screen * tiling_lock(int action)
{
	static unsigned long il = 0;
	static struct Screen *s = NULL;
	static int lock_state = 0;

	if (action == TLOCK && lock_state == 0) {
		il = LockIBase(0L);
		s = LockPubScreen(NULL);
		lock_state = 1;
	}
	if (action == TUNLOCK && lock_state == 1) {
		UnlockPubScreen(NULL, s);
		UnlockIBase(il);
		lock_state = 0;
	}
	return s;
}

int tiling_screen_info(int action, int size)
{
	static int sh = 0;
	static int sw = 0;
	int sr = 0;

	switch (action) {
	case SH_GET:
		sr = sh;
		break;
	case SH_SET:
		sr = sh = size;
		break;
	case SW_GET:
		sr = sw;
		break;
	case SW_SET:
		sr = sw = size;
		break;
	default:
		// Never reached
		break;
	}
	return sr;
}

int tiling_screen_width(void)
{
	static int sw = 0;

	if (sw == 0) {
		const struct Screen *scr = tiling_lock(TLOCK);
		sw = (int)scr->Width;
		(void)tiling_lock(TUNLOCK);
	}
	return sw;
}

int tiling_screen_height(void)
{
	static int sh = 0;

	if (sh == 0) {
		const struct Screen *scr = tiling_lock(TLOCK);
		sh = (int)scr->Height;
		(void)tiling_lock(TUNLOCK);
	}
	return sh;
}

int tiling_calc_menugap(void)
{
	static int bheight = 0;

	if (bheight == 0) {
		const struct Screen *scr = tiling_lock(TLOCK);
		bheight = (int)scr->BarHeight + 1;
		(void)tiling_lock(TUNLOCK);
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
	int gap = 0;
	static int sw = 0;
	sw = sw == 0 ? tiling_screen_width() : sw;
	sh = sh == 0 ? tiling_screen_height() : sh;

	switch(action) {
	case TOPGAP_ID:
		tgap += tgap <= ((sh - GAP_INC_OFFSET) / 2) ? gcv : 0;
		gap = tgap;
		break;
	case -TOPGAP_ID:
		tgap = (tgap - gcv) > 0 ? tgap - gcv : 0;
		gap = tgap;
		break;
	case TOPGAP_SET:
		gap = tgap = amount;
		break;
	case TOPGAP_GET:
		gap = tgap;
		break;
	case BOTTOMGAP_ID:
		gap = bgap += bgap <= ((sh - GAP_INC_OFFSET) / 2) ? gcv : 0;
		break;
	case -BOTTOMGAP_ID:
		gap = bgap = (bgap - gcv) > 0 ? bgap - gcv : 0;
		break;
	case BOTTOMGAP_SET:
		gap = bgap = amount;
		break;
	case BOTTOMGAP_GET:
		gap = bgap;
		break;
	case LEFTGAP_ID:
		gap = lgap += lgap <= ((sw - GAP_INC_OFFSET) / 2) ? gcv : 0;
		break;
	case -LEFTGAP_ID:
		gap = lgap = (lgap - gcv) > 0 ? lgap - gcv : 0;
		break;
	case LEFTGAP_SET:
		gap = lgap = amount;
		break;
	case LEFTGAP_GET:
		gap = lgap;
		break;
	case RIGHTGAP_ID:
		gap = rgap += rgap <= ((sw - GAP_INC_OFFSET) / 2) ? gcv : 0;
		break;
	case -RIGHTGAP_ID:
		gap = rgap = (rgap - gcv) > 0 ? rgap - gcv : 0;
		break;
	case RIGHTGAP_SET:
		gap = rgap = amount;
		break;
	case RIGHTGAP_GET:
		gap = rgap;
		// Fallthrough
		break;
	case GAP_CHANGE_VAL_SET:
		gap = gcv = amount;
		break;
	case GAP_CHANGE_VAL_GET:
		gap = gcv;
		break;
	default:
		// Never reached
		break;
	}

	return gap;
}
