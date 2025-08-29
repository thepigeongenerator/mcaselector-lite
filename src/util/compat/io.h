/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#if defined __unix__
#include <features.h>
#include <unistd.h>
#elif defined _WIN32
#include <io.h>
#else
#error platform not supported!
#endif

/* tests a files access with F_OK, X_OK, R_OK, W_OK OR'd together
   returns 0 upon success. -1 when errno is set and anything else when one or more of the permissions isn't set */
static inline int faccess(const char *restrict fname, int perms);

// define the constants if they haven't been
#ifndef F_OK
#define F_OK 0
#endif
#ifndef X_OK
#define X_OK 1
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef R_OK
#define R_OK 4
#endif

#if defined __unix__ && _POSIX_C_SOURCE >= 200809L
#define PATH_SEP     '/' // contains the path separator as a character. Yes it is extremely annoying that this has to exist.
#define PATH_SEP_STR "/" // contains the path separator as a string, useful for concatenation. Yes it is extremely annoying that this has to exist.

int faccess(char const *restrict fname, int perms) {
	return access(fname, perms);
}
#elif defined(_WIN32)
#define PATH_SEP     '\\' // contains the path separator as a character. Yes it is extremely annoying that this has to exist.
#define PATH_SEP_STR "\\" // contains the path separator as a string, useful for concatenation. Yes it is extremely annoying that this has to exist.

int faccess(char const *restrict fname, int perms) {
	return _access(fname, perms);
}
#else
#error platform unsupported!
#endif
