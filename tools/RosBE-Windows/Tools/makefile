# makefile - Simple makefile to build the various RosBE tools.

CC = gcc
OFLAGS = -Wall -O2 -o

buildtime:  buildtime.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) buildtime buildtime.c

cpucount:  cpucount.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) cpucount cpucount.c

echoh:  echoh.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) echoh echoh.c

flash:  flash.c
	$(CC) $(HOST_CFLAGS) -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 $(OFLAGS) flash flash.c

getdate:  getdate.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) getdate getdate.c

all:  buildtime.c cpucount.c echoh.c flash.c getdate.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) buildtime buildtime.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) cpucount cpucount.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) echoh echoh.c
	$(CC) $(HOST_CFLAGS) -DWINVER=0x0501 -D_WIN32_WINNT=0x0501 $(OFLAGS) flash flash.c
	$(CC) $(HOST_CFLAGS) $(OFLAGS) getdate getdate.c
