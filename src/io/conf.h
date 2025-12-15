#pragma once
#include "../types.h"
#include "../util/atrb.h"

/* Gets the key and value, if present. Writes the pointer for the value to `out`.
 * Returns the key index, or <0 upon failure. */
int conf_getkeyval(const char *restrict buf, const char *const restrict *restrict keys, int klen,
	const char *restrict *restrict out) NONNULL((1, 2, 4));

/* Processes the value of `type` in `val`. Outputs to `out`.
 * Returns non-zero on failure. */
int conf_procval(u8 type, const char *restrict val, void *restrict out) NONNULL((2, 3));
