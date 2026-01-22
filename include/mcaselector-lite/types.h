/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#ifndef MCASELECTOR_LITE_TYPES_H
#define MCASELECTOR_LITE_TYPES_H

typedef signed long long int llong;
typedef unsigned short int   ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;
typedef unsigned long long   ullong;
typedef __INT8_TYPE__        s8;
typedef __INT16_TYPE__       s16;
typedef __INT32_TYPE__       s32;
typedef __INT64_TYPE__       s64;
typedef __UINT8_TYPE__       u8;
typedef __UINT16_TYPE__      u16;
typedef __UINT32_TYPE__      u32;
typedef __UINT64_TYPE__      u64;
typedef __SIZE_TYPE__        usize;
typedef __INTPTR_TYPE__      intptr;
typedef __UINTPTR_TYPE__     uintptr;

#ifdef __CHECKER__
#define BITWISE __attribute__((bitwise))
#define FORCE __attribute__((force))
#else
#define BITWISE
#define FORCE
#endif

typedef u16 BITWISE be16;
typedef u32 BITWISE be32;
typedef u64 BITWISE be64;
typedef u16 BITWISE le16;
typedef u32 BITWISE le32;
typedef u64 BITWISE le64;

#if __SIZEOF_SIZE_T__ == __SIZEOF_LONG_LONG__
typedef llong ssize;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_LONG__
typedef long ssize;
#elif __SIZEOF_SIZE_T__ == __SIZEOF_INT__
typedef int ssize;
#endif

#if __SIZEOF_FLOAT__ == 4
typedef float f32;
#endif
#if __SIZEOF_DOUBLE__ == 8
typedef double f64;
#endif
#endif /* MCASELECTOR_LITE_TYPES_H */
