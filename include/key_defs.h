// Copyright 2024 Rasmus Edgar
Keys defkeys[] = {
	{TYPE_TILE, KEY_TILE, KEYTYPE_ID, tile, {0}
	 },
	{TYPE_HGRID, KEY_HGRID, KEYTYPE_ID, hgrid, {0}
	 },
	{TYPE_SPIRAL, KEY_SPIRAL, KEYTYPE_ID, fibonacci, {.i = 0}
	 },
	{TYPE_DWINDLE, KEY_DWINDLE, KEYTYPE_ID, fibonacci, {.i = 1}
	 },
	{TYPE_SWITCHF, KEY_SWITCHF, KEYTYPE_ID, switcher, {.i = 1}
	 },
	{TYPE_SWITCHB, KEY_SWITCHB, KEYTYPE_ID, switcher, {.i = 0}
	 },
	{TYPE_INCTOPGAP, KEY_INCTOPGAP, KEYTYPE_ID, changegaps, {.i = TOPGAP_ID}
	 },
	{TYPE_INCBOTTOMGAP, KEY_INCBOTTOMGAP, KEYTYPE_ID, changegaps, {.i = BOTTOMGAP_ID}
	 },
	{TYPE_INCLEFTGAP, KEY_INCLEFTGAP, KEYTYPE_ID, changegaps, {.i = LEFTGAP_ID}
	 },
	{TYPE_INCRIGHTGAP, KEY_INCRIGHTGAP, KEYTYPE_ID, changegaps, {.i = RIGHTGAP_ID}
	 },
	{TYPE_DECTOPGAP, KEY_DECTOPGAP, KEYTYPE_ID, changegaps, {.i = -TOPGAP_ID}
	 },
	{TYPE_DECBOTTOMGAP, KEY_DECBOTTOMGAP, KEYTYPE_ID, changegaps, {.i = -BOTTOMGAP_ID}
	 },
	{TYPE_DECLEFTGAP, KEY_DECLEFTGAP, KEYTYPE_ID, changegaps, {.i = -LEFTGAP_ID}
	 },
	{TYPE_DECRIGHTGAP, KEY_DECRIGHTGAP, KEYTYPE_ID, changegaps, {.i = -RIGHTGAP_ID}
	 },
	{TYPE_INCALLGAPS, KEY_INCALLGAPS, KEYTYPE_ID, changegaps, {.i = INCALLGAPS_ID}
	 },
	{TYPE_DECALLGAPS, KEY_DECALLGAPS, KEYTYPE_ID, changegaps, {.i = DECALLGAPS_ID}
	 },
	{TYPE_TILE_OFF, KEY_TILE_OFF, KEYTYPE_ID, tileoff, {0}
	 },
	{TYPE_KEY_CMD_0, KEY_CMD_0, KEYTYPE_ID, docmd, {.i = CMD_ID_0}
	 },
	{TYPE_KEY_CMD_1, KEY_CMD_1, KEYTYPE_ID, docmd, {.i = CMD_ID_1}
	 },
	{TYPE_KEY_CMD_2, KEY_CMD_2, KEYTYPE_ID, docmd, {.i = CMD_ID_2}
	 },
	{TYPE_KEY_CMD_3, KEY_CMD_3, KEYTYPE_ID, docmd, {.i = CMD_ID_3}
	 },
	{TYPE_KEY_CMD_4, KEY_CMD_4, KEYTYPE_ID, docmd, {.i = CMD_ID_4}
	 },
	{TYPE_KEY_CMD_5, KEY_CMD_5, KEYTYPE_ID, docmd, {.i = CMD_ID_5}
	 },
	{TYPE_KEY_CMD_6, KEY_CMD_6, KEYTYPE_ID, docmd, {.i = CMD_ID_7}
	 },
	{TYPE_KEY_CMD_7, KEY_CMD_7, KEYTYPE_ID, docmd, {.i = CMD_ID_7}
	 },
	{TYPE_KEY_CMD_8, KEY_CMD_8, KEYTYPE_ID, docmd, {.i = CMD_ID_8}
	 },
	{TYPE_KEY_CMD_9, KEY_CMD_9, KEYTYPE_ID, docmd, {.i = CMD_ID_9}
	 },
	{TYPE_KEY_WS_0, KEY_WS_0, KEYTYPE_ID, changews, {.u = WS_0}
	 },
	{TYPE_KEY_WS_1, KEY_WS_1, KEYTYPE_ID, changews, {.u = WS_1}
	 },
	{TYPE_KEY_WS_2, KEY_WS_2, KEYTYPE_ID, changews, {.u = WS_2}
	 },
	{TYPE_KEY_WS_3, KEY_WS_3, KEYTYPE_ID, changews, {.u = WS_3}
	 },
	{TYPE_KEY_WS_4, KEY_WS_4, KEYTYPE_ID, changews, {.u = WS_4}
	 },
	{TYPE_KEY_WS_5, KEY_WS_5, KEYTYPE_ID, changews, {.u = WS_5}
	 },
	{TYPE_KEY_CWS_0, KEY_CWS_0, KEYTYPE_ID, movetows, {.u = WS_0}
	 },
	{TYPE_KEY_CWS_1, KEY_CWS_1, KEYTYPE_ID, movetows, {.u = WS_1}
	 },
	{TYPE_KEY_CWS_2, KEY_CWS_2, KEYTYPE_ID, movetows, {.u = WS_2}
	 },
	{TYPE_KEY_CWS_3, KEY_CWS_3, KEYTYPE_ID, movetows, {.u = WS_3}
	 },
	{TYPE_KEY_CWS_4, KEY_CWS_4, KEYTYPE_ID, movetows, {.u = WS_4}
	 },
	{TYPE_KEY_CWS_5, KEY_CWS_5, KEYTYPE_ID, movetows, {.u = WS_5}
	 },
	{TYPE_KEY_TAB_NEXT, KEY_TAB_NEXT, KEYTYPE_ID, tabnextwin, {.i = 0}
	 },
	{TYPE_KEY_CXM_EXIT, KEY_CXM_EXIT, KEYTYPE_ID, exit_cxm, {.s = FALSE}
	 },
};
