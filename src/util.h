/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_UTIL_H
#define MCXEDIT_UTIL_H 1
#include "err.h"

extern const char *argv0;

#ifdef NDEBUG
#define UNREACHABLE() __builtin_unreachable()
#else
#define UNREACHABLE() errx(-5, "Reached unreachable point at %s:%d!", __FILE__, __LINE__)
#endif

#endif /* MCXEDIT_UTIL_H */
