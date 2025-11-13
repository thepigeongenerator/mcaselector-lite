/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See included LICENSE file for details. */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "../types.h"
#include "../util/atrb.h"
#include "../util/macro.h"

void error_debug(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_info(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_warn(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_error(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_fatal(uint ln, const char *restrict file, const char *restrict fmt, ...) NORET;

#define debug(...) error_debug(__LINE__, __FILE__ __VA_OPT__(, ) __VA_ARGS__)
#define info(...)  error_info(__LINE__, __FILE__ __VA_OPT__(, ) __VA_ARGS__)
#define warn(...)  error_warn(__LINE__, __FILE__ __VA_OPT__(, ) __VA_ARGS__)
#define error(...) error_error(__LINE__, __FILE__ __VA_OPT__(, ) __VA_ARGS__)
#define fatal(...) error_fatal(__LINE__, __FILE__ __VA_OPT__(, ) __VA_ARGS__)
