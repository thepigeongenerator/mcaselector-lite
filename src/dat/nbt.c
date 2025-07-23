#include "nbt.h"

#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/types.h"

/* returns the string length from a specific location in the buffer */
static inline u16 nbt_strlen(u8 const *restrict buf) {
	return be16toh(*(u16 *)(buf));
}

/* returns the length of an array from a specific location in the buffer */
static inline i32 nbt_arrlen(u8 const *restrict buf) {
	return be32toh(*(i32 *)(buf));
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

/* returns the (expected) pointer of the tag following this one.
 * `NBT_COMPOUND` and `NBT_END` tags are not valid for this function and should be handled separately.
 * `NULL` is returned if anything went wrong. */
static u8 const *nbt_nexttag(u8 const *restrict buf) {
	u8 const *nxt = buf + 1;
	nxt += nbt_strlen(nxt) + 2;

	uint mems = 0;
	switch (*buf) {
	case NBT_I8:
	case NBT_I16:
	case NBT_I32:
	case NBT_I64:
	case NBT_F32:
	case NBT_F64:
		nxt += nbt_prim_tagsize(*buf);
		return nxt;

	case NBT_ARR_I64:
		mems += sizeof(i64) - sizeof(i32);
	case NBT_ARR_I32:
		mems += sizeof(i32) - sizeof(i8);
	case NBT_ARR_I8:
		mems += 1;
		nxt += mems * nbt_arrlen(nxt);
		return nxt;
	case NBT_STR:
		nxt += nbt_strlen(nxt);
		return nxt;

	case NBT_LIST:
		mems = nbt_prim_tagsize(*nxt);
		if (mems > 0) {
			nxt += 1;
			nxt += mems * nbt_arrlen(nxt);
			return nxt;
		}
		// let case escape to `default` when `mems` `≤0`

	default: return NULL; // failure on compound/end tags; these require more nuanced logic
	}
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

int nbt_prim_tagsize(u8 tag) {
	switch (tag) {
	case NBT_I8:  return 1;
	case NBT_I16: return 2;
	case NBT_I32:
	case NBT_F32: return 4;
	case NBT_I64:
	case NBT_F64: return 8;
	default:      return -1;
	}
}

int nbt_isprim(u8 tag) {
	switch (tag) {
	case NBT_I8:
	case NBT_I16:
	case NBT_I32:
	case NBT_F32:
	case NBT_I64:
	case NBT_F64:
		return 1;
	default:
		return 0;
	}
}
