# makefile - Simple makefile to build the various RosBE tools.

.PHONY: all clean

CC := gcc
CFLAGS := ${HOST_CFLAGS} -Wall -O2 -o
LFLAGS := -s
WINVER := 0x502

all: buildtime chknewer chkslash cpucount echoh flash getdate tee

buildtime:  buildtime.c
	${CC} ${CFLAGS} buildtime buildtime.c

chknewer:  chknewer.c
	${CC} ${CFLAGS} chknewer chknewer.c

chkslash:  chkslash.c
	${CC} ${CFLAGS} chkslash chkslash.c

cpucount:  cpucount.c
	${CC} ${CFLAGS} cpucount cpucount.c

echoh:  echoh.c
	${CC} ${CFLAGS} echoh echoh.c

flash:  flash.c
	${CC} -DWINVER=${WINVER} -D_WIN32_WINNT=${WINVER} ${CFLAGS} flash flash.c

getdate:  getdate.c
	${CC} ${CFLAGS} getdate getdate.c

tee:  tee.c
	${CC} ${CFLAGS} tee tee.c

clean:
	del /f buildtime.exe chknewer.exe chkslash.exe cpucount.exe echoh.exe flash.exe getdate.exe tee.exe
