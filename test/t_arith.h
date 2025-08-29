/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include "test.h"

/* tests arithmetic shift for signed integers (rather than logical shift) */
int test_sar(void *dat) {
	(void)dat;
	return assert_true(-3 >> 5 == -1);
}
