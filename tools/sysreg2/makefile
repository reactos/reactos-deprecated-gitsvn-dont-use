TARGET := sysreg2

.PHONY: all

all: $(TARGET)

CC=gcc
CFLAGS := -g -O0 -std=c99 -D_GNU_SOURCE
LFLAGS := -L/usr/lib64
LIBS := -lvirt -lxml2
INC := -I/usr/include/libvirt/ -I/usr/include/libxml2/

SRCS := virt.c utils.c console.c options.c raddr2line.c

OBJS := $(SRCS:.c=.o) 

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o: $<
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	-@rm $(TARGET)
	-@rm $(OBJS)
					
