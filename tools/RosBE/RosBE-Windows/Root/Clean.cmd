::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Clean.cmd
:: PURPOSE:     Clean the ReactOS source directory.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
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

if "%_ROSBE_OBJPATH%" == "" (
    if not "%ROS_ARCH%" == "" (
        set _ROSBE_OBJCLEANPATH=obj-%ROS_ARCH%
    ) else (
        set _ROSBE_OBJCLEANPATH=obj-i386
    )
) else (
    set _ROSBE_OBJCLEANPATH=%_ROSBE_OBJPATH%
)

if "%_ROSBE_OUTPATH%" == "" (
    if not "%ROS_ARCH%" == "" (
        set _ROSBE_OUTCLEANPATH=output-%ROS_ARCH%
    ) else (
        set _ROSBE_OUTCLEANPATH=output-i386
    )
) else (
    set _ROSBE_OUTCLEANPATH=%_ROSBE_OUTPATH%
)

if not "%ROS_ARCH%" == "" (
    set _ROSBE_MAKEFILE=makefile-%ROS_ARCH%.auto
) else (
    set _ROSBE_MAKEFILE=makefile.auto
)

if exist "%_ROSBE_OBJCLEANPATH%\." (
    echo Cleaning ReactOS %ROS_ARCH% source directory...
    if exist "%_ROSBE_OBJCLEANPATH%\." (
        rd /s /q "%_ROSBE_OBJCLEANPATH%" 1> NUL 2> NUL
    )
    if exist "%_ROSBE_OUTCLEANPATH%\." (
        rd /s /q "%_ROSBE_OUTCLEANPATH%" 1> NUL 2> NUL
    )
    if exist "%_ROSBE_MAKEFILE%" (
        del "%_ROSBE_MAKEFILE%" 1> NUL 2> NUL
    )
    echo Done cleaning ReactOS %ROS_ARCH% source directory.
) else (
    echo ERROR: There is no %ROS_ARCH% compiler output to clean.
)

:ROS

if exist "reactos\." (
    rd /s /q "reactos" 1> NUL 2> NUL
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
