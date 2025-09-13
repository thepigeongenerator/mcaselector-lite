# Copyright (c) 2025 Quinn
# Licensed under the MIT Licence. See LICENSE for details

# build configuration, information about the current build process
NAME    = mcaselector-lite
DEBUG   ?= 0
CC      ?= cc
LD      ?= ld

# setting default compilation flags
# some of which are able to be overwritten, others are always appended
CPPFLAGS ?=
CFLAGS   ?= -O2 -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS  ?= -flto
CFLAGS   += -std=gnu99

# add a few extra flags depending on whether
# we're debugging or not
ifeq ($(DEBUG),0)
CPPFLAGS += -DNDEBUG
else
CFLAGS   += -fsanitize=address -ftrapv -g
LDFLAGS  += -fsanitize=address -ftrapv
endif
CPPFLAGS += -Iinclude -Ilib/glad/include -Ilib/glfw/include -Ilib/libarchive/libarchive
LDFLAGS  += -Llib/obj/glfw/src -Llib/obj/libarchive/libarchive
LDLIBS   += -lm -lglfw -larchive

# detect if we're compiling on Windows, meaning
# a lot of things considered "standard" are unavailable.
ifeq ($(OS),Windows_NT)
ISWIN = 1
NAME  := $(NAME).exe
$(warning Detected Windows_NT, please refer to the documentation if you encounter issues.)
endif

# TODO: find a better method to find all source files
# find all the source files using wildcards
# NOTE: MS-DOS and MS-Windows uses backslash `\`, this might break.
RES  := $(wildcard res/*)
SRC  := $(wildcard src/*.c src/*/*.c src/*/*/*.c src/*/*/*/*.c src/*/*/*/*/*.c src/*/*/*/*/*/*.c src/*/*/*/*/*/*/*.c src/*/*/*/*/*/*/*/*.c) lib/glad/src/gl.c
OBJ  := $(SRC:%.c=obj/%.o) $(RES:%=obj/%.o)
TSRC := $(wildcard test/*.c test/*/*.c test/*/*/*.c test/*/*/*/*.c test/*/*/*/*/*.c test/*/*/*/*/*/*.c test/*/*/*/*/*/*/*.c test/*/*/*/*/*/*/*/*.c)
TOBJ := $(TSRC:%.c=obj/%.o)

# TODO: potentially automatically detect whether we should compile libs, or if we can just go ahead.
.PHONY: all libs test clean clean-libs
all: bin/$(NAME)
libs: lib/obj/glfw/ lib/obj/libarchive/
test: bin/TEST_$(NAME); bin/TEST_$(NAME)
clean:
	@[ -d bin/ ] && rm -vr bin/ || true
	@[ -d obj/ ] && rm -vr obj/ || true
clean-libs:
	@[ -d lib/obj/ ] && rm -vr lib/obj/


# compiles the libraries using cmake
lib/obj/%/: lib/%/
	cmake -S $< -B $@
	$(MAKE) -C $@

# link together a runtime binary
bin/$(NAME): $(OBJ)
	$(info [CC/LD]	$@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# link together a testing binary
bin/TEST_$(NAME): $(TOBJ) $(filter-out main.o,$(OBJ))
	$(info [CC/LD]	$@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

obj/res/%.o: res/%
	$(info [LD]	$@)
	@$(LD) -r -b binary -o $@ $<

obj/%.o: %.c
	$(info [CC]	$@)
	@mkdir -p $(@D)
	@$(CC) -c $(CPPFLAGS) $(CFLAGS) -MMD -MP -o $@ $<

# Include the generated dependency files.
# Which creates rules for all dependencies,
# as a result updating an .o file when a .h is updated.
-include $(OBJ:%.o=%.d)
