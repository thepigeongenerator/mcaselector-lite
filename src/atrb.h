/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_ATRB_H
#define MCXEDIT_ATRB_H 1

#ifndef __GNUC__
#error "Compiler unsupported; use a compiler that supports GNU C extensions."
#endif

/* Function is unlikely to be executed, and shall be compiled for size. */
#define COLD __attribute__((__cold__))

/* The return value is not affected by changes to the observable
 * state of the program, and has no observable effect on such state.
 * NOTE: The function mustn't examine data pointed at by pointers. */
#define CONST __attribute__((__const__))

/* Specifies that the function takes `printf`, `scanf`, `strftime`,
 * or `strfmon` style arguments, which must be type-checked against
 * a format string. */
#define FORMAT(args) __attribute__((__format__ args))

/* Function is in a particular hot section of the code, and shall be aggressively
 * optimised for performance. Furthermore, it will be placed
 * inside a special subsection in .text, which improves locality. */
#define HOT __attribute__((__hot__))

/* Indicates that the function is `malloc`-like, i.e. the returned
 * pointer may not alias any other pointer valid when the function returns.
 * Furthermore, this pointer is assumed to not be `NULL` in most cases.
 * Specifying an argument defines the deallocator that must be used to
 * deallocate the returned pointer. Which may be the allocator function.
 * Where the second argument specifies to which parameter the pointer
 * should be written, when calling the deallocator.
 * When no arguments have been specified, MALLOC means that the
 * returned pointer is non-aliassing, due to being newly obtained.
 * `realloc` does not meet this requirement.
 * Both versions should be used if both can be satisfied. */
#define MALLOC            __attribute__((__malloc__))
#define MALLOC_ARGS(args) __attribute__((__malloc__ args))


/* Specifies that the arguments specified in the macro arguments
 * may not be NULL. It may be used for both function calls, and definitions. */
#if __has_attribute(__nonnull__)
#define NONNULL(args) __attribute__((__nonnull__ args))
#define nonnull       __attribute__((__nonnull__))
#else
#define NONNULL(args)
#define nonnull
#endif /* __has_attribute(__nonnull__) */

/* Marks that a function will not return, such as
 * entering an infinite loop, or exiting the program. */
#define NORET __attribute__((__noreturn__))

/* The function may not have an effect on the observable state of the programme.
 * Unlike `const`, a pure function may read memory, even if it changes between
 * calls. */
#define PURE __attribute__((__pure__))

/* Marks a function as unused and will prevent GCC from emitting a warning. */
#define UNUSED __attribute__((__unused__))

/* Marks a function as used, and will cause the code must be emitted, even if it appears
 * like the function is never referenced. */
#define USED __attribute__((__used__))

/* Specifies an assumption, takes a conditional expression where the value must evaluate to
 * true where it appears. */
#if __has_attribute(__assume__)
#define ASSUME(args) __attribute__((__assume__ args))
#else
#define ASSUME(args)
#endif /* __has_attribute(__assume__) */

/* Explicitly defines that a statement falls through to a following case
 * or user-defined label. */
#if __has_attribute(__fallthrough__)
#define FALLTHROUGH __attribute__((__fallthrough__))
#else
#define FALLTHROUGH
#endif /* __has_attribute(__fallthrough__) */

#endif /* MCXEDIT_ATRB_H */
