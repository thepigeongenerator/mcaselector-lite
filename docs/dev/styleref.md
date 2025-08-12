# mcaselector-lite style reference
## contents
- [recommended tool chains](#recommended-tool-chains)
- [style guide](#style-guide)

### recommended tools
- gcc
- git (duh)
- clang-format
- clang-tidy
- linux / unix-like machine

### code information
The project is written in the [GNU dialect](https://gcc.gnu.org/onlinedocs/gcc/C-Dialect-Options.html) of [C99](https://www.en.cppreference.com/w/c/99.html).
Where we have dependencies on:
| library                           | summary                                                 |
|:----------------------------------|:--------------------------------------------------------|
| [libarchive](libarchive.org)      | compression / decompression of various formats.         |
| [glfw](https://www.glfw.org/)     | window creation / input handling.                       |
| [openGL](https://www.opengl.org/) | hardware accelleration, for handling graphics.          |

It is intended to be platform-agnostic, within reason. But the main focus is for [UNIX](https://unix.org/)-based systems with [x86_64](https://en.wikipedia.org/wiki/X86-64) architecture.
Within [intdef.h](/src/util/intdef.h) there live definitions for fixed-width integer types.

### style guide
- Code must be written correctly, read [Correct C](./correct-c.md) if more information is required.
- `snake_casing` is used, with no [Hungarian notation](https://en.m.wikipedia.org/wiki/Hungarian_notation). (macros are all-uppercase, with a few exceptions)
- [K&R style braces/indentation](https://en.wikipedia.org/wiki/Indentation_style) should be used.
- For indentation tabs are used, and are assumed to be 8 spaces wide. For alignment spaces should be used.
- A space should precede a pointer `*`. Where it is attached to the name, rather than the type. For `const` clarity, and chains such as: `u8 *a, *b, *c;`. This is true for functions as well: `void *malloc(size_t n);`
- Generally speaking, British spelling is preferred. Define potential macros for, or when using alternate dialects.
- Grammar should be correct. (`don't` or `do not`, not `dont`)
- Commits should attempt to convey clearly what is being changed, for the sanity of the maintainer(s).
