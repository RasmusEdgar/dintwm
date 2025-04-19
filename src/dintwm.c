// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/dintwm.h"

int main(int argc, char **argv)
{
	static int dint_exit_state = EXIT_SUCCESS;

#ifdef FORTIFY
	Fortify_EnterScope();
#endif

	initdefaults();

	dint_exit_state = dintwmrun(argc, argv);

	window_free_lut();

#ifdef FORTIFY
	Fortify_LeaveScope();
	Fortify_OutputStatistics();
#endif

	return dint_exit_state;
}

static int dintwmrun(int argc, char **argv)
{
	int c = 0;
	int dint_opt_state = NOTSET;
	static int dint_fail_state = EXIT_SUCCESS;
	static int dint_exit_state = EXIT_SUCCESS;
	int not_known = 0;
	int sh = tiling_screen_height();
	int sw = tiling_screen_height();

	while ((c = getopt(argc, argv, "uU:B:L:R:CdghstV")) >= 0) {	/* Flawfinder: ignore */
		switch (c) {
		case 'u':
			(void)tiling_gaps(TOPGAP_SET, tiling_calc_menugap());
			break;
		case 'U':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int tgap = tiling_gaps(TOPGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (tgap > sh || tgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'B':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int bgap = tiling_gaps(BOTTOMGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (bgap > sh || bgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'L':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int lgap = tiling_gaps(LEFTGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (lgap > sw || lgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'R':
			if (*optarg == '-') {
				dint_fail_state = MISSING;
				break;
			}
			int rgap = tiling_gaps(RIGHTGAP_SET, (int)strtol(optarg, (char **)NULL, 10));
			if (rgap > sw || rgap < 0) {
				dint_fail_state = GAP_ERR;
			}
			break;
		case 'C':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = COMMODITIZE;
			}
			break;
		case 'd':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_DWINDLE;
			}
			break;
		case 'g':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_HGRID;
			}
			break;
		case 'h':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_PRINTUSAGE;
			}
			break;
		case 's':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_SPIRAL;
			}
			break;
		case 't':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = FUNC_TILE;
			}
			break;
		case 'V':
			if (dint_opt_state != NOTSET) {
				dint_fail_state = DOUBLE_OPTION_ERR;
			} else {
				dint_opt_state = PRINTVERSION;
			}
			break;
		case ':':
			dint_fail_state = MISSING;
			break;
		case '?':
			dint_fail_state = UNKNOWN;
			not_known = optopt;
			break;
		default:
			// Do nothing
			break;
		}
	}

	switch (dint_fail_state) {
	case DOUBLE_OPTION_ERR:
		printf("Do not use -%c with other option.\n", optopt);
		dint_exit_state = EXIT_FAILURE;
		break;
	case GAP_ERR:
		printf("Gap is larger or smaller than screen.\n");
		dint_exit_state = EXIT_FAILURE;
		break;
	case UNKNOWN:
		if (optopt != 0) {
			printf("unknown opt: -%c\n", not_known);
		} else {
			printf("unknown opt: -%c\n", ':');
		}
		dint_exit_state = EXIT_FAILURE;
		break;
	case MISSING:
		if (optopt != 0) {
			printf("missing arg: -%c\n", optopt);
		} else {
			printf("missing arg: -%c\n", ':');
		}
		printf("missing arg: -%c\n", optopt != 0 ? optopt : (int)':');
		dint_exit_state = EXIT_FAILURE;
		break;
	default:
		// Do nothing
		break;
	}

	// If no arg is used, set to commoditize
	if (dint_opt_state != NOTSET && dint_exit_state != EXIT_FAILURE) {
		dint_fail_state = DOUBLE_OPTION_ERR;
	} else {
		dint_opt_state = COMMODITIZE;
	}

	if (dint_exit_state != EXIT_FAILURE) {
		switch (dint_opt_state) {
		case COMMODITIZE:
			dint_exit_state = commo();
			break;
		case FUNC_PRINTUSAGE:
			dint_exit_state = printusage();
			break;
		case PRINTVERSION:
			printf("Dintwm version: %s\n", DINTWM_VERSION);
			dint_exit_state = TRUE;
			break;
		default:
			dint_exit_state = defkeys[dint_opt_state].func(&defkeys[dint_opt_state].arg);
			break;
		}
	}

	return dint_exit_state;
}

static void initdefaults(void)
{
	unsigned char nil = '\0';
	(void)option(TILE_FACT_GET, 0);
	(void)option_bool(BACKDROP_SET, FALSE);
	struct Window *window = NULL;

	if ((window_alloc_lut()) != 0) {
		printf("allocation failed\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < BAR_LAST_COLOR; ++i) {
		bar_color[i].color[0] = nil;
	}

	// Set reusable screen pointer for later usage
	(void)tiling_screen_light();

	struct Screen *scr = tiling_lock(TLOCK, NULL);
	(void)tiling_screen_info(SH_SET, scr->Height);
	(void)tiling_screen_info(SW_SET, scr->Width);

	for (window = scr->FirstWindow; window != NULL; window = window->NextWindow) {
		window_set_wptr(window);
		if (strcmp("Workbench", (const char *)window->Title) == 0) {
			window_set_wbwin(window, TRUE);
			window_set_ws_num(window, WS_0);
			continue;
		} else {
			window_set_wbwin(window, FALSE);
		}
		 window_set_ws_num(window, window_current_ws(WS_GET, 0));
	}
	(void)tiling_lock(TUNLOCK, scr);
}

static short printusage(void)
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
	       "Options:",
	       "-d: Fibonacci dwindle",
	       "-g: Horizontal grid",
	       "-t: Tile with left master",
	       "-s: Fibonacci spiral",
	       "-V: Show version",
	       "<other arg> -u: Add workbench bar gap",
	       "<other arg> -U<int>: Add custom top gap",
	       "<other arg> -B<int>: Add custom bottom gap", "<other arg> -L<int>: Add custom left gap", "<other arg> -R<int>: Add custom right gap",
	       "-h: This message");

	return EXIT_SUCCESS;
}
