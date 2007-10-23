# makefile - Simple makefile to build the various RosBE tools.

CC = gcc
CFLAGS := $(HOST_CFLAGS) -Wall -O2 -o
LFLAGS := -s
WINVER := 0x502

buildtime:  buildtime.c
	$(CC) $(CFLAGS) buildtime buildtime.c

cpucount:  cpucount.c
	$(CC) $(CFLAGS) cpucount cpucount.c

echoh:  echoh.c
	$(CC) $(CFLAGS) echoh echoh.c

flash:  flash.c
	$(CC) -DWINVER=$(WINVER) -D_WIN32_WINNT=0x$(WINVER) $(CFLAGS) flash flash.c

getdate:  getdate.c
	$(CC) $(CFLAGS) getdate getdate.c

all:  buildtime.c cpucount.c echoh.c flash.c getdate.c
	$(CC) $(CFLAGS) buildtime buildtime.c
	$(CC) $(CFLAGS) cpucount cpucount.c
	$(CC) $(CFLAGS) echoh echoh.c
	$(CC) -DWINVER=$(WINVER) -D_WIN32_WINNT=$(WINVER) $(CFLAGS) flash flash.c
	$(CC) $(CFLAGS) getdate getdate.c
