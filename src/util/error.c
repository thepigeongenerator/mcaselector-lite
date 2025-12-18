/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#include "error.h"

#include <stdarg.h>
#include <stdio.h>

#include "../types.h"

static void error_log(FILE *restrict stream, const char *restrict pfx, uint ln, const char *restrict file, const char *restrict fmt, va_list ap)
{
	fprintf(stream, "(%s:%u) [%s] '", file, ln, pfx);
	vfprintf(stream, fmt, ap);
	fprintf(stream, "'\n");
}

void error_debug(uint ln, const char *restrict file, const char *restrict fmt, ...)
{
#ifndef NDEBUG
#else
	char *env = getenv("DEBUG");
	if (env && env[0] != '1')
		return;
#endif
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[95mDBG\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_info(uint ln, const char *restrict file, const char *restrict fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[94mINF\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_warn(uint ln, const char *restrict file, const char *restrict fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[93mWAR\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_error(uint ln, const char *restrict file, const char *restrict fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[91mERR\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_fatal(uint ln, const char *restrict file, const char *restrict fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[101mFAT\033[0m", ln, file, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}
