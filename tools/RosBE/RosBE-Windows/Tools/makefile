# makefile - Simple makefile to build the various RosBE tools.

.PHONY: all clean

CC := gcc
CFLAGS := ${HOST_CFLAGS} -Wall -O2 -o
STRIP := strip
SUFFIX := .exe
WINVER := 0x500

all: buildtime chknewer chkslash echoh flash getdate rquote scut tee

buildtime:  buildtime.c
	${CC} ${CFLAGS} buildtime$(SUFFIX) buildtime.c
	$(STRIP) buildtime$(SUFFIX)

chknewer:  chknewer.c
	${CC} ${CFLAGS} chknewer chknewer.c
	$(STRIP) chknewer$(SUFFIX)

chkslash:  chkslash.c
	${CC} ${CFLAGS} chkslash chkslash.c
	$(STRIP) chkslash$(SUFFIX)

echoh:  echoh.c
	${CC} ${CFLAGS} echoh echoh.c
	$(STRIP) echoh$(SUFFIX)

flash:  flash.c
	${CC} -DWINVER=${WINVER} -D_WIN32_WINNT=${WINVER} ${CFLAGS} flash flash.c
	$(STRIP) flash$(SUFFIX)

getdate:  getdate.c
	${CC} ${CFLAGS} getdate getdate.c
	$(STRIP) getdate$(SUFFIX)

rquote:  rquote.c
	${CC} ${CFLAGS} rquote rquote.c
	$(STRIP) rquote$(SUFFIX)

scut:  scut.c
	${CC} ${CFLAGS} scut scut.c
	$(STRIP) scut$(SUFFIX)

tee:  tee.c
	${CC} ${CFLAGS} tee tee.c
	$(STRIP) tee$(SUFFIX)

clean:
	del /f buildtime.exe chknewer.exe chkslash.exe echoh.exe flash.exe getdate.exe scut.exe tee.exe
