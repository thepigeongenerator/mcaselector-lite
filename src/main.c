/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include <stdio.h>
#include <stdlib.h>


_Static_assert(-3 >> 5 == -1,
	"The platform does not compile "
	"right-shifting signed integers to an arithmetic shift!");

/* Called by _exit. */
void quit(void)
{
	printf("exited\n");
}

/* Entry-point of the application. */
int main(int argc, char **argv)
{
	(void)argc, (void)argv;
	atexit(quit); /* Register `quit` to be called on _exit. */

	return 0;
}
