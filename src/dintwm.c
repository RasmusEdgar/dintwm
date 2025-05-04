// Copyright 2024 Rasmus Edgar
#include "../include/dintwm_shared.h"
#ifdef FORTIFY
#include "../fortify.h"
#endif

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
