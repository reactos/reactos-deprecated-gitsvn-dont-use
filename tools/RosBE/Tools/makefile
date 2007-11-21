# makefile - Simple makefile to build the various RosBE tools.

.PHONY: all clean

CC := gcc
CFLAGS := ${HOST_CFLAGS} -Wall -O2 -o
STRIP := strip

ifeq ($(word 1,$(shell gcc -dumpmachine)),mingw32)
	# Windows host
	RM := del /f
	SUFFIX := .exe
else
	# Unix host
	RM := rm -f
	SUFFIX :=
endif


all: cpucount

cpucount:  cpucount.c
	${CC} ${CFLAGS} cpucount$(SUFFIX) cpucount.c
	$(STRIP) cpucount$(SUFFIX)

clean:
	$(RM) cpucount$(SUFFIX)
