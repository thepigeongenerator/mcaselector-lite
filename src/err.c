/* Copyright (C)2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include "err.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void verr(int code, const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	if (fmt) {
		vfprintf(stderr, fmt, args);
		fputs(": ", stderr);
	}
	fputs(strerror(errno), stderr);
	fputc('\n', stderr);
	exit(code);
}

void verrx(int code, const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);
	exit(code);
}

void vwarn(const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	if (fmt) {
		vfprintf(stderr, fmt, args);
		fputs(": ", stderr);
	}
	fputs(strerror(errno), stderr);
	fputc('\n', stderr);
}

void vwarnx(const char *fmt, va_list args)
{
	fprintf(stderr, "%s: ", argv0);
	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);
}

void err(int code, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	verr(code, fmt, args);
	va_end(args);
}

void errx(int code, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	verrx(code, fmt, args);
	va_end(args);
}

void warn(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarn(fmt, args);
	va_end(args);
}

void warnx(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vwarnx(fmt, args);
	va_end(args);
}
