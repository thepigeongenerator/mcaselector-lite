#pragma once
#include <stdint.h>

#include "../src/util/colour32.h"
#include "../src/util/compat/bswap.h"
#include "../src/util/types.h"
#include "test.h"

int test_colour32_endianess(void *d) {
	(void)d;
	colour32 c = {0xFF000000}; // setting just the red channel
	return assert_true(c.ch.r == 0xFF);
}

int test_bswap16(void *d) {
	u16 *arg = d;
	return assert_true(bswap16(arg[0]) == arg[1]);
}

int test_bswap32(void *d) {
	u32 *arg = d;
	return assert_true(bswap32(arg[0]) == arg[1]);
}

int test_bswap64(void *d) {
	u64 *arg = d;
	return assert_true(bswap64(arg[0]) == arg[1]);
}

int test_bswap16_impl(void *d) {
	u16 *arg = d;
	return assert_true(bswap16_impl(arg[0]) == arg[1]);
}

int test_bswap32_impl(void *d) {
	u32 *arg = d;
	return assert_true(bswap32_impl(arg[0]) == arg[1]);
}

int test_bswap64_impl(void *d) {
	u64 *arg = d;
	return assert_true(bswap64_impl(arg[0]) == arg[1]);
}
