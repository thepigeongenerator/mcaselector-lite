/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

/* variable-width integer types */
typedef unsigned int uint;         // ≥16 bit unsigned integer
typedef unsigned long ulong;       // ≥32 bit unsigned integer
typedef signed long long llong;    // ≥64 bit signed integer
typedef unsigned long long ullong; // ≥64 bit unsigned integer

/* fixed-width integer types */
#include <stdint.h>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/* floating point types */
typedef float f32;  // single-precision floating-point
typedef double f64; // double-precision floating-point
