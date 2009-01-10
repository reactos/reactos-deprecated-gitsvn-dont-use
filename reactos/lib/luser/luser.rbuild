<?xml version="1.0"?>
<!DOCTYPE module SYSTEM "../../tools/rbuild/project.dtd">
<module name="luser" type="staticlibrary">
  <define name="_NTOSKRNL_"/>
  <define name="LUSER"/>
  <include base="ReactOS">include/reactos/libs/luser</include>
  <include base="luser">.</include>
  <file>context.S</file>
  <file>fs.c</file>
  <file>io.c</file>
  <file>maps.c</file>
  <file>signals.c</file>
  <file>__syscall_error.c</file>
  <file>sysprim.c</file>
  <file>syscall.S</file>
  <file>vsprintf.c</file>
  <file>vfprintf.c</file>
</module>
