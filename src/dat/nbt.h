// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../util/atrb.h"
#include "../util/compat/endian.h"
#include "../util/intdef.h"

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

/* TODO: write doc */
struct nbt_path {
	const char **restrict pat; // specifies the NBT path components as separate elements
	i16 len;                   // specifies the length of the NBT elements
};

/* TODO: write doc */
struct nbt_procdat {
	const struct nbt_path *pats;
	const char *restrict *cpat;
	u32 npats;
	i16 dpt, mdpt;
};

struct nbt_array {
	i32 len;
	u8 dat[];
};

/* searches for the end of a named tag without processing data, the final pointer is returned.
 * `NULL` is returned upon failure, the otherwise returned pointer is not guaranteed to be valid. */
const u8 *nbt_nexttag(const u8 *restrict buf) NONNULL((1)) PURE;

/* Processes the tag entered in `buf`, `buf` is assumed to be the start of a named tag. Where `slen` shall be the string length.
 * The data in `buf` is processed and outputted to `out`. A pointer to the next tag, or `NULL` is returned.
 * On little-endian systems, the data is processed from big-endian to little-endian. So it can be used like normal.
 * - In the case for all basic types, `out` will require to be the width of said type.
 * - In the case of arrays (and lists), a malloc'd pointer shall be written to `out`, pointing to `struct nbt_array`. This might be `NULL`
 * - In the case of lists, the above is valid, as long as the list contains any of the following types:
 *   `NBT_I8`, `NBT_I16`, `NBT_I32`, `NBT_I64`, `NBT_F32` or `NBT_F64`. Anything else will result in a `NULL` pointer.
 * Upon failure, like the tag not being able to be processed, `NULL` is returned. */
const u8 *nbt_proctag(const u8 *restrict buf, u16 slen, void *restrict out) NONNULL((1, 3));

/* initialises a data structure used whilst processing the tags */
struct nbt_procdat nbt_initproc(struct nbt_path const *restrict pats, uint npats) NONNULL((1)) PURE;
