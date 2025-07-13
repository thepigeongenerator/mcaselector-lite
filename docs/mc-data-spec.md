# Minecraft data specification
## NBT specification
NBT (named binary tag) is a tree data structure. Tags have a numeric type ID, name and a payload. The human-readable variant of this data is SNBT (stringified named binary tag)
NBT files are a compressed `compound` tag. GZip is the compression used in most cases, in some rare cases it's stored uncompressed.
A tag is an individual part of the data tree, where the first byte is the ID, followed by a `uint16_t` for the length of the name, then the name as `UTF-8`. (end tag does not contain name)

note: UUID are stored as an integer array.

### tag types
| ID   | tag name     | payload specification               |
|-----:|:-------------|:------------------------------------|
|  `0` | `end`        | -                                   |
|  `1` | `byte`       | `int8_t`                            |
|  `2` | `short`      | `int16_t` (BE[^be])                 |
|  `3` | `int`        | `int32_t` (BE)                      |
|  `4` | `long`       | `int64_t` (BE)                      |
|  `5` | `float`      | `float`   (BE)                      |
|  `6` | `double`     | `double`  (BE)                      |
|  `7` | `byte array` | `int32_t`     (len) -> `int8_t`     |
|  `8` | `string`     | `uint16_t`    (len) -> `UTF-8`      |
|  `9` | `list`       | ID: `int32_t` (len) -> ID           |
| `10` | `compound`   | list of tags delimited with end tag |
| `11` | `int array`  | `int32_t`     (len) -> `int32_t`    |
| `12` | `long array` | `int32_t`     (len) -> `int64_t`    |
[^be] [big-endian](https://en.wikipedia.org/wiki/Endianness)

## world data
There is a difference between \*.mca and \*.mcr files.
Where \*.mca files are the newer variant.
- mcr = MinecraftRegion
- mca = Anvil

### file locations
| dim       | file path            |
|:----------|:---------------------|
| overworld | `world/region`       |
| nether    | `world/DIM-1/region` |
| the end   | `world/DIM1/region`  |

### coordinate conversions
```c
return (x >> 4); // block->chunk
return (x >> 5); // chunk->region
```


### chunk format specification
Chunks are stored as NBT-formatted data.
The actual structure of this is best illustrated by [the wiki](https://minecraft.wiki/w/Chunk_format#NBT_structure).

#### notable tags
| path                                       | tag type     | description                                                                   |
|:-------------------------------------------|:-------------|-------------------------------------------------------------------------------|
| `DataVersion`                              | `int`        | version of the NBT structure                                                  |
| `xPos`                                     | `int`        | X chunk position of the chunk from the world origin                           |
| `zPos`                                     | `int`        | Z chunk position of the chunk from the world origin                           |
| `yPos`                                     | `int`        | lowest Y section position in the chunk                                        |
| `InhabitedTime`                            | `long`       | the accumilative number of ticks that players have been present in this chunk |
| `sections`                                 | `list`       | list of compounds containing the chunk sections                               |
| `sections[i].Y`                            | `byte/int`   | the Y block position of this section (can be int in versions ≥`1.18`          |
| `sections[i].block_states.palette`         | `list`       | contains the block states for this particular section                         |
| `sections[i].block_states.palette[i].Name` | `string`     | block [resource location](https://minecraft.wiki/w/Resource_location)         |
| `sections[i].block_states.data`            | `long array` | [view below](#block-state-data)                                               |
##### block state data
Contains 4096 indices, which are packed in a specific way.
All indices are the same with, though the width is decided by the minimum width required to index the palette. In versions ≥`1.16`, the indices are not packed across multiple elements of the array.
So they have an alignment requirement of 64 bits.
There might be an additional section at the top and or bottom of the world used to store light, so that light travels properly over and under the world limits.
This is an example on how to access individual block info from a single section;
```c
unsigned w = 0;
int32_t tmp = block_states->palette.len;
while (tmp >>= 1) w++; // get the width of the palette length

// get the data for calculating the index (NOTE: this process is only applicable for mc versions <1.16)
unsigned pos = (y << 8) + (z << 4) + x; // acquire the position in the data array according to; 16²•y + 16•z + x
unsigned bit = pos * w;                 // calc which bit the value starts at.
unsigned sgmt = bit / 64;               // calc which segment the data lives in
unsigned offs = bit & 63;               // calc the offset within the segment

// calculate the correct index
uint16_t idx;
idx = (block_states->dat[sgmt] >> offs);                                // acquire the data in the segment
idx |= -!!((offs + w) > 64) & (block_states->dat[sgmt+1] << (64-offs)); // complete with the data from the other segment, if present
idx &= (1 << w) - 1;                                                    // truncate the data to only contain what we desire.

struct blockdat blk = block_states->palette.dat[idx];
```

### MCR format specification
#### header
Region files have an 8KiB header, split in two 4KiB tables.
The first containing the offsets of chunks in the region file itself, the second providing timestamps for the last updates of those chunks.
The offset of a chunk (x,z) (in chunk coordinates) in the first table can be found by using this formula:
`4 * ((x & 31) + (z & 31) * 32)`. The timestamp can be found 4096 bytes later in the file.
| range | `0x00`—`0x0FFF`    | `0x1000`—`0x1FFF`   | `0x2000`—               |
|:------|:-------------------|:--------------------|:------------------------|
| data  | locations (4B)     | timestamps (4B)     | chunks and unused space |

##### chunk location
Location info for a chunk is stored as a 32 bit big-endian integer, where the first three bytes are an offset in 4KiB sectors from the start of the file.
The last byte gives the length of the chunk in 4KiB sectors. (rounded up, of course). Where chunks are always less than 1MiB in size.
If a chunk isn't present in the region file (e.g. because it hasn't been generated or migrated yet), both fields are zero.

##### timestamps
Timestamp data are 32 bit big-endian integers, representing the last modification time of an individual chunk in epoch seconds.

#### payload
Chunk data starts with a big-endian 32 bit signed integer which contains the exact length of the data in bytes.
The first byte of this data indicates the compression scheme used for the data. The rest of the data is the actual compression data. (len-1 remaining now)
The data has an alignment requirement of 4KiB.

##### compression
| value | type                                |
|------:|:------------------------------------|
|   `1` | GZip (RFC1952) (unused in practice) |
|   `2` | Zlib (RFC1950)                      |
|   `3` | uncompressed (<`1.15.1`)            |
|   `4` | LZ4 (≥`24w04a`)                     |
| `127` | custom algorithm\* (≥`24w04a`)      |

\*: A namespaced string must follow representing the algorithm used. The string is preceded by its length, encoded as an unsigned 16-bit integer.
The uncompressed data is in NBT format and follows the chunk format.
If the value of compression scheme increases by 128, the compressed data is saved in a file called `c.x.z.mcc`, where x and z are the chunk's coordinates, instead of the usual position.

### MCA format specification
The Anvil file format has some edits compared to the region file format;
- Build height increase from 128 to 256
- Empty sections of the world are not loaded into memory or saved to disk.
- Maximum block ID increased from 265 to 4096. By adding a 4-bit data layer (similar to how metadata is stored).
- Block ordering has been changed from XZY to YZX in order to improve compression.
- Packets for sending chunks has been optimized (a chunk without air is smaller than in the same chunk in the old format, and a chunk with lots of air is even smaller).
- Biomes are saved per X,Z column, rather than being calculated on the fly.

## sources
- https://minecraft.wiki/w/Region_file_format
- https://minecraft.wiki/w/Anvil_file_format
- https://minecraft.wiki/w/Chunk_format
