// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#if __INCLUDE_LEVEL__ > 0
#include <stdio.h>
#include <stdlib.h>

#include "util/macro.h"
#endif

#define debug(s, ...) printf("\033[95m" __FILE__ ":" MACRO_STR2(__LINE__) ": [DBG]: " s "\033[0m\n", ##__VA_ARGS__)
#define info(s, ...)  printf(__FILE__ ":" MACRO_STR2(__LINE__) ": [INF]: " s "\n", ##__VA_ARGS__)
#define warn(s, ...)  fprintf(stderr, "\033[93m" __FILE__ ":" MACRO_STR2(__LINE__) ": [WAR]: " s "\033[0m\n", ##__VA_ARGS__)
#define error(s, ...) fprintf(stderr, "\033[91m" __FILE__ ":" MACRO_STR2(__LINE__) ": [ERR]: " s "\033[0m\n", ##__VA_ARGS__)

#define fatal(s, ...)                                                                                           \
	do {                                                                                                    \
		printf("\033[101m" __FILE__ ":" MACRO_STR2(__LINE__) ": [FAT]: " s "\033[0m\n", ##__VA_ARGS__); \
		exit(1);                                                                                        \
	} while (0)
