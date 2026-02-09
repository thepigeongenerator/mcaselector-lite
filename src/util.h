/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#ifndef MCASELECTOR_LITE_UTIL_H
#define MCASELECTOR_LITE_UTIL_H

/* Acquires the next power of two of value `x`.
 * Automatically determines the type (and therefore the width) of `x`.
 * Explicitly cast `x` to a desired width, if necessary. */
#define bit_ceil(x) (1 << (sizeof(__typeof__(x)) * 8 - __builtin_clzg(((x) - !!(x)) | 1)))

#endif /* MCASELECTOR_LITE_UTIL_H */
