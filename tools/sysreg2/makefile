TARGET := sysreg2

.PHONY: all

all: $(TARGET)

CC=gcc
CFLAGS := -O2 -std=c99 -D_GNU_SOURCE
LFLAGS := -s -L/usr/lib64
LIBS := -lgcc -lm -lvirt -ltasn1 -lz -lxml2 -lgnutls
INC := -I/usr/include/libvirt/ -I/usr/include/libxml2/

SRCS := virt.c utils.c console.c

OBJS := $(SRCS:.c=.o) 

$(TARGET): $(OBJS)
	$(CC) $(LFLAGS) -o $@ $(OBJS) $(LIBS)

.c.o: $<
	$(CC) $(INC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	-@rm $(TARGET)
	-@rm $(OBJS)
					
