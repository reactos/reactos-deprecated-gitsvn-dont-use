
spec2def(kdgdb.dll kdgdb.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    gdb_input.c
    gdb_receive.c
    gdb_send.c
    kdgdb.c
    kdpacket.c
    utils.c
    kdgdb.h)

# TODO: AMD64, ARM...
if(ARCH STREQUAL "i386")
    list(APPEND SOURCE i386_sup.c)
endif()

add_library(kdgdb SHARED
    ${SOURCE}
    kdgdb.rc
    ${CMAKE_CURRENT_BINARY_DIR}/kdgdb.def)

set_entrypoint(kdgdb 0)
set_subsystem(kdgdb native)
set_image_base(kdgdb 0x00010000)
add_importlibs(kdgdb ntoskrnl hal)
target_link_libraries(kdgdb cportlib)
add_pch(kdgdb kdgdb.h SOURCE)
add_cd_file(TARGET kdgdb DESTINATION reactos/system32 NO_CAB FOR all)
