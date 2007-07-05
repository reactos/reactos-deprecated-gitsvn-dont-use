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
        %ROSBEBASEDIR%\Tools\svn.exe update %_ROSSOURCEDIR% -r %2
    ) else (
        %ROSBEBASEDIR%\Tools\svn.exe update %_ROSSOURCEDIR%
    )
goto :ExitSVN
)
if "%1" == "cleanup" (
    title Cleaning...
    %ROSBEBASEDIR%\Tools\svn.exe cleanup %_ROSSOURCEDIR%
    goto :ExitSVN
)
::
:: Check if the Folder is empty. If not, output an error.
::

if "%1" == "create" (
    title Creating...
    if exist "%_ROSSOURCEDIR%\.svn\." (
        echo Folder already cotains a Reposority. Exiting
        goto :ExitSVN
    )
    dir /b %_ROSSOURCEDIR% 2>nul|"%ROSBEBASEDIR%\Tools\grep.exe" -e ".*" >nul
    if errorlevel 1 (
        %ROSBEBASEDIR%\Tools\svn.exe checkout svn://svn.reactos.org/reactos/trunk/reactos %_ROSSOURCEDIR%
    ) else (
        echo Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :ExitSVN
)
if "%1" == "status" (
    title Status
    call "%ROSBEBASEDIR%\Tools\svn.exe" info %_ROSSOURCEDIR% > "%ROSBEBASEDIR%\x.tmp"
    echo Recent Offline Revision:
    call "%ROSBEBASEDIR%\Tools\grep.exe" Revision: %ROSBEBASEDIR%\x.tmp|"%ROSBEBASEDIR%\Tools\cut.exe" -d " " -f 2
    del "%ROSBEBASEDIR%\x.tmp"

    call "%ROSBEBASEDIR%\Tools\svn.exe" info svn://svn.reactos.org/reactos/trunk/reactos > "%ROSBEBASEDIR%\x.tmp"
    echo Online HEAD Revision:
    call "%ROSBEBASEDIR%\Tools\grep.exe" Revision: %ROSBEBASEDIR%\x.tmp|"%ROSBEBASEDIR%\Tools\cut.exe" -d " " -f 2
    del "%ROSBEBASEDIR%\x.tmp"
    goto :ExitSVN
)

if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)

:ExitSVN
title ReactOS Build Environment 0.3.7
