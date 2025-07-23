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

/* acquires the array's byte size (excluding name + size spec)
 * returns `0` upon failure */
static size_t nbt_arrbsize(u8 const *restrict buf) {
	i32 mems = 0;

	switch (*buf) {
	case NBT_ARR_I64: mems += sizeof(i64) - sizeof(i32); __attribute__((fallthrough));
	case NBT_ARR_I32: mems += sizeof(i32) - sizeof(i8); __attribute__((fallthrough));
	case NBT_ARR_I8:  return +mems * nbt_arrlen(buf); // mems+1 multiplied by the array length

	case NBT_STR: return nbt_strlen(buf);

	case NBT_LIST:
		mems = nbt_primsize(*buf);
		if (mems > 0) return mems * nbt_arrlen(buf + 1);
		return 0;
	default: return 0;
	}
}

/* returns the (expected) pointer of the tag following this one.
 * `NBT_COMPOUND` and `NBT_END` tags are not valid for this function and should be handled separately.
 * `NULL` is returned if anything went wrong. */
static u8 const *nbt_nexttag(u8 const *restrict buf) {
	u8 const *nxt = buf + 1;
	nxt += nbt_strlen(nxt) + 2;

	i32 tmp;
	switch (*buf) {
	case NBT_I8:  __attribute__((fallthrough));
	case NBT_I16: __attribute__((fallthrough));
	case NBT_I32: __attribute__((fallthrough));
	case NBT_I64: __attribute__((fallthrough));
	case NBT_F32: __attribute__((fallthrough));
	case NBT_F64:
		tmp = nbt_primsize(*buf);
		return (tmp >= 0) ? (nxt + tmp) : 0;

	case NBT_LIST:    nxt += 5; break;
	case NBT_STR:     nxt += 2; break;
	case NBT_ARR_I8:  __attribute__((fallthrough));
	case NBT_ARR_I32: __attribute__((fallthrough));
	case NBT_ARR_I64: nxt += 4; break;

	default: return NULL; // failure on compound/end tags; these require more nuanced logic
	}

	tmp = nbt_arrbsize(buf);
	return (tmp >= 0) ? (nxt + tmp) : NULL;
}


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

int nbt_primsize(u8 tag) {
	switch (tag) {
	case NBT_I8:  return 1;
	case NBT_I16: return 2;
	case NBT_I32: __attribute__((fallthrough));
	case NBT_F32: return 4;
	case NBT_I64: __attribute__((fallthrough));
	case NBT_F64: return 8;
	default:      return -1;
	}
}

int nbt_isprim(u8 tag) {
	switch (tag) {
	case NBT_I8:  __attribute__((fallthrough));
	case NBT_I16: __attribute__((fallthrough));
	case NBT_I32: __attribute__((fallthrough));
	case NBT_F32: __attribute__((fallthrough));
	case NBT_I64: __attribute__((fallthrough));
	case NBT_F64:
		return 1;
	default:
		return 0;
	}
}
