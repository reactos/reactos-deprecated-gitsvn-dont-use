# mingw32 makefile for ccache

TARGET := ccache.exe

.PHONY: all

all: $(TARGET)

CC=gcc
CFLAGS := -Wall -pedantic -D_WIN32_WINNT=0x0500 -D_WIN32_IE=0x0400 -O2
LFLAGS := -s
SRCS := args.c ccache.c cleanup.c execute.c getopt.c hash.c mdfour.c snprintf.c stats.c unify.c util.c win32-dirent.c
OBJS := $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS)

.c.o: $<
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-@del $(TARGET)
	-@del $(OBJS)
