/* Copyright (c) 2025 Quinn
 * Licensed under the MIT Licence. See LICENSE for details */
#pragma once

#include <assert.h>
#include <endian.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../types.h"
#include "../util/atrb.h"

/* NBT (named binary tag) is a tree data structure. Tags have a numeric type ID, name and a payload.
 * NBT files are a compressed `compound` tag. GZip is the compression used in most cases,
 * in some (rare) cases it's stored uncompressed.
 * A tag is an individual part of the data tree.
 * The first byte is the ID, followed by a `u16`, for the length of the name.
 * Then the name as `UTF-8`. (end tag does not contain name) */


/* specifies the NBT tag IDs.
 * NOTE: every type is stored as BE (big-endian) in the file. */
enum nbt_tagid {
	NBT_END = 0x00,      // signifies the end of a compound tag
	NBT_I8 = 0x01,       // next byte is for an 8 bit signed integer.
	NBT_I16 = 0x02,      // next 2 bytes are for a 16 bit signed integer
	NBT_I32 = 0x03,      // next 4 bytes are for a 32 bit signed integer
	NBT_I64 = 0x04,      // next 8 bytes are for a 64 bit signed integer
	NBT_F32 = 0x05,      // next 4 bytes are for a single-precision floating-point
	NBT_F64 = 0x06,      // next 8 bytes are for a double-precision floating-point
	NBT_ARR_I8 = 0x07,   // starts with a i32, denoting size, followed by the i8 data
	NBT_STR = 0x08,      // starts with a u16, denoting size, followed by the UTF-8 data
	NBT_LIST = 0x09,     // starts with an ID, followed by a 32 bit signed integer denoting the size
	NBT_COMPOUND = 0x0A, // compound tag, contains tags and is delimited by `NBT_END`
	NBT_ARR_I32 = 0x0B,  // starts with a i32, denoting size, followed by the i32 data
	NBT_ARR_I64 = 0x0C,  // starts with a i32, denoting size, followed by the u32 data
};

struct nbt_array {
	i32 nmemb;
	void *dat;
};


/* Acquires the data contained by the named tag.
 * - `buf` points to the start of the tag.
 * - `slen` contains the string length of the name.
 * - `out` points to where the data should be written.
 * if `buf` points to `NBT_I8`, `NBT_I16`, `NBT_I32`, `NBT_I64`, `NBT_F32`, or `NBT_F64`, `*out` is assumed
 * to have the available byte width for one of these types. In the case of `NBT_ARR*` and `NBT_LIST`
 * it must point to a `struct nbt_array*`. Where in the case of `NBT_LIST`, it must be one of the previous static-width types. */
const u8 *nbt_proctag(const u8 *restrict buf, u16 slen, void *restrict out) NONNULL((1, 3));

/* searches for the end of a named tag without processing data, the final pointer is returned.
 * `NULL` is returned upon failure, the otherwise returned pointer is not guaranteed to be valid. */
const u8 *nbt_nexttag(const u8 *restrict buf) NONNULL((1)) PURE;
