:: Copyright (c) Daniel Reimer. All rights reserved.
::
:: A Basic Config.rbuild Creator for ReactOS.
::
@echo off

title ReactOS Build Configurator

echo Architecture to build for. Not Editable.
echo
echo Right now
grep \"ARCH\" config.rbuild|cut -d "\"" -f 4
echo

echo Sub-architecture to build for.
echo Default is: none
echo
echo Right now
grep \"SARCH\" config.rbuild|cut -d "\"" -f 4
SET /P X= (), (xbox) 
sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%X%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%X%\"/g" config.rbuild > config2.rbuild
cls

echo Which CPU ReactOS should be optimized for.
echo
echo Examples: i486, i586, pentium, pentium2, pentium3, pentium4, athlon-xp,
echo athlon-mp, k6-2
echo See GCC manual for more CPU names and which CPUs GCC can optimize for.
echo Default is: pentium
echo
echo Right now
grep \"OARCH\" config.rbuild|cut -d "\"" -f 4
SET /P XX= 
sed "s/\"OARCH\" value=\".*\"/\"OARCH\" value=\"%XX%\"/g" config2.rbuild > config21.rbuild
cls

echo What level do you want ReactOS to be optimized at.
echo this setting does not work if GDB is set
echo 0 = off
echo 1 = Normal compiling. Recommended, it is default setting in
echo official build and debug build.
echo warning : 2,3,4,5 is not tested on ReactOS. Change at own risk. 
echo
echo Right now
grep \"OPTIMIZE\" config.rbuild|cut -d "\"" -f 4
SET /P Y= (0), (1), (2), (3), (4), (5) 
sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%Y%\"/g" config21.rbuild > config3.rbuild
cls

echo Whether to compile for an uniprocessor or multiprocessor machine.
echo Default is: 0
echo
echo Right now
grep \"MP\" config.rbuild|cut -d "\"" -f 4
SET /P Z= (0), (1)
sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%Z%\"/g" config3.rbuild > config4.rbuild
cls

echo Whether to compile in the integrated kernel debugger.
echo Default is: 0
echo
echo Right now
grep \"KDBG\" config.rbuild|cut -d "\"" -f 4
SET /P A= (0), (1)
sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%A%\"/g" config4.rbuild > config5.rbuild
cls

echo Whether to compile for debugging. No compiler optimizations will be
echo performed.
echo Default is: 1
echo
echo Right now
grep \"DBG\" config.rbuild|cut -d "\"" -f 4
SET /P B= (0), (1)
sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%B%\"/g" config5.rbuild > config6.rbuild
cls

echo Whether to compile for debugging with GDB. If you don't use GDB, don't
echo enable this.
echo Default is: 0
echo
echo Right now
grep \"GDB\" config.rbuild|cut -d "\"" -f 4
SET /P C= (0), (1)
sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%C%\"/g" config6.rbuild > config7.rbuild
cls

echo Whether to compile apps/libs with features covered software patents or not.
echo If you live in a country where software patents are valid/apply, don't
echo enable this (except they/you purchased a license from the patent owner).
echo Default is: 0
echo
echo Right now
grep \"NSWPAT\" config.rbuild|cut -d "\"" -f 4
SET /P D= (0), (1)
sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%D%\"/g" config7.rbuild > config8.rbuild
cls

echo Whether to compile with NT-compatible LPC Semantics. This should always be 1.
echo
echo Right now
grep \"NTLPC\" config.rbuild|cut -d "\"" -f 4
SET /P E= (0), (1)
sed "s/\"NTLPC\" value=\"[0-1]\"/\"NTLPC\" value=\"%E%\"/g" config8.rbuild > config9.rbuild
cls

echo Whether to compile with the KD protocol. This will disable support for KDBG
echo as well as rossym and symbol lookups, and allow WinDBG to connect to ReactOS.
echo This is currently not fully working, and requires kdcom from Windows 2003 or
echo TinyKRNL. Booting into debug mode with this flag enabled will result in a
echo failure to enter GUI mode. Do not enable unless you know what you're doing.
echo Default is: 0
echo
echo Right now
grep \"_WINKD_\" config.rbuild|cut -d "\"" -f 4
SET /P F= (0), (1)
sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%F%\"/g" config9.rbuild > config10.rbuild

copy config10.rbuild config.tmp
del *.rbuild
copy config.tmp config.rbuild
del *.tmp
