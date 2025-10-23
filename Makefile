# Copyright (c) 2025 Quinn
# Licensed under the MIT Licence. See LICENSE for details
SHELL = /bin/sh
.SUFFIXES:

NAME    = mcaselector-lite

CC     ?= cc
RM     ?= rm -vf
CMAKE  ?= cmake -G 'Unix Makefiles'

CPPFLAGS ?= -DNDEBUG
CFLAGS   ?= -O2
LDFLAGS  ?= -flto

CPPFLAGS += -DGLFW_INCLUDE_NONE
CPPFLAGS += -Iinclude -Ilib/glad/include -Ilib/glfw/include -Ilib/libarchive/libarchive
LDFLAGS  += -Llib/obj/glfw/src -Llib/obj/libarchive/libarchive
LDLIBS   += -lglfw3 -larchive -lm
CFLAGS   += -std=gnu99 -g -MMD -MP
CFLAGS   += -Wall -Wextra -Wpedantic -Wno-pointer-arith

# detect if we're compiling on Windows, meaning
# a lot of things considered "standard" are unavailable.
ifeq ($(OS),Windows_NT)
NAME   := $(NAME).exe
LDLIBS += -lopengl32 -lgdi32
$(warning Detected  Windows_NT, please refer to the documentation if you encounter issues.)
# in the case of Mac OS X
else ifeq ($(shell uname -s),Darwin)
LDLIBS += -framework Coca -framework OpenGL -framework IOKit
$(info Mac OS X detected.)
endif

# find all the source files using wildcards
# TODO: find a better method to find all source files
# NOTE: MS-DOS and MS-Windows uses backslash `\`, this might break.
RES  := $(wildcard res/*)
SRC  := $(wildcard src/*.c src/*/*.c src/*/*/*.c src/*/*/*/*.c src/*/*/*/*/*.c src/*/*/*/*/*/*.c src/*/*/*/*/*/*/*.c src/*/*/*/*/*/*/*/*.c)
SRC  += lib/glad/src/gl.c
TSRC := $(wildcard test/*.c test/*/*.c test/*/*/*.c test/*/*/*/*.c test/*/*/*/*/*.c test/*/*/*/*/*/*.c test/*/*/*/*/*/*/*.c test/*/*/*/*/*/*/*/*.c)

OBJ   := $(RES:%=obj/%.o) $(SRC:%.c=obj/%.o)
TOBJ  := $(TSRC:%.c=obj/%.o)

# TODO: potentially automatically detect whether we should compile libs, or if we can just go ahead.

.PHONY: all libs check clean clean-libs
all: bin/$(NAME)
libs: lib/obj/glfw/ lib/obj/libarchive/
check: bin/TEST_$(NAME); ./$<
clean:;      @-$(RM) -r bin/ obj/
clean-libs:; @-$(RM) -r lib/obj/

.PHONY:
install: all
ifneq ($(OS),Windows_NT)
	# TODO: POSIX-compliant installation
else
	# TODO: WINDOWS_NT installation
endif

.PHONY:
install-strip: install
	# TODO: strip the produced installation

# compiles the libraries using cmake
lib/obj/%/: lib/%/
	$(CMAKE) -S $< -B $@
	$(MAKE) -C $@

# link together a runtime binary
bin/$(NAME): $(OBJ)
	$(info [LD]	$@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# link together a testing binary
bin/TEST_$(NAME): $(TOBJ) $(filter-out obj/src/main.o,$(OBJ))
	$(info [LD]	$@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

obj/res/%.c: res/%
	$(info [XXD]	$@)
	@mkdir -p $(@D)
	@cd res/ && xxd -i $(patsubst res/%,%,$<) $(abspath $@)

obj/%.o: %.c
	$(info [CC]	$@)
	@mkdir -p $(@D)
	@$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

obj/%.o: obj/%.c
	$(info [CC]	$@)
	@mkdir -p $(@D)
	@$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

# Include the generated dependency files.
# Which creates rules for all dependencies,
# as a result updating an .o file when a .h is updated.
-include $(OBJ:%.o=%.d)
-include $(TOBJ:%.o=%.d)
