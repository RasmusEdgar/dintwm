// Copyright 2025 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/wbar_config.h"

static inline void mapws(void);
static inline unsigned char *maptm(void);

short assign_bar_item(Bar_Text *b, enum bar_texts x, const char *c)
{
        int res = snprintf((char *)b[x].text, TT_MAX_LENGTH, "%s", c);
        if (res < 0) {
                return FALSE;
        }
        return TRUE;
}

static inline void mapws(void)
{
	short vws_on = option_bool(VWS_ON_GET, TRUE);

	if (vws_on == FALSE) {
		wstext_zero.FrontPen = *bar_color[fp_ws].color;
		wstext_zero.BackPen = *bar_color[bp_ws].color;
		return;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_0) {
		wstext_one.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen = wstext_four.FrontPen = wstext_five.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_one.BackPen = wstext_two.BackPen = wstext_three.BackPen = wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_zero.FrontPen = *bar_color[fp_cur].color;
		wstext_zero.BackPen = *bar_color[bp_cur].color;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_1) {
		wstext_zero.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen = wstext_four.FrontPen = wstext_five.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_two.BackPen = wstext_three.BackPen = wstext_four.BackPen = wstext_five.BackPen =
		    *bar_color[bp_ws].color;;

		wstext_one.FrontPen = *bar_color[fp_cur].color;
		wstext_one.BackPen = *bar_color[bp_cur].color;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_2) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_three.FrontPen = wstext_four.FrontPen = wstext_five.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_three.BackPen = wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_two.FrontPen = *bar_color[fp_cur].color;
		wstext_two.BackPen = *bar_color[bp_cur].color;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_3) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen = wstext_four.FrontPen = wstext_five.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen = wstext_four.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_three.FrontPen = *bar_color[fp_cur].color;
		wstext_three.BackPen = *bar_color[bp_cur].color;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_4) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen = wstext_five.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen = wstext_three.BackPen = wstext_five.BackPen = *bar_color[bp_ws].color;

		wstext_four.FrontPen = *bar_color[fp_cur].color;
		wstext_four.BackPen = *bar_color[bp_cur].color;
	}

	if ((window_current_ws(WS_GET, 0)) == WS_5) {
		wstext_zero.FrontPen = wstext_one.FrontPen = wstext_two.FrontPen = wstext_three.FrontPen = wstext_four.FrontPen =
		    *bar_color[fp_ws].color;

		wstext_zero.BackPen = wstext_one.BackPen = wstext_two.BackPen = wstext_three.BackPen = wstext_four.BackPen = *bar_color[bp_ws].color;

		wstext_five.FrontPen = *bar_color[fp_cur].color;
		wstext_five.BackPen = *bar_color[bp_cur].color;
	}
}

static inline unsigned char *maptm(void)
{
	int t_layout = tiling_layout(TL_GET, 0);
	if (t_layout == 0) {
		return bar_text[mode_tile].text;
	}
	if (t_layout == 1) {
		return bar_text[mode_grid].text;
	}
	if (t_layout == 2) {
		return bar_text[mode_dwindle].text;
	}
	if (t_layout == 3) {
		return bar_text[mode_spiral].text;
	}

	return bar_text[err].text;
}

void wbarcwb(void)
{
	int bgap = tiling_gaps(BOTTOMGAP_GET, 0);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int wbar_height = option(WBAR_HEIGHT_GET, 0);
	struct Window *wbarw = window_wbar(NULL);

	if ((option_bool(BAR_ON_GET, TRUE)) == FALSE) {
		return;
	}

	struct IntuiMessage *msg;

	struct Screen *scr = tiling_lock(TLOCK, NULL);

	cwb(wbarw, lgap, scr->Height - bgap, scr->Width - (lgap + rgap), wbar_height);

	(void)tiling_lock(TUNLOCK, scr);

	(void)WaitPort(wbarw->UserPort);
	while ((msg = (struct IntuiMessage *)GetMsg(wbarw->UserPort)) != NULL) {
		if (msg->Class == (unsigned long)IDCMP_SIZEVERIFY) {
			ReplyMsg((struct Message *)msg);
		}
	}
}

