LXLOAD_BASE = $(TOOLS_BASE_)lxload
LXLOAD_BASE_ = $(LXLOAD_BASE)$(SEP)
LXLOAD_INT = $(INTERMEDIATE_)$(LXLOAD_BASE)
LXLOAD_INT_ = $(LXLOAD_INT)$(SEP)
LXLOAD_OUT = $(OUTPUT_)$(LXLOAD_BASE)
LXLOAD_OUT_ = $(LXLOAD_OUT)$(SEP)

$(LXLOAD_INT): | $(TOOLS_INT)
	$(ECHO_MKDIR)
	${mkdir} $@

ifneq ($(INTERMEDIATE),$(OUTPUT))
$(LXLOAD_OUT): | $(TOOLS_OUT)
	$(ECHO_MKDIR)
	${mkdir} $@
endif

LXLOAD_TARGET = \
	$(LXLOAD_OUT_)lxload$(EXEPOSTFIX)

LXLOAD_SOURCES = \
	$(LXLOAD_BASE_)cmos.c \
	$(LXLOAD_BASE_)com.c \
        $(LXLOAD_BASE_)devicelist.c \
        $(LXLOAD_BASE_)font.c \
        $(LXLOAD_BASE_)hwemul.c \
        $(LXLOAD_BASE_)ide.c \
        $(LXLOAD_BASE_)lxload.c \
        $(LXLOAD_BASE_)pci.c \
        $(LXLOAD_BASE_)pic.c \
        $(LXLOAD_BASE_)pit.c 

LXLOAD_OBJECTS = \
	$(addprefix $(INTERMEDIATE_), $(LXLOAD_SOURCES:.c=.o))

LXLOAD_HOST_CFLAGS = $(TOOLS_CFLAGS) -g -Iinclude/reactos

LXLOAD_HOST_LFLAGS = $(TOOLS_LFLAGS) -lSDL -Ttext=0x10000000

$(LXLOAD_TARGET): $(LXLOAD_OBJECTS) | $(LXLOAD_OUT)
	$(ECHO_LD)
	${host_gcc} $(LXLOAD_OBJECTS) $(LXLOAD_HOST_LFLAGS) -o $@

$(LXLOAD_INT_)cmos.o: $(LXLOAD_BASE_)cmos.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)com.o: $(LXLOAD_BASE_)com.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)devicelist.o: $(LXLOAD_BASE_)devicelist.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)font.o: $(LXLOAD_BASE_)font.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)hwemul.o: $(LXLOAD_BASE_)hwemul.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)ide.o: $(LXLOAD_BASE_)ide.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)lxload.o: $(LXLOAD_BASE_)lxload.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)pci.o: $(LXLOAD_BASE_)pci.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)pic.o: $(LXLOAD_BASE_)pic.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

$(LXLOAD_INT_)pit.o: $(LXLOAD_BASE_)pit.c | $(LXLOAD_INT)
	$(ECHO_CC)
	${host_gcc} $(LXLOAD_HOST_CFLAGS) -c $< -o $@

.PHONY: lxload
lxload: $(LXLOAD_TARGET)

.PHONY: lxload_clean
lxload_clean:
	-@$(rm) $(LXLOAD_TARGET) $(LXLOAD_OBJECTS) 2>$(NUL)
clean: lxload_clean
