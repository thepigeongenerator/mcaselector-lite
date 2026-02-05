/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#ifndef MCASELECTOR_LITE_ENDIAN_H
#define MCASELECTOR_LITE_ENDIAN_H

#include <mcaselector-lite/types.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
static inline u16  cvt_le16toh(le16 le) { return (FORCE u16)le; }
static inline u32  cvt_le32toh(le32 le) { return (FORCE u32)le; }
static inline u64  cvt_le64toh(le64 le) { return (FORCE u64)le; }
static inline le16 cvt_htole16(u16 u) { return (FORCE le16)u; }
static inline le32 cvt_htole32(u32 u) { return (FORCE le32)u; }
static inline le64 cvt_htole64(u64 u) { return (FORCE le64)u; }
static inline u16  cvt_be16toh(be16 be) { return __builtin_bswap16((FORCE u16)be); }
static inline u32  cvt_be32toh(be32 be) { return __builtin_bswap32((FORCE u32)be); }
static inline u64  cvt_be64toh(be64 be) { return __builtin_bswap64((FORCE u64)be); }
static inline be16 cvt_htobe16(u16 u) { return (FORCE be16)__builtin_bswap16(u); }
static inline be32 cvt_htobe32(u32 u) { return (FORCE be32)__builtin_bswap32(u); }
static inline be64 cvt_htobe64(u64 u) { return (FORCE be64)__builtin_bswap64(u); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
static inline u16  cvt_le16toh(le16 le) { return __builtin_bswap16((FORCE u16)le); }
static inline u32  cvt_le32toh(le32 le) { return __builtin_bswap32((FORCE u32)le); }
static inline u64  cvt_le64toh(le64 le) { return __builtin_bswap64((FORCE u64)le); }
static inline le16 cvt_htole16(u16 u) { return (FORCE le16)__builtin_bswap16(u); }
static inline le32 cvt_htole32(u32 u) { return (FORCE le32)__builtin_bswap32(u); }
static inline le64 cvt_htole64(u64 u) { return (FORCE le64)__builtin_bswap64(u); }
static inline u16  cvt_be16toh(be16 be) { return (FORCE u16)be; }
static inline u32  cvt_be32toh(be32 be) { return (FORCE u32)be; }
static inline u64  cvt_be64toh(be64 be) { return (FORCE u64)be; }
static inline be16 cvt_htobe16(u16 u) { return (FORCE be16)u; }
static inline be32 cvt_htobe32(u32 u) { return (FORCE be32)u; }
static inline be64 cvt_htobe64(u64 u) { return (FORCE be64)u; }
#else
#error "Machine architecture unsupported! Expected either big-endian or little-endian."
#endif

#endif /* MCASELECTOR_LITE_ENDIAN_H */
