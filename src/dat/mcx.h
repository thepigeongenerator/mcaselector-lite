/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include <stdlib.h>

#include "../types.h"
#include "../util/atrb.h"

/* contains chunk metadata */
struct mcx_chunk {
	usize idx;  // byte offset for start of chunk data
	u32   len;  // byte length of chunk (+ padding)
	u32   time; // modification time in epoch seconds
};

/* Deletes a single chunk (`chunk`) out of `buf`.
 * The chunk's location data shall become `0`, and timestamp data the current time.
 * All succeeding chunks shall be moved back, freeing space.
 * Returns the amount of bytes removed. */
usize mcx_delchunk(u8 *restrict buf, int chunk) NONNULL((1));

/* Deletes the range defined by `start`—`end` (inclusive) of chunks out of `buf`.
 * The chunk's location data shall become `0`, and timestamp data the current time.
 * All succeeding chunks shall be moved back, freeing space.
 * Returns the amount of bytes removed */
usize mcx_delchunk_range(u8 *restrict buf, int start, int end) NONNULL((1));

/* Deletes a `chunkc` chunks from `chunks` out of `buf`.
 * If the `chunks` indices are known to be sequential, i.e. have a constant difference of `1`, `mcx_delchunk_range` should be preferred.
 * The chunk's location data shall become `0`, and timestamp data the current time.
 * All succeeding chunks shall be moved back, freeing space.
 * Returns the amount of bytes removed */
usize mcx_delchunk_bulk(u8 *restrict buf, const u16 *restrict chunks, int chunkc) NONNULL((1, 2));

/* Computes the byte size of the `*.mcX` file in `buf` and returns it. */
usize mcx_calcsize(const u8 *restrict buf) NONNULL((1)) PURE;
