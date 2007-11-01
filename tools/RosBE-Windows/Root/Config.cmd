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
if /i "%1" == "delete" (
    echo config.rbuild will be permanently deleted. All your settings will be gone.
    echo Continue?
    set /p YESNO="(yes), (no)"
    if /i "%YESNO%"=="yes" goto :CONT
    if /i "%YESNO%"=="no" goto :NOK
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
if /i "%1" == "update" (
    echo old config.rbuild will be deleted and will be updated with a recent,
    echo default one. You will need to reconfigure it to your wishes later.
    echo Continue?
    set /p YESNO="(yes), (no)"
    if /i "%YESNO%"=="yes" goto :CONT2
    if /i "%YESNO%"=="no" goto :NOK
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
:: Help prevent non-useful bug reports/questions.
::
echo.
echo *** Configurations other than release/debug are not useful for ***
echo *** posting bug reports, and generally not very useful for     ***
echo *** IRC/Forum discussion. Please refrain from doing so unless  ***
echo *** you are sure about what you are doing.                     ***
echo.

set /p YESNO="(yes), (no)"

if /i "%YESNO%"=="yes" goto :OK
if /i "%YESNO%"=="no" goto :NOK
goto :NOK

:OK

::
:: Check if config.template.rbuild is newer than config.rbuild, if it is then
:: inform the user and offer an update.
::
setlocal enabledelayedexpansion
if exist ".\config.rbuild" (
    chknewer ".\config.template.rbuild" ".\config.rbuild"
    if !errorlevel! == 1 (
        echo.
        echo *** config.template.rbuild is newer than working config.rbuild ***
        echo *** The Editor cannot continue with this file. Do you wanna    ***
        echo *** update to the most recent one? You need to reset all your  ***
        echo *** previously made settings.                                  ***
        echo.
        set /p YESNO="(yes), (no)"
        if /i "%YESNO%"=="yes" del "%_ROSBE_BASEDIR%\*.rbuild" | del "config.rbuild" | copy "config.template.rbuild" "%_ROSBE_BASEDIR%\config.rbuild" | goto :OK
        if /i "%YESNO%"=="no" goto :NOK
        goto :NOK
    )
)
endlocal

::
:: Start with reading settings from config.rbuild and let the user edit them.
::
echo Sub-Architecture to build for.
echo Default is: none
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "SARCH" | find "property name""`) do set SARCH=%%i
set SARCH=%SARCH:~7,-1%
echo Right now: %SARCH%
set /p SARCH_CH="(), (xbox)"
sed "s/\"SARCH\" value=\"\"/\"SARCH\" value=\"%SARCH_CH%\"/g;s/\"SARCH\" value=\"xbox\"/\"SARCH\" value=\"%SARCH_CH%\"/g" "%_ROSBE_BASEDIR%\config.rbuild" > "%TEMP%\config2.rbuild"
cls

echo Which CPU ReactOS should be optimized for.
echo.
echo Examples: i486, i586, pentium, pentium2, pentium3, pentium4,
echo athlon-xp, athlon-mp, k6-2
echo See GCC manual for more CPU names and which CPUs GCC can optimize for.
echo Default is: pentium
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "OARCH" | find "property name""`) do set OARCH=%%i
set OARCH=%OARCH:~7,-1%
echo Right now: %OARCH%
set /p OARCH_CH=
if "%OARCH_CH%" == "" (
    set OARCH_CH=pentium
)
sed "s/\"OARCH\" value=\".*\"/\"OARCH\" value=\"%OARCH_CH%\"/g" "%TEMP%\config2.rbuild" > "%TEMP%\config3.rbuild"
cls

echo What level do you want ReactOS to be optimized at.
echo This setting does not work if GDB is set.
echo 0 = off
echo 1 = Normal compiling. Recommended. It is the default setting in
echo official release builds and debug builds.
echo warning : 2,3,4,5 is not tested on ReactOS. Change at own risk.
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "OPTIMIZE" | find "property name""`) do set OPTIMIZE=%%i
set OPTIMIZE=%OPTIMIZE:~7,-1%
echo Right now: %OPTIMIZE%
set /p OPTIMIZE_CH="(0), (1), (2), (3), (4), (5)"
if "%OPTIMIZE_CH%" == "" (
    set OPTIMIZE_CH=1
)
sed "s/\"OPTIMIZE\" value=\"[0-5]\"/\"OPTIMIZE\" value=\"%OPTIMIZE_CH%\"/g" "%TEMP%\config3.rbuild" > "%TEMP%\config4.rbuild"
cls

echo Whether to compile for an uniprocessor or multiprocessor machine.
echo Default is: 0
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "MP" | find "property name""`) do set MP=%%i
set MP=%MP:~7,-1%
echo Right now: %MP%
set /p MP_CH="(0), (1)"
if "%MP_CH%" == "" (
    set MP_CH=0
)
sed "s/\"MP\" value=\"[0-1]\"/\"MP\" value=\"%MP_CH%\"/g" "%TEMP%\config4.rbuild" > "%TEMP%\config5.rbuild"
cls

echo Whether to compile in the integrated kernel debugger.
echo Default is: 0
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "KDBG" | find "property name""`) do set KDBG=%%i
set KDBG=%KDBG:~7,-1%
echo Right now: %KDBG%
set /p KDBG_CH="(0), (1)"
if "%KDBG_CH%" == "" (
    set KDBG_CH=0
)
sed "s/\"KDBG\" value=\"[0-1]\"/\"KDBG\" value=\"%KDBG_CH%\"/g" "%TEMP%\config5.rbuild" > "%TEMP%\config6.rbuild"
cls

echo Whether to compile for debugging. No compiler optimizations will be
echo performed.
echo Default is: 1
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "DBG" | find "property name" | find /V "KDBG""`) do set DBG=%%i
set DBG=%DBG:~7,-1%
echo Right now: %DBG%
set /p DBG_CH="(0), (1)"
if "%DBG_CH%" == "" (
    set DBG_CH=1
)
sed "s/\"DBG\" value=\"[0-1]\"/\"DBG\" value=\"%DBG_CH%\"/g" "%TEMP%\config6.rbuild" > "%TEMP%\config7.rbuild"
cls

echo Whether to compile for debugging with GDB. If you don't use GDB,
echo don't enable this.
echo Default is: 0
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "GDB" | find "property name""`) do set GDB=%%i
set GDB=%GDB:~7,-1%
echo Right now: %GDB%
set /p GDB_CH="(0), (1)"
if "%GDB_CH%" == "" (
    set GDB_CH=0
)
sed "s/\"GDB\" value=\"[0-1]\"/\"GDB\" value=\"%GDB_CH%\"/g" "%TEMP%\config7.rbuild" > "%TEMP%\config8.rbuild"
cls

echo Whether to compile apps/libs with features covered software patents
echo or not. If you live in a country where software patents are
echo valid/apply, don't enable this (except they/you purchased a license
echo from the patent owner).
echo Default is: 0
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "NSWPAT" | find "property name""`) do set NSWPAT=%%i
set NSWPAT=%NSWPAT:~7,-1%
echo Right now: %NSWPAT%
set /p NSWPAT_CH="(0), (1)"
if "%NSWPAT_CH%" == "" (
    set NSWPAT_CH=0
)
sed "s/\"NSWPAT\" value=\"[0-1]\"/\"NSWPAT\" value=\"%NSWPAT_CH%\"/g" "%TEMP%\config8.rbuild" > "%TEMP%\config9.rbuild"
cls

echo Whether to compile with the KD protocol. This will disable support for
echo KDBG as well as rossym and symbol lookups, and allow WinDBG to connect
echo to ReactOS. This is currently not fully working, and requires kdcom
echo from Windows 2003 or TinyKRNL. Booting into debug mode with this flag
echo enabled will result in a failure to enter GUI mode. Do not enable
echo unless you know what you're doing.
echo Default is: 0
echo.
for /f "usebackq tokens=3" %%i in (`"type "%_ROSBE_BASEDIR%\config.rbuild" | find "_WINKD_" | find "property name""`) do set WINKD=%%i
set WINKD=%WINKD:~7,-1%
echo Right now: %WINKD%
set /p WINKD_CH="(0), (1)"
if "%WINKD_CH%" == "" (
    set WINKD_CH=0
)
sed "s/\"_WINKD_\" value=\"[0-1]\"/\"_WINKD_\" value=\"%WINKD_CH%\"/g" "%TEMP%\config9.rbuild" > "%TEMP%\config10.rbuild"

::
:: Generate a config.rbuild, copy it to the Source Tree and delete temp files.
::
copy "%TEMP%\config10.rbuild" "%TEMP%\config.tmp"
del "%TEMP%\*.rbuild"
copy "%TEMP%\config.tmp" "%_ROSBE_BASEDIR%\config.rbuild"
del "%TEMP%\*.tmp"
copy "%_ROSBE_BASEDIR%\config.rbuild" "config.rbuild"

goto :NOK

:NOK
title ReactOS Build Environment %_ROSBE_VERSION%

::
:: Unload all used Vars.
::
set YESNO=
set SARCH_CH=
set OARCH_CH=
set OPTIMIZE_CH=
set MP_CH=
set KDBG_CH=
set DBG_CH=
set GDB_CH=
set NSWPAT_CH=
set WINKD_CH=
set SARCH=
set OARCH=
set OPTIMIZE=
set MP=
set KDBG=
set DBG=
set GDB=
set NSWPAT=
set WINKD=
