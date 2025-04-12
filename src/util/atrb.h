#pragma once


// default attribute definitions (empty)
#define atrb_deprecated
#define atrb_unused
#define atrb_pure
#define atrb_const
#define atrb_format()
#define atrb_nonnull()

// define the attributes where possible
#if defined(__GNUC__) || defined(__clang__)

# if __has_attribute(deprecated)
#  undef atrb_deprecated
#  define atrb_deprecated __attribute__((deprecated))
# endif

# if __has_attribute(unused)
#  undef atrb_unused
#  define atrb_unused __attribute__((unused))
# endif

# if __has_attribute(pure)
#  undef atrb_pure
#  define atrb_pure __attribute__((pure))
# endif


# if __has_attribute(const)
#  undef atrb_const
#  define atrb_const __attribute__((const))
# endif

# if __has_attribute(format)
#  undef atrb_format
#  define atrb_format(...) __attribute__((format(__VA_ARGS__)))
# endif

# if __has_attribute(nonnull)
#  undef atrb_nonnull
#  define atrb_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
# endif
#elif defined(_MSC_VER)
# undef atrb_depatrb_deprecated
# define atrb_deprecated __declspec(deprecated)
#endif

