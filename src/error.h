#pragma once
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "util/atrb.h"

#define ERROR_BUF_SIZE 128

atrb_nonnull(1) atrb_format(printf, 1, 2) static inline void debug(char const*, ...);          // prints a '\n' suffixed debug msg to stdout with a maximum buf size of @ref ERROR_BUF_SIZE
atrb_nonnull(1) atrb_format(printf, 1, 2) static inline void info(char const*, ...);           // prints a '\n' suffixed info msg to stdout with a maximum buf size of @ref ERROR_BUF_SIZE
atrb_nonnull(1) atrb_format(printf, 1, 2) static inline void warn(char const*, ...);           // prints a '\n' suffixed warn msg to stderr with a maximum buf size of @ref ERROR_BUF_SIZE
atrb_nonnull(1) atrb_format(printf, 1, 2) static inline void error(char const*, ...);          // prints a '\n' suffixed error msg to stderr with a maximum buf size of @ref ERROR_BUF_SIZE
atrb_nonnull(1) atrb_format(printf, 1, 2) noreturn static inline void fatal(char const*, ...); // prints a '\n' suffixed fatal msg to stderr with a maximum buf size of @ref ERROR_BUF_SIZE and immediately aborts execution

// macro to write fmt vardiac args to buf
#define WRITE_VA_ARGS(buf, fmt)                    \
	{                                              \
		va_list args;                              \
		va_start(args, fmt);                       \
		vsnprintf(buf, ERROR_BUF_SIZE, fmt, args); \
		va_end(args);                              \
	};

static inline void debug(char const* restrict fmt, ...) {
	char buf[ERROR_BUF_SIZE] = {0};
	WRITE_VA_ARGS(buf, fmt);
	printf("\033[95mD: \"%s\"\033[0m\n", buf); // reduced parameter use by not using fprintf
}

static inline void info(char const* fmt, ...) {
	char buf[ERROR_BUF_SIZE] = {0};
	WRITE_VA_ARGS(buf, fmt);
	printf("I: \"%s\"\n", buf);
}

static inline void warn(char const* fmt, ...) {
	char buf[ERROR_BUF_SIZE] = {0};
	WRITE_VA_ARGS(buf, fmt);
	fprintf(stderr, "\033[93mW: \"%s\"\033[0m\n", buf);
}

static inline void error(char const* fmt, ...) {
	char buf[ERROR_BUF_SIZE] = {0};
	WRITE_VA_ARGS(buf, fmt);
	fprintf(stderr, "\033[91mE: \"%s\"\033[0m\n", buf);
}

static inline void fatal(char const* fmt, ...) {
	char buf[ERROR_BUF_SIZE] = {0};
	WRITE_VA_ARGS(buf, fmt);
	fprintf(stderr, "\033[101mF: \"%s\"\033[0m\n", buf);
	abort();
}

#undef WRITE_VA_ARGS
