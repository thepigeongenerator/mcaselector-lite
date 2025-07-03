// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once
#include <stdio.h>

char const *test_ctest;
size_t test_runs = 0;

// evaluates the test
// returns 1 upon error
static inline int assert_helper(int cond, char const *restrict fname, unsigned ln, char const *restrict fnname, char const *restrict expr) {
	test_runs++;
	if (cond)
		printf("[\033[32;1m  OK  \033[0m] %s %s -> %s:%u (%s)\n", test_ctest, fnname, fname, ln, expr);
	else
		printf("[\033[31;1m FAIL \033[0m] %s %s -> %s:%u (%s)\n", test_ctest, fnname, fname, ln, expr);
	return !cond;
}

#define assert_true(expr)  assert_helper(!!(expr), __FILE__, __LINE__, __func__, #expr) // evaluation expected to be true
#define assert_false(expr) assert_helper(!(expr), __FILE__, __LINE__, __func__, #expr)  // evaluation expected to be false

// contains the data for executing a single test
struct testdat {
	char const *name;    // test name
	int (*test)(void *); // test, returns 0 upon success, non-zero upon failure
	void *args;          // arguments to the test
};
typedef struct testdat testdat;

// executes the tests, returns the amount of failed tests; >0: failure
static inline size_t exec_tests(testdat *dat, size_t ntests) {
	size_t i;
	size_t err = 0;

	// perform tests and count the error state
	for (i = 0; i < ntests; i++) {
		test_ctest = dat[i].name;
		err += !!(dat[i].test(dat[i].args));
	}

	// give final score
	if (!err)
		fprintf(stdout, "tests completed! (%zu/%zu)\n", test_runs - err, test_runs);
	else
		fprintf(stderr, " tests failed! (%zu/%zu)\n", test_runs - err, test_runs);
	return err;
}
