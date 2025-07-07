// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

/* NBT (named binary tag) is a tree data structure. Tags have a numeric type ID, name and a payload.
 * NBT files are a compressed `compound` tag. GZip is the compression used in most cases,
 * in some (rare) cases it's stored uncompressed.
 * A tag is an individual part of the data tree.
 * The first byte is the ID, followed by a `u16`, for the length of the name.
 * Then the name as `UTF-8`. (end tag does not contain name) */


/* specifies the NBT tag IDs.
 * NOTE: every type is stored as BE (big-endian) in the file. */
enum nbt_tagid {
	NBT_END,      // signifies the end of a compound tag
	NBT_I8,       // next byte is for an 8 bit signed integer.
	NBT_I16,      // next 2 bytes are for a 16 bit signed integer
	NBT_I32,      // next 4 bytes are for a 32 bit signed integer
	NBT_I64,      // next 8 bytes are for a 64 bit signed integer
	NBT_F32,      // next 4 bytes are for a single-precision floating-point
	NBT_F64,      // next 8 bytes are for a double-precision floating-point
	NBT_ARR_I8,   // starts with a i32, denoting size, followed by the i8 data
	NBT_STR,      // starts with a u16, denoting size, followed by the UTF-8 data
	NBT_LIST,     // starts with an ID, followed by a 32 bit signed integer denoting the size
	NBT_COMPOUND, // compound tag, contains tags and is delimited by `NBT_END`
	NBT_ARR_I32,  // starts with a i32, denoting size, followed by the i32 data
	NBT_ARR_I64,  // starts with a i32, denoting size, followed by the u32 data
};
