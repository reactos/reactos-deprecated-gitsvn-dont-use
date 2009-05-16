::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Buildflags.cmd
:: PURPOSE:     Sets the RBuild
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion

if /i "%1" == "verbose" (
    REM Be verbose.
    set ROS_RBUILDFLAGS=-v
) else if /i "%1" == "lowhddspace" (
    REM Clean as you go. Delete generated files as soon as they are not needed anymore.
    set ROS_RBUILDFLAGS=-c
) else if /i "%1" == "noautodep" (
    REM Disable automatic dependencies.
    set ROS_RBUILDFLAGS=-dd
) else if /i "%1" == "autodep" (
    REM Enable automatic dependencies.
    set ROS_RBUILDFLAGS=-da
) else if /i "%1" == "fullautodep" (
    REM Enable full dependencies.
    set ROS_RBUILDFLAGS=-df
) else if /i "%1" == "depforx" (
    REM Check only automatic dependencies for this module.
    set ROS_RBUILDFLAGS=-dm{%2}
) else if /i "%1" == "noprecompheaders" (
    REM Disable precompiled headers.
    set ROS_RBUILDFLAGS=-hd
) else if /i "%1" == "makegendir" (
    REM Let make handle creation of install directories. Rbuild will not generate the directories.
    set ROS_RBUILDFLAGS=-mi
) else if /i "%1" == "proxymakefiles" (
    REM Generate proxy makefiles in source tree instead of the output tree.
    set ROS_RBUILDFLAGS=-ps
) else if /i "%1" == "nocompunits" (
    REM Disable compilation units.
    set ROS_RBUILDFLAGS=-ud
) else if /i "%1" == "inputxml" (
    REM Input XML
    set ROS_RBUILDFLAGS=-r
) else if /i "%1" == "installpath" (
    REM This variable controls where to install output files to when using
    REM 'make install'. N.B. Don't put a path separator at the end. The variable
    REM defaults to .\{ROS_CDOUTPUT}.
    set ROS_INSTALL=%2
) else if /i "%1" == "buildengine" (
    REM The Build engine to be used. The variable defaults to rbuild (RBUILD_TARGET)
    set ROS_BUILDENGINE=%2
) else if /i "%1" == "reset" (
    set ROS_RBUILDFLAGS=
    set ROS_INSTALL=
    set ROS_BUILDENGINE=
)
endlocal & set ROS_RBUILDFLAGS=%ROS_RBUILDFLAGS%& set ROS_INSTALL=%ROS_INSTALL%& set ROS_BUILDENGINE=%ROS_BUILDENGINE%
