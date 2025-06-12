#pragma once
#include <math.h>
#include <stdint.h>

#include "../src/util/colour32.h"
#include "../src/util/compat/endian.h"
#include "../src/util/vec/float3.h"
#include "test.h"

int test_float3_norm(void* d) {
	float* arg = d;
	float3 v = {arg[0], arg[1], arg[2]};
	float3 r = float3_norm(v);
	float n = r.x * r.x + r.y * r.y + r.z * r.z;

	// check if the value is within 1 millionth of the one we expect
	return assert_true(fabsf(n - 1.0F) < 1e-6F);
}

int test_colour32_endianess(void* d) {
	(void)d;
	colour32 c = {0xFF000000}; // setting just the red channel
	return assert_true(c.ch.r == 0xFF);
}

int test_compat_endianess(void* d) {
	uint32_t x = *((uint32_t*)d + 0);
	int res = 0;

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	res |= assert_false(be32ton(x) == x);
	res |= assert_false(ntobe32(x) == x);
	res |= assert_true(be32ton(ntobe32(x)) == x);
	res |= assert_true(le32ton(x) == x);
	res |= assert_true(ntole32(x) == x);
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	res |= assert_false(le32ton(x) == x);
	res |= assert_false(ntole32(x) == x);
	res |= assert_true(le32ton(ntole32(x)) == x);
	res |= assert_true(be32ton(x) == x);
	res |= assert_true(ntobe32(x) == x);
#else
	res |= assert_false("platform unsupported!!");
#endif
	return res;
}
