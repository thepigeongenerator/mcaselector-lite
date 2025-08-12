# Correct C
A monster list about bad code practices for the absolute beginners.
Things obviously should be taken with a grain of salt, just stating the obvious.

- Functions without parameters will be defined with `void` in the parameter list, as opposed to leaving it empty. (e.i. `(void)` rather than `()`)
- (public-facing) names mustn't be prefixed with `_`, this is a reserved identifier.
- `typedef`s mustn't be suffixed with `_t`, this is a POSIX reserved identifier.
- `typedef`s are discouraged, unless the name alias is clear in what it stores. (e.i. `u32` for a 32 bit unsigned integer)
- Functions should do one thing, and do it well.
- `inline` functions should serve as a replacement for macro definitions, don't put things in here that you wouldn't put in a macro. (block ≤5 lines)
- Mark all implementations in a `*.c` file with `static` if there isn't a matching definition in a `*.h` file. (unless there's a clear reason to)
- When a function parameter takes in a pointer, and does not modify the pointed at data, a const pointer should be used. (`const int *ptr`)
- Try to limit yourself at ~112 columns, but generally avoid overly long lines.
