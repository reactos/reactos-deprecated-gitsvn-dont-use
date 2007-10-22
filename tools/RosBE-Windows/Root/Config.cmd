::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/Config.cmd
:: PURPOSE:     A Basic Config.rbuild Creator for ReactOS.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title ReactOS Build Configurator

::
:: Receive the first Parameter and decide what to do.
::
if "%1" == "delete" (
    echo config.rbuild will be permanently deleted. All your settings will be gone.
    echo Continue?
    set /p XY="(yes), (no)"
    if /i "%XY%"=="yes" goto :CONT
    if /i "%XY%"=="no" goto :NOK
    goto :NOK

    :CONT
    if exist "config.rbuild" (
        del "config.rbuild"
        echo Main Configuration File was found and deleted.
    ) else (
        echo Main Configuration File was not found in ReactOS Source Tree.
    )
    if exist "%_ROSBE_BASEDIR%\config.rbuild" (
        del "%_ROSBE_BASEDIR%\config.rbuild"
        echo Working Configuration File was found and deleted.
    ) else (
        echo Working Configuration File was not found in ReactOS Source Tree.
    )
goto :NOK
)
if "%1" == "update" (
    echo old config.rbuild will be deleted and will be updated with a recent,
    echo default one. You will need to reconfigure it to your wishes later.
    echo Continue?
    set /p XY="(yes), (no)"
    if /i "%XY%"=="yes" goto :CONT2
    if /i "%XY%"=="no" goto :NOK
    :CONT2
    del "%_ROSBE_BASEDIR%\*.rbuild"
    del "config.rbuild"
    copy "config.template.rbuild" "%_ROSBE_BASEDIR%\config.rbuild"
    echo Successfully Updated.
    goto :NOK
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :NOK
)

::
:: Check if config.rbuild already exists. If not, get a working copy.
::
if not exist "%_ROSBE_BASEDIR%\config.rbuild" (
    copy "config.template.rbuild" "%_ROSBE_BASEDIR%\config.rbuild"
)

::
:: N00b Blocker :-P
::
echo WARNING:
echo Non-Default-Configurations are not useable for pasteing Bugs to
echo Bugzilla or asking quesions about in Forum/Chat!
echo.
echo DID YOU UNDERSTAND?

set /p XY="(yes), (no)"

if /i "%XY%"=="yes" goto :OK
if /i "%XY%"=="no" goto :NOK
goto :NOK

:OK

::
:: Check if config.template.rbuild is newer than config.rbuild, if it is then
:: inform the user and offer an update.
::
if exist "%_ROSBE_BASEDIR%\config.rbuild" (
    test "config.template.rbuild" -nt "%_ROSBE_BASEDIR%\config.rbuild"
    if not errorlevel 1 (
        echo.
        echo *** config.template.rbuild is newer than working config.rbuild ***
        echo *** The Editor cannot continue with this file. Do you wanna    ***
        echo *** update to the most recent one? You need to reset all your  ***
        echo *** previously made settings.                                  ***
        echo.
        set /p XY="(yes), (no)"
        if /i "%XY%"=="yes" del "%_ROSBE_BASEDIR%\*.rbuild" | del "config.rbuild" | copy "config.template.rbuild" "%_ROSBE_BASEDIR%\config.rbuild" | goto :OK
        if /i "%XY%"=="no" goto :NOK
        goto :NOK
    )
)

::
:: Start with reading settings from config.rbuild and let the user edit them.
::
echo Sub-Architecture to build for.
echo Default is: none
echo.
echo Right now:
grep \"SARCH\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p X="(), (xbox)"
sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%X%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%X%\"/g" "%_ROSBE_BASEDIR%\config.rbuild" > "%_ROSBE_BASEDIR%\config2.rbuild"
cls

echo Which CPU ReactOS should be optimized for.
echo.
echo Examples: i486, i586, pentium, pentium2, pentium3, pentium4,
echo athlon-xp, athlon-mp, k6-2
echo See GCC manual for more CPU names and which CPUs GCC can optimize for.
echo Default is: pentium
echo.
echo Right now:
grep \"OARCH\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p XX=
if "%XX%" == "" (
    set XX=pentium
)
sed "s/\"OARCH\" value=\".*\"/\"OARCH\" value=\"%XX%\"/g" "%_ROSBE_BASEDIR%\config2.rbuild" > "%_ROSBE_BASEDIR%\config3.rbuild"
cls

