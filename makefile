# Copyright (c) 2025 Quinn
# Licensed under the MIT Licence. See LICENSE for details
SHELL = bash
.SHELLFLAGS = -O globstar -c

# build configuration, information about the current build process
NAME    = mcaselector-lite
VERSION = 0.0.0
DEBUG   ?= 0
CC      ?= cc
LD      ?= ld
CFLAGS  += -c -std=gnu99 -Wall -MMD -MP
CFLAGS  += -Ilib/glad/include
LDFLAGS += -flto -lm
MARCH   ?= $(shell uname -m)
KERNEL  ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')

# compilation flags
CFLAGS  = -c -std=gnu99 -Wall -Wextra -Wpedantic -MMD -MP
LDFLAGS = -flto

# architecture/OS detection
ifeq ($(KERNEL),)
ISWIN := $(if $(filter $(OS),Windows_NT),1,0)
ifeq ($(ISWIN),1)
KERNEL = mingw
MARCH  = x86_64
else
MARCH  := $(shell uname -m)
KERNEL := $(shell uname -s | tr '[:upper:]' '[:lower:]')
endif
else
ISWIN := $(if $(filter $(KERNEL),mingw),1,0)
endif
ifeq ($(MARCH),)
$(error must also set MARCH when manually setting KERNEL)
endif

# profiles
ifeq ($(DEBUG),1)
PROF = dbg
CFLAGS  += -UNDEBUG -Og -g -Wextra -Wpedantic
CFLAGS  += $(if $(filter 1,$(ISWIN)),,-fsanitize=address,undefined)
LDFLAGS += $(if $(filter 1,$(ISWIN)),,-fsanitize=address,undefined)
# |--profile: testing
else ifeq ($(DEBUG),test)
PROF = test
CFLAGS  += -UNDEBUG -O2 -g
CFLAGS  += $(if $(filter 1,$(ISWIN)),,-fsanitize=address)
LDFLAGS += $(if $(filter 1,$(ISWIN)),,-fsanitize=address)
else
PROF = rel
CFLAGS += -DNDEBUG -O2
endif

CFLAGS  += $(shell pkg-config --cflags glfw3 libarchive) -Ilib/glad/include
LDFLAGS += $(shell pkg-config --libs glfw3 libarchive) -lm

# get source files
SRC := $(shell echo src/**/*.c) lib/glad/src/gl.c
RES := $(wildcard res/*.glsl)
ifeq ($(DEBUG),test)
SRC := $(filter-out src/main.c, $(SRC)) $(shell echo test/**/*.c)
endif

NAME += $(if $(filter 1,$(ISWIN)),.exe,)
DIR_BIN := bin/$(MARCH)-$(KERNEL)/$(VERSION)/$(PROF)
DIR_OBJ := obj/$(MARCH)-$(KERNEL)/$(VERSION)/$(PROF)

# output files
BIN := $(DIR_BIN)/$(NAME)
OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o) $(RES:%=$(DIR_OBJ)/%.o)
DEP := $(OBJ:%.o=%.d)

.PHONY:
run: compile
	$(if $(filter 1,$(ISWIN)),wine,) $(BIN)

.PHONY:
compile: $(BIN)

.PHONY .NOTPARALLEL:
clean:
	@rm -rv obj/
	@rm -rv bin/

$(BIN): $(OBJ)
	$(info [CC/LD] $@)
	@mkdir -p $(@D)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(DIR_OBJ)/%.o: %.c
	$(info [CC] $@)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ $<

$(DIR_OBJ)/res/%.o: res/%
	$(info [LD] $@)
	@mkdir -p $(@D)
	@$(LD) -r -b binary -o $@ $<

# some definitions for "default" and assumed compilers, for bulk selection
.PHONY: x86_64-linux-gnu-gcc x86_64-w64-mingw32-gcc
x86_64-linux-gnu-gcc:;   $(MAKE) $(CALL) $(MAKEFLAGS) CC=$@ MARCH=x86_64 KERNEL=linux
x86_64-w64-mingw32-gcc:; $(MAKE) $(CALL) $(MAKEFLAGS) CC=$@ MARCH=x86_64 KERNEL=mingw

-include $(DEP)
