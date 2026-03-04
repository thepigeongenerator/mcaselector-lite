> [!CAUTION]
> This application is __not__ ready for usage, and shouldn't be used.
> It is under active development and most features are considered "béta",
> Once a pre-release, or release has been created in [the release tab](https://github.com/thepigeongenerator/mcxedit/releases),
> it is ready for general usage and testing.

[![licence](https://img.shields.io/github/license/thepigeongenerator/mcxedit.svg)](https://github.com/thepigeongenerator/mcxedit/blob/main/COPYING)
[![issues](https://img.shields.io/github/issues/thepigeongenerator/mcxedit.svg)](https://github.com/thepigeongenerator/mcxedit/issues/)

## What does it do
MCX Edit is a tool used to edit [region files](https://minecraft.wiki/w/Region_file_format)
of your [Minecraft Java](https://minecraft.wiki/w/Java_Edition) worlds.
This is useful for reducing world size, general world management,
perhaps copying chunks from one world to another or various other administrative tasks.
Although not the intended main use case, it is also possible to use this tool to view the world map.
Though, it is recommended to use something like [dynmap](https://dynmap.wiki.gg/wiki/Home) instead (if applicable).

# Planned Features
* * [ ] Generating a map of the blocks,
  producing a [ppm](https://en.wikipedia.org/wiki/Netpbm) file of a respective Y level.
* Support both `.mca` and `.mcr` files, and various levels of compression.
* * [ ] Filtering chunks based on player time.
* * [X] Pruning unused sectors, such as in-between entries and towards the end.
* * [ ] Region file repairs, as far as possible that is.
* * [ ] Converting `.mcr` to `.mca`.
* * [ ] Changing chunk compression.
* * [ ] sNBT based chunk filtering.
* * [ ] View/change chunk sNBT.
* * [ ] CLI / Terminal - based input.
* * [ ] GUI / Graphical - based input.
> [!NOTE]
> These entries are mostly ideas, and may not be
> realistic to implement, or even reflect what direction
> I end up taking the project.

# Similar software
* [MCA-Selector](https://github.com/Querz/mcaselector) (inspiration of this project)
