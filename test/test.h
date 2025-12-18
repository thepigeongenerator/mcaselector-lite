/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once
#include <stdio.h>

#include "../src/types.h"

extern uint test_okay;
extern uint test_fail;

/* Prints the result `res` of a test.
 * Returns `res` back, for ease of chaining. */
int test_process(int res, const char *restrict file, uint ln, const char *restrict function, const char *restrict expression);

#define assert_true(expr)  test_process((expr), __FILE__, __LINE__, __func__, #expr)
#define assert_false(expr) assert_true(!(expr))
