/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "mcx.h"

#include <assert.h>
#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../util/intdef.h"

#define SECTOR 0x1000 // sector size
#define TABLE  0x800  // table (total) element count
#define CHUNKS 0x400  // amount of chunks in a file

/* Moves chunks `src_s` to `src_e` (inclusive) from `src`, back onto `dst`. */
static void mvchunks(u8 *dst, u8 *src, u32 *restrict table, int src_s, int src_e) {
	assert(src > dst);
	size_t len = src - dst; // acquire the amount of bytes that we shall move
	assert(!(len % SECTOR));

	// count how many bytes we need to move, whilst updating location data
	size_t blen = 0;
	for (src_s++; src_s <= src_e; src_s++) {
		blen += (be32toh(table[src_s]) & 0xFF) * SECTOR;
		table[src_s] -= htobe32((len / SECTOR) << 8);
	}
	memmove(dst, src, blen);
}

/* Deletes chunk `sidx` by moving chunks up to `eidx` back over `sidx` in `buf`.
 * `rmb` is an optional additional offset that can be applied, and signifies bytes already removed.
 * Returns the bytes removed by this function. */
static size_t delchunk(u8 *restrict buf, u32 *restrict table, size_t rmb, int sidx, int eidx) {
	// load the table data
	size_t slen, bidx, blen;
	slen = be32toh(table[sidx]) & 0xFF;          // acquire the sector length of the chunk
	bidx = (be32toh(table[sidx]) >> 8) * SECTOR; // acquire and compute the byte offset the chunk starts at
	blen = slen * SECTOR;                        // compute the byte length of the chunk

	// reset the table data
	table[sidx] = 0;
	table[sidx + CHUNKS] = htobe32(time(NULL)); // assign the current time to the timestamp, for correctness  NOTE: might need to zero-out instead

	// move the succeeding chunks over the deleted chunk
	u8 *dst = buf + bidx - rmb;
	u8 *src = buf + bidx + blen;
	mvchunks(dst, src, table, sidx, eidx - 1);
	return blen;
}

/* Call `delchunk` with the parameters and some defaults. Ensuring the table is copied correctly as well.
 * This is done instead of `delchunk` being globally linked, because
 * `delchunk` requests more specific parameters, which is confusing outside this module. */
size_t mcx_delchunk(u8 *restrict buf, int chunk) {
	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));
	size_t res = delchunk(buf, table, 0, chunk, CHUNKS);
	memcpy(buf, table, sizeof(table));
	return res;
}

size_t mcx_delchunk_range(u8 *restrict buf, int start, int end) {
	assert(start < end && end < CHUNKS);
	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));
	u8 *dst = buf + (be32toh(table[start]) >> 8) * SECTOR;
	u8 *src = buf + (be32toh(table[end]) >> 8) * SECTOR;
	src += (be32toh(table[end]) & 0xFF) * SECTOR;

	// zeroes-out the chunk data within this range. (and set the timestamp)
	u32 ts = htobe32(time(NULL));
	for (int i = start; i <= end; i++) {
		table[i] = 0;
		table[i + CHUNKS] = ts;
	}

	// move the remaining chunks down
	if (end < (CHUNKS - 1))
		mvchunks(dst, src, table, end, (CHUNKS - 1));
	memcpy(buf, table, sizeof(table));
	return src - dst;
}

/* comparer function for to be inputted into `qsort` to compare two */
static int cmp_chunkids(const void *restrict x, const void *restrict y) {
	u16 x2 = *(u16 *)x;
	u16 y2 = *(u16 *)y;
	return (x2 > y2) - (x2 < y2);
}

/* Sorts the chunks marked for deletion from smallest to greatest index.
 * Then performs the deletion in this order. Making sure to only update the chunks up to the next. */
size_t mcx_delchunk_bulk(u8 *restrict buf, const u16 *restrict chunks, int chunkc) {
	// ensure the chunks ids we're working on are sorted from least to greatest
	u16 chunkids[chunkc + 1];
	memcpy(chunkids, chunks, chunkc);
	qsort(chunkids, chunkc, sizeof(int), cmp_chunkids);
	chunkids[chunkc] = CHUNKS; // set the spare chunk to the max chunks, so the rest of the chunks are moved

	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));

	size_t rmb = 0;
	for (int i = 0; i < chunkc; i++)
		rmb += delchunk(buf, table, rmb, chunkids[i], chunkids[i + 1]);

	memcpy(buf, table, sizeof(table));
	return rmb;
}

/* Sum together the 4th byte in each location integer to compute the sector size of all chunks.
 * Multiplying by `SECTOR`, and adding the size of the table itself. */
size_t mcx_calcsize(const u8 *restrict buf) {
	size_t size = 0;
	for (uint i = 0; i < CHUNKS; i++)
		size += *(buf + (i * 4) + 3);
	return (size * SECTOR) + (TABLE * 4);
}
