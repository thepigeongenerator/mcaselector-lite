#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>

/* defines the primitive types available in the config file */
enum config_primitive {
	CONF_STR = 0,               // expects: `char**`, will output malloc'd data !!must be freed!!
	CONF_I8 = 1,                // expects: `int8_t*`, will point to a location in memory where an i8 is stored.
	CONF_I16 = 2,               // expects: `int16_t*`, will point to a location in memory where an i16 is stored.
	CONF_I32 = 4,               // expects: `int32_t*`, will point to a location in memory where an i32 is stored.
	CONF_I64 = 8,               // expects: `int64_t*`, will point to a location in memory where an i64 is stored.
	CONF_U8 = CONF_I8 | 0x80,   // expects: `uint8_t*`, will point to a location in memory where an u8 is stored.
	CONF_U16 = CONF_I16 | 0x80, // expects: `uint16_t*`, will point to a location in memory where an u16 is stored.
	CONF_U32 = CONF_I32 | 0x80, // expects: `uint32_t*`, will point to a location in memory where an u32 is stored.
	CONF_U64 = CONF_I64 | 0x80, // expects: `uint64_t*`, will point to a location in memory where an u64 is stored.
	CONF_F32 = CONF_I32 | 0x40, // expects: `float*`, will point to a location in memory where an f32 is stored.
	CONF_F64 = CONF_I64 | 0x40, // expects: `double*`, will point to a location in memory where an f64 is stored.
	CONF_FSTR = 0x40,           // expects: `struct conf_fstr*`, which contains the data for a fixed-width string
};

/* for outputting a fixed string as this config field */
struct conf_fstr {
	size_t len; // length in BYTES of the output data
	char* datl; // where we will output the data
};

/* defines the structure of a config file entry */
struct conf_entry {
	char const* key; // the key of this entry
	void* out;       // the pointer to which the data is written value is read if the given option is incorrect or missing
	uint8_t type;    // the primitive type which we are querying for
};

/* acquires the config file path, appending str to the end (you need to handle path separators yourself)
 * expecting str to be null-terminated
 * - linux:   reads $XDG_CONFIG_HOME, if empty $HOME/.config is used, if $HOME is empty NULL is returned.
 * - windows: reads %APPDATA%, if empty %USERPROFILE%\AppData\Roaming is used, if both are empty NULL is returned.
 * - osx:     reads $HOME, uses $HOME/Library/Application Support, if $HOME is empty NULL is returned.
 * !! A malloc'd null-terminated string is returned !! */
__attribute_malloc__ __attribute_nonnull__((1)) char* conf_getpat(char const*);

/* loads the config file with cnt of dat options */
int conf_loadfile(char const* pat, struct conf_entry* dat, size_t cnt);
