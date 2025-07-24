// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

// define the attributes where possible
#if defined(__GNUC__) || defined(__clang__)
#define atrb(...)         __attribute__(__VA_ARGS__)
#define atrb_deprecated   __attribute__((depricated))
#define atrb_unused       __attribute__((unused))
#define atrb_pure         __attribute__((pure))
#define atrb_const        __attribute__((const))
#define atrb_noreturn     __attribute__((noreturn))
#define atrb_malloc       __attribute__((malloc))
#define atrb_format(...)  __attribute__((format(__VA_ARGS__)))
#define atrb_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
#elif defined(_MSC_VER)
#define atrb(...)       __declspec(__VA_ARGS__)
#define atrb_deprecated __declspec(deprecated)
#define atrb_noreturn   __declspec(noreturn)
#else
#define atrb()
#define atrb_deprecated
#define atrb_unused
#define atrb_pure
#define atrb_const
#define atrb_noreturn
#define atrb_malloc
#define atrb_format()
#define atrb_nonnull()
#endif
