/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include <stdint.h>
#include <stdio.h>

#include "test.h"
#include "test_conf.h"

int main(void)
{
	assert_true(-3 >> 5 == -1); // checking for arithmetic shift, rather than logical shift
	assert_true(sizeof(u8) == 1);
	assert_true(sizeof(u16) == 2);
	assert_true(sizeof(u32) == 4);
	assert_true(sizeof(u64) == 8);
	assert_true(sizeof(f32) == 4);
	assert_true(sizeof(f64) == 8);
	assert_true(sizeof(usize) == sizeof(size_t));
	test_conf_procbuf("key=val", "key", "val", 0);
	test_conf_procbuf("sometxt", "sometxt", "", CONF_ESYNTAX);
	test_conf_procbuf("# comment", "", "", CONF_ENODAT);
	test_conf_procbuf("", "", "", CONF_ENODAT);
	test_conf_procbuf("\n", "", "", CONF_ENODAT);
	test_conf_procbuf("\r\n", "", "", CONF_ENODAT);
	test_conf_procbuf("k=v\na", "k", "v", 0);
	test_conf_procbuf("k=v\r\na", "k", "v", 0);
	test_conf_getpat();

	struct conf_entry opts[] = {
		{"key0", NULL, 0},
		{"key1", NULL, 0},
		{"key2", NULL, 0}
        };
	test_conf_matchopt(opts, 3, "key0", 0);
	test_conf_matchopt(opts, 3, "key1", 1);
	test_conf_matchopt(opts, 3, "key2", 2);
	test_conf_matchopt(opts, 3, "key3", -1);

	test_conf_procval_int("42", 42, CONF_I32);
	test_conf_procval_int("-42", (u32)-42, CONF_I32);
	test_conf_procval_int("4294967295", UINT32_MAX, CONF_U32);
	test_conf_procval_int("9223372036854775807", INT64_MAX, CONF_I64);
	test_conf_procval_int("256", 255, CONF_U8);

	// TODO: write more tests for float parsing
	test_conf_procval_f32("0.0", 0.0f);
	test_conf_procval_f32("3.14159", 3.14159f);
	test_conf_procval_f32("3.1428571428", 22.0f / 7.0f);
	test_conf_procval_f32("-5.0", -5.0f);

	/* return 1 if there were failed tests. */
	printf("test results: (%u/%u)\n", test_okay, test_okay + test_fail);
	return !!test_fail;
}
