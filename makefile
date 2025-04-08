# dependencies:
#  - make
#  - clang
#  - rustc
#  - bear (debug)
#  - git bash (windows)
NAME    := mcaselector-lite
DEBUG   ?= 0
ARCH    ?= 0

# C compiler options
CC      := clang
CLANG   := c
CSTD    := c17
CFLAGS  := -Wall -Wextra -Wpedantic -Wno-pointer-arith -static
LDFLAGS :=

# Rust compiler options
RUSTC   := cargo rustc
RSFLAGS :=

ifeq ($(DEBUG),1)
CFLAGS  += -DDEBUG -g -Og
RSOUT   := debug
PROF    := dbg
else
CFLAGS  += -O2 -Werror
RSOUT   := release
RSFLAGS := --release
PROF    := rel
endif

ifneq ($(MAKECMDGOALS),clean)
ifeq      ($(ARCH),linux-x86_64)
CFLAGS  += -target x86_64-pc-linux-gnu
LDFLAGS += -target x86_64-pc-linux-gnu
RSFLAGS += --target=x86_64-unknown-linux-gnu
RSOUT   := target/x86_64-unknown-linux-gnu/$(RSOUT)
else ifeq ($(ARCH),win-x86_64)
CFLAGS  += -target x86_64-pc-windows-gnu
LDFLAGS += -target x86_64-pc-windows-gnu
RSFLAGS += --target=x86_64-pc-windows-gnu
RSOUT   := target/x86_64-pc-windows-gnu/$(RSOUT)
EXT     := .exe
else
$(error you must set the ARCH environment variable to one of these: 'linux-x86_64' 'win-x86_64')
endif
endif

ifneq ($(ARCH),0)
C_SRC  := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
C_OBJ  := $(patsubst src/%,obj/$(ARCH)/$(PROF)/%,$(C_SRC:.c=.o))
C_DEP  := $(C_OBJ:.o=.d)
RS_SRC := $(wildcard src/*.rs) $(wildcard src/**/*.rs) $(wildcard src/**/**/*.rs) $(wildcard src/**/**/**/*.rs) $(wildcard src/**/**/**/**/*.rs)
RS_LIB := $(RSOUT)/libmcaselector_lite.a
RS_DEP := $(RSOUT)/libmcaselector_lite.d
RSOUT  :=

DIR       := bin/$(ARCH)/$(PROF) $(sort obj/$(ARCH)/$(PROF) $(dir $(C_SRC))  $(dir $(RS_SRC)))
DIR_BUILD := bin/$(ARCH)/$(PROF)
TARGET    := $(DIR_BUILD)/$(NAME)$(EXT)
endif

define wr_colour
	@printf "\033[%sm%s\033[0m\n" $(2) $(1)
endef

# compiles and executes the produced binary
run: compile
	./$(TARGET)
compile: compile_commands.json $(DIR) $(TARGET)
clean:
	rm -rf bin/ obj/ target/ compile_commands.json
# TODO: write a structure for the unit tests in this

# create the binary (linking step)
$(TARGET): $(C_OBJ) $(RS_LIB)
	@$(call wr_colour,"RUSTC: '$(RUSTC)'",94)
	@$(call wr_colour,"CC: '$(CC)'",94)
	@$(call wr_colour,"CFLAGS: '$(CFLAGS)'",94)
	@$(call wr_colour,"RSFLAGS: '$(RSFLAGS)'",94)
	@$(call wr_colour,"LDFLAGS: '$(LDFLAGS)'",94)
	@$(call wr_colour,"linking to: '$@'",92)

	@$(CC) $(LDFLAGS) -o $@ $(C_OBJ) $(RS_LIB)
	@$(call wr_colour,"current profile: '$(PROF)'",93)

# create .o and .d files for C sources
$(C_OBJ): $(C_SRC)
	@$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	@$(CC) $(CFLAGS) -c -MD -MP -std=$(CSTD) -x $(CLANG) -o $@ $<

# create .o and .d files for RUST sources
$(RS_LIB): $(RS_SRC)
	$(RUSTC) $(RSFLAGS)

# create directories
$(DIR):
	mkdir -p $@

# update compile commands if the makefile has been updated (for linting)
ifeq ($(DEBUG),1)
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make compile
else
compile_commands.json:
endif

# include the dependencies
-include $(C_DEP)
-include $(RS_DEP)
