// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#include "../include/supplemental.h"

short docmd(const Arg *arg)
{
	int cmdid = arg->i - (int)CMD_ID_0;
	struct TagItem stags[5];
	long int file;
	unsigned char conline[TT_MAX_LENGTH];
	unsigned char cmd[TT_MAX_LENGTH];

	size_t conlen = strnlen((const char *)cons->strings[cmdid], TT_MAX_LENGTH);
	size_t cmdlen = strnlen((const char *)cmds->strings[cmdid], TT_MAX_LENGTH);

	if (conlen != 0U) {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s", cons->strings[cmdid]);
	} else {
		(void)snprintf((char *)conline, TT_MAX_LENGTH, "%s", DEFCON);
	}

	if (cmdlen != 0U) {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s", cmds->strings[cmdid]);
	} else {
		(void)snprintf((char *)cmd, TT_MAX_LENGTH, "%s", DEFCMD);
	}

	if ((file = Open(conline, MODE_NEWFILE)) != 0L) {
		stags[0].ti_Tag = SYS_Input;
		stags[0].ti_Data = (long unsigned int)file;
		stags[1].ti_Tag = SYS_Output;
		stags[1].ti_Data = 0;
		stags[2].ti_Tag = SYS_Asynch;
		stags[2].ti_Data = TRUE;
		stags[3].ti_Tag = SYS_UserShell;
		stags[3].ti_Data = TRUE;
		stags[4].ti_Tag = TAG_DONE;

		if ((SystemTagList(cmd, stags)) == -1) {
			const char dcwarn[] = "Custom CMD/CONLINE is not correct. Quitting";
			info_window(dcwarn);
			return FALSE;
		}

		return TRUE;
	} else {
		return FALSE;
	}
}

int cstring_cmp(const void *a, const void *b)
{
	const char **ia = (const char **)a;
	const char **ib = (const char **)b;
	return strcmp(*ia, *ib);
}

size_t strnlen(const char *s, size_t maxlen)
{
	size_t len;

	for (len = (size_t)0; len < maxlen; len++, s++) {
		if (*s == '\0') {
			break;
		}
	}
	return len;
}

short exit_cxm(const Arg *arg)
{
	return arg->s;
}
