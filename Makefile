# This file is part of MCA-Selector-lite,
# and is licensed under GPL-2.0-only.
# Copyright (C)2026 quinnthepigeon@proton.me Quinn
# For further information, view COPYING and CONTRIBUTORS
# at: www.github.com/thepigeongenerator/mcaselector-lite
SHELL = /bin/sh
.SUFFIXES:

VERSION = 0.0
NAME    = mcaselector-lite

XXD    ?= xxd
TAR    ?= tar
SPARSE ?= sparse

RES := $(wildcard res/*.glsl)
SRC := $(shell find src/ -name '*.c' -print) $(addsuffix .c,$(RES)) lib/glad/src/gl.c
OBJ := $(addsuffix .o,$(SRC))
DEP := $(addsuffix .d,$(SRC))

CFLAGS   := -O2 $(CFLAGS) -g -std=gnu99\
	    -Wall -Wextra -Wpedantic -Wno-pointer-arith
CPPFLAGS := -DNDEBUG $(CPPFLAGS) -DGLFW_INCLUDE_NONE\
	    -Iinclude -Ilib/glad/include
LDFLAGS  := -flto $(LDFLAGS)
LDLIBS   := $(LDLIBS) -lm

# Use pkg-config to locate dependencies, and set the correct flags.
ifeq (,$(shell command -v pkg-config))
$(error Failed to locate pkg-config, please make sure it is installed or acessible through PATH.)
else
CPPFLAGS += $(shell pkg-config --cflags-only-I glfw3 libarchive)
LDFLAGS  += $(shell pkg-config --libs-only-L   glfw3 libarchive)
LDLIBS   += $(shell pkg-config --libs-only-l   glfw3 libarchive)
endif

msg-cc    = $(info	[CC]	$(1))
msg-clean = $(info	[CLEAN]	$(1))
msg-ld    = $(info	[LD]	$(1))
msg-mkdir = $(info	[MKDIR]	$(1))
msg-tar   = $(info	[TAR]	$(1))
msg-xxd   = $(info	[XXD]	$(1))

# Set Q to @ to silence commands being printed, unless --no-silent has been set
ifeq (0, $(words $(findstring --no-silent,$(MAKEFLAGS))))
Q=@
endif

# detect if we're compiling on Windows, meaning
# a lot of things considered "standard" are unavailable.
ifeq ($(OS),Windows_NT)
NAME   := $(NAME).exe
# WARN: Is this still valid?
LDLIBS += -lopengl32 -lgdi32
$(warning Detected  Windows_NT, please refer to the documentation if you encounter issues.)
endif

# Default target; compiles everything.
.PHONY:
all: bin/$(NAME) bin/stripped_$(NAME)

# Install a binary on a POSIX-compliant system.
.PHONY:
install: bin/mcaselector-lite
	install -m0755 bin/$(NAME) $(DESTDIR)/bin/$(NAME)

# Install a stripped binary on a POSIX-compliant system
.PHONY:
install-strip: bin/mcaselector-lite.stripped
	install -m0755 bin/stripped_$(NAME) $(DESTDIR)/bin/$(NAME)

.PHONY:
uninstall:
	$(RM) $(DESTDIR)/bin/$(NAME)

# Executes checks upon the code.
# The standard requires code to already be built at this stage.
# This isn't necessary for this, but may be added in the future,
# if so add the binary in the prerequisites.
.PHONY:
check: $(SRC)
	$(Q)$(SPARSE) $(CPPFLAGS) $<

.PHONY:
clean:
	-$(Q)$(RM) $(OBJ) $(DEP)
	-$(Q)$(RM) -r bin/

# Links together the object files into the final binary.
bin/$(NAME): $(OBJ) | bin/
	$(Q)$(call msg-ld,$@)
	$(Q)$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^
bin/stripped_$(NAME): $(OBJ) | bin/
	$(Q)$(call msg-ld,$@)
	$(Q)$(CC) -s $(LDFLAGS) $(LDLIBS) -o $@ $^

# Compiles C sources into object files
%.c.o: %.c
	$(Q)$(call msg-cc,$@)
	$(Q)$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

# Embed files in /res as plain binary into the final binary.
# TODO: Find some fix for this clogging up the workspace
.INTERMEDIATE:
res/%.c: res/%
	$(Q)$(call msg-xxd,$@)
	$(Q)cd $(<D) && $(XXD) -i $(patsubst res/%,%,$<) $(abspath $@)

%/:
	$(Q)$(call msg-mkdir,$@)
	$(Q)mkdir $@

# Generate and include dependencies,
# ignoring any errors that may occur when doing so.
%.c.d: %.c; $(Q)$(CC) -MM $(CPPFLAGS) -MF $@ $<
ifeq (0, $(words $(findstring $(MAKECMDGOALS), clean)))
-include $(DEP)
endif
