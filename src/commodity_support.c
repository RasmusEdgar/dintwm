// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/commodity_support.h"
#include "../include/commodity_shared.h"
// initialize commodity options and keycombos
#include "../include/commodity_types.h"
#include "../include/rawkey_combos.h"
//#include "../include/commodity.h"
#include "../include/key_txts.h"
#include "../include/key_defs.h"
#include "../include/opt_defs.h"

static short attachtooltypes(CxObj *broker, struct MsgPort *port, struct DiskObject *diskobj)
{
	short rc = TRUE;
	unsigned char nil = '\0';
	size_t keyarrsize;
	size_t optarrsize;
	struct Popkeys *keys;
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
	bar_default_text[mode_spiral] = DEF_BAR_TEXT_MODE_SPIRAL;
	bar_default_text[sep_one] = DEF_BAR_TEXT_SEP_ONE;
	bar_default_text[sep_two] = DEF_BAR_TEXT_SEP_TWO;
	bar_default_text[space] = DEF_BAR_TEXT_SPACE;
	bar_default_text[err] = DEF_BAR_TEXT_ERR;

	optarrsize = sizeof(defopts) / sizeof(*defopts);
	keyarrsize = sizeof(defkeys) / sizeof(*defkeys);

	bar_text = (Bar_Text *) malloc(BAR_LAST_TEXT * sizeof(*bar_text));
	for (int i = 0; i < BAR_LAST_TEXT; i++) {
		bar_text[i].text = malloc(TT_MAX_LENGTH * 2);
		bar_text[i].text[0] = nil;
	}

	for (size_t i = 0; i < optarrsize; ++i) {
		const char *tt_optvalue =
		    (char *)FindToolType((const unsigned char **)diskobj->do_ToolTypes, (const unsigned char *)defopts[i].optname);
		size_t tt_length = strnlen(tt_optvalue, TT_MAX_LENGTH);
		if (tt_optvalue && tt_length < (size_t)TT_MAX_LENGTH) {
			rc = apply_options(defopts, tt_optvalue, i);
		}
		if (rc == FALSE) {
			break;
		}
	}

	if (rc == TRUE && option_bool(BAR_ON_GET, TRUE)) {
		for (int i = 0; i < BAR_LAST_TEXT; i++) {
			if (bar_text[i].text[0] != nil) {
				continue;
			}
			if ((assign_bar_item(bar_text, i, bar_default_text[i]) == FALSE)) {
				return FALSE;
			}
		}

		// Bar color assignment
		if (*bar_color[bg].color == nil) {
			bar_color[bg].color[0] = DEF_BAR_BG_COL;
		}
		if (*bar_color[fp_ws].color == nil) {
			bar_color[fp_ws].color[0] = DEF_BAR_FP_WS_COL;
		}
		if (*bar_color[bp_ws].color == nil) {
			bar_color[bp_ws].color[0] = DEF_BAR_BP_WS_COL;
		}
		if (*bar_color[fp_cur].color == nil) {
			bar_color[fp_cur].color[0] = DEF_BAR_FP_CUR_COL;
		}
		if (*bar_color[bp_cur].color == nil) {
			bar_color[bp_cur].color[0] = DEF_BAR_BP_CUR_COL;
		}
		if (*bar_color[fp_tm].color == nil) {
			bar_color[fp_tm].color[0] = DEF_BAR_FP_TM_COL;
		}
		if (*bar_color[bp_tm].color == nil) {
			bar_color[bp_tm].color[0] = DEF_BAR_BP_TM_COL;
		}
		if (*bar_color[fp_ti].color == nil) {
			bar_color[fp_ti].color[0] = DEF_BAR_FP_TI_COL;
		}
		if (*bar_color[bp_ti].color == nil) {
			bar_color[bp_ti].color[0] = DEF_BAR_BP_TI_COL;
		}
		if (*bar_color[fp_sep_one].color == nil) {
			bar_color[fp_sep_one].color[0] = DEF_BAR_FP_SEP_ONE_COL;
		}
		if (*bar_color[bp_sep_one].color == nil) {
			bar_color[bp_sep_one].color[0] = DEF_BAR_BP_SEP_ONE_COL;
		}
		if (*bar_color[fp_sep_two].color == nil) {
			bar_color[fp_sep_two].color[0] = DEF_BAR_FP_SEP_TWO_COL;
		}
		if (*bar_color[bp_sep_two].color == nil) {
			bar_color[bp_sep_two].color[0] = DEF_BAR_BP_SEP_TWO_COL;
		}
	}

	if (rc == FALSE) {
		return rc;
	}

	if (option_bool(EXCLUDE_WTYPE_GET, TRUE)) {
		qsort(excls->strings, WTYPE_MAX, sizeof(*excls->strings), cstring_cmp);
	}

	if (option_bool(INCLUDE_WTYPE_GET, TRUE)) {
		qsort(incls->strings, WTYPE_MAX, sizeof(*incls->strings), cstring_cmp);
	}

	keys = malloc(sizeof(*keys) * keyarrsize);

	if (keys != NULL) {
		for (size_t i = 0; i < keyarrsize; ++i) {
			keys[i].rawcombo =
			    (char *)FindToolType((const unsigned char **)diskobj->do_ToolTypes, (const unsigned char *)defkeys[i].optname);

			if (keys[i].rawcombo == NULL) {
				keys[i].rawcombo = defkeys[i].defaultval;
			}

			if (keys[i].rawcombo != NULL) {
				CxObj *filter;
				if ((filter = HotKey((const unsigned char *)keys[i].rawcombo, port, (long int)i)) != NULL) {
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
	static unsigned char COMMODITY_NAME[] = "DintWM commodity";
	static unsigned char COMMODITY_TITLE[] = "DintWM - a tiling window manager";
	static unsigned char COMMODITY_DESC[] = "To change hotkeys edit tooltypes";
	static struct NewBroker MyBroker = {
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
	unsigned char iconlib[] = "icon.library";
	unsigned char diskobjname[] = "PROGDIR:dintwm";
	running = TRUE;
	maintask = NULL;
	subtask = NULL;

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

void subactionchk(void)
{
	const char *warn_messages[] = {
		"Dintwm only supports up to 1023 windows. Dintwm applauds the effort!"
	};
	struct timeval currentval;
	struct timerequest *tr;
	struct Screen const *scr = tiling_screen_light();

	if (!(tr = create_timer(UNIT_VBLANK))) {
		running = FALSE;
	}

	currentval.tv_secs = 0UL;
	currentval.tv_micro = option_ul(AUTO_INTERVAL_DELAY_GET, 0);

	while (running == TRUE) {
		time_delay(tr, &currentval);
		int wincnt = countwindows(scr);

		if (wincnt > (DIVISOR - 2)) {
			running = FALSE;
			info_window(warn_messages[WIWARN]);
			continue;
		}

		if ((option_bool(TILE_OFF_GET, TRUE)) == FALSE && (option_bool(AUTOTILE_GET, TRUE)) == TRUE) {
			//if (winnum_start != wincnt) {
			if (win_start(WIN_START_GET, scr) != wincnt) {
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			if (awin_comp == NULL || awin_comp != (window_active(AW_GET, 0UL)) || (awin_comp->Flags & (unsigned long)WFLG_WINDOWACTIVE) == 0U) {
				getactive();
				awin_comp = window_active(AW_GET, 0UL);
				update_wbar();
				Signal(maintask, mainsig);
				(void)Wait((subsig));
			}

			// If previous active window is no longer active, refresh bar
			if ((awin_comp->Flags & (unsigned long)WFLG_WINDOWACTIVE) == 0U) {
				getactive();
				// Don't retile if workbench screen
				awin_comp = window_active(AW_GET, 0UL);
				if ((window_get_wbwin(awin_comp)) == TRUE) {
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

static struct timerequest *create_timer(unsigned long unit)
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

	TimerIO = (struct timerequest *)CreateExtIO(timerport, sizeof(struct timerequest));
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

static void time_delay(struct timerequest *tr, const struct timeval *tv)
{
	unsigned short traddreq = TR_ADDREQUEST;
	tr->tr_node.io_Command = traddreq;	/* add a new timer request */

	/* structure assignment */
	tr->tr_time = *tv;

	/* post request to the timer -- will go to sleep till done */
	(void)DoIO((struct IORequest *)tr);
}

static void delete_timer(struct timerequest *tr)
{
	if (tr != 0) {
		struct MsgPort *tp;
		tp = tr->tr_node.io_Message.mn_ReplyPort;

		if (tp != 0) {
			DeletePort(tp);
		}

		if (!(CheckIO((struct IORequest *)tr))) {
			AbortIO((struct IORequest *)tr);	/* Ask device to abort any pending requests */
		}
		(void)WaitIO((struct IORequest *)tr);	/* Clean up */
		CloseDevice((struct IORequest *)tr);
		DeleteExtIO((struct IORequest *)tr);
	}
}

void cleanup(struct Cmo *cmo)
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

	if (option_bool(BAR_ON_GET, TRUE)) {
		struct Window const *wbarw = window_wbar(NULL);
		if (wbarw != NULL) {
			CloseWindow((struct Window *)wbarw);
		}
		for (int i = 0; i < BAR_LAST_TEXT; i++) {
			free(bar_text[i].text);
		}
		free(bar_text);
	}
}
