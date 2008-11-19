::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

setlocal
setlocal enableextensions
setlocal enabledelayedexpansion

if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

::
:: Set Title
::
title Updating...

::
:: The Update Server.
::
set _ROSBE_URL=http://mitglied.lycos.de/reimerdaniel/rosbe

::
:: Default Variables.
::
set _ROSBE_OPATH=%~dp0
set _ROSBE_OPATH=%_ROSBE_OPATH:~0,-1%
set _ROSBE_CMDS=yes
set _ROSBE_GCC=yes
set _ROSBE_TOOLS=yes

::
:: Update the Vars if the params say so.
::
if "%1" == "" (
    goto :next
)
elseif "%1" == "nocmds" (
    set _ROSBE_CMDS=no
)
elseif "%1" == "nogcc" (
    set _ROSBE_GCC=no
)
elseif "%1" == "notools" (
    set _ROSBE_TOOLS=no
) else (
    cls
    echo Unknown first parameter specified. Exiting.
    goto :EOU
)

if "%2" == "" (
    goto :next
)
elseif "%2" == "nocmds" (
    set _ROSBE_CMDS=no
)
elseif "%2" == "nogcc" (
    set _ROSBE_GCC=no
)
elseif "%2" == "notools" (
    set _ROSBE_TOOLS=no
) else (
    cls
    echo Unknown second parameter specified. Exiting.
    goto :EOU
)

if "%3" == "" (
    goto :next
)
elseif "%3" == "nocmds" (
    set _ROSBE_CMDS=no
)
elseif "%3" == "nogcc" (
    set _ROSBE_GCC=no
)
elseif "%3" == "notools" (
    set _ROSBE_TOOLS=no
) else (
    cls
    echo Unknown third parameter specified. Exiting.
    goto :EOU
)

:next

cd /d "%_ROSBE_BASEDIR%"

if %_ROSBE_CMDS% == yes (

    REM First check for a new Updater

    for %%F in (update.cmd) do set _ROSBE_UPDDATE=%%~tF
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/update.cmd
    for %%F in (update.cmd) do set _ROSBE_UPDDATE2=%%~tF

    if !_ROSBE_UPDDATE! NEQ !_ROSBE_UPDDATE2! (
        cls
        echo Updater got updated and needs to be restarted.
        goto :EOU
    )

    REM PS1 Files.

    if exist "Build.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Build.ps1
    )
    if exist "chdefgcc.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/chdefgcc.ps1
    )
    if exist "Clean.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Clean.ps1
    )
    if exist "Help.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Help.ps1
    )
    if exist "MinGW.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/MinGW.ps1
    )
    if exist "RosBE.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/RosBE.ps1
    )
    if exist "rosbe-gcc-env.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/rosbe-gcc-env.ps1
    )

    REM Arch Changer Files.

    if exist "charch.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/charch.cmd
    )
    if exist "charch.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/charch.ps1
    )

    REM Options Files.

    if exist "options.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/options.cmd
    )
    if exist "options.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/options.ps1
    )

    REM SVN Files.

    if exist "sSVN.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/sSVN.cmd
    )
    if exist "sSVN.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/sSVN.ps1
    )

    REM SCut Files.

    if exist "scut.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/scut.cmd
    )
    if exist "scut.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/scut.ps1
    )

    REM RelAddr2Line Files.

    if exist "reladdr2line.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/reladdr2line.cmd
    )
    if exist "reladdr2line.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/reladdr2line.ps1
    )

    REM Other Tools Files.

    if exist "Config.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Config.cmd
    )
    if exist "Config.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Config.ps1
    )
    if exist "chdefdir.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/chdefdir.cmd
    )
    if exist "chdefdir.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/chdefdir.ps1
    )

    REM Default Files.

    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Build.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/ChangeLog.txt
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/chdefgcc.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Clean.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Help.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/LICENSE.TXT
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/MinGW.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/mingw.ico
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/MinGW.mac
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/README.pdf
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/RosBE.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/rosbe.ico
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/RosBE.mac
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/rosbe-gcc-env.cmd
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/TimeDate.cmd
)

if %_ROSBE_GCC% == yes (

    REM Add Dates into Vars and load GCC packages if needed.

    if exist GCC.7z (
        for %%F in (GCC.7z) do set _ROSBE_GCCDATE=%%~tF
    )

    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/GCC.7z

    REM Add the maybe Updated Dates to another Var.

    for %%F in (GCC.7z) do set _ROSBE_GCCDATE2=%%~tF

    REM Extract GCC.

    if !_ROSBE_GCCDATE! NEQ !_ROSBE_GCCDATE2! (
        "Tools\7z.exe" x GCC.7z "%_ROSBE_BASEDIR%\i386"
    )
)
if %_ROSBE_TOOLS% == yes (

    REM Add Dates into Vars and load Tool SRC packages if needed.

    if exist Tools.7z (
        for %%F in (Tools.7z) do set _ROSBE_TOOLSDATE=%%~tF
    )

    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Tools.7z

    REM Add the maybe Updated Dates to another Var.

    for %%F in (Tools.7z) do set _ROSBE_TOOLSDATE2=%%~tF

    if !_ROSBE_TOOLSDATE! NEQ !_ROSBE_TOOLSDATE2! (
        "Tools\7z.exe" x Tools.7z "%TEMP%"

        REM Build the tools

        make -f %TEMP%\makefile
        copy %TEMP%\*.exe "%_ROSBE_BASEDIR%\Tools"
    )
)

:EOU

cd /d "%_ROSBE_OPATH%"

if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

::
:: Unload Vars.
::
set _ROSBE_URL=
set _ROSBE_GCCDATE=
set _ROSBE_TOOLSDATE=
set _ROSBE_GCCDATE2=
set _ROSBE_TOOLSDATE2=
set _ROSBE_CMDS=
set _ROSBE_GCC=
set _ROSBE_TOOLS=
set _ROSBE_OPATH=
set _ROSBE_UPDDATE=
set _ROSBE_UPDDATE2=
