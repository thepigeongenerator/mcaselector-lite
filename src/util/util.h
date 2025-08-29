/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

/* Acquires the next power of two of value `x`.
 * Automatically determines the type (and therefore the width) of `x`.
 * Explicitly cast `x` to a desired width, if necessary. */
#define bit_ceil(x) (1 << (sizeof(__typeof__(x)) * 8 - __builtin_clzg(((x) - !!(x)) | 1)))
