// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#undef GLAD_GL_IMPLEMENTATION

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "dat.h" // contains the test data
#include "test.h"

int main(void) {
	// get test count
	size_t n = sizeof(tests) / sizeof(tests[0]);

	return exec_tests(tests, n);
}
