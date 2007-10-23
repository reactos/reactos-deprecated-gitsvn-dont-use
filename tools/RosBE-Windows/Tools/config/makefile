TARGET := options.exe

.PHONY: all

all: $(TARGET)

CC=gcc
CFLAGS := $(HOST_CFLAGS) -DWIN32 -DUNICODE -O2 -std=c99
LFLAGS := -mwindows -s
LIBS :=

SRCS := options.c
RC := options.rc

OBJS := $(SRCS:.c=.o) $(RC:.rc=.coff)

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o: $<
	$(CC) $(CFLAGS) -c $< -o $@

$(RC:.rc=.coff):
	windres $(HOST_CFLAGS) $(RCFLAGS) -o $@ $(RC)

.PHONY: clean
clean:
	-@del $(TARGET)
	-@del $(OBJS)
