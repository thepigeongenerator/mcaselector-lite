// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#define WIDTHOF(t)       (sizeof(t) * 8) // gets the bit width of a type
#define MACRO_CAT(x, y)  x##y            // concatenate two macro variables together
#define MACRO_CAT2(x, y) MACRO_CAT(x, y) // concatenate two macro variables together
#define MACRO_STR(v)     #v              // for converting macro variable into a string
#define MACRO_STR2(v)    MACRO_STR(v)    // for a recursive string generation
