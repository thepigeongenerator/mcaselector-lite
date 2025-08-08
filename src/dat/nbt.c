// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

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

// BUG: recursive, been figuring out how to not have it recursive
const u8 *nbt_nextcompound(const u8 *restrict cdat) {
	const u8 *ptr = cdat;
	do {
		u16 slen = be16toh(*(u16 *)(ptr + 1));
		uint mems = 0;

		switch (*ptr) {
		case NBT_I8:
		case NBT_I16:
		case NBT_I32:
		case NBT_F32:
		case NBT_I64:
		case NBT_F64:
			ptr += 3 + slen + nbt_primsize(*ptr);
			continue; // continue the while loop; no more to be done

		case NBT_ARR_I64: mems += sizeof(i64) - sizeof(i32); __attribute__((fallthrough));
		case NBT_ARR_I32: mems += sizeof(i32) - sizeof(i8); __attribute__((fallthrough));
		case NBT_ARR_I8:  ptr += 3 + slen + ++mems * (i32)be32toh(*(u32 *)(ptr)) + 4; continue;
		case NBT_STR:     ptr += 3 + slen + be16toh(*(u16 *)(ptr)) + 2; continue;

		case NBT_LIST: ptr = nbt_nextlist(3 + slen + ptr); continue;
		}

	} while (ptr && *ptr != NBT_END);
	return ptr + 1;
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

struct nbt_procdat nbt_initproc(struct nbt_path const *restrict pats, uint npats) {
	i16 mdpt = 0;

	// acquire the maximum depth that we'll need to go (exclusive)
	for (uint i = 0; i < npats; i++) {
		int tmp = pats[i].len - mdpt;
		mdpt += -(tmp > 0) & tmp;
	}
	assert(mdpt > 0);

	// storing the segments of the current path
	const char **cpat = (const char **)calloc(sizeof(void *), mdpt - 1);

	// return the initialised structure.
	return (struct nbt_procdat){
		pats,
		cpat,
		npats,
		0,
		mdpt,
	};
}
