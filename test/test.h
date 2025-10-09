/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
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
