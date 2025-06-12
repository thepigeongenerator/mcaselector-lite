// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#define ERROR_STR_PRE(v) #v
#define ERROR_STR(v)     ERROR_STR_PRE(v)

#define debug(s, ...)    printf("\033[95m" __FILE__ ":" ERROR_STR(__LINE__) ": [DBG]: " s "\033[0m\n", ##__VA_ARGS__)
#define info(s, ...)     printf(__FILE__ ":" ERROR_STR(__LINE__) ": [INF]: " s "\n", ##__VA_ARGS__)
#define warn(s, ...)     fprintf(stderr, "\033[93m" __FILE__ ":" ERROR_STR(__LINE__) ": [WAR]: " s "\033[0m\n", ##__VA_ARGS__)
#define error(s, ...)    fprintf(stderr, "\033[91m" __FILE__ ":" ERROR_STR(__LINE__) ": [ERR]: " s "\033[0m\n", ##__VA_ARGS__)

#define fatal(s, ...)                                                                              \
	do {                                                                                           \
		printf("\033[101m" __FILE__ ":" ERROR_STR(__LINE__) ": [FAT]: " s "\033[0m\n", ##__VA_ARGS__); \
		exit(1);                                                                                   \
	} while (0)
