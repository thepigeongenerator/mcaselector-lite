#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/types.h"

const u8 *nbt_nexttag(const u8 *restrict buf, u16 naml) {
	size_t len = nbt_tagdatlen(buf);
	if (!len) return NULL; // TODO: compound tags should be handled here
	return buf + naml + len + 3;
}

/* compares the string in `buf` to `matstr`.
 * returns `=0` if equal, `>0` if buf is greater, `<0` if matstr is greater. */
static int nbt_cmpstr(const char *restrict matstr, const u8 *restrict buf) {
	u16 len = be16toh(*(u16 *)buf);

	// allocate and copy bytes
	char str[len + 1];
	memcpy(str, buf + 2, len);
	str[len] = '\0';

	return strncmp(str, matstr, len);
}

// TODO: not actually doing anything
/* readies the output data for export, returns the new buffer position, or `NULL` upon an error (may be out of bounds) */
static const u8 *nbt_proctag(const u8 *restrict buf, u16 slen) {
	const u8 *ptr = buf + 3 + slen;
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

/* finds which of `pats` is equivalent to `cmp`, assumes `cmp` is `≥len` bytes long */
static const char *getpat(struct nbt_path const *restrict pats, uint npats, i16 dpt, const char *restrict cmp, u16 len) {
	for (uint i = 0; i < npats; i++) {
		if (strncmp(pats[i].pat[dpt], cmp, len) == 0)
			return pats[i].pat[dpt];
	}
	return NULL;
}

// TODO: make the user do the looping
int nbt_proc(struct nbt_path const *restrict pats, uint npats, const u8 *restrict buf, size_t len) {
	// ensure first and last tag(s) are valid
	if (buf[0] != NBT_COMPOUND || buf[len - 1] != NBT_END)
		return 1;

	i16 dpt = 0;
	i16 mdpt = 0;

	// acquire the maximum depth that we'll need to go (exclusive)
	for (uint i = 0; i < npats; i++) {
		int tmp = pats[i].len - mdpt;
		mdpt += -(tmp > 0) & tmp;
	}
	assert(mdpt > 0);

	// storing the segments of the current path
	const char *cpat[mdpt - 1];
	memset((void *)cpat, 0, mdpt - 1);

	// looping through the different tags
	const u8 *ptr = buf + nbt_namelen(buf) + 3;
	while (ptr < (buf + len) && dpt >= 0) {
		u16 naml = nbt_namelen(ptr);
		const char *mat = getpat(pats, npats, dpt, (char *)(ptr + 3), naml);
		cpat[dpt] = mat;

		if (mat) {
			switch (*ptr) {
			case NBT_END:      dpt--; break;
			case NBT_COMPOUND: dpt++; break;
			default:           ptr = nbt_proctag(ptr, naml); break;
			}
		} else {
			ptr = nbt_nexttag(ptr, naml);
			if (!ptr) return 1;
		}
	}

	// TODO: finish function
	return !dpt;
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

size_t nbt_tagdatlen(const u8 *restrict buf) {
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
	case NBT_ARR_I8:  return +mems * (i32)be32toh(*(u32 *)(buf)) + 4;

	case NBT_STR: return be16toh(*(u16 *)buf) + 2;

	case NBT_LIST:
		mems = nbt_primsize(*buf);
		if (mems > 0) return mems * (i32)be32toh(*(u32 *)(buf + 1)) + 5;
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
