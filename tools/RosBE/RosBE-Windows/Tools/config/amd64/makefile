TARGET := options64.exe

.PHONY: all

all: $(TARGET)

CC=gcc
CFLAGS := -DWIN32 -DUNICODE -O2 -std=c99 -D_WIN32_IE=0x0400 -D_WIN32_WINNT=0x0500
LFLAGS := -mwindows -s
LIBS := -lshell32

SRCS := options64.c
RC := options64.rc

OBJS := $(SRCS:.c=.o) $(RC:.rc=.coff)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o: $<
	$(CC) $(CFLAGS) -c $< -o $@

$(RC:.rc=.coff):
	windres $(RCFLAGS) -o $@ $(RC)

.PHONY: clean
clean:
	-@del $(TARGET)
	-@del $(OBJS)
