/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include "nbt.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../endian.h"
#include "../types.h"

#define MAX_DEPTH 512

/* Extracts a big endian 16 bit integer from address `buf`, converts it to host byte size if needed and returns. */
static inline u16 buftoh16(const void *restrict buf)
{
	be16 i;
	memcpy(&i, buf, sizeof(i));
	return cvt_be16toh(i);
}

/* Extracts a big endian 32 bit integer from address `buf`, converts it to host byte size if needed and returns. */
static inline u32 buftoh32(const void *restrict buf)
{
	be32 i;
	memcpy(&i, buf, sizeof(i));
	return cvt_be32toh(i);
}

/* Extracts a big endian 64 bit integer from address `buf`, converts it to host byte size if needed and returns. */
static inline u64 buftoh64(const void *restrict buf)
{
	be64 i;
	memcpy(&i, buf, sizeof(i));
	return cvt_be64toh(i);
}

/* Processes the incoming array data in `buf`. Which contains `nmem` items of `size`.
 * The data shall be converted to little-endian on little-endian systems
 * Outputs the allocated data to `out`, returns where the next pointer would be. */
static const u8 *procarr(const u8 *restrict buf, s32 nmemb, uint size, struct nbt_array *restrict out)
{
	usize len = nmemb * size;
	*out      = (struct nbt_array){
                nmemb,
                malloc(len)};
	if (!out->dat)
		return buf + len;

	memcpy(out->dat, buf, len);
	buf += len;

	/* Only include this code for little-endian systems. Since only they require this logic.
	 * Producing optimised code for other platforms. */
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	if (size == 1)
		return buf;
	s32 i = 0;
	while (i < nmemb) {
		switch (size) {
		// BUG: Violation of strict aliasing
		case 2:  ((u16 *)out->dat)[i] = cvt_be16toh(((u16 *)out->dat)[i]); break;
		case 4:  ((u32 *)out->dat)[i] = cvt_be16toh(((u32 *)out->dat)[i]); break;
		case 8:  ((u64 *)out->dat)[i] = cvt_be16toh(((u64 *)out->dat)[i]); break;
		default: __builtin_unreachable(); // this should be impossible
		}
		i += size;
	}
#endif
	return buf;
}

/* calls `procarr` for the simple types available. */
static const u8 *proclist(const u8 *restrict buf, struct nbt_array *restrict out)
{
	uint size;

	switch (*(u8 *)buf) {
	case NBT_I8:  size = 1; break;
	case NBT_I16: size = 2; break;
	case NBT_I32: // fall through
	case NBT_F32: size = 4; break;
	case NBT_I64: // fall though
	case NBT_F64: size = 8; break;
	default:      return NULL;
	}

	buf++;
	be32 tmp;
	memcpy(&tmp, buf, 4);
	s32 len = cvt_be32toh(tmp);
	buf += 4;
	return procarr(buf, len, size, out);
}

const u8 *nbt_proctag(const u8 *restrict buf, u16 slen, void *restrict out)
{
	const u8 *ptr, *tmp;
	ptr = buf + 3 + slen;

	s32  nmem;
	uint size;

	switch (*buf) {
	case NBT_I8:  *(u8 *)out = *ptr; return ptr + 1;
	case NBT_I16: *(u16 *)out = buftoh16(ptr); return ptr + 2;
	case NBT_I32: // fall through
	case NBT_F32: *(u32 *)out = buftoh32(ptr); return ptr + 4;
	case NBT_I64: // fall through
	case NBT_F64: *(u64 *)out = buftoh64(ptr); return ptr + 8;

	case NBT_STR:     nmem = buftoh16(ptr), size = 1, ptr += 2; break;
	case NBT_ARR_I8:  nmem = buftoh32(ptr), size = 1, ptr += 4; break;
	case NBT_ARR_I32: nmem = buftoh32(ptr), size = 4, ptr += 4; break;
	case NBT_ARR_I64: nmem = buftoh32(ptr), size = 8, ptr += 4; break;

	case NBT_LIST:
		return proclist(ptr, (struct nbt_array *)out);
		return tmp;

	default: return NULL;
	}

	return procarr(ptr, nmem, size, (struct nbt_array *)out);
}


/* handles incrementing to the next tag in the case of `NBT_LIST`. This function shan't return `NULL`.
 * `ptr` is assumed to be the start of the `NBT_LIST` data, e.i. The list's ID, followed by the list's length.
 * If `ID` is `NBT_I8`, `NBT_I16`, `NBT_I32`, `NBT_I64`, `NBT_F32`, or `NBT_F64`, the entire list length is computed and returned.
 * For other types this won't be possible, and thus will add `1` to `dpt`, and write the list data to `lens` and `tags` at this new `dpt`. */
static const u8 *nexttag_list(const u8 *restrict ptr, uint *restrict const dpt, s32 *restrict const lens, u8 *restrict const tags)
{
	const u8 *tag = ptr;
	ptr++;
	switch (*tag) {
	case NBT_END: break;
	case NBT_I8:  ptr += (s32)buftoh32(ptr) * 1; break;
	case NBT_I16: ptr += (s32)buftoh32(ptr) * 2; break;
	case NBT_I32: // fall through
	case NBT_F32: ptr += (s32)buftoh32(ptr) * 4; break;
	case NBT_I64: // fall through
	case NBT_F64: ptr += (s32)buftoh32(ptr) * 8; break;
	default:
		// TODO: handle out of bounds... Might not be required if we use flexible array member
		(*dpt)++;
		tags[*dpt] = *tag;
		lens[*dpt] = (s32)buftoh32(ptr);
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
static const u8 *nexttag(const u8 *restrict tag, uint *restrict const dpt, s32 *restrict const lens, u8 *restrict const tags)
{
	u8        type;
	const u8 *ptr = tag;
	if (lens[*dpt]) {
		type = tags[*dpt];
		lens[*dpt]--;
		*dpt -= !lens[*dpt];
	} else {
		type = *tag;
		ptr += buftoh16(tag + 1) + 3;
	}

	switch (type) {
	case NBT_I8:  ptr += 1; break;
	case NBT_I16: ptr += 2; break;
	case NBT_I32: // fall through
	case NBT_F32: ptr += 4; break;
	case NBT_I64: // fall through
	case NBT_F64: ptr += 8; break;

	case NBT_ARR_I8:  ptr += 4 + (s32)buftoh32(ptr) * 1; break;
	case NBT_ARR_I32: ptr += 4 + (s32)buftoh32(ptr) * 4; break;
	case NBT_ARR_I64: ptr += 4 + (s32)buftoh32(ptr) * 8; break;
	case NBT_STR:     ptr += 2 + (u16)buftoh16(ptr) * 1; break;

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
const u8 *nbt_nexttag(const u8 *restrict buf)
{
	const u8 *tag;
	u8        tags[MAX_DEPTH] = {0};
	s32       lens[MAX_DEPTH] = {0};
	uint      dpt             = 0;

	tag = buf;
	do {
		tag = nexttag(tag, &dpt, lens, tags);
	} while (tag && dpt > 0);
	return tag;
}
