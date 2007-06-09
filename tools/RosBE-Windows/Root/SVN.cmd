:: Copyright (c) Peter Ward and Daniel Reimer. All rights reserved.
::
:: SVN Client
::
@echo off

::
:: Execute the Build command
::
if "%1" == "update" (
    title Updating...
    %ROSBEBASEDIR%\Tools\svn.exe update %_ROSSOURCEDIR% %2
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
if exist "%_ROSSOURCEDIR%\.svn\." (
    echo Folder already cotains a Reposority. Exiting
    goto :ExitSVN
)
if exist "%_ROSSOURCEDIR%\*.*" (
    echo Folder is not empty. Continuing is dangerous and can cause errors.
    goto :SVN
)
)

:SVN
%ROSBEBASEDIR%\Tools\svn.exe checkout svn://svn.reactos.org/reactos/trunk/reactos %_ROSSOURCEDIR%
goto :ExitSVN

:ExitSVN
title ReactOS Build Environment 0.3.7
pause
