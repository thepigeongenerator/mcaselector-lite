# dependencies:
#  - make
#  - clang
#  - rustc
#  - git bash (windows)
NAME    := mcaselector-lite
DEBUG   ?= 0
ARCH    ?= 0

# C compiler options
CC      := clang
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
DIR_BIN   := bin/$(ARCH)/$(PROF)
DIR_OBJ   := obj/$(ARCH)/$(PROF)
BIN       := $(DIR_BIN)/$(NAME)$(EXT)

C_SRC  := $(shell find src/ -name '*.c')
C_OBJ  := $(patsubst src/%,$(DIR_OBJ)/%,$(C_SRC:.c=.o))
C_DEP  := $(C_OBJ:.o=.d)
RS_SRC := $(shell find src/ -name '*.rs')
RS_LIB := $(RSOUT)/libmcaselector_lite.a
RS_DEP := $(RSOUT)/libmcaselector_lite.d
RSOUT  :=
endif

define log_col
	@printf "\033[%sm%s\033[0m\n" $(2) $(1)
endef
comp = $(call log_col,$(1),92)
mesg = $(call log_col,$(1),94)
warn = $(call log_col,$(1),93)
fail = $(call log_col,$(1),91)

# compiles and executes the produced binary
run: compile
	./$(BIN)
compile: compile_commands.json $(BIN)
clean:
	@$(call warn,"cleaning!")
	rm -rf bin/ obj/ target/ compile_commands.json
# TODO: write a structure for the unit tests in this

# create the binary (linking step)
$(BIN): $(C_OBJ) $(RS_LIB)
	@$(call mesg,"RUSTC: '$(RUSTC)'")
	@$(call mesg,"CC: '$(CC)'")
	@$(call mesg,"CFLAGS: '$(CFLAGS)'")
	@$(call mesg,"RSFLAGS: '$(RSFLAGS)'")
	@$(call mesg,"LDFLAGS: '$(LDFLAGS)'")
	@$(call comp,"linking to: '$@'")

	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) -o $@ $(C_OBJ) $(RS_LIB)
	@$(call warn,"current profile: '$(PROF)'")

# create .o and .d files for C sources
$(DIR_OBJ)/%.o: src/%.c
	@$(call comp,"compiling $(notdir $@) from $(notdir $<)")
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -MD -MP -std=$(CSTD) -x c -o $@ $<

# create .o and .d files for the entire rust codebase
$(RS_LIB): $(RS_SRC)
	$(RUSTC) $(RSFLAGS)

# update compile commands if the makefile has been updated (for linting)
ifneq ($(shell which bear),)
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make compile
else
compile_commands.json:
endif

# disable implicit rules
.SUFFIXES:

# include the dependencies
-include $(C_DEP)
-include $(RS_DEP)
