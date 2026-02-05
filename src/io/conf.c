/* Copyright (C)2025 MCA-Selector-Lite
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcaselector-lite. */
#include "conf.h"

#include <assert.h>
#include <inttypes.h>
#include <mcaselector-lite/atrb.h>
#include <mcaselector-lite/types.h>
#include <stdlib.h>
#include <string.h>


/* Matches s1 with s2, returns a pointer to s1 where the match stopped. */
static const char *strmat(const char *s1, const char *s2) PURE NONNULL((1, 2));
static const char *strmat(const char *s1, const char *s2)
{
	while ((*s1 == *s2) && *s1)
		s1++, s2++;
	return s1;
}

int conf_getkeyval(const char *restrict buf,
	const char *const restrict *restrict keys, int klen,
	const char *restrict *restrict out)
{
	const char *tmp = NULL;

	ASSUME((klen > 0));
	int i = 0;
	for (; i < klen && !tmp; i++) {
		tmp = strmat(buf, keys[i]);
		tmp = keys[i][buf - tmp] ? tmp : NULL;
	}

	if (!tmp || *tmp != '=')
		return -1;

	*out = tmp + 1;
	return i;
}

int conf_procval(u8 type, const char *val, void *out)
{
	char *end;
	switch (type) {
	case CONF_STR: *(char **)out = strdup(val); return 0;

	case CONF_S8:  *(s8 *)out = strtoimax(val, &end, 0); return (end && !*end);
	case CONF_S16: *(s16 *)out = strtoimax(val, &end, 0); return (end && !*end);
	case CONF_S32: *(s32 *)out = strtoimax(val, &end, 0); return (end && !*end);
	case CONF_S64: *(s64 *)out = strtoimax(val, &end, 0); return (end && !*end);

	case CONF_U8:  *(u8 *)out = strtoumax(val, &end, 0); return (end && !*end);
	case CONF_U16: *(u16 *)out = strtoumax(val, &end, 0); return (end && !*end);
	case CONF_U32: *(u32 *)out = strtoumax(val, &end, 0); return (end && !*end);
	case CONF_U64: *(u64 *)out = strtoumax(val, &end, 0); return (end && !*end);

#if __SIZEOF_FLOAT__ == 4
	case CONF_F32: *(f32 *)out = strtof(val, &end); return (end && !*end);
#else
#error float is not 4 bytes wide
#endif
#if __SIZEOF_DOUBLE__ == 8
	case CONF_F64: *(f64 *)out = strtod(val, &end); return (end && !*end);
#elif __SIZEOF_LONG_DOUBLE__ == 8
	case CONF_F64: *(f64 *)out = strtold(val, &end); return (end && !*end);
#else
#error double or long double are not 8 bytes wide
#endif
	default: assert("invalid type");
	}
	return 0;
}
