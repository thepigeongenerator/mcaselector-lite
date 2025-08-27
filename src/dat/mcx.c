#include "mcx.h"

#include <endian.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

void mcx_delchunk(u8 *restrict buf, int chunk) {
	// load the table data, and clear it
	u32 *table = (u32 *)buf;
	size_t bidx = (table[chunk] >> 8) * 0x1000;   // compute the byte offset the chunk starts at
	size_t blen = (table[chunk] & 0xFF) * 0x1000; // compute the byte length of the chunk
	table[chunk] = 0;
	table[chunk + 0x400] = time(NULL); // assign the current time to the timestamp, for correctness  NOTE: might need to zero-out instead

	// store the head and tail end of the current chunk
	u8 *head = buf + bidx;
	u8 *tail = buf + bidx + blen;

	// count the amount of bytes that we must move
	blen = 0;
	for (chunk++; chunk < 0x400; chunk++)
		blen += table[chunk] & 0xFF * 0x1000;
	memmove(head, tail, blen);
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
