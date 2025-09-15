#include "error.h"

#include <stdarg.h>
#include <stdio.h>

#include "util/intdef.h"

static void error_log(FILE *restrict stream, const char *restrict pfx, uint ln, const char *restrict file, const char *restrict fmt, va_list ap) {
	fprintf(stream, "(%s:%u) [%s] '", file, ln, pfx);

	vfprintf(stream, fmt, ap);

	fprintf(stream, "'\n");
}

void error_dbg(uint ln, const char *restrict file, const char *restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[95mDBG\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_inf(uint ln, const char *restrict file, const char *restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[94mINF\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_war(uint ln, const char *restrict file, const char *restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[93mWAR\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_err(uint ln, const char *restrict file, const char *restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[91mERR\033[0m", ln, file, fmt, ap);
	va_end(ap);
}

void error_fat(uint ln, const char *restrict file, const char *restrict fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	error_log(stdout, "\033[101mFAT\033[0m", ln, file, fmt, ap);
	va_end(ap);

	exit(EXIT_FAILURE);
}
