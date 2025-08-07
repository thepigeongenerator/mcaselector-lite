#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/types.h"

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

int nbt_primsize(u8 tag) {
	switch (tag) {
	case NBT_I8:  return 1;
	case NBT_I16: return 2;
	case NBT_I32: // fall through
	case NBT_F32: return 4;
	case NBT_I64: // fall through
	case NBT_F64: return 8;
	default:      return -1;
	}
}

const u8 *nbt_nextcompound(const u8 *restrict cdat) {
	return NULL;
}

const u8 *nbt_nextlist(const u8 *restrict ldat) {
	const u8 *ptr = ldat + 5;
	i32 len = be32toh(*(u32 *)(ldat + 1));

	switch (*ldat) {
	case NBT_I8:
	case NBT_I16:
	case NBT_I32:
	case NBT_I64:
	case NBT_F32:
	case NBT_F64:
		return ptr + nbt_primsize(*ldat) * len;

	// loop for each compound, whilst `ptr` isn't `NULL`
	case NBT_COMPOUND: {
		for (u32 i = 0; i < (u32)len && ptr; i++) {
			ptr = nbt_nextcompound(ptr);
		}
		return ptr;
	}

	// TODO: implement list/array/string handling
	case NBT_STR:
	case NBT_ARR_I8:
	case NBT_ARR_I32:
	case NBT_ARR_I64:
	case NBT_LIST:
	default:
		return NULL;
	}
}

const u8 *nbt_nexttag(const u8 *restrict buf, u16 naml) {
	const u8 *ptr = buf + naml + 3;
	size_t mems = 0;

	switch (*buf) {
	case NBT_I8:
	case NBT_I16:
	case NBT_I32:
	case NBT_F32:
	case NBT_I64:
	case NBT_F64:
		return ptr + nbt_primsize(*buf);

	case NBT_ARR_I64: mems += sizeof(i64) - sizeof(i32); __attribute__((fallthrough));
	case NBT_ARR_I32: mems += sizeof(i32) - sizeof(i8); __attribute__((fallthrough));
	case NBT_ARR_I8:  return ptr + ++mems * (i32)be32toh(*(u32 *)(ptr)) + 4;

	case NBT_STR:  return ptr + be16toh(*(u16 *)(ptr)) + 2;
	case NBT_LIST: return nbt_nextlist(ptr);

	default: return NULL;
	}
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
	case NBT_I32: // fall through
	case NBT_F32: *(u32 *)dat = be16toh(*(u32 *)ptr); return ptr + 4;
	case NBT_I64: // fall through
	case NBT_F64: *(u64 *)dat = be16toh(*(u64 *)ptr); return ptr + 8;

	// arrays, handled differently
	case NBT_LIST:
	case NBT_ARR_I8:
	case NBT_STR:
	case NBT_ARR_I32:
	case NBT_ARR_I64:
		// TODO: arrlen = nbt_arrbsize(ptr);
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
