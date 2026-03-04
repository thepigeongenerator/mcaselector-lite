/* Copyright (C)2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_ERR_H
#define MCXEDIT_ERR_H 1

#include <stdarg.h>

#include "atrb.h"

void verr(int code, const char *fmt, va_list args) COLD NORET;
void verrx(int code, const char *fmt, va_list args) COLD NORET;
void vwarn(const char *fmt, va_list args) COLD;
void vwarnx(const char *fmt, va_list args) COLD;
void err(int code, const char *fmt, ...) COLD FORMAT((printf, 2, 3)) NORET;
void errx(int code, const char *fmt, ...) COLD FORMAT((printf, 2, 3)) NORET;
void warn(const char *fmt, ...) COLD FORMAT((printf, 1, 2));
void warnx(const char *fmt, ...) COLD FORMAT((printf, 1, 2));

#endif /* MCXEDIT_ERR_H */
