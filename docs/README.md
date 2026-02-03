> [!CAUTION]
> This application is __not__ ready for usage, and shouldn't be used.
> It is under active development and most features are considered "béta",
> Once a pre-release, or release has been created in [the release tab](https://github.com/thepigeongenerator/mcaselector-lite/releases),
> it is ready for general usage and testing.

# MCA-Selector-Lite
A from-scratch rewrite of [MCA-Selector](https://github.com/Querz/mcaselector) in C.

[![licence](https://img.shields.io/github/license/thepigeongenerator/mcaselector-lite.svg)](https://github.com/thepigeongenerator/mcaselector-lite/blob/main/COPYING)
[![issues](https://img.shields.io/github/issues/thepigeongenerator/mcaselector-lite.svg)](https://github.com/thepigeongenerator/mcaselector-lite/issues/)

## What does it do
MCA-Selector-Lite is a tool used to edit [region files](https://minecraft.wiki/w/Region_file_format)
of your [Minecraft Java](https://minecraft.wiki/w/Java_Edition) worlds.
This is useful for reducing world size, general world management,
perhaps copying chunks from one world to another or various other administrative tasks.
Although not the intended main use case, it is also possible to use this tool to view the world map.
Though, it is recommended to use something like [dynmap](https://dynmap.wiki.gg/wiki/Home) instead (if applicable).

## Why does this project exist?
MCA-Selector is a tool written in Java. Which, at the time of writing depends upon JRE21 and JavaFX.
Where the application is primarily developed for Windows users, where other platforms get a `.jar` file.
I personally find this clunky to work with, and isn't too clean to install.
The goal of this project is to provide an alternative for MCA-Selector,
applying C to have a focus on performance and minimalism.
I chose C, since it is my preferred language to work in & aligns with how I personally view software.
I may decide to use another language, such as C++ for UI work, but the central functionality shall be C.
The project is still under active development, the main part that I plan to offer with the tool is
chunk deletion, world map viewing, and filtering. From there I may expand to offer more functionality
with MCA-Selector, or go in my own direction.

# Roadmap
- [ ] First, I shall focus on writing code to view
  chunks, creating a map of the blocks. The code shall
  produce a [ppm](https://en.wikipedia.org/wiki/Netpbm) file of a respective Y level.
  This is to verify whether the binary parsing works correctly. It is intended to support
  both `.mca` and `.mcr` files, and various levels of compression.
- [ ] This shall expand into a "mode" that filters the chunks based on player time,
  further testing the NBT parsing capabilities, and deletion of chunks.
- [ ] A GUI shall be built, which is still unclear how this'll be implemented.
  For starters, the GUI shall purely provide viewing capabilities, at which point the roadmap
  shall be updated for features such as chunk selection, et cetera.

# Miscellaneous planned features:
*note that these entries aren't guaranteed to be implemented.
Due to their viability and usefulness still needing to be measured.*
- Changing chunk compression
- View chunk metadata
- Change chunk sNBT
- "Upgrading" `.mcr` regions to `.mca`.
