/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See included LICENSE file for details. */
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
