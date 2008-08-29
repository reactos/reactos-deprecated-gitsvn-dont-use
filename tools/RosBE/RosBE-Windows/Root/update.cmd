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
set _ROSBE_PARAM=no

::
:: Update the Vars if the params say so.
::
if "%1" == "nocmds" (
    set _ROSBE_CMDS=no
    set _ROSBE_PARAM=yes
)
if "%1" == "nogcc" (
    set _ROSBE_GCC=no
    set _ROSBE_PARAM=yes
)
if "%1" == "notools" (
    set _ROSBE_TOOLS=no
    set _ROSBE_PARAM=yes
)
if not "%1" == "" (
    if %_ROSBE_PARAM% == no (
        cls
        echo Unknown parameter specified. Exiting.
        goto :EOU
    )
)

cd /d "%_ROSBE_BASEDIR%"

if %_ROSBE_CMDS% == yes (
    ::
    :: First check for a new Updater
    ::
    for %%F in (update.cmd) do set _ROSBE_UPDDATE=%%~tF
    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/update.cmd
    for %%F in (update.cmd) do set _ROSBE_UPDDATE2=%%~tF

    if !_ROSBE_UPDDATE! NEQ !_ROSBE_UPDDATE2! (
        cls
        echo Updater got updated and needs to be restarted.
        goto :EOU
    )
    ::
    :: PS1 Files.
    ::
    if exist "Build.ps1" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Build.ps1
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

    ::
    :: Options Files.
    ::
    if exist "options.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/options.cmd
    )

    ::
    :: SVN Files.
    ::
    if exist "sSVN.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/sSVN.cmd
    )

    ::
    :: SCut Files.
    ::
    if exist "scut.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/scut.cmd
    )

    ::
    :: RelAddr2Line Files.
    ::
    if exist "reladdr2line.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/reladdr2line.cmd
    )

    ::
    :: Other Tools Files.
    ::
    if exist "Config.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Config.cmd
    )
    if exist "chdefdir.cmd" (
        "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/chdefdir.cmd
    )

    ::
    :: Default Files.
    ::
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
    ::
    :: Add Dates into Vars and load GCC packages if needed.
    ::
    if exist GCC.7z (
        for %%F in (GCC.7z) do set _ROSBE_GCCDATE=%%~tF
    ) else (
        set _ROSBE_GCCDATE=0
    )

    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/GCC.7z

    ::
    :: Add the maybe Updated Dates to another Var.
    ::
    for %%F in (GCC.7z) do set _ROSBE_GCCDATE2=%%~tF

    ::
    :: Extract GCC.
    ::
    if !_ROSBE_GCCDATE! NEQ !_ROSBE_GCCDATE2! (
        "Tools\7z.exe" x GCC.7z "%_ROSBE_BASEDIR%\4.1.3"
    )
)
if %_ROSBE_TOOLS% == yes (
    ::
    :: Add Dates into Vars and load Tool SRC packages if needed.
    ::
    if exist Tools.7z (
        for %%F in (Tools.7z) do set _ROSBE_TOOLSDATE=%%~tF
    ) else (
        set _ROSBE_TOOOLSDATE=0
    )

    "Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/Tools.7z

    ::
    :: Add the maybe Updated Dates to another Var.
    ::
    for %%F in (Tools.7z) do set _ROSBE_TOOLSDATE2=%%~tF

    if !_ROSBE_TOOLSDATE! NEQ !_ROSBE_TOOLSDATE2! (
        "Tools\7z.exe" x Tools.7z "%TEMP%"
        ::
        :: Build the tools
        ::
        make -f %TEMP%\makefile
        copy %TEMP%\*.exe "%_ROSBE_BASEDIR%\Tools"
    )
)

:EOU

cd /d "%_ROSBE_OPATH%"

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
set _ROSBE_PARAM=
set _ROSBE_OPATH=
set _ROSBE_UPDDATE=
set _ROSBE_UPDDATE2=


if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)
