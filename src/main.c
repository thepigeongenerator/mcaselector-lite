/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#include <stdio.h>
#include <stdlib.h>

#include "util/error.h"

#if __has_extension(c_static_assert)
__extension__ _Static_assert(-3 >> 5 == -1,
	"The platform does not compile right-shifting signed integers to an arithmetic shift!");
#endif

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
