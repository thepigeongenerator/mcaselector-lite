#include "nbt.h"

#include <endian.h>
#include <stddef.h>

#include "../util/types.h"

/* the data structure that functions passes between functions, so it can communicate with the central function */
struct nbt_procdat {
	u32 ncomp; // the number of compound tags we've encountered
};

/* returns the string length from a specific location in the buffer */
static inline u16 nbt_strlen(u8 const *restrict buf) {
	return be16toh(*(u16 *)(buf));
}

int nbt_proc(void **restrict datout, u8 const *restrict buf, size_t len) {
	struct nbt_procdat procdat = {0};

	// first byte should be a compound tag
	if (*buf != NBT_COMPOUND) return 1;

	// ignore the first tag + its name, so we start with the juicy data
	uint tmp = nbt_strlen(buf + 1) + 3;
	buf += tmp;
	len -= tmp;
	procdat.ncomp++;

	// TODO: finish function
	return 0;
}
