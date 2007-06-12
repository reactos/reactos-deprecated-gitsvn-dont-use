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

if not exist "%ROSBEBASEDIR%\Tools\config.rbuild" (
    copy "%_ROSSOURCEDIR%\config.template.rbuild" "%ROSBEBASEDIR%\Tools\config.rbuild"
)

if "%1" == "delete" (
    del %_ROSSOURCEDIR%\config.rbuild
    del %ROSBEBASEDIR%\Tools\config.rbuild
    goto :NOK
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
call "%ROSBEBASEDIR%\Tools\grep.exe" \"ARCH\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
echo.

echo Sub-architecture to build for.
echo Default is: none
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"SARCH\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P X= (), (xbox)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%X%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%X%\"/g" %ROSBEBASEDIR%\Tools\config.rbuild > %ROSBEBASEDIR%\Tools\config2.rbuild
cls

echo Which CPU ReactOS should be optimized for.
echo.
echo Examples: i486, i586, pentium, pentium2, pentium3, pentium4,
echo athlon-xp, athlon-mp, k6-2
echo See GCC manual for more CPU names and which CPUs GCC can optimize for.
echo Default is: pentium
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"OARCH\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P XX=
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"OARCH\" value=\".*\"/\"OARCH\" value=\"%XX%\"/g" %ROSBEBASEDIR%\Tools\config2.rbuild > %ROSBEBASEDIR%\Tools\config21.rbuild
cls

echo What level do you want ReactOS to be optimized at.
echo This setting does not work if GDB is set.
echo 0 = off
echo 1 = Normal compiling. Recommended. It is the default setting in
echo official release builds and debug builds.
echo warning : 2,3,4,5 is not tested on ReactOS. Change at own risk.
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"OPTIMIZE\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P Y= (0), (1), (2), (3), (4), (5)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%Y%\"/g" %ROSBEBASEDIR%\Tools\config21.rbuild > %ROSBEBASEDIR%\Tools\config3.rbuild
cls

echo Whether to compile for an uniprocessor or multiprocessor machine.
echo Default is: 0
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"MP\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P Z= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%Z%\"/g" %ROSBEBASEDIR%\Tools\config3.rbuild > %ROSBEBASEDIR%\Tools\config4.rbuild
cls

echo Whether to compile in the integrated kernel debugger.
echo Default is: 0
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"KDBG\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P A= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%A%\"/g" %ROSBEBASEDIR%\Tools\config4.rbuild > %ROSBEBASEDIR%\Tools\config5.rbuild
cls

echo Whether to compile for debugging. No compiler optimizations will be
echo performed.
echo Default is: 1
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"DBG\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P B= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%B%\"/g" %ROSBEBASEDIR%\Tools\config5.rbuild > %ROSBEBASEDIR%\Tools\config6.rbuild
cls

echo Whether to compile for debugging with GDB. If you don't use GDB,
echo don't enable this.
echo Default is: 0
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"GDB\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P C= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%C%\"/g" %ROSBEBASEDIR%\Tools\config6.rbuild > %ROSBEBASEDIR%\Tools\config7.rbuild
cls

echo Whether to compile apps/libs with features covered software patents
echo or not. If you live in a country where software patents are
echo valid/apply, don't enable this (except they/you purchased a license
echo from the patent owner).
echo Default is: 0
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"NSWPAT\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P D= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%D%\"/g" %ROSBEBASEDIR%\Tools\config7.rbuild > %ROSBEBASEDIR%\Tools\config8.rbuild
cls

echo Whether to compile with NT-compatible LPC Semantics. This should
echo always be 1.
echo.
echo Right now
call "%ROSBEBASEDIR%\Tools\grep.exe" \"NTLPC\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P E= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"NTLPC\" value=\"[0-1]\"/\"NTLPC\" value=\"%E%\"/g" %ROSBEBASEDIR%\Tools\config8.rbuild > %ROSBEBASEDIR%\Tools\config9.rbuild
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
call "%ROSBEBASEDIR%\Tools\grep.exe" \"_WINKD_\" %ROSBEBASEDIR%\Tools\config.rbuild|"%ROSBEBASEDIR%\Tools\cut.exe" -d "\"" -f 4
SET /P F= (0), (1)
call "%ROSBEBASEDIR%\Tools\sed.exe" "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%F%\"/g" %ROSBEBASEDIR%\Tools\config9.rbuild > %ROSBEBASEDIR%\Tools\config10.rbuild

copy %ROSBEBASEDIR%\Tools\config10.rbuild %ROSBEBASEDIR%\Tools\config.tmp
del %ROSBEBASEDIR%\Tools\*.rbuild
copy %ROSBEBASEDIR%\Tools\config.tmp %ROSBEBASEDIR%\Tools\config.rbuild
del %ROSBEBASEDIR%\Tools\*.tmp
copy %ROSBEBASEDIR%\Tools\config.rbuild %_ROSSOURCEDIR%\config.rbuild

:NOK
