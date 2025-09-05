/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "util/atrb.h"
#include "util/intdef.h"
#include "util/macro.h"

void error_dbg(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_inf(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_war(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_err(uint ln, const char *restrict file, const char *restrict fmt, ...);
void error_fat(uint ln, const char *restrict file, const char *restrict fmt, ...) NORET;

#define debug(s, ...) error_dbg(__LINE__, __FILE__, s, ##__VA_ARGS__)
#define info(s, ...)  error_inf(__LINE__, __FILE__, s, ##__VA_ARGS__)
#define warn(s, ...)  error_war(__LINE__, __FILE__, s, ##__VA_ARGS__)
#define error(s, ...) error_err(__LINE__, __FILE__, s, ##__VA_ARGS__)
#define fatal(s, ...) error_fat(__LINE__, __FILE__, s, ##__VA_ARGS__)
