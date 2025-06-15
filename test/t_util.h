#pragma once
#include <math.h>
#include <stdint.h>

#include "../src/util/colour32.h"
#include "../src/util/compat/bswap.h"
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

int test_bswap16(void* d) {
	uint16_t* arg = d;
	return assert_true(bswap16(arg[0]) == arg[1]);
}

int test_bswap32(void* d) {
	uint32_t* arg = d;
	return assert_true(bswap32(arg[0]) == arg[1]);
}

int test_bswap64(void* d) {
	uint64_t* arg = d;
	return assert_true(bswap64(arg[0]) == arg[1]);
}

int test_bswap16_impl(void* d) {
	uint16_t* arg = d;
	return assert_true(bswap16_impl(arg[0]) == arg[1]);
}

int test_bswap32_impl(void* d) {
	uint32_t* arg = d;
	return assert_true(bswap32_impl(arg[0]) == arg[1]);
}

int test_bswap64_impl(void* d) {
	uint64_t* arg = d;
	return assert_true(bswap64_impl(arg[0]) == arg[1]);
}
