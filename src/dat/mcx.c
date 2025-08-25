#include "mcx.h"

#include <endian.h>
#include <stdint.h>
#include <stdlib.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

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
