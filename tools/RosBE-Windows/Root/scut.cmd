::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/scut.cmd
:: PURPOSE:     Shortcut Creator, Remover and Switcher.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Choose your Source Folder...

::
:: Receive the first Parameter and decide what to do.
::
if /I "%1"=="" (
    goto :DEF
)
if /I "%1"=="add" (
    goto :ADD
)
if /I "%1"=="rem" (
    goto :REM
)
if /I "%1"=="edit" (
    goto :EDIT
)
if /I "%1"=="def" (
    goto :DEF2
)
if /I "%1"=="run" (
    for /f "usebackq tokens=1-2 delims=," %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
        if %%i == Base (
            echo Active Shortcut: %%j
            echo.
            ::
            : Set new source directory, if needed.
            ::
            if not "%%j" == "Default" (
                call "%_ROSBE_BASEDIR%\scut.cmd" %%j
            )
        )
    )
    goto :END
) else (
    set XY=%1
    goto :RUN
)
goto :END

::
:: Load Shortcut from txt and set it active.
::
:DEF
echo Choose your Source Folder:
set /P XY=
:RUN
for /f "usebackq tokens=1-2 delims=, skip=1" %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
    if %%i == %XY% (
        cd /d %%j
        goto :END
    )
)
echo ERROR: The Shortcut %XY% does not exist.
goto :END

::
:: Add new Shortcut to txt.
::
:ADD
if not "%2" == "" (
    if not "%3" == "" (
        set CUT=%2
        set DIR=%3
        goto :ADD23
    ) else (
        set CUT=%2
        goto :ADD2
    )
) else (
    goto :ADD1
)

:ADD1
echo Choose your Shortcut:
set /P CUT=
:ADD2
echo Choose your Source Folder:
set /P DIR=
:ADD23
for /f "usebackq tokens=1-2 delims=, skip=1" %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
    if %%i == %CUT% (
        echo ERROR: A Shortcut with the name %CUT% does already exist.
        goto :END
    )
)
echo %CUT%,%DIR%>> "%_ROSBE_BASEDIR%\srclist.txt"
goto :END

::
:: Remove Shortcut from txt.
::
:REM
if not "%2" == "" (
    set CUTREM=%2
    goto :REM1
)
echo Choose your Shortcut:
set /P CUTREM=
:REM1
set _ROSBE_REM=0
for /f "usebackq tokens=1-2 delims=," %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
    if not %%i == %CUTREM% (
        echo %%i,%%j>> "%_ROSBE_BASEDIR%\srclist2.txt"
    ) else (
        set _ROSBE_REM=1
    )
)
del "%_ROSBE_BASEDIR%\srclist.txt"
ren "%_ROSBE_BASEDIR%\srclist2.txt" srclist.txt
if %_ROSBE_REM% == 0 (
    echo ERROR: The Shortcut %CUTREM% does not exist.
)
goto :END


::
:: Set Default Shortcut.
::
:DEF2
if not "%2" == "" (
    set DIR=%2
    goto :DEF1
)
echo Choose your new Default Shortcut:
set /P DIR=
:DEF1
call "%_ROSBE_BASEDIR%\scut.cmd" edit Base %DIR%

::
:: Edit a existing Shortcut.
::
:EDIT
if not "%2" == "" (
    if not "%3" == "" (
        set CUT=%2
        set DIR=%3
        goto :EDIT23
    ) else (
        set CUT=%2
        goto :EDIT2
    )
) else (
    goto :EDIT1
)

:EDIT1
echo Choose your Shortcut:
set /P CUT=
:EDIT2
echo Choose your Source Folder:
set /P DIR=
:EDIT23
set _ROSBE_EDIT=0
for /f "usebackq tokens=1-2 delims=," %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
    if %%i == %CUT% (
        set _ROSBE_EDIT=1
        if exist "%_ROSBE_BASEDIR%\temporaryfile.tmp" (
            echo %%i,%DIR%>> "%_ROSBE_BASEDIR%\temporaryfile.tmp"
        ) else (
            echo %%i,%DIR%> "%_ROSBE_BASEDIR%\temporaryfile.tmp"
        )
    ) else (
        if exist "%_ROSBE_BASEDIR%\temporaryfile.tmp" (
            echo %%i,%%j>> "%_ROSBE_BASEDIR%\temporaryfile.tmp"
        ) else (
            echo %%i,%%j> "%_ROSBE_BASEDIR%\temporaryfile.tmp"
        )
    )
)
del "%_ROSBE_BASEDIR%\srclist.txt"
ren "%_ROSBE_BASEDIR%\temporaryfile.tmp" srclist.txt
if %_ROSBE_EDIT% == 0 (
    echo ERROR: The Shortcut %CUT% does not exist.
)

:END
title ReactOS Build Environment %_ROSBE_VERSION%
