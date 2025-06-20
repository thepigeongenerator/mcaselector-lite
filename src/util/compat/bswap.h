#pragma once

#include "../types.h"

#if defined(__GNUC__) || defined(__clang__)
#define bswap16 __builtin_bswap16
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#elif __has_include(<byteswap.h>)
#include <byteswap.h>
#define bswap16 bswap_16
#define bswap32 bswap_32
#define bswap64 bswap_64
#else
#define bswap16 bswap16_impl
#define bswap32 bswap32_impl
#define bswap64 bswap64_impl
#define IMPL_BSWAP
#endif

#if defined(IMPL_BSWAP) || !defined(NDEBUG)
static inline u16 bswap16_impl(u16 x) {
	return (x << 8) | (x >> 8);
}

static inline u32 bswap32_impl(u32 x) {
	return (x << 24) |
		((0x0000FF00U & x) << 8) |
		((0x00FF0000U & x) >> 8) |
		(x >> 24);
}

static inline u64 bswap64_impl(u64 x) {
	return (x << 56) |
		((0x000000000000FF00ULL & x) << 40) |
		((0x0000000000FF0000ULL & x) << 24) |
		((0x00000000FF000000ULL & x) << 8) |
		((0x000000FF00000000ULL & x) >> 8) |
		((0x0000FF0000000000ULL & x) >> 24) |
		((0x00FF000000000000ULL & x) >> 40) |
		(x >> 56);
}
#endif
#undef IMPL_BSWAP
