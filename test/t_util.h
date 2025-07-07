#pragma once
#include <stdint.h>

#include "../src/util/colour32.h"
#include "test.h"

int test_colour32_endianess(void *d) {
	(void)d;
	colour32 c = {0xFF000000}; // setting just the red channel
	return assert_true(c.ch.r == 0xFF);
}
