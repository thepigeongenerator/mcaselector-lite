/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_TYPES_H
#define MCXEDIT_TYPES_H 1

#ifdef __CHECKER__
#define BITWISE __attribute__((bitwise))
#define FORCE   __attribute__((force))
#else
#define BITWISE
#define FORCE
#endif

typedef signed char        schar;
typedef long long          llong;
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

/* TODO: Convert these into their standard variations. */
#if __SIZEOF_SIZE_T__ == __SIZEOF_INT__
typedef uint size_t;
typedef int  ssize_t;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_LONG__
typedef ulong size_t;
typedef long  ssize_t;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_LONG_LONG__
typedef ullong size_t;
typedef llong  ssize_t;
#endif

typedef __INT8_TYPE__   __s8;
typedef __INT16_TYPE__  __s16;
typedef __INT32_TYPE__  __s32;
typedef __INT64_TYPE__  __s64;
typedef __UINT8_TYPE__  __u8;
typedef __UINT16_TYPE__ __u16;
typedef __UINT32_TYPE__ __u32;
typedef __UINT64_TYPE__ __u64;

typedef __u16 BITWISE __be16;
typedef __u32 BITWISE __be32;
typedef __u64 BITWISE __be64;
typedef __u16 BITWISE __le16;
typedef __u32 BITWISE __le32;
typedef __u64 BITWISE __le64;

#if __SIZEOF_FLOAT__ == 4
typedef float __f32;
#endif
#if __SIZEOF_DOUBLE__ == 8
typedef double __f64;
#endif

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
typedef __f32  f32;
typedef __f64  f64;
#endif

#endif /* MCXEDIT_TYPES_H */
