/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */

#ifndef PORTABLE_ENDIAN_H
#define PORTABLE_ENDIAN_H 1

#if defined(__GNUC__)

/* test for the byteswap header */
#if __has_include(<byteswap.h>)
#include <byteswap.h>
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)
#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define be16toh(x) bswap_16(x)
#define be32toh(x) bswap_32(x)
#define be64toh(x) bswap_64(x)
#define htobe16(x) bswap_16(x)
#define htobe32(x) bswap_32(x)
#define htobe64(x) bswap_64(x)
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define le16toh(x) bswap_16(x)
#define le32toh(x) bswap_32(x)
#define le64toh(x) bswap_64(x)
#define htole16(x) bswap_16(x)
#define htole32(x) bswap_32(x)
#define htole64(x) bswap_64(x)
#define be16toh(x) (x)
#define be32toh(x) (x)
#define be64toh(x) (x)
#define htobe16(x) (x)
#define htobe32(x) (x)
#define htobe64(x) (x)
#else
#error machine architecture unsupported! Expected either big-endian or little-endian, make sure to use a compiler which defines __BYTE_ORDER__ (like clang or gcc)
#endif /* byte order */

/* otherwise, utilise the compiler built-ins */
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
#endif /* byte order */

#endif /* has byteswap.h */

#else
#error GNU C is unavailable
#endif /* __GNUC__ */

#endif /* PORTABLE_ENDIAN_H */
