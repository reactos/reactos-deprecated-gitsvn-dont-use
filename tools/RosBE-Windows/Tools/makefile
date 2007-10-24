# makefile - Simple makefile to build the various RosBE tools.

.PHONY: all clean

CC := gcc
CFLAGS := ${HOST_CFLAGS} -Wall -O2 -o
LFLAGS := -s
WINVER := 0x502

all: buildtime chknewer cpucount echoh flash getdate

buildtime:  buildtime.c
	${CC} ${CFLAGS} buildtime buildtime.c

chknewer:  chknewer.c
	${CC} ${CFLAGS} chknewer chknewer.c

cpucount:  cpucount.c
	${CC} ${CFLAGS} cpucount cpucount.c

echoh:  echoh.c
	${CC} ${CFLAGS} echoh echoh.c

flash:  flash.c
	${CC} -DWINVER=${WINVER} -D_WIN32_WINNT=${WINVER} ${CFLAGS} flash flash.c

getdate:  getdate.c
	${CC} ${CFLAGS} getdate getdate.c

clean:
	del /f buildtime.exe chknewer.exe cpucount.exe echoh.exe flash.exe getdate.exe
