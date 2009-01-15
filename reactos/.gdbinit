#add-symbol-file freeldr.sym 0x8000
add-symbol-file output-luser/ntoskrnl/ntoskrnl.nostrip.exe 0x80801000
add-symbol-file output-luser/drivers/base/bootvid/bootvid.nostrip.dll 0x80b04000
add-symbol-file output-luser/hal/halx86/hal.nostrip.dll 0x80b12000
#add-symbol-file output-luser/drivers/base/kdcom/kdcom.nostrip.dll 0x80b2e000
add-symbol-file output-luser/dll/ntdll/ntdll.nostrip.dll 0x7c901000
add-symbol-file output-luser/dll/win32/smdll/smdll.nostrip.dll 0x6b3b1000
add-symbol-file output-luser/subsystems/win32/csrss/csrss.nostrip.exe 0x401000
set follow-fork-mode child
handle SIGSEGV noprint nostop pass
handle SIGBUS noprint nostop pass
set args -f output-luser/boot/freeldr/freeldr/freeldr.sys -d ../../../reactos.img -c "/DEBUG /DEBUGPORT=COM1"

source modules.gdb
source luser.gdb
source unistr.gdb
