::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Config.cmd
:: PURPOSE:     A Basic Config.rbuild Creator for ReactOS.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title ReactOS Build Configurator

if "%1" == "delete" (
    if exist "%_ROSSOURCEDIR%\config.rbuild" (
        del %_ROSSOURCEDIR%\config.rbuild
        echo Main Configuration File was found and deleted.
    ) else (
        echo Main Configuration File was not found in ReactOS Source Tree.
    )
    if exist "%ROSBEBASEDIR%\config.rbuild" (
        del %ROSBEBASEDIR%\config.rbuild
        echo Working Configuration File was found and deleted.
    ) else (
        echo Working Configuration File was not found in ReactOS Source Tree.
    )
goto :NOK
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :NOK
)

if not exist "%ROSBEBASEDIR%\config.rbuild" (
    copy "%_ROSSOURCEDIR%\config.template.rbuild" "%ROSBEBASEDIR%\config.rbuild"
)

echo WARNING:
echo Non-Default-Configurations are not useable for pasteing Bugs to
echo Bugzilla or asking quesions about in Forum/Chat!
echo.
echo DID YOU UNDERSTAND?

SET /P XY=(yes), (no)

if /I "%XY%"=="yes" goto :OK
if /I "%XY%"=="no" goto :NOK
goto :NOK

:OK
echo Architecture to build for. Not Editable.
echo.
echo Right now
call grep \"ARCH\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
echo.

echo Sub-architecture to build for.
echo Default is: none
echo.
echo Right now
call grep \"SARCH\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P X= (), (xbox)
call sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%X%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%X%\"/g" %ROSBEBASEDIR%\config.rbuild > %ROSBEBASEDIR%\config2.rbuild
cls

echo Which CPU ReactOS should be optimized for.
echo.
echo Examples: i486, i586, pentium, pentium2, pentium3, pentium4,
echo athlon-xp, athlon-mp, k6-2
echo See GCC manual for more CPU names and which CPUs GCC can optimize for.
echo Default is: pentium
echo.
echo Right now
call grep \"OARCH\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P XX=
call sed "s/\"OARCH\" value=\".*\"/\"OARCH\" value=\"%XX%\"/g" %ROSBEBASEDIR%\config2.rbuild > %ROSBEBASEDIR%\config21.rbuild
cls

echo What level do you want ReactOS to be optimized at.
echo This setting does not work if GDB is set.
echo 0 = off
echo 1 = Normal compiling. Recommended. It is the default setting in
echo official release builds and debug builds.
echo warning : 2,3,4,5 is not tested on ReactOS. Change at own risk.
echo.
echo Right now
call grep \"OPTIMIZE\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P Y= (0), (1), (2), (3), (4), (5)
call sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%Y%\"/g" %ROSBEBASEDIR%\config21.rbuild > %ROSBEBASEDIR%\config3.rbuild
cls

echo Whether to compile for an uniprocessor or multiprocessor machine.
echo Default is: 0
echo.
echo Right now
call grep \"MP\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P Z= (0), (1)
call sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%Z%\"/g" %ROSBEBASEDIR%\config3.rbuild > %ROSBEBASEDIR%\config4.rbuild
cls

echo Whether to compile in the integrated kernel debugger.
echo Default is: 0
echo.
echo Right now
call grep \"KDBG\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P A= (0), (1)
call sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%A%\"/g" %ROSBEBASEDIR%\config4.rbuild > %ROSBEBASEDIR%\config5.rbuild
cls

echo Whether to compile for debugging. No compiler optimizations will be
echo performed.
echo Default is: 1
echo.
echo Right now
call grep \"DBG\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P B= (0), (1)
call sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%B%\"/g" %ROSBEBASEDIR%\config5.rbuild > %ROSBEBASEDIR%\config6.rbuild
cls

echo Whether to compile for debugging with GDB. If you don't use GDB,
echo don't enable this.
echo Default is: 0
echo.
echo Right now
call grep \"GDB\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P C= (0), (1)
call sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%C%\"/g" %ROSBEBASEDIR%\config6.rbuild > %ROSBEBASEDIR%\config7.rbuild
cls

echo Whether to compile apps/libs with features covered software patents
echo or not. If you live in a country where software patents are
echo valid/apply, don't enable this (except they/you purchased a license
echo from the patent owner).
echo Default is: 0
echo.
echo Right now
call grep \"NSWPAT\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P D= (0), (1)
call sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%D%\"/g" %ROSBEBASEDIR%\config7.rbuild > %ROSBEBASEDIR%\config8.rbuild
cls

echo Whether to compile with NT-compatible LPC Semantics. This should
echo always be 1.
echo.
echo Right now
call grep \"NTLPC\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P E= (0), (1)
call sed "s/\"NTLPC\" value=\"[0-1]\"/\"NTLPC\" value=\"%E%\"/g" %ROSBEBASEDIR%\config8.rbuild > %ROSBEBASEDIR%\config9.rbuild
cls

echo Whether to compile with the KD protocol. This will disable support for
echo KDBG as well as rossym and symbol lookups, and allow WinDBG to connect
echo to ReactOS. This is currently not fully working, and requires kdcom
echo from Windows 2003 or TinyKRNL. Booting into debug mode with this flag
echo enabled will result in a failure to enter GUI mode. Do not enable
echo unless you know what you're doing.
echo Default is: 0
echo.
echo Right now
call grep \"_WINKD_\" %ROSBEBASEDIR%\config.rbuild|cut -d "\"" -f 4
SET /P F= (0), (1)
call sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%F%\"/g" %ROSBEBASEDIR%\config9.rbuild > %ROSBEBASEDIR%\config10.rbuild

copy %ROSBEBASEDIR%\config10.rbuild %ROSBEBASEDIR%\config.tmp
del %ROSBEBASEDIR%\*.rbuild
copy %ROSBEBASEDIR%\config.tmp %ROSBEBASEDIR%\config.rbuild
del %ROSBEBASEDIR%\*.tmp
copy %ROSBEBASEDIR%\config.rbuild %_ROSSOURCEDIR%\config.rbuild

goto :NOK

:NOK
title ReactOS Build Environment 0.3.7
