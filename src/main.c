/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include <stdio.h>
#include <stdlib.h>

#include "util/error.h"

_Static_assert(-3 >> 5 == -1,
	"The platform does not compile "
	"right-shifting signed integers to an arithmetic shift!");

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	(void)argc, (void)argv;
	printf("debug: [DBG], info: [INF], warning: [WAR], error: [ERR], fatal: [FAT]\n");
	// atexit(quit);

	/* return success, since some architectures do not follow 0=success
	 * This action will call `quit`. */
	return EXIT_SUCCESS;
}
