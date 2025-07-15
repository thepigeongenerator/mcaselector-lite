#include "nbt.h"

#include <endian.h>
#include <stddef.h>
#include <string.h>

#include "../util/types.h"

/* returns the string length from a specific location in the buffer */
static inline u16 nbt_strlen(u8 const *restrict buf) {
	return be16toh(*(u16 *)(buf));
}

/* compares the string in `buf` to `matstr`.
 * returns `=0` if equal, `>0` if buf is greater, `<0` if matstr is greater. */
static int nbt_cmpstr(char const *restrict matstr, u8 const *restrict buf) {
	u16 len = nbt_strlen(buf);

	// allocate and copy bytes
	char str[len + 1];
	memcpy(str, buf + 2, len);
	str[len] = '\0';

	return strncmp(str, matstr, len);
}

int nbt_proc(void **restrict datout, u8 const *restrict buf, size_t len) {

	// first byte should be a compound tag
	if (*buf != NBT_COMPOUND) return 1;
	uint ncomp = 1;

	// ignore the first tag + its name, so we start with the juicy data
	uint tmp = nbt_strlen(buf + 1) + 3;
	buf += tmp;
	len -= tmp;

	// TODO: finish function
	return 0;
}
