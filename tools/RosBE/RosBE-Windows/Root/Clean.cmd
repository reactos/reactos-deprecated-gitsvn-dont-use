::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Clean.cmd
:: PURPOSE:     Clean the ReactOS source directory.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

title Cleaning...

if "%1" == "" (
    call :DEL
    goto :EOC
)
if /i "%1" == "logs" (
    call :LOG
    goto :EOC
)
if /i "%1" == "all" (
    call :DEL
    call :LOG
    goto :EOC
)
if not "%1" == "" (
    "%_ROSBE_BASEDIR%\Build.cmd" %1_clean
    goto :EOC
)

::
:: Check if we have any logs to clean, if so, clean them.
::

:LOG

if exist "%_ROSBE_LOGDIR%\*.txt" (
    echo Cleaning build logs...
    del /f "%_ROSBE_LOGDIR%\*.txt" 1> NUL 2> NUL
    echo Done cleaning build logs.
) else (
    echo ERROR: There are no logs to clean.
)
goto :EOF

::
:: Check if we have something to clean, if so, clean it.
::

:DEL

::
:: Apply modified obj and out paths for deletion.
::

if "%ROS_ARCH%" == "" (
    set ROS_ARCH=i386
)

if "%_ROSBE_OBJPATH%" == "" (
    set _ROSBE_OBJCLEANPATH=%_ROSBE_ROSSOURCEDIR%\obj-%ROS_ARCH%
) else (
    set _ROSBE_OBJCLEANPATH=%_ROSBE_OBJPATH%
)

if "%_ROSBE_OUTPATH%" == "" (
    set _ROSBE_OUTCLEANPATH=%_ROSBE_ROSSOURCEDIR%\output-%ROS_ARCH%
) else (
    set _ROSBE_OUTCLEANPATH=%_ROSBE_OUTPATH%
)

if "%ROS_ARCH%" == "i386" (
    set _ROSBE_MAKEFILE=%_ROSBE_ROSSOURCEDIR%\makefile.auto
) else (
    set _ROSBE_MAKEFILE=%_ROSBE_ROSSOURCEDIR%\makefile-%ROS_ARCH%.auto
)

if exist "%_ROSBE_MAKEFILE%" (
    del "%_ROSBE_MAKEFILE%" 1> NUL 2> NUL
)

if exist "%_ROSBE_OBJCLEANPATH%\." (
    echo Cleaning ReactOS %ROS_ARCH% source directory...
    if exist "%_ROSBE_OBJCLEANPATH%\." (
        rd /s /q "%_ROSBE_OBJCLEANPATH%" 1> NUL 2> NUL
    )
    if exist "%_ROSBE_OUTCLEANPATH%\." (
        rd /s /q "%_ROSBE_OUTCLEANPATH%" 1> NUL 2> NUL
    )
    echo Done cleaning ReactOS %ROS_ARCH% source directory.
) else (
    echo ERROR: There is no %ROS_ARCH% compiler output to clean.
)

if "%ROS_ARCH%" == "i386" (
    set ROS_ARCH=
)

:ROS

if exist "%_ROSBE_ROSSOURCEDIR%\reactos\." (
    rd /s /q "%_ROSBE_ROSSOURCEDIR%\reactos" 1> NUL 2> NUL
)
goto :EOF

:EOC

if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

::
:: Unload all used Vars.
::
set _ROSBE_OBJCLEANPATH=
set _ROSBE_OUTCLEANPATH=
set _ROSBE_MAKEFILE=
