/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#pragma once

#if defined(__unix__)
#define unixonly(_exec) _exec // executes inline code when __unix__ is defined, otherwise is no-op
#define winonly(_exec)        // (no-op) executes inline code when _WIN32 is defined, otherwise is no-op
#elif defined(_WIN32)
#define unixonly(_exec)       // (no-op) executes inline code when __unix__ is defined, otherwise is no-op
#define winonly(_exec)  _exec // executes inline code when _WIN32 is defined, otherwise is no-op
#else
#error platform unsupported!!
#endif
