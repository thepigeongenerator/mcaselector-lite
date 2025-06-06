// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/util/colour32.h"
#include "../src/util/compat/endian.h"
#include "../src/util/vec/float3.h"
#include "test.h"

static int test_float3_norm(void* d) {
	float* arg = d;
	float3 v = {arg[0], arg[1], arg[2]};
	float3 r = float3_norm(v);
	float n = r.x * r.x + r.y * r.y + r.z * r.z;

	// check if the value is within 1 millionth of the one we expect
	return assert_true(fabsf(n - 1.0F) < 1e-6F);
}

static int test_colour32_endianess(void* d) {
	(void)d;
	colour32 c = {0xFF000000}; // setting just the red channel
	return assert_true(c.ch.r == 0xFF);
}

static int test_compat_endianess(void* d) {
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

int main(void) {
	// tests that should be performed
	testdat tests[] = {
		{&test_float3_norm,        &(float3){2.0F, 0.67F, 5.0F}},
		{&test_float3_norm,        &(float3){0.2F, 0.4F, 0.1F} },
		{&test_colour32_endianess, (uint32_t[]){0, 1}          },
		{&test_compat_endianess,   (uint32_t[]){0x15F59267}    }
    };

	// get test count
	size_t n = sizeof(tests) / sizeof(tests[0]);

	return exec_tests(tests, n);
}
