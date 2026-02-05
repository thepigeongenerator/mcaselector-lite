/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#pragma once
#include <mcaselector-lite/atrb.h>
#include <mcaselector-lite/types.h>

enum conf_type {
	CONF_STR = 0x00,
	CONF_S8  = 0x00 | 0x01,
	CONF_S16 = 0x00 | 0x02,
	CONF_S32 = 0x00 | 0x04,
	CONF_S64 = 0x00 | 0x08,
	CONF_U8  = 0x10 | 0x01,
	CONF_U16 = 0x10 | 0x02,
	CONF_U32 = 0x10 | 0x04,
	CONF_U64 = 0x10 | 0x08,
	CONF_F32 = 0x20 | 0x04,
	CONF_F64 = 0x20 | 0x08,
};

/* Gets the key and value, if present. Writes the pointer for the value to `out`.
 * Returns the key index, or <0 upon failure. */
int conf_getkeyval(const char *restrict buf, const char *const restrict *restrict keys, int klen,
	const char *restrict *restrict out) NONNULL((1, 2, 4));

/* Processes the value of `type` in `val`. Outputs to `out`.
 * Returns non-zero on failure. */
int conf_procval(u8 type, const char *restrict val, void *restrict out) NONNULL((2, 3));
