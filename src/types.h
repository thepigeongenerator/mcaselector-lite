/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See included LICENSE file for details. */
#pragma once

typedef signed long long int llong;
typedef unsigned short int   ushort;
typedef unsigned int         uint;
typedef unsigned long        ulong;
typedef unsigned long long   ullong;
typedef __INT8_TYPE__        i8;
typedef __INT16_TYPE__       i16;
typedef __INT32_TYPE__       i32;
typedef __INT64_TYPE__       i64;
typedef __UINT8_TYPE__       u8;
typedef __UINT16_TYPE__      u16;
typedef __UINT32_TYPE__      u32;
typedef __UINT64_TYPE__      u64;
typedef __SIZE_TYPE__        usize;
typedef __INTPTR_TYPE__      intptr;
typedef __UINTPTR_TYPE__     uintptr;

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
