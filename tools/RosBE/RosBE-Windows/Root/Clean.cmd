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

if not "%ROS_ARCH%" == "" (
    if exist "obj-%ROS_ARCH%\." (
        echo Cleaning ReactOS %ROS_ARCH% source directory...
        if exist "obj-%ROS_ARCH%\." (
            rd /s /q "obj-%ROS_ARCH%" 1> NUL 2> NUL
        )
        if exist "output-%ROS_ARCH%\." (
            rd /s /q "output-%ROS_ARCH%" 1> NUL 2> NUL
        )
        if exist "makefile-%ROS_ARCH%.auto" (
            del "makefile-%ROS_ARCH%.auto" 1> NUL 2> NUL
        )
        echo Done cleaning ReactOS %ROS_ARCH% source directory.
    ) else (
        echo ERROR: There is no %ROS_ARCH% compiler output to clean.
    )
    goto :ROS
)

if exist "obj-i386\." (
    echo Cleaning ReactOS i386 source directory...
    if exist "obj-i386\." (
        rd /s /q "obj-i386" 1> NUL 2> NUL
    )
    if exist "output-i386\." (
        rd /s /q "output-i386" 1> NUL 2> NUL
    )
    if exist "makefile.auto" (
        del "makefile.auto" 1> NUL 2> NUL
    )
    echo Done cleaning ReactOS i386 source directory.
) else (
    echo ERROR: There is no i386 compiler output to clean.
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
