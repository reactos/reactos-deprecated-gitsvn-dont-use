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
    echo Unknown parameter specified. Try 'help [COMMAND]'.
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

if .%ROS_ARCH%. == .arm. (
    if exist "obj-arm\." (
        echo Cleaning ReactOS ARM source directory...
        if exist "obj-arm\." (
            rd /s /q "obj-arm" 1> NUL 2> NUL
        )
        if exist "output-arm\." (
            rd /s /q "output-arm" 1> NUL 2> NUL
        )
        if exist "makefile-arm.auto" (
            del "makefile-arm.auto" 1> NUL 2> NUL
        )
        echo Done cleaning ReactOS ARM source directory.
    ) else (
        echo ERROR: There is no compiler output to clean.
    )
    goto :ROS
)
if .%ROS_ARCH%. == .ppc. (
    if exist "obj-ppc\." (
        echo Cleaning ReactOS PPC source directory...
        if exist "obj-ppc\." (
            rd /s /q "obj-ppc" 1> NUL 2> NUL
        )
        if exist "output-ppc\." (
            rd /s /q "output-ppc" 1> NUL 2> NUL
        )
        if exist "makefile-ppc.auto" (
            del "makefile-ppc.auto" 1> NUL 2> NUL
        )
        echo Done cleaning ReactOS PPC source directory.
    ) else (
        echo ERROR: There is no compiler output to clean.
    )
    goto :ROS
)
if .%ROS_ARCH%. == .amd64. (
    if exist "obj-amd64\." (
        echo Cleaning ReactOS X86_64 source directory...
        if exist "obj-amd64\." (
            rd /s /q "obj-amd64" 1> NUL 2> NUL
        )
        if exist "output-amd64\." (
            rd /s /q "output-amd64" 1> NUL 2> NUL
        )
        if exist "makefile-amd64.auto" (
            del "makefile-amd64.auto" 1> NUL 2> NUL
        )
        echo Done cleaning ReactOS X86_64 source directory.
    ) else (
        echo ERROR: There is no compiler output to clean.
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
    echo ERROR: There is no compiler output to clean.
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
