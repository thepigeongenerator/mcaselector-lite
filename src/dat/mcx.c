/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#include "mcx.h"

#include <stdlib.h>
#include <string.h>

#include "../endian.h"
#include "../err.h"
#include "../types.h"

/* TODO: check for overlapping. */
void mcx_check(const void *mcx, usize size, const char *pat)
{
	const be32 *tbl = mcx;
	const be32 *end = mcx + MCX_TABLE;
	u32         tmp, chpos, chlen, chend;
	for (int i = 0; ++tbl < end; ++i) {
		if (!*tbl) continue;
		tmp   = cvt_be32toh(*tbl);
		chlen = tmp & 0xFF;
		chpos = tmp >> 8;
		chlen = tmp & 0xFF;
		chend = (chpos + chlen) * MCX_SECTOR;

		if (chpos < 2 || !chlen) {
			warnx("%s:\t(%2u,%2u) Has invalid sectors.",
				pat, i % 32, i / 32);
		} else if (chend > size) {
			warnx("%s: (%2u,%2u) Exeeds maximum file size. (+%zuB)",
				pat, i % 32, i / 32, chend - size);
		}
	}
}

usize mcx_repair(void *mcx, usize size)
{
	be32 *tbl = mcx;
	be32 *end = mcx + MCX_TABLE;
	u32   chpos, chlen, chend;
	u32   max = 0, tmp;
	do {
		if (!*tbl) continue; /* TODO: may be faster without? */
		tmp   = cvt_be32toh(*tbl);
		chpos = tmp >> 8;
		chlen = tmp & 0xFF;
		chend = (chpos + chlen) * MCX_SECTOR;

		if (chpos < 2 || !chlen) {
			*tbl = 0;
			continue;
		} else if (chend > size) {
			/* TODO: Add functionality that looks at the payload data,
			 * and attempts to restore from that, if possible. */
			*tbl = 0;
			continue;
		}
		max = max < tmp ? tmp : max;
	} while (++tbl < end);
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

/* Comparison function used for qsort.
 * Returns <0 if ma is less, =0 if equal, >0 if mb is less. */
static int mcx_defrag_compar(const void *ma, const void *mb)
{
	const u32 *a = ma, *b = mb;
	return *a - *b;
}

/* Sort the table based on offset,
 * Then move the chunks down into empty space. */
usize mcx_defrag(void *mcx)
{
	u32   chunks[MCX_TABLE_LEN * 2];
	u32  *chunk = chunks;
	be32 *tbl   = mcx;
	for (int i = 0; i < MCX_TABLE_LEN; ++i) {
		*chunk++ = cvt_be32toh(tbl[i]);
		*chunk++ = i;
	}
	qsort(chunks, sizeof(chunks), sizeof(*chunks) * 2, mcx_defrag_compar);
	u32 *end = chunk;
	chunk    = chunks;

	u32 pos = 2;
	u32 chpos, chlen;
	do {
		chpos = *chunks >> 8;
		chlen = *chunks & 0xFF;
		if (chpos == pos || chpos < 2)
			goto next_table_item;

		/* BUG: Not checking for overlapping chunks causing issues. */
		memmove(mcx + pos * MCX_SECTOR, mcx + chpos * MCX_SECTOR, chlen * MCX_SECTOR);
		tbl[*(chunk + 1)] = cvt_htobe32(chlen | (pos << 8));
next_table_item:
		pos += chlen;
	} while ((chunk += 2) < end);
	if (pos == 2)
		return 0;
	return pos * MCX_SECTOR;
}

usize mcx_calcsize(const void *mcx)
{
	const be32 *tbl = mcx;
	const be32 *end = tbl + MCX_TABLE_LEN;
	u32         max = 0, tmp;
	do {
		tmp = cvt_be32toh(*tbl);
		max = max < tmp ? tmp : max;
	} while (++tbl < end);
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

usize mcx_sumsize(const void *mcx)
{
	const be32 *tbl = mcx;
	const be32 *end = tbl + MCX_TABLE_LEN;

	usize sum = 0;
	do {
		const u8 *m = (const u8 *)tbl;
		sum += m[3];
	} while (++tbl < end);
	return sum * MCX_SECTOR + MCX_TABLES;
}
