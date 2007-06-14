::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/SVN.cmd
:: PURPOSE:     Integrated SVN Client
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
if "%1" == "create" (
    title Creating...
    if not exist "%_ROSSOURCEDIR%\." (
        mkdir %_ROSSOURCEDIR%
        goto :SVN
    )
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)

::
:: Check if the Folder is empty. If not, output an error.
::
if exist "%_ROSSOURCEDIR%\.svn\." (
    echo Folder already cotains a Reposority. Exiting
    goto :ExitSVN
)
if exist "%_ROSSOURCEDIR%\*.*" (
    echo Folder is not empty. Continuing is dangerous and can cause errors.
    echo Press Strg+C to abort.
    pause
    goto :SVN
)

:SVN
%ROSBEBASEDIR%\Tools\svn.exe checkout svn://svn.reactos.org/reactos/trunk/reactos %_ROSSOURCEDIR%
goto :ExitSVN

:ExitSVN
title ReactOS Build Environment 0.3.7
