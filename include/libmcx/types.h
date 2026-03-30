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

typedef int8_t   mcx_s8;
typedef int16_t  mcx_s16;
typedef int32_t  mcx_s32;
typedef int64_t  mcx_s64;
typedef uint8_t  mcx_u8;
typedef uint16_t mcx_u16;
typedef uint32_t mcx_u32;
typedef uint64_t mcx_u64;

typedef mcx_u16 BITWISE mcx_be16;
typedef mcx_u32 BITWISE mcx_be32;
typedef mcx_u64 BITWISE mcx_be64;
typedef mcx_u16 BITWISE mcx_le16;
typedef mcx_u32 BITWISE mcx_le32;
typedef mcx_u64 BITWISE mcx_le64;

#ifdef MCXEDIT_SOURCE
typedef mcx_s8   s8;
typedef mcx_s16  s16;
typedef mcx_s32  s32;
typedef mcx_s64  s64;
typedef mcx_u8   u8;
typedef mcx_u16  u16;
typedef mcx_u32  u32;
typedef mcx_u64  u64;
typedef mcx_be16 be16;
typedef mcx_be32 be32;
typedef mcx_be64 be64;
typedef mcx_le16 le16;
typedef mcx_le32 le32;
typedef mcx_le64 le64;

#if __SIZEOF_FLOAT__ == 4
typedef float f32;
#endif
#if __SIZEOF_DOUBLE__ == 8
typedef double f64;
#endif
#endif /* MCXEDIT_SOURCE */

#endif /* MCXEDIT_TYPES_H */
