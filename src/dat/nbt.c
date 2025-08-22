// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

#define MAX_DEPTH 512

/* handles incrementing to the next tag in the case of `NBT_LIST`. This function shan't return `NULL`.
 * `ptr` is assumed to be the start of the `NBT_LIST` data, e.i. The list's ID, followed by the list's length.
 * If `ID` is `NBT_I8`, `NBT_I16`, `NBT_I32`, `NBT_I64`, `NBT_F32`, or `NBT_F64`, the entire list length is computed and returned.
 * For other types this won't be possible, and thus will add `1` to `dpt`, and write the list data to `lens` and `tags` at this new `dpt`. */
static const u8 *nexttag_list(const u8 *restrict ptr, uint *restrict const dpt, i32 *restrict const lens, u8 *restrict const tags) {
	const u8 *tag = ptr;
	ptr++;
	switch (*tag) {
	case NBT_END: break;
	case NBT_I8:  ptr += (i32)be32toh(*(u32 *)ptr) * 1; break;
	case NBT_I16: ptr += (i32)be32toh(*(u32 *)ptr) * 2; break;
	case NBT_I32: // fall through
	case NBT_F32: ptr += (i32)be32toh(*(u32 *)ptr) * 4; break;
	case NBT_I64: // fall through
	case NBT_F64: ptr += (i32)be32toh(*(u32 *)ptr) * 8; break;
	default:
		// TODO: handle out of bounds... Might not be required if we use flexible array member
		(*dpt)++;
		tags[*dpt] = *tag;
		lens[*dpt] = (i32)be32toh(*(u32 *)ptr);
		break;
	}
	ptr += 4;
	return ptr;
}

/* increments to the next tag and returns it (or `NULL`)
 * - `tag` represents the start of the tag, e.i. The tag ID, or in the case of `NBT_LIST` data, the start of this data.
 * - `dpt` shall point to the "depth" we're at, this is used as index for `lens` and `tags`
 * - `lens` shall contain `MAX_DEPTH` of items representing the list length, if the current item is non-zero we shall assume we're in a list.
 *     Where the value is decremented until we reach `0`.
 * - `tags` shall contain `MAX_DEPTH` of items representing the list's stored type. */
static const u8 *nexttag(const u8 *restrict tag, uint *restrict const dpt, i32 *restrict const lens, u8 *restrict const tags) {
	u8 type;
	const u8 *ptr = tag;
	if (lens[*dpt]) {
		type = tags[*dpt];
		lens[*dpt]--;
		*dpt -= !lens[*dpt];
	} else {
		type = *tag;
		ptr += be16toh(*(u16 *)(tag + 1)) + 3;
	}

	switch (type) {
	case NBT_I8:  ptr += 1; break;
	case NBT_I16: ptr += 2; break;
	case NBT_I32: // fall through
	case NBT_F32: ptr += 4; break;
	case NBT_I64: // fall through
	case NBT_F64: ptr += 8; break;

	case NBT_ARR_I8:  ptr += 4 + (i32)be32toh(*(u32 *)ptr) * 1; break;
	case NBT_ARR_I32: ptr += 4 + (i32)be32toh(*(u32 *)ptr) * 4; break;
	case NBT_ARR_I64: ptr += 4 + (i32)be32toh(*(u32 *)ptr) * 8; break;
	case NBT_STR:     ptr += 2 + (u16)be16toh(*(u16 *)ptr) * 1; break;

	case NBT_END:      (*dpt)--; break;
	case NBT_COMPOUND: (*dpt)++; break;

	case NBT_LIST: ptr = nexttag_list(ptr, dpt, lens, tags); break;

	default: return NULL; // unexpected value; buffer is likely corrupt
	}

	return ptr;
}

/* TODO: write test cases for this function:
 * - list:compound...
 * - non-existent type
 * - compound:list:int32
 * - string
 */
const u8 *nbt_nexttag(const u8 *restrict buf) {
	const u8 *tag;
	u8 tags[MAX_DEPTH] = {0};
	i32 lens[MAX_DEPTH] = {0};
	uint dpt = 0;

	tag = buf;
	do {
		tag = nexttag(tag, &dpt, lens, tags);
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
	case NBT_I8:  *dat = *ptr; return ptr + 1;
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
	return ptr; // TODO: return end of array
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
