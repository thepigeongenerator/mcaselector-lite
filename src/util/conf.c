#include "conf.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#define BUFS 64

/* contains the basic structure for a string */
struct str {
	char* dat;    // pointer to the string data
	unsigned cap; // current string capacity
};

/* utility function for conf_getpat to concatenate 3 strings, where we already know the size */
__attribute_nonnull__((1, 3)) static inline char* conf_getpat_concat(
	char const* restrict s1, char const* restrict s2, char const* restrict s3,
	size_t s1len, size_t s2len, size_t s3len) {
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

/* appends src onto dest.dat, if test.dat hasn't been allocated, or does not have enough space.
   The size is the nearest 2ˣ rounded-up value of src_len.
   returns 0 upon success, 1 upon failure. */
__attribute_nonnull__((1, 3)) static inline int str_put(struct str* restrict dest, size_t dest_len, char const* restrict src, size_t src_len) {
	// check if (re)allocation needs to occur
	if (dest->cap <= src_len) {                                 // check for equality as well due to `\0`
		dest->cap = src_len;                                    // using src_len as-is, since it is string length, thus already one short.
		for (unsigned i = 1; i < (sizeof(size_t) * 8); i <<= 1) // then loop through each 2ˣ bit in size_t
			dest->cap |= dest->cap >> i;                        // OR together the shifted result (shifted 1, 2, 4, 8, 16 on 32 bit systems)
		dest->cap++;                                            // round to the most significant bit (0111 -> 1000)

		// (re)allocate the array
		if (!dest->dat) {
			dest->dat = malloc(dest->cap); // allocate memory for this capacity
			if (!dest->dat) return 1;      // return 1 upon failure
		} else {
			void* ptr = realloc(dest->dat, dest->cap); // reallocate to the new capacity
			if (!ptr) {
				free(dest->dat); // free up resources by the old (still valid) pointer, and return failure
				return 1;
			}
			dest->dat = ptr;
		}
	}


	// copy the missing data to the end of the destination
	memcpy(dest->dat + dest_len, src, src_len - dest_len);
	dest->dat[src_len] = '\0'; // null-terminate the destination
	return 0;
}

/* loads the config file */
int conf_loadfile(char const* pat, struct conf_entry* dat, size_t cnt) {
	FILE* f = fopen(pat, "r");
	if (!f) return 1;

	size_t ln = 0;
	char buf[BUFS];
	while (fgets(buf, BUFS, f)) {
		bool isval = 0;     // whether we're reading the value at the moment (used as index)
		bool nodat = false; // whether there is no more data after this point
		bool brk = false;   // whether we broke out of the loop
		unsigned vallen = 0;
		unsigned keylen = 0;

		// loop through each character
		for (unsigned i = 0; i < BUFS; i++) {
			//
			switch (buf[i]) {
			case '\n':
			case '\0':
				brk = true;
				break; // break out of the switch
			case '#':
				nodat = true; // specify that there is no more data from this point onwards
				continue;     // continue in the loop
			case '=':
				if (!isval) {
					isval = 1;
					continue;
				} else break;
			}
			if (brk) break;
			if (nodat) continue;

			if (isval) vallen++;
			else keylen++;

			// TODO: key/value are initialized NULL
			// TODO: key/value caches the pointer to the data in the buffer
		}
		// TODO: key/value are checked for NULL, if either are NULL; something went wrong.
		// TODO: append the key/value to the str_put thingie
		// TODO: check what type we're converting the data to and go and do that.
		// FIX: rewrite the whole thing to make testing easier. i.e. don't handle file reading AND conf parsing in the same funciton
	}

	fclose(f);
	return 0;
}
