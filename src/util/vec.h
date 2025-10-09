/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include "../types.h"

#if defined(__has_attribute) && __has_attribute(vector_size)
typedef float fvec2 __attribute__((vector_size(sizeof(float) * 2))); // SMID vector for 2 `float`
typedef float fvec4 __attribute__((vector_size(sizeof(float) * 4))); // SMID vector for 4 `float`

typedef u8 u8vec2 __attribute__((vector_size(sizeof(u8) * 2))); // SMID vector for 2 `u8`
typedef u8 u8vec4 __attribute__((vector_size(sizeof(u8) * 4))); // SMID vector for 4 `u8`
#else
#error the platform is unsupported, attribute vector_size must be available (and so does __has_attribute)
#endif

#define VX 0
#define VY 1
#define VZ 2
#define VW 3
