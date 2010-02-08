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


all: cpucount scut getincludes

cpucount:  cpucount.c
	${CC} ${CFLAGS} cpucount$(SUFFIX) cpucount.c
	$(STRIP) cpucount$(SUFFIX)

getincludes:  getincludes.c
	${CC} ${CFLAGS} getincludes$(SUFFIX) getincludes.c
	$(STRIP) getincludes$(SUFFIX)

scut:  scut.c
	${CC} ${CFLAGS} scut$(SUFFIX) scut.c
	$(STRIP) scut$(SUFFIX)

clean:
	$(RM) cpucount$(SUFFIX)
	$(RM) getincludes$(SUFFIX)
	$(RM) scut$(SUFFIX)
