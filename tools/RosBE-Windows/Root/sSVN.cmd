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
    for /f "usebackq" %%i IN (`svnversion .`) DO @set OFFSVN=%%i
    svn info svn://svn.reactos.org/reactos/trunk/reactos|find "Revision:"|cutz svn > "%ROSBEBASEDIR%\onsvn.tmp"
    set /P ONSVN=< "%ROSBEBASEDIR%\onsvn.tmp"
    call "%ROSBEBASEDIR%\sSVNs.cmd"
    goto :ExitSVN
)

if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)

:ExitSVN
title ReactOS Build Environment %_VER%
