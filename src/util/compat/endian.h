/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#if __has_include(<endian.h>)
#include <endian.h>
#else
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define le16toh(x) __uint16_identity(x)
#define le32toh(x) __uint32_identity(x)
#define le64toh(x) __uint64_identity(x)
#define htole16(x) __uint16_identity(x)
#define htole32(x) __uint32_identity(x)
#define htole64(x) __uint64_identity(x)
#define be16toh(x) __builtin_bswap16(x)
#define be32toh(x) __builtin_bswap32(x)
#define be64toh(x) __builtin_bswap64(x)
#define htobe16(x) __builtin_bswap16(x)
#define htobe32(x) __builtin_bswap32(x)
#define htobe64(x) __builtin_bswap64(x)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define le16toh(x) __builtin_bswap16(x)
#define le32toh(x) __builtin_bswap32(x)
#define le64toh(x) __builtin_bswap64(x)
#define htole16(x) __builtin_bswap16(x)
#define htole32(x) __builtin_bswap32(x)
#define htole64(x) __builtin_bswap64(x)
#define be16toh(x) __uint16_identity(x)
#define be32toh(x) __uint32_identity(x)
#define be64toh(x) __uint64_identity(x)
#define htobe16(x) __uint16_identity(x)
#define htobe32(x) __uint32_identity(x)
#define htobe64(x) __uint64_identity(x)
#else
#error machine architecture unsupported! Expected either big-endian or little-endian, make sure to use a compiler which defines __BYTE_ORDER__ (like clang or gcc)
#endif
#endif
