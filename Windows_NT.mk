# Copyright (C)2026 MCA-Selector-Lite
# Licensed under GPL-2.0-only. For further information,
# view `git log`, and the COPYING and CONTRIBUTORS files
# at www.github.com/thepigeongenerator/mcaselector-lite.
#
# This file is specifically for Windows_NT systems.
# It is included in Makefile if Windows_NT is detected.
$(warning Detected  Windows_NT, please refer to the documentation if you encounter issues.)

# BUG: I am purposefully neglecting this at the moment.

# Append .exe to the name to match the PE format.
NAME := $(NAME).exe
LDLIBS += -lopengl32 -lgdi32
CFLAGS += -D_CRT_NONSTDC_NO_WARNINGS
