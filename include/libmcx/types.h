/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_TYPES_H
#define MCXEDIT_TYPES_H 1

#include <stddef.h>
#include <stdint.h>

/* __CHECKER__ is defined by sparse, here we use it to define some
 * sparse-specific attributes we'll be using in this header. */
#ifdef __CHECKER__
#define BITWISE __attribute__((bitwise))
#define FORCE   __attribute__((force))
#else
#define BITWISE
#define FORCE
#endif /* __CHECKER__ */

typedef signed char        schar;
typedef long long          llong;
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

typedef int8_t   __s8;
typedef int16_t  __s16;
typedef int32_t  __s32;
typedef int64_t  __s64;
typedef uint8_t  __u8;
typedef uint16_t __u16;
typedef uint32_t __u32;
typedef uint64_t __u64;

typedef __u16 BITWISE __be16;
typedef __u32 BITWISE __be32;
typedef __u64 BITWISE __be64;
typedef __u16 BITWISE __le16;
typedef __u32 BITWISE __le32;
typedef __u64 BITWISE __le64;

#ifdef MCXEDIT_SOURCE
typedef __s8   s8;
typedef __s16  s16;
typedef __s32  s32;
typedef __s64  s64;
typedef __u8   u8;
typedef __u16  u16;
typedef __u32  u32;
typedef __u64  u64;
typedef __be16 be16;
typedef __be32 be32;
typedef __be64 be64;
typedef __le16 le16;
typedef __le32 le32;
typedef __le64 le64;

#if __SIZEOF_FLOAT__ == 4
typedef float f32;
#endif
#if __SIZEOF_DOUBLE__ == 8
typedef double f64;
#endif
#endif /* MCXEDIT_SOURCE */

#endif /* MCXEDIT_TYPES_H */
