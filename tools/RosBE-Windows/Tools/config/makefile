TARGET := options.exe

.PHONY: all

all: $(TARGET)

CPP=mingw32-gcc
CFLAGS := -DWIN32 -DUNICODE -O2 -std=c99
LFLAGS := -mwindows -s
LIBS := 

SRCS := options.c
RC := options.rc

OBJS := $(SRCS:.c=.o) $(RC:.rc=.coff)

$(TARGET): $(OBJS)
	$(CPP) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o: $<
	$(CPP) $(CFLAGS) -c $< -o $@

$(RC:.rc=.coff):
	windres $(RCFLAGS) -o $@ $(RC)

.PHONY: clean
clean:
	-@del $(TARGET)
	-@del $(OBJS)
