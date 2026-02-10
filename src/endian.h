/* Copyright (C)2025-2026 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#ifndef MCASELECTOR_LITE_ENDIAN_H
#define MCASELECTOR_LITE_ENDIAN_H

#include "types.h"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define cvt_le16toh(x) ((FORCE u16)(le16)x)
#define cvt_le32toh(x) ((FORCE u32)(le32)x)
#define cvt_le64toh(x) ((FORCE u64)(le64)x)
#define cvt_htole16(x) ((FORCE le16)(u16)x)
#define cvt_htole32(x) ((FORCE le32)(u32)x)
#define cvt_htole64(x) ((FORCE le64)(u64)x)
#define cvt_be16toh(x) (__builtin_bswap16((FORCE u16)(be16)x))
#define cvt_be32toh(x) (__builtin_bswap32((FORCE u32)(be32)x))
#define cvt_be64toh(x) (__builtin_bswap64((FORCE u64)(be64)x))
#define cvt_htobe16(x) ((FORCE be16)__builtin_bswap16(x))
#define cvt_htobe32(x) ((FORCE be32)__builtin_bswap32(x))
#define cvt_htobe64(x) ((FORCE be64)__builtin_bswap64(x))
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define cvt_le16toh(x) (__builtin_bswap16((FORCE u16)(le16)x))
#define cvt_le32toh(x) (__builtin_bswap32((FORCE u32)(le32)x))
#define cvt_le64toh(x) (__builtin_bswap64((FORCE u64)(le64)x))
#define cvt_htole16(x) ((FORCE le16)__builtin_bswap16(x))
#define cvt_htole32(x) ((FORCE le32)__builtin_bswap32(x))
#define cvt_htole64(x) ((FORCE le64)__builtin_bswap64(x))
#define cvt_be16toh(x) ((FORCE u16)(be16)x)
#define cvt_be32toh(x) ((FORCE u32)(be32)x)
#define cvt_be64toh(x) ((FORCE u64)(be64)x)
#define cvt_htobe16(x) ((FORCE be16)(u16)x)
#define cvt_htobe32(x) ((FORCE be32)(u32)x)
#define cvt_htobe64(x) ((FORCE be64)(u64)x)
#else
#error "Machine architecture unsupported! Expected either big-endian or little-endian."
#endif

#endif /* MCASELECTOR_LITE_ENDIAN_H */
