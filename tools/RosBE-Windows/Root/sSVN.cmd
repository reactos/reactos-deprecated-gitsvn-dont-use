::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/SVN.cmd
:: PURPOSE:     Integrated SVN Client.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

::
:: Receive the first Parameter and decide what to do.
::
if "%1" == "" (
    echo No parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)
if "%1" == "update" (
    title Updating...
    if not "%2" == "" (
        svn update "%CD%" -r %2
    ) else (
        svn update "%CD%"
    )
goto :ExitSVN
)
if "%1" == "cleanup" (
    title Cleaning...
    svn cleanup "%CD%"
    goto :ExitSVN
)
::
:: Check if the Folder is empty. If not, output an error.
::

if "%1" == "create" (
    title Creating...
    if exist "%CD%\.svn\." (
        echo Folder already cotains a Reposority. Exiting
        goto :ExitSVN
    )
    dir /b "%CD%" 2>nul|findstr "." >nul
    if errorlevel 1 (
        svn checkout svn://svn.reactos.org/reactos/trunk/reactos "%CD%"
    ) else (
        echo Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :ExitSVN
)
if "%1" == "status" (
    title Status
    call svn info "%CD%" > "%ROSBEBASEDIR%\x.tmp"
    call grep Revision: "%ROSBEBASEDIR%\x.tmp"|cutz svn > "%ROSBEBASEDIR%\offsvn.tmp"
    set /P OFFSVN=< "%ROSBEBASEDIR%\offsvn.tmp"
    call svn info svn://svn.reactos.org/reactos/trunk/reactos > "%ROSBEBASEDIR%\y.tmp"
    call grep Revision: "%ROSBEBASEDIR%\y.tmp"|cutz svn > "%ROSBEBASEDIR%\onsvn.tmp"
    set /P ONSVN=< "%ROSBEBASEDIR%\onsvn.tmp"
    echo Recent Offline Revision: %OFFSVN%
    echo Online HEAD Revision: %ONSVN%
    del "%ROSBEBASEDIR%\offsvn.tmp"
    del "%ROSBEBASEDIR%\onsvn.tmp"
    del "%ROSBEBASEDIR%\x.tmp"
    del "%ROSBEBASEDIR%\y.tmp"
    echo.
    if %OFFSVN% LSS %ONSVN% (
        echo Your Tree is not Up to date. Do you want to update it?
        goto :UP
    )
    if %OFFSVN% EQU %ONSVN% (
    echo Your Tree is Up to date.
    )
    goto :ExitSVN
)

if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)

:UP
SET /P XY=(yes), (no)

if /I "%XY%"=="yes" call %ROSBEBASEDIR%\ssvn update
if /I "%XY%"=="no" goto :ExitSVN

:ExitSVN
title ReactOS Build Environment %_VER%
