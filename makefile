# dependencies:
#  - make
#  - clang
#  - rustc
#  - bear (debug)
#  - git bash (windows)
NAME  := mcaselector-lite
VER   := 0.0.1

RUSTC   := rustc

# C compiler options
CC      := clang
CLANG   := c
CSTD    := c17
CFLAGS  := -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS :=
DEBUG   ?= 0
ARCH    ?= 0

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -g -Og
PROF   := dbg
else
CFLAGS += -O2
PROF   := rel
endif

ifneq ($(MAKECMDGOALS),clean)
ifeq      ($(ARCH),linux-x86_64)
CFLAGS += -target x86_64-pc-linux-gnu
else ifeq ($(ARCH),win-x86_64)
CFLAGS += -target x86_64-pc-windows-gnu
EXT    := .exe
else
$(error you must set the ARCH environment variable to one of these: 'linux-x86_64' 'win-x86_64')
endif
endif

ifneq ($(ARCH),0)
C_SRC  := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
C_OBJ  := $(patsubst src/%,obj/$(ARCH)/$(PROF)/%,$(C_SRC:.c=.o))
C_DEP  := $(C_OBJ:.o=.d)
RS_SRC := $(wildcard src/*.rs) $(wildcard src/**/*.rs) $(wildcard src/**/**/*.rs) $(wildcard src/**/**/**/*.rs) $(wildcard src/**/**/**/**/*.rs)
RS_OBJ := $(patsubst src/%,obj/$(ARCH)/$(PROF)/%,$(RS_SRC:.rs=.o))

DIR       := bin/$(ARCH)/$(PROF) $(sort obj/$(ARCH)/$(PROF) $(dir $(C_SRC))  $(dir $(RS_SRC)))
DIR_BUILD := bin/$(ARCH)/$(PROF)
TARGET    := $(DIR_BUILD)/$(NAME)$(EXT)
endif

define wr_colour
	@echo -e "\033[$(2)m$(1)\033[0m"
endef

# compiles and executes the produced binary
run: compile
	./$(TARGET)
compile: compile_commands.json $(DIR) $(TARGET)
clean:
	rm -rf bin/ obj/ compile_commands.json

# create the binary (linking step)
$(TARGET): $(C_OBJ) $(RS_OBJ)
	@$(call wr_colour,"RUSTC: '$(RUSTC)'",94)
	@$(call wr_colour,"CC: '$(CC)'",94)
	@$(call wr_colour,"CFLAGS: '$(CFLAGS)'",94)
	@$(call wr_colour,"LDFLAGS: '$(LDFLAGS)'",94)
	@$(call wr_colour,"linking to: '$@'",92)

	@$(CC) $(LDFLAGS) $(C_OBJ) $(RS_OBJ) -o $(TARGET)
	@$(call wr_colour,"current profile: '$(PROF)'",93)

# create .o and .d files for C sources
$(C_OBJ): $(C_SRC)
	$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	@$(CC) $(CFLAGS) -c -MD -MP -std=$(CSTD) -x $(CLANG) -o $@ $<

# create .o files for RUST sources
$(RS_OBJ): $(RS_SRC)
	$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	$(RUSTC) --crate-type=staticlib --emit=obj -C debuginfo=2 -C panic=abort -o $@ $<

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
