# Copyright (C)2026 mcxedit
# Licensed under GPL-2.0-only. For further information,
# view `git log`, and the COPYING and CONTRIBUTORS files
# at www.github.com/thepigeongenerator/mcxedit.
SHELL = /bin/sh
.SUFFIXES:
.SECONDARY:

SRC  := $(wildcard *.c)
OBJ  := $(addsuffix .o,${SRC})
DEP  := $(addsuffix .d,${SRC})

include ../../Common.mk

ifneq (${OS},Windows_NT)
../../mcxedit: ${OBJ}
	$(call msg,LD,$(notdir $@))
	${Q}${CC} ${LDFLAGS} ${LDLIBS} -lmcx -o $@ $^
../../mcxedit.static: ${OBJ} ${srcdir}/libmcx.a
	$(call msg,LD,$(notdir $@))
	${Q}${CC} ${LDFLAGS} ${LDLIBS} -static -o $@ $^
else
../../mcxedit.exe:    ${OBJ} ${srcdir}/libmcx.a
	$(call msg,LD,$(notdir $@))
	${Q}${CC} ${LDFLAGS} ${LDLIBS} -static -o $@ $^
endif

%.c.o: %.c
	$(call msg,CC,src/mcxedit/$@)
	${Q}${CC} -c ${CPPFLAGS} ${CFLAGS} -o $@ $<

mcxedit:        ../../mcxedit
mcxedit.static: ../../mcxedit.static
mcxedit.exe:    ../../mcxedit.exe
PHONY += libmcx.a libmcx.so libmcx.dll

clean:
	$(call msg,CLEAN,src/mcxedit)
	${Q}${RM} ${OBJ}
	${Q}${RM} ${DEP}
	${Q}${RM} ../../mcxedit ../../mcxedit.static ../../mcxedit.exe
PHONY += clean

.PHONY: ${PHONY}
