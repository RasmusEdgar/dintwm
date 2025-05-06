// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"

void cleanup_dintwm(void)
{
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

	window_free_lut();
	free_misc_opts();
}
