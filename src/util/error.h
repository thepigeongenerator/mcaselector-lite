/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#pragma once

#include <mcaselector-lite/atrb.h>
#include <mcaselector-lite/types.h>
#include <stdio.h>
#include <stdlib.h>

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
