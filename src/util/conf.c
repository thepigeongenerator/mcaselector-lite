// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#include "conf.h"

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../error.h"
#include "atrb.h"
#include "types.h"

int conf_procbuf(char const* restrict buf, char* restrict kout, char* restrict vout, size_t len) {
	bool feq = false; // whether we've found the equal sign

	// data traversal
	char* pos = kout; // will point to the next point in the buffer, where we'll write data

	// acquire data
	for (size_t i = 0; i < len; i++) {
		// handling of termination tokens
		bool brk = false; // whether we broke out of the loop, and are done reading
		switch (buf[i]) {
		case '\n':
		case '\r':
		case '\0':
		case '#':
			brk = true;
			break;
		}
		if (brk) break;

		// everything after `=` is interpreted as a value
		if (!feq && buf[i] == '=') {
			feq = true;
			*pos = '\0'; // terminate string
			pos = vout;  // move pointer to start of value data
			continue;
		}
		*pos = buf[i]; // copy over the buffer's data
		pos++;         // increment the position pointer
	}
	// null-terminate what we've got now (yes, there should be enough space for this since \0 isn't stored)
	// this also ensures the value is valid, even if none is given
	*pos = '\0';

	// no data if we didn't move from the key position
	// syntax error if we couldn't find the equal sign
	return (pos == kout)
		? CONF_ENODAT
		: (!feq ? CONF_ESYNTAX : 0);
}

struct conf_entry const* conf_matchopt(struct conf_entry const* opts, size_t optc, char const* restrict key) {
	// find a match for the current key
	size_t i = 0;
	for (; i < optc; i++) {
		if (strcmp(opts[i].key, key) == 0)
			return opts + i;
	}
	return NULL;
}

int conf_procval(struct conf_entry const* opt, char const* restrict val) {
	// parse the data
	errno = 0;
	char* end;
	u8 dat[sizeof(u64)];

	switch (opt->type) {
	// signed integer data parsing
	case CONF_I8:
	case CONF_I16:
	case CONF_I32:
	case CONF_I64:
		*(i64*)dat = strtoll(val, &end, 10); // for signed integer types
		break;
	// unsigned integer data parsing
	case CONF_U8:
	case CONF_U16:
	case CONF_U32:
	case CONF_U64:
		*(u64*)dat = strtoull(val, &end, 10); // for unsigned integer types
		break;

	// floating-point data parsing
	case CONF_F32: *(f32*)dat = strtof(val, &end); break;
	case CONF_F64: *(f64*)dat = strtod(val, &end); break;

	// string data parsing
	case CONF_STR:
		if (*(char**)opt->out) {
			free(*(char**)opt->out); // if the same key is given multiple times, free the memory so we don't leak.
			warn("encountered a dynamic string multiple times, this is sub-optimal.");
		}
		*(char**)opt->out = strdup(val);
		return 0;
	case CONF_FSTR: {
		struct conf_fstr* s = opt->out;
		strncpy(s->out, val, s->len);
		s->out[s->len - 1] = '\0'; // ensure the string is null-terminated
		return 0;
	}
	default: return CONF_EINVALIDTYPE; // return error code indicating function not implemented
	}

	if (errno || end == val || *end != '\0') {
		error("failed to parse '%s' as a numeric value", val);
		return CONF_EPARSE;
	}

	switch (opt->type) {
	case CONF_U8: *(u8*)opt->out = *(u64*)dat >= UINT8_MAX ? UINT8_MAX : *(u64*)dat; return 0;
	case CONF_U16: *(u16*)opt->out = *(u64*)dat >= UINT16_MAX ? UINT16_MAX : *(u64*)dat; return 0;
	case CONF_U32: *(u32*)opt->out = *(u64*)dat >= UINT32_MAX ? UINT32_MAX : *(u64*)dat; return 0;
	case CONF_U64: *(u64*)opt->out = *(u64*)dat >= UINT64_MAX ? UINT64_MAX : *(u64*)dat; return 0;
	case CONF_I8: *(i8*)opt->out = *(i64*)dat >= INT8_MAX ? INT8_MAX : (*(i64*)dat <= INT8_MIN ? INT8_MIN : *(i64*)dat); return 0;
	case CONF_I16: *(i16*)opt->out = *(i64*)dat >= INT16_MAX ? INT16_MAX : (*(i64*)dat <= INT16_MIN ? INT16_MIN : *(i64*)dat); return 0;
	case CONF_I32: *(i32*)opt->out = *(i64*)dat >= INT32_MAX ? INT32_MAX : (*(i64*)dat <= INT32_MIN ? INT32_MIN : *(i64*)dat); return 0;
	case CONF_I64: *(i64*)opt->out = *(i64*)dat >= INT64_MAX ? INT64_MAX : (*(i64*)dat <= INT64_MIN ? INT64_MIN : *(i64*)dat); return 0;
	case CONF_F32: *(f32*)opt->out = *(f32*)dat; return 0;
	case CONF_F64: *(f64*)opt->out = *(f64*)dat; return 0;
	default: fatal("invalid switch state, all cases should be handled already"); // abort; this shouldn't be possible, so I blame the programmer
	}
}

/* utility function for conf_getpat to concatenate 3 strings, where we already know the size */
atrb_nonnull(1, 3) static inline char* conf_getpat_concat(char const* restrict s1, char const* restrict s2, char const* restrict s3, size_t s1len, size_t s2len, size_t s3len) {
	assert(s2 || (!s2 && !s2len)); // ensuring the programmer passes both s2 and s2len as 0, if they intend to
	char *buf, *ptr;

	// allocate enough data for all three to the buffer
	ptr = malloc(s1len + s2len + s3len + 1);
	if (!ptr) return NULL;
	buf = ptr; // store the head pointer into buf

	// copy data to the buffer
	ptr = mempcpy(ptr, s1, s1len);            // copy s1 data to the buffer
	if (s2len) ptr = mempcpy(ptr, s2, s2len); // copy s2 data to the buffer (excluding null-terminator)
	(void)strcpy(ptr, s3);                    // copy s3 as a string, thus including null-terminator

	// return the buffer
	return buf;
}

/* appends str to the config directory string we acquire from environment variables. */
char* conf_getpat(char const* restrict str) {
	char* buf = NULL;
	size_t len;
	size_t str_len = strlen(str);
#if defined(__linux__)
	buf = getenv("XDG_CONFIG_HOME");
	if (!buf) {
		buf = getenv("HOME");
		if (!buf) return NULL;
		len = strlen(buf);
		return conf_getpat_concat(buf, "/.config", str, len, 8, str_len);
	}
	return conf_getpat_concat(buf, NULL, str, strlen(buf), 0, str_len);
#elif defined(__APPLE__)
	buf = getenv("HOME");
	if (!buf) return NULL;
	len = strlen(buf);
	return conf_getpat_concat(buf, "/Library/Application Support", str, len, 28, str_len);
#elif defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	buf = getenv("APPDATA");
	if (!buf) {
		buf = getenv("USERPROFILE");
		if (!buf) return NULL;
		len = strlen(buf);
		return conf_getpat_concat(buf, "\\AppData\\Roaming", str, len, 16, str_len);
	}
	return conf_getpat_concat(buf, NULL, str, strlen(buf), 0, str_len);
#else
#error platform unsupported!
#endif
}
