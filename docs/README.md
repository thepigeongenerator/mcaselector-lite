> [!CAUTION]
> this application is under active development and it is not recommended to use yet.

# mca selector lite
A from-scratch rewrite of [mcaselector](https://github.com/Querz/mcaselector) in C.

[![licence](https://img.shields.io/github/license/thepigeongenerator/mcaselector-lite.svg)](https://github.com/thepigeongenerator/mcaselector-lite/blob/main/LICENSE)
[![issues](https://img.shields.io/github/issues/thepigeongenerator/mcaselector-lite.svg)](https://github.com/thepigeongenerator/mcaselector-lite/issues/)

[![CI](https://github.com/thepigeongenerator/mcaselector-lite/actions/workflows/ci.yaml/badge.svg)](https://github.com/thepigeongenerator/mcaselector-lite/actions/workflows/ci.yaml)
[![release](https://github.com/thepigeongenerator/mcaselector-lite/actions/workflows/release.yaml/badge.svg)](https://github.com/thepigeongenerator/mcaselector-lite/actions/workflows/release.yaml)

## what does it do
MCA selector lite is a tool used to edit [region files](https://minecraft.wiki/w/Region_file_format) of your [Minecraft java](https://minecraft.wiki/w/Java_Edition) worlds.
This is useful for reducing world size, general world management, perhaps copying chunks from one world to another or various other administrative tasks.
Although not the intended main use case, it is also possible to use this tool to view the world map. Though, it is recommended to use something like [dynmap](https://dynmap.wiki.gg/wiki/Home) instead (if applicable).

## why does this project exist?
MCA selector is a tool written in Java. Where it depends upon JRE21 and JavaFX. Where the application is primarily developed for Windows users, where other platforms get a `.jar` file, which can at times be clunky to work with.
The goal of this project is to create a version of the original MCA selector, but written in C, leveraging the improved performance due to native execution and more low-level control.
I have picked C as the language for the core portions of the application, where I prefer fine-grained control with little abstractions.
This version is not intended to serve to entirely replace MCA selector, just to offer an alternative. Both tools will have their strengths and weaknesses.
