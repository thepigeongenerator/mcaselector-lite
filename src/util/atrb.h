/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See the project's LICENSE file for details. */
#pragma once

#if defined(__GNUC__)
#if __has_attribute(__pure__)
#define PURE __attribute__((__pure__))
#else
#define PURE
#endif

#if __has_attribute(__const__)
#define CONST __attribute__((__const__))
#else
#define CONST
#endif

#if __has_attribute(__noreturn__)
#define NORET __attribute__((__noreturn__))
#else
#define NORET
#endif

#if __has_attribute(__malloc__)
#define MALLOC __attribute__((__malloc__))
#else
#define MALLOC
#endif

#if __has_attribute(__used__)
#define USED __attribute__((__used__))
#else
#define USED
#endif

#if __has_attribute(__unused__)
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

#if __has_attribute(__deprecated__)
#define DEPRECATED __attribute__((__deprecated__))
#else
#define DEPRECATED
#endif

#if __has_attribute(__format__)
#define FORMAT(args) __attribute__((format args))
#else
#define FORMAT(args)
#endif

#if __has_attribute(__nonnull__)
#define NONNULL(args) __attribute__((__nonnull__ args))
#else
#define NONNULL(args)
#endif
#endif
