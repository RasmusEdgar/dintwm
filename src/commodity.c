// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/commodity_shared.h"

short int commo(void)
{
	struct Cmo cmo = { 0, 0, 0, 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 }};
	const char *cmo_fail_msgs[] = {
		"Message port allocation failed. Exiting.",
		"Iconbase allocation failed. Exiting.",
		"Diskobject allocatoin failed. Exiting.",
		"Broker allocation failed. Exiting.",
		"Main signal allocation failed. Exiting.",
		"Sub signal allocation failed. Exiting.",
		"Activating CxObj failed. Exiting.",
		"Attaching tooltypes failed. Exiting."
	};
	init_cmo(&cmo);
	size_t cmo_arr_length = sizeof(cmo.failarr) / sizeof(cmo.failarr[0]);

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
	struct Screen const *scr = tiling_screen_light();

	if (running == TRUE) {
		CxMsg *msg;
		CloseLibrary(cmo.iconbase);
		cmo.iconbase = NULL;
		FreeDiskObject(cmo.diskobj);
		cmo.diskobj = NULL;
		static unsigned char subactionchkname[] = "CXM_window_state_checker";
		const char *warn_messages[] = {
			"Virtual WSs are incompatible with backdropped WB. Unbackdrop WB or remove TT VWS_ON. Quitting.",
			"Commodity is already running. Quitting.",
		};

		// Muting GCC warning here. Following official Amiga CreateTask example
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
		subtask = CreateTask(subactionchkname, 0L, (void *)subactionchk, 2048L);
#pragma GCC diagnostic pop
		if (subtask == NULL) {
			running = FALSE;
		}

		//if (vws_on == TRUE) {
		if ((option_bool(VWS_ON_GET, TRUE)) == TRUE) {
			(void)countwindows(scr);
			getactive();
			//if (backdropped == TRUE) {
			if (option_bool(BACKDROP_GET, TRUE)) {
				//if (info_on == TRUE) {
				if (option_bool(INFO_ON_GET, TRUE)) {
					info_window(warn_messages[BDWARN]);
				}
				//bar_on = FALSE;
				(void)option_bool(BAR_ON_SET, FALSE);
				cleanup(&cmo);
				return EXIT_FAILURE;
			}
		}

		if ((option_bool(BAR_ON_GET, TRUE)) && running == TRUE) {
			running = init_wbar();
			struct Window const *wbarw = window_wbar(NULL);

			if (running == TRUE && wbarw) {
				getactive();
				awin_comp =  window_active(AW_GET, 0UL);
				update_wbar();
			}
		}
		// Initial tile
		running = defkeys[0].func(&defkeys[0].arg);

		//Main Loop
		while (running == TRUE) {
			//winnum_start = countwindows(scr);
			(void)win_start(WIN_START_SET, scr);

			wakeupsigs = Wait((mainsig) | (1UL << cmo.mp->mp_SigBit));

			if ((wakeupsigs & mainsig) != 0UL) {
				if ((window_get_wbwin(window_active(AW_GET, 0UL))) == TRUE) {
					Signal(subtask, subsig);
					continue;
				}
				//if (tile_off == FALSE) {
				if ((option_bool(TILE_OFF_GET, TRUE)) == FALSE) {
					int t_layout = tiling_layout(TL_GET, 0);
					running = defkeys[t_layout].func(&defkeys[t_layout].arg);
					update_wbar();
					//winnum_start = countwindows(scr);
					(void)win_start(WIN_START_SET, scr);
				}
				Signal(subtask, subsig);
			}

			while ((msg = (void *)GetMsg(cmo.mp)) != NULL) {
				long id = CxMsgID(msg);
				unsigned long type = CxMsgType(msg);

				ReplyMsg((struct Message *)msg);

				if (type == (unsigned long)CXM_COMMAND) {
					switch (id) {
					case CXCMD_UNIQUE:
						if (option_bool(INFO_ON_GET, TRUE)) {
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
						(void)tiling_layout(TL_SET, id);
					}
					running = defkeys[id].func(&defkeys[id].arg);
					if (running == FALSE) {
						Signal(subtask, subsig);
						(void)Wait((mainsig) | (1UL << cmo.mp->mp_SigBit));
					}
					if (option_bool(BAR_ON_GET, TRUE)) {
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
