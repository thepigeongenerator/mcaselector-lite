// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

// WARN: does not have public-facing definition
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

const u8 *nbt_nexttag(const u8 *restrict buf) {
	const u8 *tag, *ptr, *tmp;
	tag = buf;
	uint dpt = 0;
	size_t mems = 0;

	// looping through the named tags
	do {
		ptr = tag + be16toh(*(u16 *)(tag + 1)) + 3; // set `ptr` to start of data
		mems = 0;

		switch (*tag) {
		case NBT_I8:
		case NBT_I16:
		case NBT_I32:
		case NBT_F32:
		case NBT_I64:
		case NBT_F64:
			ptr += nbt_primsize(*buf);
			break;

		case NBT_ARR_I64: mems += sizeof(i64) - sizeof(i32); __attribute__((fallthrough));
		case NBT_ARR_I32: mems += sizeof(i32) - sizeof(i8); __attribute__((fallthrough));
		case NBT_ARR_I8:  ptr += (mems + 1) * (i32)be32toh(*(u32 *)(ptr)) + 4; break;
		case NBT_STR:     ptr += be16toh(*(u16 *)(ptr)) + 2; break;

		case NBT_END:      dpt--; break;
		case NBT_COMPOUND: dpt++; break;

		// TODO: handle (compound) lists somehow

		default: return NULL; // unexpected value; buffer is likely corrupt
		}

		tag = ptr;
	} while (dpt > 0);
	return tag;
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
	const char **cpat = (const char **)calloc(mdpt - 1, sizeof(void *));

	// return the initialised structure.
	return (struct nbt_procdat){
		pats,
		cpat,
		npats,
		0,
		mdpt,
	};
}
