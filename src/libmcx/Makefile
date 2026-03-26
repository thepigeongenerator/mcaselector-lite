# Copyright (C)2026 mcxedit
# Licensed under GPL-2.0-only. For further information,
# view `git log`, and the COPYING and CONTRIBUTORS files
# at www.github.com/thepigeongenerator/mcxedit.
SHELL = /bin/sh
.SUFFIXES:
.SECONDARY:

SRC  := $(wildcard *.c)
OBJ  := $(addsuffix .o,${SRC})
SOBJ := $(addsuffix .so.o,${SRC})
DEP  := $(addsuffix .d,${SRC})

include ../../Common.mk

../../libmcx.a: ${OBJ}
	$(call msg,AR,$(notdir $@))
	${Q}${AR} -rsc $@ $<

ifneq (${OS},Windows_NT)
../../libmcx.so:  ${SOBJ}
	$(call msg,LD,$(notdir $@))
	${Q}${CC} ${LDFLAGS} -shared ${LDLIBS} -o $@ $^
else
../../libmcx.dll: ${SOBJ}
	$(call msg,LD,$(notdir $@))
	${Q}${CC} ${LDFLAGS} -shared ${LDLIBS} -o $@ $^
endif

%.c.o: %.c
	$(call msg,CC,src/libmcx/$@)
	${Q}${CC} -c ${CPPFLAGS} ${CFLAGS} -o $@ $<
%.c.so.o: %.c
	$(call msg,CC,src/libmcx/$@)
	${Q}${CC} -c ${CPPFLAGS} -fPIC ${CFLAGS} -o $@ $<

libmcx.a:   ../../libmcx.a
libmcx.so:  ../../libmcx.so
libmcx.dll: ../../libmcx.dll
PHONY += libmcx.a libmcx.so libmcx.dll

clean:
	$(call msg,CLEAN,src/libmcx)
	${Q}${RM} ${SOBJ}
	${Q}${RM} ${OBJ}
	${Q}${RM} ${DEP}
	${Q}${RM} ../../libmcx.a ../../libmcx.so ../../libmcx.dll
PHONY += clean
check:; ${Q}sparse ${CPPFLAGS} ${CFLAGS} ${SRC}
PHONY += check

.PHONY: ${PHONY}
