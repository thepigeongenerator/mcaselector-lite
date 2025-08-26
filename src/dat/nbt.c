// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../util/compat/endian.h"
#include "../util/intdef.h"

#define MAX_DEPTH 512

/* Processes the incoming array data in `buf`. Which contains `nmem` items of `size`.
 * The data shall be converted to little-endian on little-endian systems
 * Outputs the allocated data to `out`, returns where the next pointer would be. */
static const u8 *procarr(const u8 *restrict buf, i32 nmem, uint size, struct nbt_array *restrict *restrict out) {
	size_t len = nmem * size;
	*out = malloc(sizeof(struct nbt_array) + len);
	if (!*out) return buf + len;

	memcpy((*out)->dat, buf, len);
	(*out)->len = nmem;
	buf += len;

	/* Only include this code for little-endian systems. Since only they require this logic.
	 * Producing optimised code for other platforms. */
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	if (size == 1) return buf;
	size_t i = 0;
	while (i < len) {
		switch (size) {
		case 2:  *(u16 *)((*out)->dat + i) = be16toh(*(u16 *)((*out)->dat + i)); break;
		case 4:  *(u32 *)((*out)->dat + i) = be32toh(*(u32 *)((*out)->dat + i)); break;
		case 8:  *(u64 *)((*out)->dat + i) = be64toh(*(u64 *)((*out)->dat + i)); break;
		default: __builtin_unreachable(); // this should be impossible
		}
		i += size;
	}
#endif
	return buf;
}

/* calls `procarr` for the simple types available. */
static const u8 *proclist(const u8 *restrict buf, struct nbt_array *restrict *restrict out) {
	uint size;

	*out = NULL;

	switch (*buf) {
	case NBT_I8:  size = 1; break;
	case NBT_I16: size = 2; break;
	case NBT_I32: // fall through
	case NBT_F32: size = 4; break;
	case NBT_I64: // fall though
	case NBT_F64: size = 8; break;
	default:      return NULL;
	}

	buf++;
	i32 len = (i32)be32toh(*(u32 *)buf);
	buf += 4;
	return procarr(buf, len, size, out);
}

const u8 *nbt_proctag(const u8 *restrict buf, u16 slen, void *restrict out) {
	const u8 *ptr, *tmp;
	ptr = buf + 3 + slen;

	i32 nmem;
	uint size;

	switch (*buf) {
	case NBT_I8:  *(u8 *)out = *ptr; return ptr + 1;
	case NBT_I16: *(u16 *)out = be16toh(*(u16 *)ptr); return ptr + 2;
	case NBT_I32: // fall through
	case NBT_F32: *(u32 *)out = be16toh(*(u32 *)ptr); return ptr + 4;
	case NBT_I64: // fall through
	case NBT_F64: *(u64 *)out = be16toh(*(u64 *)ptr); return ptr + 8;

	case NBT_STR:     nmem = be16toh(*(u16 *)ptr), size = 1, ptr += 2; break;
	case NBT_ARR_I8:  nmem = be32toh(*(u32 *)ptr), size = 1, ptr += 4; break;
	case NBT_ARR_I32: nmem = be32toh(*(u32 *)ptr), size = 4, ptr += 4; break;
	case NBT_ARR_I64: nmem = be32toh(*(u32 *)ptr), size = 8, ptr += 4; break;

	case NBT_LIST:
		return proclist(ptr, (struct nbt_array **)out);
		return tmp;

	default: return NULL;
	}

	return procarr(ptr, nmem, size, (struct nbt_array **)out);
}


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
