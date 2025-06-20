// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include <stdint.h>

#include "../atrb.h"
#include "../types.h"
#include "bswap.h"

/* little endian */
atrb_const static inline u16 le16ton(u16); // converts little-endian (LE) encoding to native for a 16 bit integer. (NOOP if native is LE)
atrb_const static inline u32 le32ton(u32); // converts little-endian (LE) encoding to native for a 32 bit integer. (NOOP if native is LE)
atrb_const static inline u64 le64ton(u64); // converts little-endian (LE) encoding to native for a 64 bit integer. (NOOP if native is LE)
atrb_const static inline u16 ntole16(u16); // converts native encoding to little-endian (LE) for a 16 bit integer. (NOOP if native is LE)
atrb_const static inline u32 ntole32(u32); // converts native encoding to little-endian (LE) for a 32 bit integer. (NOOP if native is LE)
atrb_const static inline u64 ntole64(u64); // converts native encoding to little-endian (LE) for a 64 bit integer. (NOOP if native is LE)

/* big endian */
atrb_const static inline u16 be16ton(u16); // converts big-endian (BE) encoding to native for a 16 bit integer. (NOOP if native is BE)
atrb_const static inline u32 be32ton(u32); // converts big-endian (BE) encoding to native for a 32 bit integer. (NOOP if native is BE)
atrb_const static inline u64 be64ton(u64); // converts big-endian (BE) encoding to native for a 64 bit integer. (NOOP if native is BE)
atrb_const static inline u16 ntobe16(u16); // converts native encoding to big-endian (BE) for a 16 bit integer. (NOOP if native is BE)
atrb_const static inline u32 ntobe32(u32); // converts native encoding to big-endian (BE) for a 32 bit integer. (NOOP if native is BE)
atrb_const static inline u64 ntobe64(u64); // converts native encoding to big-endian (BE) for a 64 bit integer. (NOOP if native is BE)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
u16 le16ton(u16 x) { return x; }
u32 le32ton(u32 x) { return x; }
u64 le64ton(u64 x) { return x; }
u16 ntole16(u16 x) { return x; }
u32 ntole32(u32 x) { return x; }
u64 ntole64(u64 x) { return x; }
u16 be16ton(u16 x) { return bswap16(x); }
u32 be32ton(u32 x) { return bswap32(x); }
u64 be64ton(u64 x) { return bswap64(x); }
u16 ntobe16(u16 x) { return bswap16(x); }
u32 ntobe32(u32 x) { return bswap32(x); }
u64 ntobe64(u64 x) { return bswap64(x); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
u16 le16ton(u16 x) { bswap_16(x); }
u32 le32ton(u32 x) { bswap_32(x); }
u64 le64ton(u64 x) { bswap_64(x); }
u16 ntole16(u16 x) { bswap_16(x); }
u32 ntole32(u32 x) { bswap_32(x); }
u64 ntole64(u64 x) { bswap_64(x); }
u16 be16ton(u16 x) { return x; }
u32 be32ton(u32 x) { return x; }
u64 be64ton(u64 x) { return x; }
u16 ntobe16(u16 x) { return x; }
u32 ntobe32(u32 x) { return x; }
u64 ntobe64(u64 x) { return x; }
#else
#error machine architecture unsupported! Expected either big-endian or little-endian, make sure to use a compiler which defines __BYTE_ORDER__ (like clang or gcc)
#endif
