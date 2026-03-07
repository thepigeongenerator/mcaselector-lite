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

/* Binds an indices with the value in a mcX table.
 * Used for when sorting the table. */
struct mcx_table_item {
	u32 val;
	u16 idx;
};

/* Comparison function used for qsort.
 * Returns <0 if ma is less, =0 if equal, >0 if mb is less. */
static int mcx_table_item_compar(const void *ma, const void *mb)
{
	const struct mcx_table_item *a = ma, *b = mb;
	return a->val - b->val;
}

usize mcx_repair(void *mcx, usize size)
{
	u32   max = 0, tmp; /* Using similar approach as in mcx_calcsize */
	u32   chpos, chlen, chend;
	be32 *tbl = mcx;
	for (int i = 0; i < MCX_TABLE_LEN; ++i) {
		if (!tbl[i]) continue;
		tmp   = cvt_be32toh(tbl[i]);
		chlen = tmp & 0xFF;
		chpos = tmp >> 8;
		chend = (chlen + chpos) * MCX_SECTOR;

		if (chpos < 2 || !chlen) {
			warnx("(%u,%u): Chunk has invalid sectors.",
				i % 32, i / 32);
			tbl[i] = 0;
			continue;
		}

		/* TODO: Add functionality that looks at the payload data,
		 * and attempts to restore from that, if possible. */
		if (chend > size) {
			warnx("(%u,%u): Chunk exeeds the maximum file size, need %zuB, got %zuB",
				i % 32, i / 32, (usize)chend, size);
			tbl[i] = 0;
			continue;
		}

		max = max < tmp ? tmp : max;
	}

	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

/* Sort the table based on offset,
 * Then move the chunks down into empty space. */
usize mcx_defrag(void *mcx)
{
	struct mcx_table_item tbl[MCX_TABLE_LEN];

	be32 *mcx_tbl = mcx;
	for (int i = 0; i < MCX_TABLE_LEN; ++i) {
		u32 v  = cvt_be32toh(mcx_tbl[i]);
		tbl[i] = (struct mcx_table_item){v, i};
	}

	qsort(tbl, MCX_TABLE_LEN, sizeof(*tbl), mcx_table_item_compar);
	u32 pos = 2;
	u32 chpos, chlen;
	for (int i = 0; i < MCX_TABLE_LEN; ++i) {
		chpos = tbl[i].val & 0xFF;
		chlen = tbl[i].val >> 8;
		if (chpos == pos || chpos < 2)
			goto next_table_item; /* If pos <2, len=0. */

		memmove(mcx + pos * MCX_SECTOR, mcx + chpos * MCX_SECTOR, chlen * MCX_SECTOR);
		tbl[i].val          = chlen | (pos << 8);
		mcx_tbl[tbl[i].idx] = cvt_htobe32(tbl->val);
next_table_item:
		pos += chlen;
	}
	return pos * MCX_SECTOR;
}

usize mcx_calcsize(const void *mcx)
{
	const be32 *tbl = mcx;
	const be32 *end = tbl + MCX_TABLE_LEN;

	/* We can get away with comparing without masking,
	 * Since the sector data is the least significant byte,
	 * and thus won't weigh into our results. */
	u32 max = 0, tmp;
	while (tbl < end) {
		tmp = cvt_be32toh(*tbl++);
		max = max < tmp ? tmp : max;
	}
	return ((max >> 8) + (max & 0xFF)) * MCX_SECTOR;
}

usize mcx_minsize(const void *mcx)
{
	const be32 *tbl = mcx;
	const be32 *end = tbl + MCX_TABLE_LEN;

	usize sum = 0;
	while (tbl < end) {
		const u8 *m = (const u8 *)tbl;
		sum += m[3];
	}
	return sum * MCX_SECTOR + MCX_TABLES;
}
