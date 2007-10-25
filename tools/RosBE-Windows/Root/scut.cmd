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
if /i "%1"=="" (
    goto :DEF
)
if /i "%1"=="add" (
    goto :ADD
)
if /i "%1"=="rem" (
    goto :REM
)
if /i "%1"=="edit" (
    goto :EDIT
)
if /i "%1"=="def" (
    goto :DEF2
)
if /i "%1"=="list" (
    if /i "%2"=="" (
        echo Shortcuts:
        for /f "usebackq tokens=1-2 delims=, skip=1" %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do echo %%i
    )
    if /i "%2"=="path" (
        echo Shortcuts, Paths:
        for /f "usebackq tokens=1-2 delims=, skip=1" %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do echo %%i, %%j
    )
    goto :END
)
if /i "%1"=="run" (
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
set /p XY="Choose your Shortcut: "

:RUN
if /i "%XY%" == "" (
    echo ERROR: You must enter a valid Shortcut.
    goto :END
)
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
set /p CUT="Choose your Shortcut: "
if /i "%CUT%" == "" (
    echo ERROR: You must enter a valid Shortcut.
    goto :END
)

:ADD2
set /p DIR="Choose your Source Folder: "

:ADD23
if not exist "%DIR%\." (
    echo ERROR: The path specified doesn't seem to exist.
    goto :END
)
if /i "%DIR%" == "" (
    echo ERROR: You must enter a valid directory.
    goto :END
)
for /f "usebackq tokens=1-2 delims=, skip=1" %%i in (`type "%_ROSBE_BASEDIR%\srclist.txt"`) do (
    if %%i == %CUT% (
        echo A Shortcut with the name %CUT% does already exist. Editing the existing one.
        call "%_ROSBE_BASEDIR%\scut.cmd" edit %CUT% %DIR%
        goto :END
    )
)
echo %CUT%,%DIR%>> "%_ROSBE_BASEDIR%\srclist.txt"
echo Shortcut %CUT% successfully added.
goto :END

::
:: Remove Shortcut from txt.
::
:REM
if not "%2" == "" (
    set CUTREM=%2
    goto :REM1
)
set /p CUTREM="Choose your Shortcut: "
if /i "%CUTREM%" == "" (
    echo ERROR: You must enter a valid Shortcut.
    goto :END
)

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
) else (
    echo Shortcut %CUTREM% successfully deleted.
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
set /p DIR="Choose your new Default Shortcut: "
if /i "%DIR%" == "" (
    echo ERROR: You must enter a valid Shortcut.
    goto :END
)

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
set /p CUT="Choose your Shortcut: "
if /i "%CUT%" == "" (
    echo ERROR: You must enter a valid Shortcut.
    goto :END
)

:EDIT2
set /p DIR="Choose your Source Folder: "

:EDIT23
if not exist "%DIR%\." (
    echo ERROR: The path specified doesn't seem to exist.
    goto :END
)
if /i "%DIR%" == "" (
    echo ERROR: You must enter a valid directory.
    goto :END
)
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
) else (
    echo Shortcut %CUT% successfully edited.
)

:END
title ReactOS Build Environment %_ROSBE_VERSION%