echo What level do you want ReactOS to be optimized at.
echo This setting does not work if GDB is set.
echo 0 = off
echo 1 = Normal compiling. Recommended. It is the default setting in
echo official release builds and debug builds.
echo warning : 2,3,4,5 is not tested on ReactOS. Change at own risk.
echo.
echo Right now:
grep \"OPTIMIZE\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p Y="(0), (1), (2), (3), (4), (5)"
if "%Y%" == "" (
    set Y=1
)
sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%Y%\"/g" "%_ROSBE_BASEDIR%\config3.rbuild" > "%_ROSBE_BASEDIR%\config4.rbuild"
cls

echo Whether to compile for an uniprocessor or multiprocessor machine.
echo Default is: 0
echo.
echo Right now:
grep \"MP\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p Z="(0), (1)"
if "%Z%" == "" (
    set Z=0
)
sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%Z%\"/g" "%_ROSBE_BASEDIR%\config4.rbuild" > "%_ROSBE_BASEDIR%\config5.rbuild"
cls

echo Whether to compile in the integrated kernel debugger.
echo Default is: 0
echo.
echo Right now:
grep \"KDBG\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p A="(0), (1)"
if "%A%" == "" (
    set A=0
)
sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%A%\"/g" "%_ROSBE_BASEDIR%\config5.rbuild" > "%_ROSBE_BASEDIR%\config6.rbuild"
cls

echo Whether to compile for debugging. No compiler optimizations will be
echo performed.
echo Default is: 1
echo.
echo Right now:
grep \"DBG\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p B="(0), (1)"
if "%B%" == "" (
    set B=1
)
sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%B%\"/g" "%_ROSBE_BASEDIR%\config6.rbuild" > "%_ROSBE_BASEDIR%\config7.rbuild"
cls

echo Whether to compile for debugging with GDB. If you don't use GDB,
echo don't enable this.
echo Default is: 0
echo.
echo Right now:
grep \"GDB\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p C="(0), (1)"
if "%C%" == "" (
    set C=0
)
sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%C%\"/g" "%_ROSBE_BASEDIR%\config7.rbuild" > "%_ROSBE_BASEDIR%\config8.rbuild"
cls

echo Whether to compile apps/libs with features covered software patents
echo or not. If you live in a country where software patents are
echo valid/apply, don't enable this (except they/you purchased a license
echo from the patent owner).
echo Default is: 0
echo.
echo Right now:
grep \"NSWPAT\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p D="(0), (1)"
if "%D%" == "" (
    set D=0
)
sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%D%\"/g" "%_ROSBE_BASEDIR%\config8.rbuild" > "%_ROSBE_BASEDIR%\config9.rbuild"
cls

echo Whether to compile with the KD protocol. This will disable support for
echo KDBG as well as rossym and symbol lookups, and allow WinDBG to connect
echo to ReactOS. This is currently not fully working, and requires kdcom
echo from Windows 2003 or TinyKRNL. Booting into debug mode with this flag
echo enabled will result in a failure to enter GUI mode. Do not enable
echo unless you know what you're doing.
echo Default is: 0
echo.
echo Right now:
grep \"_WINKD_\" "%_ROSBE_BASEDIR%\config.rbuild"|cut -d "\"" -f 4
set /p F="(0), (1)"
if "%F%" == "" (
    set F=0
)
sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%F%\"/g" "%_ROSBE_BASEDIR%\config9.rbuild" > "%_ROSBE_BASEDIR%\config10.rbuild"

::
:: Generate a config.rbuild, copy it to the Source Tree and delete temp files.
::
copy "%_ROSBE_BASEDIR%\config10.rbuild" "%_ROSBE_BASEDIR%\config.tmp"
del "%_ROSBE_BASEDIR%\*.rbuild"
ren "%_ROSBE_BASEDIR%\config.tmp" "config.rbuild"
copy "%_ROSBE_BASEDIR%\config.rbuild" "config.rbuild"

goto :NOK

:NOK
title ReactOS Build Environment %_ROSBE_VERSION%
