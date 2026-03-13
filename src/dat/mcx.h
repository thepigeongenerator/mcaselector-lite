/* Copyright (C)2025-2026 mcxedit
 * Licensed under GPL-2.0-only. For further information,
 * view `git log`, and the COPYING and CONTRIBUTORS files
 * at www.github.com/thepigeongenerator/mcxedit. */
#ifndef MCXEDIT_MCX_H
#define MCXEDIT_MCX_H 1

#include "../atrb.h"
#include "../types.h"

#define MCX_SECTOR     0x1000 /* Size in bytes for a sector. */
#define MCX_TABLE      0x1000 /* Size in bytes for one table. */
#define MCX_TABLES     0x2000 /* Size in bytes for the tables. */
#define MCX_TABLE_LEN  0x400  /* Amount of elements within one table. */
#define MCX_TABLES_LEN 0x800  /* Amount of elements within the tables. */

void mcx_check(const void *mcx, usize size, const char *pat);

/* Repairs faults in the file format,
 * outputting warnings describing the repaired fault & affected chunk.
 * Returns the size of the file, which may be larger than,
 * less than, or equal to the original size. */
usize mcx_repair(void *mcx, usize size);

/* Prunes the unused sections in a .mcX file.
 * It is assumed that the table is formatted correctly and
 * that all data is accessible.
 * Returns the new file size.*/
usize mcx_defrag(void *mcx);

/* Computes the size in bytes of the entire .mcX file,
 * according to the table.
 * This is done by summing the chunk sector offset and length
 * and taking the highest value.
 *
 * This value may not be reflective of the actual value due to unused
 * sectors at the end, or that overlapping values are present.
 * If this value is higher than the actual file size,
 * then corruption has taken place. */
usize mcx_calcsize(const void *mcx) PURE;

/* Computes the minimum size in bytes required for the entire .mcX file,
 * according to the table.
 * This is done through summing together the length sectors of each chunk,
 * and adding 2 for the table.
 *
 * This value is a very optimistic value, and unlikely to be reflective
 * of the actual size due to the presence of unused sectors.
 * In the edge case of overlapping sectors, this value may be larger
 * than the actual file size. */
usize mcx_minsize(const void *mcx) PURE;

#endif /* MCXEDIT_MCX_H */
