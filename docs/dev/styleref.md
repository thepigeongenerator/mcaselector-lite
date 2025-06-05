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
- parameterless functions should have the `void` parameter.[^cstd]
- symbols mustn't be prefixed with `_`; this is a C standard reserved symbol.[^cstd]
- typedefs (or anything else for that matter) mustn't be suffixed with `_t`, this is reserved by POSIX.[^cstd]
- functions should do one thing, and do that thing well.[^cstd]
- K&R style braces/indentation[^wikiindent]
- typedefs are discouraged
- snake_case is used for all user-defined symbols. Macros are often all-uppercase, same goes for enums and other types of compile-time constants.
- tabs are used for indentation, spaces are used for alignment.
- British spelling is preferred, but not enforced. What is enforced is that British variants of the symbols are available.
- commits should attempt to convey clearly what is being changed, for the sanity of the maintainer(s).

[cstd]: this is a general C practice, included since it is something commonly done incorrectly.
[wikiindent]: <https://en.wikipedia.org/wiki/Indentation_style>
