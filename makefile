# dependencies:
#  - make
#  - C compiler
#  - glfw3 (install glfw3:x64-mingw-dynamic via vcpkg for cross compilation)
#  - xxd (tinyxxd recommended; faster)
#  - (windows) git bash (recommended)

# build configuration, information about the current build process
NAME    := mcaselector-lite
VERSION := 0.0.0
DEBUG   ?= 0
CC      ?= cc
LD      ?= ld
CFLAGS  += -c -std=gnu99 -Wall -Wextra -Wpedantic -Ilib -MMD -MP
LDFLAGS += -lm
MARCH   ?= $(shell uname -m)
KERNEL  ?= $(shell uname -s | tr '[:upper:]' '[:lower:]')

# check whether KERNEL is something nonsensical
ISWIN := $(if $(filter linux darwin freebsd netbsd openbsd,$(KERNEL)),0,1)

# profiles
ifeq ($(DEBUG),1)         # check whether we're debugging
CFLAGS  += -Og -g
PROF    := dbg
ifeq ($(ISWIN),0)
CFLAGS  += -fsanitize=address,undefined
LDFLAGS += -fsanitize=address,undefined
endif
else ifeq ($(DEBUG),test) # check whether we're perhaps testing
CFLAGS  += -O2 -g
PROF    := test
ifeq ($(ISWIN),0)
CFLAGS  += -fsanitize=address
LDFLAGS += -fsanitize=address
endif
else                      # otherwise, assume release
CFLAGS  += -O2 -DNDEBUG
PROF    := rel
endif

# setup the VCPKG_TRIPLET
# because microsoft thinks they should use a different method of classifying stuff than the standard
ifneq ($(VCPKG_ROOT),)
VCPKG_TRIPLET ?= $(strip                        \
	$(if $(filter x86_64,$(MARCH)),x64,           \
))-$(strip                                      \
	$(if $(filter linux,$(KERNEL)),linux-dynamic, \
	$(if $(filter darwin,$(KERNEL)),osx-dynamic,  \
	mingw-static))                                \
)

# override the pkg config path, so it is used instead of system packages
export PKG_CONFIG_PATH := $(VCPKG_ROOT)/installed/$(VCPKG_TRIPLET)/lib/pkgconfig
$(info using PKG_CONFIG_PATH: '$(PKG_CONFIG_PATH)')
else ifneq ($(shell which pkg-config),)
$(warning couldn't find VCPKG_ROOT, attempting to use system packages using pkg-config!)
else
$(error neither VCPKG_ROOT nor pkg-config were available!)
endif

# use pkg-config to set the include and linker information
CFLAGS  += $(shell pkg-config --cflags glfw3)
LDFLAGS += $(shell pkg-config --libs   glfw3)

# windows specific handling
ifeq ($(ISWIN),1)
NAME    := $(NAME).exe
endif

# build directory structure
DIR_BIN := bin/$(MARCH)-$(KERNEL)/$(VERSION)/$(PROF)
DIR_OBJ := obj/$(MARCH)-$(KERNEL)/$(VERSION)/$(PROF)

# get source files
ifneq ($(DEBUG),test)
SRC := $(shell find src/ -name '*.c')
else
SRC    := $(filter-out src/main.c, $(shell find test/ src/ -name '*.c'))
endif
RES   := $(shell find res/ -type f)

# output files
BIN := $(DIR_BIN)/$(NAME)
OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o) $(RES:%=$(DIR_OBJ)/%.o)
DEP := $(OBJ:.o=.d)

COMPILE_COMMANDS := $(DIR_OBJ)/compile_commands.json

.PHONY: run compile echo
run:     echo compile_commands $(BIN)
ifeq ($(ISWIN),0)
	$(BIN)
else
	wine $(BIN)
endif
compile: echo compile_commands $(BIN)
echo:
	$(info $(shell printf "\033[36m")compiling for: $(MARCH), $(KERNEL)$(shell printf "\033[0m"))
	$(info $(shell printf "\033[36m")using compiler: $(CC)$(shell printf "\033[0m"))

# some definitions for "default" and assumed compilers, for bulk selection
.PHONY: all x86_64-linux-gnu-gcc x86_64-w64-mingw32-gcc
all: x86_64-linux-gnu-gcc x86_64-w64-mingw32-gcc
x86_64-linux-gnu-gcc:;   $(MAKE) $(CALL) $(MAKEFLAGS) CC=$@ MARCH=x86_64 KERNEL=linux NOCMDS=1
x86_64-w64-mingw32-gcc:; $(MAKE) $(CALL) $(MAKEFLAGS) CC=$@ MARCH=x86_64 KERNEL=mingw NOCMDS=1

# creates the binary (linking step)
$(BIN): $(OBJ)
	@mkdir -p $(@D)
	$(CC) -o $@ $^ $(LDFLAGS)

# compilation rule
$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<

# use linker to embed the resources into the final binary
$(DIR_OBJ)/res/%.o: res/%
	@mkdir -p $(@D)
	$(LD) -r -b binary -o $@ $<

.PHONY .NOTPARALLEL:
clean:
	rm -rf obj bin compile_commands.json

# update compile commands if the makefile has been updated (for linting)
compile_commands: # default, empty rule
ifneq ($(shell which bear),)
ifneq ($(COMPILE_COMMANDS),)
ifeq ($(NOCMDS),)
.PHONY .NOTPARALLEL:
compile_commands: $(COMPILE_COMMANDS)
	ln -sf $< compile_commands.json

.NOTPARALLEL:
$(COMPILE_COMMANDS): makefile
	@$(warning regenerating compile_commands.json thus recompiling.)
	@mkdir -p ${@D} # ensure the target directory exists
	@touch $@       # create the file so it isn't retriggered (will just change modification time if already exists)
	@bear --output $@ -- make -B compile NOCMDS=1 # rebuild the current target using bear, to create the compile commands
endif
endif
endif

-include $(DEP)
