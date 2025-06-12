#include "conf.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "atrb.h"

int conf_procbuf(char const* restrict buf, size_t buflen, struct conf_entry const* opts, size_t optc) {
	enum {
		ERR_ABSENT = 1,
		ERR_NOMATCH = 2,
		ERR_INVALID_TYPE = 3,
	};

	unsigned idx = 0;         // index to the data below
	char const* dat[2] = {0}; // stores key-value data
	unsigned len[2] = {0};    // stores the length of the data
	for (size_t i = 0; i < buflen; i++) {
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
		if (buf[i] == '=' && !idx) {
			idx++;
			continue;
		}

		// set the value pointer to the current buffer char
		dat[idx] = dat[idx] ? dat[idx] : &buf[i];
		len[idx]++;
	}
	if (!(dat[0] && dat[1])) return ERR_ABSENT;

	// find a match for the current key
	size_t i = 0;
	for (; i < optc; i++) {
		if (strcmp(opts[i].key, dat[0]) == 0) break;
	}
	if (i >= optc) return ERR_NOMATCH; // no match was found

	// TODO: keys and vals are not stored as null-terminated strings, thus we should do that
	switch (opts[i].type) {
	case CONF_I8: break;
	case CONF_I16: break;
	case CONF_I32: break;
	case CONF_I64: break;
	case CONF_U8: break;
	case CONF_U16: break;
	case CONF_U32: break;
	case CONF_U64: break;
	case CONF_F32: break;
	case CONF_F64: break;
	case CONF_STR: break;
	case CONF_FSTR: break;
	default: return ERR_INVALID_TYPE;
	}

	return 0;
}

/* utility function for conf_getpat to concatenate 3 strings, where we already know the size */
atrb_nonnull(1, 3) static inline char* conf_getpat_concat(char const* restrict s1, char const* restrict s2, char const* restrict s3, size_t s1len, size_t s2len, size_t s3len) {
	char *buf, *ptr;

	// allocate enough data for all three to the buffer
	ptr = malloc(s1len + s2len + s3len + 1);
	if (!ptr) return NULL;

	// copy data to the buffer
	buf = strcpy(ptr, s1);                           // copy s1 data to the buffer
	if (s2len) ptr = memcpy(ptr + s1len, s2, s2len); // increment ptr by s1len, and copy s2 data to the buffer (excluding null-terminator)
	(void)strcpy(ptr + s2len, s3);                   // add s2len to the pointer and copy s3 as a string, thus including null-terminator

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
		cfgpat = getenv("USERPROFILE");
		if (!buf) return NULL;
		len = strlen(buf);
		return conf_getpat_concat(buf, "\\AppData\\Roaming", str, buf, 16, str_len);
	}
	return conf_getpat_concat(buf, NULL, str, strlen(buf), 0, str_len);
#else
#error platform unsupported!
#endif
}
