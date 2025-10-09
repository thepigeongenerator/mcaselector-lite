/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#include "mcx.h"

#include <archive.h>
#include <assert.h>
#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../types.h"
#include "../util/error.h"

#define SECTOR 0x1000 // sector size
#define TABLE  0x800  // table (total) element count
#define CHUNKS 0x400  // amount of chunks in a file

enum mcx_compression {
	MCX_COMPRESSION_GZIP   = 0x01,
	MCX_COMPRESSION_ZLIB   = 0x02,
	MCX_COMPRESSION_NONE   = 0x03,
	MCX_COMPRESSION_LZ4    = 0x04,
	MCX_COMPRESSION_CUSTOM = 0x7F,
};

/* first 4 bytes is an i32 indicating remaining bytes, the following byte defines the compression scheme */
static int mcx_loadchunk(const u8 *restrict buf, const i32 *restrict table, int idx) {
	const u8 *chunk = buf + (be32toh(table[idx]) >> 8) * SECTOR;

	i32 len;
	memcpy(&len, chunk, 4);
	len = be32toh(len);
	chunk += 4;

	struct archive *archive = archive_read_new();
	archive_read_support_format_raw(archive);
	switch (*chunk) {
	case MCX_COMPRESSION_GZIP: /* fall-through; ZLIB is handled under the GZIP filter */
	case MCX_COMPRESSION_ZLIB:   archive_read_support_filter_gzip(archive); break;
	case MCX_COMPRESSION_NONE:   archive_read_support_filter_none(archive); break;
	case MCX_COMPRESSION_LZ4:    archive_read_support_filter_lz4(archive); break;
	case MCX_COMPRESSION_CUSTOM: archive_read_support_filter_all(archive); break;
	default:                     fatal("compression type of '%i' is unsupported!", *chunk);
	}

	if (archive_read_open_memory(archive, chunk, len) != ARCHIVE_OK) {
		error("failed to decompress %i bytes of compression type %i", len, *chunk);
		return 1;
	}

	struct archive_entry *entry;
	if (archive_read_next_header(archive, &entry) != ARCHIVE_OK) {
		error("failed to decompress %i bytes of compression type %i", len, *chunk);
		return 1;
	}

	ssize size = -1;
	for (;;) {
		// TODO: handle buffer
		// size = archive_read_data(archive, , );
		if (size < 0) {
			error("failed to decompress %i bytes of compression type %i", len, *chunk);
			return 1;
		}
		if (size == 0) break;
		// TODO: handle data
	}

	return 0;
}

/* Moves chunks `src_s` to `src_e` (inclusive) from `src`, back onto `dst`. */
static void mvchunks(u8 *dst, u8 *src, u32 *restrict table, int src_s, int src_e) {
	assert(src > dst);
	usize len = src - dst; // acquire the amount of bytes that we shall move
	assert(!(len % SECTOR));

	// count how many bytes we need to move, whilst updating location data
	usize blen = 0;
	for (src_s++; src_s <= src_e; src_s++) {
		blen += (be32toh(table[src_s]) & 0xFF) * SECTOR;
		table[src_s] -= htobe32((len / SECTOR) << 8);
	}
	memmove(dst, src, blen);
}

/* Deletes chunk `sidx` by moving chunks up to `eidx` back over `sidx` in `buf`.
 * `rmb` is an optional additional offset that can be applied, and signifies bytes already removed.
 * Returns the bytes removed by this function. */
static usize delchunk(u8 *restrict buf, u32 *restrict table, usize rmb, int sidx, int eidx) {
	// load the table data
	usize slen, bidx, blen;
	slen = be32toh(table[sidx]) & 0xFF;          // acquire the sector length of the chunk
	bidx = (be32toh(table[sidx]) >> 8) * SECTOR; // acquire and compute the byte offset the chunk starts at
	blen = slen * SECTOR;                        // compute the byte length of the chunk

	// reset the table data
	table[sidx]          = 0;
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
usize mcx_delchunk(u8 *restrict buf, int chunk) {
	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));
	usize res = delchunk(buf, table, 0, chunk, CHUNKS);
	memcpy(buf, table, sizeof(table));
	return res;
}

usize mcx_delchunk_range(u8 *restrict buf, int start, int end) {
	assert(start < end && end < CHUNKS);
	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));
	u8 *dst = buf + (be32toh(table[start]) >> 8) * SECTOR;
	u8 *src = buf + (be32toh(table[end]) >> 8) * SECTOR;
	src += (be32toh(table[end]) & 0xFF) * SECTOR;

	// zeroes-out the chunk data within this range. (and set the timestamp)
	u32 ts = htobe32(time(NULL));
	for (int i = start; i <= end; i++) {
		table[i]          = 0;
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
usize mcx_delchunk_bulk(u8 *restrict buf, const u16 *restrict chunks, int chunkc) {
	// ensure the chunks ids we're working on are sorted from least to greatest
	u16 chunkids[chunkc + 1];
	memcpy(chunkids, chunks, chunkc);
	qsort(chunkids, chunkc, sizeof(int), cmp_chunkids);
	chunkids[chunkc] = CHUNKS; // set the spare chunk to the max chunks, so the rest of the chunks are moved

	u32 table[TABLE];
	memcpy(table, buf, sizeof(table));

	usize rmb = 0;
	for (int i = 0; i < chunkc; i++)
		rmb += delchunk(buf, table, rmb, chunkids[i], chunkids[i + 1]);

	memcpy(buf, table, sizeof(table));
	return rmb;
}

/* Sum together the 4th byte in each location integer to compute the sector size of all chunks.
 * Multiplying by `SECTOR`, and adding the size of the table itself. */
usize mcx_calcsize(const u8 *restrict buf) {
	usize size = 0;
	for (uint i = 0; i < CHUNKS; i++)
		size += *(buf + (i * 4) + 3);
	return (size * SECTOR) + (TABLE * 4);
}
