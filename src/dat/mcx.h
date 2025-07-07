// Copyright (c) 2025 Quinn
// Licensed under the MIT Licence. See LICENSE for details
#pragma once

/* the MCR (Minecraft region) and MCA (Minecraft anvil) files are similar
 * MCA is the newer variant, where it includes:
 *  - a world height of 256, rather than 128.
 *  - block IDs have been extended to 4096 from 256
 *  - block ordering is now YZX, rather than XZY.
 *  - biomes are included in the data per XZ column.
 * MCR start with an 8KiB header, split in two 4KiB tables
 * - the first containing the offsets of chunks in the region file itself.
 * - the second providing timestamps on when they were last updated.
 * -
 *
 *
 *
 *
 */
