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

### style guide
- Code must be written correctly, read [Correct C](./correct-c.md) if more information is required.
- K&R style braces/[indentation](https://en.wikipedia.org/wiki/Indentation_style)
- typedefs are discouraged
- snake_case is used for all user-defined symbols. Macros are often all-uppercase, same goes for enums and other types of compile-time constants.
- tabs are used for indentation, spaces are used for alignment.
- British spelling is preferred, but not enforced. What is enforced is that British variants of the symbols are available.
- commits should attempt to convey clearly what is being changed, for the sanity of the maintainer(s).
