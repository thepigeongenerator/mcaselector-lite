# Minecraft data specification
There is a difference between \*.mca and \*.mcr files.
Where \*.mca files are the newer variant.
- mcr = MinecraftRegion
- mca = Anvil

## file locations
| dim       | file path            |
|:----------|:---------------------|
| overworld | `world/region`       |
| nether    | `world/DIM-1/region` |
| the end   | `world/DIM1/region`  |

## coordinate conversions
```c
// block->chunk:
return (x / 16) - sgn(x);
return (x >> 4) - (x & 0x80000000);

// chunk->region:
return (x / 32) - sgn(x);
return (x >> 5) - (x & 0x80000000);
```

## MCR format specification
## structure
### header
Region files have an 8KiB header, split in two 4KiB tables.
The first containing the offsets of chunks in the region file itself, the second providing timestamps for the last updates of those chunks.
The offset of a chunk (x,z) (in chunk coordinates) in the first table can be found by using this formula:
`4 * ((x & 32) + (z & 31) * 32)`. The timestamp can be found 4096 bytes later in the file.
| range | 0x00—0x0FFF    | 0x1000—0x1FFF   | 0x2000—                 |
|:------|:---------------|:----------------|:------------------------|
| data  | locations (4b) | timestamps (4b) | chunks and unused space |

#### chunk location
Location info for a chunk is stored as a 32 bit big-endian integer, where the first three bytes are an offset in 4KiB sectors from the start of the file.
The last byte gives the length of the chunk in 4KiB sectors. (rounded up, of course). Where chunks are always less than 1MiB in size.
If a chunk isn't present in the region file (e.g. because it hasn't been generated or migrated yet), both fields are zero.

#### timestamps
Timestamp data are 32 bit big-endian integers, representing the last modification time of an individual chunk in epoch seconds.

### payload
Chunk data starts with a big-endian 32 bit signed integer which contains the exact length of the data in bytes.
The first byte of this data indicates the compression scheme used for the data. The rest of the data is the actual compression data. (len-1 remaining now)
The data has an alignment requirement of 4KiB.

#### compression
| value | type                                |
|------:|:------------------------------------|
|     1 | GZip (RFC1952) (unused in practice) |
|     2 | Zlib (RFC1950)                      |
|     3 | uncompressed (<1.15.1)              |
|     4 | LZ4 (≥24w04a)                       |
|   127 | custom algorithm[^1] (≥24w04a)      |
[^1]: A namespaced string must follow representing the algorithm used. The string is preceded by its length, encoded as an unsigned 16-bit integer.
The uncompressed data is in NBT format and follows the chunk format.

<!-- TODO: perform further research by reading the articles and writing the information down here. -->

## sources
- [https://minecraft.wiki/w/Region_file_format](https://minecraft.wiki/w/Region_file_format)
- [https://minecraft.wiki/w/Anvil_file_format](https://minecraft.wiki/w/Anvil_file_format)
- [https://minecraft.wiki/w/Chunk_format](https://minecraft.wiki/w/Chunk_format)
