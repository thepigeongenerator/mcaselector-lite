#pragma once

#include "test.h"

int test_sar(void *dat) {
	(void)dat;
	return assert_true(-3 >> 5 == -1);
}
