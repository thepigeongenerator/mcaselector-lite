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
CFLAGS  := -Wall -Wextra -Wpedantic -Wno-pointer-arith -Ilib
LDFLAGS :=

# profiles
ifneq ($(DEBUG),0)
CFLAGS  += -g -Og -fsanitize=address,undefined
LDFLAGS += -fsanitize=address,undefined
PROF    := dbg
else
CFLAGS  += -DNDEBUG -O2 -Werror
PROF    := rel
endif

# targets
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

# output files
ifneq ($(ARCH),0)
DIR_BIN   := bin/$(ARCH)/$(PROF)
DIR_OBJ   := obj/$(ARCH)/$(PROF)
BIN       := $(DIR_BIN)/$(NAME)$(EXT)

C_SRC  := $(shell find src/ -name '*.c')
C_OBJ  := $(patsubst src/%,$(DIR_OBJ)/%,$(C_SRC:.c=.o))
C_DEP  := $(C_OBJ:.o=.d)

COMPILE_COMMANDS := $(DIR_OBJ)/compile_commands.json
endif

define log_col
	@printf "\033[%sm%s\033[0m\n" $(2) $(1)
endef
comp = $(call log_col,$(1),92)
mesg = $(call log_col,$(1),94)
warn = $(call log_col,$(1),93)
fail = $(call log_col,$(1),91)

# creates the binary (linking step)
define link_bin
$1: $2
	@$$(call mesg,"CC: '$$(CC)'")
	@$$(call mesg,"CFLAGS: '$$(CFLAGS)'")
	@$$(call mesg,"LDFLAGS: '$$(LDFLAGS)'")
	@$$(call comp,"linking to: '$$@'")

	@mkdir -p $$(@D)
	@$$(CC) $$(LDFLAGS) -o $$@ $$^
	@$$(call mesg,"current profile: '$$(PROF)'")

endef

# creates .o and .d files
define compile_obj
$1/%.o: $2/%.c
	@$$(call comp,"compiling $$@	from $$<")
	@mkdir -p $$(@D)
	@$$(CC) $$(CFLAGS) -c -MD -MP -std=$$(CSTD) -x c -o $$@ $$<
endef

# compiles and executes the produced binary
run: compile; cd $(DIR_BIN) && ./$(NAME)$(EXT)
compile: compile_commands $(BIN)

.NOTPARALLEL:
clean:
	@$(call warn,"cleaning!")
	rm -rf bin/ obj/ compile_commands.json
# TODO: write a structure for the unit tests in this

# compilation macros
$(eval $(call link_bin,$(BIN),$(C_OBJ)))                       # link the binary
$(eval $(call compile_obj,$(DIR_OBJ),src))                     # compile the objects for the binary

# update compile commands if the makefile has been updated (for linting)
compile_commands: # default, empty rule
ifneq ($(shell which bear),)
ifneq ($(COMPILE_COMMANDS),)
ifeq ($(NOCMDS),)
.NOTPARALLEL .PHONY:
compile_commands: $(COMPILE_COMMANDS)
	@[ "$(readlink compile_commands.json)" != "$<" ] && ln -sf $< compile_commands.json

.NOTPARALLEL:
$(COMPILE_COMMANDS): makefile
	@$(call warn,"regenerating compile_commands.json thus recompiling.")
	@mkdir -p ${@D} # ensure the target directory exists
	@touch $@       # create the file so it isn't retriggered (will just change modification time if already exists)
	@bear --output $@ -- make -B compile NOCMDS=1 # rebuild the current target using bear, to create the compile commands
endif
endif
endif

# disable implicit rules
.SUFFIXES:

# include the dependencies
-include $(C_DEP)
