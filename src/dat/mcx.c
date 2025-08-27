#include "mcx.h"

#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

/* Deletes chunk `sidx`, by moving chunks up to `eidx` back over `sidx` in `buf`.
 * `rmb` is an optional additional offset that can be applied, and signifies bytes already removed.
 * Returns the bytes removed by this function. */
static size_t delchunk(u8 *restrict buf, size_t rmb, int sidx, int eidx) {
	// load the table data, and clear it
	u32 *table = (u32 *)buf;
	size_t slen, bidx, blen;
	slen = be32toh(table[sidx] & 0xFF);        // acquire the sector length of the chunk
	bidx = be32toh(table[sidx] >> 8) * 0x1000; // acquire and compute the byte offset the chunk starts at
	blen = slen * 0x1000;                      // compute the byte length of the chunk
	table[sidx] = 0;
	table[sidx + 0x400] = time(NULL); // assign the current time to the timestamp, for correctness  NOTE: might need to zero-out instead

	u8 *dst = buf + bidx - rmb;
	u8 *src = buf + bidx + blen;
	rmb = blen;
	blen = 0;
	for (sidx++; sidx < eidx; sidx++) {
		blen += be32toh(table[sidx] & 0xFF) * 0x1000;
		table[sidx] -= htobe32(slen << 8);
	}
	memmove(dst, src, blen);
	return rmb;
}

size_t mcx_delchunk(u8 *restrict buf, int chunk) {
	return delchunk(buf, 0, chunk, 0x400);
}

/* Sum together the 4th byte in each location integer to compute the sector size of all chunks.
 * Multiplying by `0x1000`, and adding the size of the table itself. */
size_t mcx_calcsize(const u8 *restrict buf) {
	size_t size = 0;
	for (uint i = 0; i < 0x400; i++)
		size += *(buf + (i * 4) + 3);
	return (size * 0x1000) + 0x2000;
}

/* an `*.mcX` contains a `0x2000` byte long table, the first `0x1000` containing
 * `0x400` entries of chunk data.
 * This chunk data is big-endian, where bytes `0xFFFFFF00` represent the `0x1000` sector offset.
 * From the start, and bytes `0x000000FF` represent the length in `0x1000` sectors. */
void mcx_index(const u8 *restrict buf, struct mcx_chunk *restrict chunks) {
	const u32 *ptr = (u32 *)buf;
	for (uint i = 0; i < 0x400; i++) {
		u32 dat = be32toh(ptr[i]);
		chunks[i] = (struct mcx_chunk){
			.idx = (dat >> 8) * 0x1000,
			.len = (dat & 0xFF) * 0x1000,
			.time = be32toh(ptr[i + 0x400]),
		};
	}
}
