/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#pragma once

#include <mcaselector-lite/atrb.h>
#include <mcaselector-lite/types.h>
#include <stdio.h>
#include <stdlib.h>

FORMAT((printf, 3, 4))
void error_debug(uint ln, const char *restrict file, const char *restrict fmt, ...);

FORMAT((printf, 3, 4))
void error_info(uint ln, const char *restrict file, const char *restrict fmt, ...);

FORMAT((printf, 3, 4))
void error_warn(uint ln, const char *restrict file, const char *restrict fmt, ...);

FORMAT((printf, 3, 4))
void error_error(uint ln, const char *restrict file, const char *restrict fmt, ...);

NORET
FORMAT((printf, 3, 4))
void error_fatal(uint ln, const char *restrict file, const char *restrict fmt, ...);

#define debug(...) error_debug(__LINE__, __FILE__, __VA_ARGS__)
#define info(...)  error_info(__LINE__, __FILE__, __VA_ARGS__)
#define warn(...)  error_warn(__LINE__, __FILE__, __VA_ARGS__)
#define error(...) error_error(__LINE__, __FILE__, __VA_ARGS__)
#define fatal(...) error_fatal(__LINE__, __FILE__, __VA_ARGS__)
