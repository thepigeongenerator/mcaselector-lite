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
Within [intdef.h](/src/util/intdef.h) there live definitions for static-width integer types.

### style guide
- Code must be written correctly, read [Correct C](./correct-c.md) if more information is required.
- K&R style braces/[indentation](https://en.wikipedia.org/wiki/Indentation_style)
- typedefs are discouraged
- snake_case is used for all user-defined symbols. Macros are often all-uppercase, same goes for enums and other types of compile-time constants.
- tabs are used for indentation, spaces are used for alignment.
- British spelling is (generally) preferred, but not enforced.
- commits should attempt to convey clearly what is being changed, for the sanity of the maintainer(s).
