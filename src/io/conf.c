/* This file is part of MCA-Selector-lite,
 * and is licensed under GPL-2.0-only.
 * Copyright (C)2025 quinnthepigeon@proton.me Quinn
 * For further information, view COPYING and CONTRIBUTORS
 * at: www.github.com/thepigeongenerator/mcaselector-lite */
#include "conf.h"

#include <string.h>

#include "../types.h"
#include "../util/atrb.h"

/* Matches s1 with s2, returns a pointer to s1 where the match stopped. */
static const char *strmat(const char *s1, const char *s2) PURE NONNULL((1, 2));
static const char *strmat(const char *s1, const char *s2)
{
	while ((*s1 == *s2) & !!*s1)
		s1++, s2++;
	return s1;
}

int conf_getkeyval(const char *restrict buf, const char *const restrict *restrict keys, int klen, const char *restrict *restrict out)
{
	const char *tmp = NULL;

	ASSUME((klen > 0));
	int i = 0;
	for (; i < klen && !tmp; i++) {
		tmp = strmat(buf, keys[i]);
		tmp = keys[i][buf - tmp] ? tmp : 0;
	}

	if (!tmp || *tmp != '=')
		return -1;

	*out = tmp + 1;
	return i;
}

int conf_val(int);