short update_wbar(void)
{
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int swidth = tiling_screen_info(SW_GET, 0);
	int wbar_height = option(WBAR_HEIGHT_GET, 0);
	struct Window *wbarw = window_wbar(NULL);

	if ((option_bool(BAR_ON_GET, TRUE)) == FALSE) {
		return TRUE;
	}
	static short barbdata[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	barbdata[0] = 1;
	barbdata[1] = 1;
	barbdata[2] = (short)((swidth - (lgap + rgap)) - 1);
	barbdata[3] = 1;
	barbdata[4] = (short)((swidth - (lgap + rgap)) - 1);
	barbdata[5] = (short)(wbar_height - 1);
	barbdata[6] = 1;
	barbdata[7] = (short)(wbar_height - 1);
	barbdata[8] = 1;
	barbdata[9] = 1;

	mapws();
	wbarmodetext.IText = maptm();

	(void)snprintf((char *)awintitle, TT_MAX_LENGTH, "%s", (window_active(AW_GET, 0))->Title);

	wbarwtitle.IText = awintitle;

	barb.XY = barbdata;

	SetRast(wbarw->RPort, *bar_color[bg].color);
	PrintIText(wbarw->RPort, &wstext_zero, 4, 0);
	DrawBorder(wbarw->RPort, &barb, 0, 0);

	return TRUE;
}

static short int wbartextwidth(int lei, unsigned char *it)
{
	struct TextExtent *barte = &teinit;
	short int le = (short int)lei;
	struct Window *wbarw = window_wbar(NULL);

	if (TextExtent(wbarw->RPort, it, strnlen((const char *)it, TT_MAX_LENGTH), barte) != 0) {
		if (barte == NULL) {
			return FALSE;
		}
	}
	return (short int)(le + (short int)barte->te_Width);
}

static unsigned char *padwbartext(Bar_Text *b, enum bar_texts x)
{
	const unsigned char *tmp = b[x].text;
	(void)snprintf((char *)b[x].text, TT_MAX_LENGTH * 2, "%s%s", tmp, bar_text[space].text);
	return b[x].text;
}

short init_wbar(void)
{
	int bgap = tiling_gaps(BOTTOMGAP_SET, WBAR_HEIGHT);
	int lgap = tiling_gaps(LEFTGAP_GET, 0);
	int rgap = tiling_gaps(RIGHTGAP_GET, 0);
	int wbar_height = option(WBAR_HEIGHT_GET, 0);
	short vws_on = option_bool(VWS_ON_GET, TRUE);

	struct TagItem tagitem[7];

	struct Screen *scr = tiling_lock(TLOCK, NULL);

	tagitem[0].ti_Tag = WA_Width;
	tagitem[0].ti_Data = (unsigned long)scr->Width - ((unsigned long)lgap + (unsigned long)rgap);
	tagitem[1].ti_Tag = WA_Height;
	tagitem[1].ti_Data = (unsigned long)wbar_height;
	tagitem[2].ti_Tag = WA_Top;
	tagitem[2].ti_Data = ((unsigned long)scr->Height - (unsigned long)bgap);
	tagitem[3].ti_Tag = WA_Borderless;
	tagitem[3].ti_Data = 1;
	tagitem[4].ti_Tag = WA_SmartRefresh;
	tagitem[4].ti_Data = 1;
	tagitem[5].ti_Tag = WA_IDCMP;
	tagitem[5].ti_Data = IDCMP_REFRESHWINDOW | IDCMP_CHANGEWINDOW;
	tagitem[6].ti_Tag = TAG_DONE;

	struct Window *wbarw = window_wbar(OpenWindowTagList(NULL, tagitem));

	if (!wbarw) {
		(void)tiling_lock(TUNLOCK, scr);
		return FALSE;
	}
	(void)tiling_lock(TUNLOCK, scr);

	WindowToFront(wbarw);

	if (vws_on == TRUE) {
		wstext_five = wbartext;
		wstext_five.IText = padwbartext(bar_text, ws_five);

		wstext_four = wbartext;
		wstext_four.IText = padwbartext(bar_text, ws_four);

		wstext_three = wbartext;
		wstext_three.IText = padwbartext(bar_text, ws_three);

		wstext_two = wbartext;
		wstext_two.IText = padwbartext(bar_text, ws_two);

		wstext_one = wbartext;
		wstext_one.IText = padwbartext(bar_text, ws_one);

		wstext_zero = wbartext;
		wstext_zero.IText = padwbartext(bar_text, ws_zero);
	} else {
		wstext_zero = wbartext;
		wstext_zero.IText = padwbartext(bar_text, ws_wb);
	}

	wbarsep_one = wbartext;
	wbarsep_one.IText = padwbartext(bar_text, sep_one);
	wbarsep_one.FrontPen = *bar_color[fp_sep_one].color;
	wbarsep_one.BackPen = *bar_color[bp_sep_one].color;

	wbarsep_two = wbartext;
	wbarsep_two.IText = padwbartext(bar_text, sep_two);
	wbarsep_two.FrontPen = *bar_color[fp_sep_two].color;
	wbarsep_two.BackPen = *bar_color[bp_sep_two].color;

	bar_text[mode_tile].text = padwbartext(bar_text, mode_tile);
	bar_text[mode_grid].text = padwbartext(bar_text, mode_grid);
	bar_text[mode_dwindle].text = padwbartext(bar_text, mode_dwindle);
	bar_text[mode_spiral].text = padwbartext(bar_text, mode_spiral);

	wbarmodetext = wbartext;
	wbarmodetext.IText = bar_text[mode_tile].text;
	wbarmodetext.FrontPen = *bar_color[fp_tm].color;
	wbarmodetext.BackPen = *bar_color[bp_tm].color;

	wbarwtitle = wbartext;
	wbarwtitle.FrontPen = *bar_color[fp_ti].color;
	wbarwtitle.BackPen = *bar_color[bp_ti].color;

	if (vws_on == TRUE) {
		wstext_zero.NextText = &wstext_one;
		wstext_one.NextText = &wstext_two;
		wstext_two.NextText = &wstext_three;
		wstext_three.NextText = &wstext_four;
		wstext_four.NextText = &wstext_five;
		wstext_five.NextText = &wbarsep_one;
		wbarsep_one.NextText = &wbarmodetext;
		wbarmodetext.NextText = &wbarsep_two;
		wbarsep_two.NextText = &wbarwtitle;
	} else {
		wstext_zero.NextText = &wbarsep_one;
		wbarsep_one.NextText = &wbarmodetext;
		wbarmodetext.NextText = &wbarsep_two;
		wbarsep_two.NextText = &wbarwtitle;
	}

	if (vws_on == TRUE) {
		wstext_one.LeftEdge = wbartextwidth(0, wstext_zero.IText);
		wstext_two.LeftEdge = wbartextwidth(wstext_one.LeftEdge, wstext_one.IText);
		wstext_three.LeftEdge = wbartextwidth(wstext_two.LeftEdge, wstext_two.IText);
		wstext_four.LeftEdge = wbartextwidth(wstext_three.LeftEdge, wstext_three.IText);
		wstext_five.LeftEdge = wbartextwidth(wstext_four.LeftEdge, wstext_four.IText);
		wbarsep_one.LeftEdge = wbartextwidth(wstext_five.LeftEdge, wstext_five.IText);
		wbarmodetext.LeftEdge = wbartextwidth(wbarsep_one.LeftEdge, wbarsep_one.IText);
		wbarsep_two.LeftEdge = wbartextwidth(wbarmodetext.LeftEdge, wbarmodetext.IText);
		wbarwtitle.LeftEdge = wbartextwidth(wbarsep_two.LeftEdge, wbarsep_two.IText);
	} else {
		wbarsep_one.LeftEdge = wbartextwidth(0, wstext_zero.IText);
		wbarmodetext.LeftEdge = wbartextwidth(wbarsep_one.LeftEdge, wbarsep_one.IText);
		wbarsep_two.LeftEdge = wbartextwidth(wbarmodetext.LeftEdge, wbarmodetext.IText);
		wbarwtitle.LeftEdge = wbartextwidth(wbarsep_two.LeftEdge, wbarsep_two.IText);
	}

	wbarcwb();
	return TRUE;
}
