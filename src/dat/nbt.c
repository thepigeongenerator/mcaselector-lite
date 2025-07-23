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
 * `NULL` is returned if anything went wrong. */
static u8 const *nbt_nexttag(u8 const *restrict buf, u16 naml) {
	size_t len = nbt_tagdatlen(buf);
	if (!len) return NULL; // TODO: compound tags should be handled here
	return buf + naml + len + 3;
}

// TODO: not actually doing anything
/* readies the output data for export, returns the new buffer position, or `NULL` upon an error (may be out of bounds) */
static u8 const *nbt_proctag(u8 const *restrict buf, u16 slen) {
	u8 const *ptr = buf + 3 + slen;
	u8 dat[8];
	size_t arrlen = 0;

	switch (*buf) {
	// integral types
	case NBT_I8:  *dat = *ptr; return ptr;
	case NBT_I16: *(u16 *)dat = be16toh(*(u16 *)ptr); return ptr + 2;
	case NBT_I32: __attribute__((fallthrough));
	case NBT_F32: *(u32 *)dat = be16toh(*(u32 *)ptr); return ptr + 4;
	case NBT_I64: __attribute__((fallthrough));
	case NBT_F64: *(u64 *)dat = be16toh(*(u64 *)ptr); return ptr + 8;

	// arrays, handled differently
	case NBT_LIST:    __attribute__((fallthrough));
	case NBT_ARR_I8:  __attribute__((fallthrough));
	case NBT_STR:     __attribute__((fallthrough));
	case NBT_ARR_I32: __attribute__((fallthrough));
	case NBT_ARR_I64:
		// arrlen = nbt_arrbsize(ptr);
		break;

	default: return NULL;
	}
	if (!arrlen) return NULL;
	return ptr + nbt_primsize(*buf);
}

int nbt_proc(void **restrict datout, u8 const *restrict buf, size_t len) {
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

size_t nbt_tagdatlen(u8 const *restrict buf) {
	size_t mems = 0;

	switch (*buf) {
	case NBT_I8:  __attribute__((fallthrough));
	case NBT_I16: __attribute__((fallthrough));
	case NBT_I32: __attribute__((fallthrough));
	case NBT_F32: __attribute__((fallthrough));
	case NBT_I64: __attribute__((fallthrough));
	case NBT_F64:
		mems = nbt_primsize(*buf);
		return -(mems >= 0) & mems;

	case NBT_ARR_I64: mems += sizeof(i64) - sizeof(i32); __attribute__((fallthrough));
	case NBT_ARR_I32: mems += sizeof(i32) - sizeof(i8); __attribute__((fallthrough));
	case NBT_ARR_I8:  return +mems * nbt_arrlen(buf) + 4;

	case NBT_STR: return nbt_strlen(buf) + 2;

	case NBT_LIST:
		mems = nbt_primsize(*buf);
		if (mems > 0) return mems * nbt_arrlen(buf + 1) + 5;
		return 0;
	default: return 0;
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
