# Copyright (C)2026 mcxedit
# Licensed under GPL-2.0-only. For further information,
# view `git log`, and the COPYING and CONTRIBUTORS files
# at www.github.com/thepigeongenerator/mcxedit.

VERONLY  = v0.0
ifndef GIT_TAG
GIT_TAG := $(shell git describe --tags --match=${VERONLY} 2>/dev/null)
endif
VERSION := $(if ${GIT_TAG},${GIT_TAG},${VERONLY})
export VERSION GIT_TAG


# Configure the prefix directories for installation rules.
prefix = /usr/local
bindir = ${prefix}/bin
libdir = ${prefix}/lib
mandir = ${prefix}/share

# srcdir shall be set by the first make, and no subsequent makes.
export srcdir ?= ${CURDIR}

# Include persistent user configurations.
-include ${srcdir}/.config.mk

# Flags, including the flag in the definition so it may be overridden.
# Generally speaking, anything prior to the recursive reference of the variable
# is able to be overridden, anything afterwards is applied no matter what.
CPPFLAGS := -DNDEBUG -U_GNU_SOURCE ${CPPFLAGS}\
	    -DMCXEDIT_VERSION=\"${VERSION}\" -DMCXEDIT_SOURCE\
	    -I${srcdir}/include
CFLAGS   := -O2 ${CFLAGS} -g -std=gnu17\
	    -Wall -Wextra -Wpedantic -Wno-pointer-arith -Wvla
LDFLAGS  := ${LDFLAGS} -L${srcdir}

# Rules so simple that it is fine to put them here.
# Not putting stuff such as C compilation here, since that should
# be handled by the module itself.
%.gz: %
	$(call msg,GZIP,$@)
	${Q}gzip -fk $<
%/:
	$(call msg,MKDIR,$@)
	${Q}mkdir -p $@

# Set Q to @ to silence commands being printed, unless --no-silent has been set
ifeq (0, $(words $(findstring --no-silent,${MAKEFLAGS})))
msg=@printf ' %-8s %s\n' "${1}" "${2}"
Q=@
else
msg=
Q=
endif

# Generate and include dependencies,
# ignoring any errors that may occur when doing so.
%.c.d: %.c; ${Q}${CC} -MM ${CPPFLAGS} -MF $@ $<
ifeq (0, $(words $(findstring ${MAKECMDGOALS}, clean)))
-include ${DEP}
endif
