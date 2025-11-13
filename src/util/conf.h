/* Copyright (c) 2025 Quinn.
 * This is a file from the project MCA-Selector-Lite and is
 * licensed under the MIT Licence. See included LICENSE file for details. */
#pragma once

#include <stddef.h>
#include <stdlib.h>

#include "../types.h"
#include "atrb.h"

/* error codes */
enum conf_err {
	CONF_ENODAT       = -1, // no data was found
	CONF_ESYNTAX      = 1,  // couldn't extract clear key/val data
	CONF_ENOMATCH     = 2,  // couldn't find a match for the inputted key
	CONF_EINVALIDTYPE = 3,  // the type inputted in conf_entry was invalid
	CONF_EPARSE       = 4,  // something went wrong whilst parsing
};

/* defines the primitive types available in the config file */
enum conf_primitive {
	CONF_STR  = 0,               // expects: `char**`, will output malloc'd data !!must be freed!!
	CONF_I8   = 1,               // expects: `int8_t*`, will point to a location in memory where an i8 is stored.
	CONF_I16  = 2,               // expects: `int16_t*`, will point to a location in memory where an i16 is stored.
	CONF_I32  = 4,               // expects: `int32_t*`, will point to a location in memory where an i32 is stored.
	CONF_I64  = 8,               // expects: `int64_t*`, will point to a location in memory where an i64 is stored.
	CONF_U8   = CONF_I8 | 0x80,  // expects: `uint8_t*`, will point to a location in memory where an u8 is stored.
	CONF_U16  = CONF_I16 | 0x80, // expects: `uint16_t*`, will point to a location in memory where an u16 is stored.
	CONF_U32  = CONF_I32 | 0x80, // expects: `uint32_t*`, will point to a location in memory where an u32 is stored.
	CONF_U64  = CONF_I64 | 0x80, // expects: `uint64_t*`, will point to a location in memory where an u64 is stored.
	CONF_F32  = CONF_I32 | 0x40, // expects: `float*`, will point to a location in memory where an f32 is stored.
	CONF_F64  = CONF_I64 | 0x40, // expects: `double*`, will point to a location in memory where an f64 is stored.
	CONF_FSTR = 0x40,            // expects: `struct conf_fstr*`, which contains the data for a fixed-width string
};

/* for outputting a fixed string as this config field */
struct conf_fstr {
	usize len; // length in BYTES of the output data
	char *out; // where we will output the data
};

/* defines the structure of a config file entry */
struct conf_entry {
	const char *key;  // the key of this entry
	void       *out;  // the pointer to which the data is written value is read if the given option is incorrect or missing
	u8          type; // the primitive type which we are querying for
};

/* processes an incoming buffer.
 * `buf`, `kout` and `vout` mustn't overlap, and must be (at least) `len` bytes long!
 * `kout` and `vout` will contain a null-terminated string if the function returned successfully.
 * returns `0` on success, `<0` when no data was found. `>0` when data was invalid but something went wrong.
 * see `CONF_E*` or `enum conf_err` */
int conf_procbuf(const char *restrict buf, char *restrict kout, char *restrict vout, usize len);

/* matches the key with one of the options and returns the pointer. Returns NULL if none could be found. */
struct conf_entry const *conf_matchopt(struct conf_entry const *opts, usize optc, const char *restrict key);

/* processes the value belonging to the key and outputs the result to opts.
 * - `val` points to a null-terminated string which contains the key and value.
 * returns `0` upon success, non-zero upon failure. For information about specific error codes, see `enum conf_err` */
int conf_procval(struct conf_entry const *opts, const char *restrict val);

/* acquires the config file path, appending str to the end (you need to handle path separators yourself)
 * expecting str to be null-terminated
 * - linux:   reads $XDG_CONFIG_HOME, if empty $HOME/.config is used, if $HOME is empty NULL is returned.
 * - windows: reads %APPDATA%, if empty %USERPROFILE%\AppData\Roaming is used, if both are empty NULL is returned.
 * - osx:     reads $HOME, uses $HOME/Library/Application Support, if $HOME is empty NULL is returned.
 * !! A malloc'd null-terminated string is returned !! */
char *conf_getpat(const char *) MALLOC NONNULL((1));
