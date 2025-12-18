/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#include "test.h"

#include "../src/types.h"

uint test_okay = 0;
uint test_fail = 0;

int test_process(int res, const char *restrict file, uint ln, const char *restrict function, const char *restrict expression)
{
	const char *status = res ?
		"[\033[32;1m  OK  \033[0m]" :
		"[\033[31;1m FAIL \033[0m]";
	test_okay += !!res;
	test_fail += !res;

	printf("%s\t%s:%u-%s\t-> %s\n", status, file, ln, function, expression);
	return res;
}
