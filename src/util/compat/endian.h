// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include <byteswap.h>
#include <stdint.h>

#include "../atrb.h"

/* little endian */
atrb_const static inline uint16_t le16ton(uint16_t); // converts little-endian (LE) encoding to native for a 16 bit integer. (NOOP if native is LE)
atrb_const static inline uint32_t le32ton(uint32_t); // converts little-endian (LE) encoding to native for a 32 bit integer. (NOOP if native is LE)
atrb_const static inline uint64_t le64ton(uint64_t); // converts little-endian (LE) encoding to native for a 64 bit integer. (NOOP if native is LE)
atrb_const static inline uint16_t ntole16(uint16_t); // converts native encoding to little-endian (LE) for a 16 bit integer. (NOOP if native is LE)
atrb_const static inline uint32_t ntole32(uint32_t); // converts native encoding to little-endian (LE) for a 32 bit integer. (NOOP if native is LE)
atrb_const static inline uint64_t ntole64(uint64_t); // converts native encoding to little-endian (LE) for a 64 bit integer. (NOOP if native is LE)

/* big endian */
atrb_const static inline uint16_t be16ton(uint16_t); // converts big-endian (BE) encoding to native for a 16 bit integer. (NOOP if native is BE)
atrb_const static inline uint32_t be32ton(uint32_t); // converts big-endian (BE) encoding to native for a 32 bit integer. (NOOP if native is BE)
atrb_const static inline uint64_t be64ton(uint64_t); // converts big-endian (BE) encoding to native for a 64 bit integer. (NOOP if native is BE)
atrb_const static inline uint16_t ntobe16(uint16_t); // converts native encoding to big-endian (BE) for a 16 bit integer. (NOOP if native is BE)
atrb_const static inline uint32_t ntobe32(uint32_t); // converts native encoding to big-endian (BE) for a 32 bit integer. (NOOP if native is BE)
atrb_const static inline uint64_t ntobe64(uint64_t); // converts native encoding to big-endian (BE) for a 64 bit integer. (NOOP if native is BE)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
uint16_t le16ton(uint16_t x) { return x; }
uint32_t le32ton(uint32_t x) { return x; }
uint64_t le64ton(uint64_t x) { return x; }
uint16_t ntole16(uint16_t x) { return x; }
uint32_t ntole32(uint32_t x) { return x; }
uint64_t ntole64(uint64_t x) { return x; }
uint16_t be16ton(uint16_t x) { return bswap_16(x); }
uint32_t be32ton(uint32_t x) { return bswap_32(x); }
uint64_t be64ton(uint64_t x) { return bswap_64(x); }
uint16_t ntobe16(uint16_t x) { return bswap_16(x); }
uint32_t ntobe32(uint32_t x) { return bswap_32(x); }
uint64_t ntobe64(uint64_t x) { return bswap_64(x); }
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
uint16_t le16ton(uint16_t x) { bswap_16(x); }
uint32_t le32ton(uint32_t x) { bswap_32(x); }
uint64_t le64ton(uint64_t x) { bswap_64(x); }
uint16_t ntole16(uint16_t x) { bswap_16(x); }
uint32_t ntole32(uint32_t x) { bswap_32(x); }
uint64_t ntole64(uint64_t x) { bswap_64(x); }
uint16_t be16ton(uint16_t x) { return x; }
uint32_t be32ton(uint32_t x) { return x; }
uint64_t be64ton(uint64_t x) { return x; }
uint16_t ntobe16(uint16_t x) { return x; }
uint32_t ntobe32(uint32_t x) { return x; }
uint64_t ntobe64(uint64_t x) { return x; }
#else
#error machine architecture unsupported! Expected either big-endian or little-endian, make sure to use a compiler which defines __BYTE_ORDER__ (like clang or gcc)
#endif
