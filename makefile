# dependencies:
#  - make
#  - clang
#  - glfw3 (install glfw3:x64-mingw-dynamic via vcpkg for win cross compilation)
#  - vcpkg (win cross-compilation)
NAME    := mcaselector-lite
DEBUG   ?= 0
ARCH    ?= 0

# C compiler options
CC      := clang
CSTD    := c17
CFLAGS  := -Wall -Wextra -Wpedantic -Wno-pointer-arith -static
LDFLAGS :=

ifeq ($(DEBUG),1)
CFLAGS  += -g -Og
PROF    := dbg
else
CFLAGS  += -DNDEBUG -O2 -Werror
PROF    := rel
endif

ifneq ($(MAKECMDGOALS),clean)
ifeq      ($(ARCH),linux-x86_64)
CFLAGS  += -target x86_64-pc-linux-gnu $(shell pkg-config --cflags glfw3)
LDFLAGS += -target x86_64-pc-linux-gnu $(shell pkg-config --libs glfw3)
else ifeq ($(ARCH),win-x86_64)
CFLAGS  += -target x86_64-pc-windows-gnu -I$(VCPKG_ROOT)/installed/x64-mingw-dynamic/include
LDFLAGS += -target x86_64-pc-windows-gnu -L$(VCPKG_ROOT)/installed/x64-mingw-dynamic/lib -lglfw3dll -fuse-ld=lld
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
	rm -rf bin/ obj/ compile_commands.json
# TODO: write a structure for the unit tests in this

# create the binary (linking step)
$(BIN): $(C_OBJ)
	@$(call mesg,"CC: '$(CC)'")
	@$(call mesg,"CFLAGS: '$(CFLAGS)'")
	@$(call mesg,"LDFLAGS: '$(LDFLAGS)'")
	@$(call comp,"linking to: '$@'")

	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) -o $@ $(C_OBJ)
	@$(call mesg,"current profile: '$(PROF)'")

# create .o and .d files for C sources
$(DIR_OBJ)/%.o: src/%.c
	@$(call comp,"compiling $(notdir $@) from $(notdir $<)")
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -MD -MP -std=$(CSTD) -x c -o $@ $<

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
