::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/sSVN.cmd
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
::
:: These two are directly parsed to svn.
::
if /i "%1" == "update" (
    title Updating...
    if not "%2" == "" (
        svn update -r %2
    ) else (
        svn update
    )
goto :ExitSVN
)
if /i "%1" == "cleanup" (
    title Cleaning...
    svn cleanup
    goto :ExitSVN
)
::
:: Check if the Folder is empty. If not, output an error.
::

if /i "%1" == "create" (
    title Creating...
    if exist ".svn\." (
        echo ERROR: Folder already cotains a Reposority. Exiting
        goto :ExitSVN
    )
    dir /b 2>nul|findstr "." >nul
    if errorlevel 1 (
        svn checkout svn://svn.reactos.org/reactos/trunk/reactos
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :ExitSVN
)
::
:: Output the rev of your and the Online Tree and tell the User if
:: its Up to Date or not.
::
if /i "%1" == "status" (
    title Status
    for /f "usebackq tokens=2" %%i in (`"svn info | find "Revision:""`) do set OFFSVN=%%i
    for /f "usebackq tokens=2" %%j in (`"svn info svn://svn.reactos.org/reactos/trunk/reactos | find "Revision:""`) do set ONSVN=%%j
    call :UP
    goto :ExitSVN
)

if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :ExitSVN
)

:UP
echo Recent Offline Revision: %OFFSVN%
echo Online HEAD Revision: %ONSVN%
echo.
if %OFFSVN% lss %ONSVN% (
    echo Your Tree is not Up to date. Do you want to update it?
    goto :UP2
)
if %OFFSVN% equ %ONSVN% (
    echo Your Tree is Up to date.
    goto :ExitSVN
)

:UP2
set /p XY="(yes), (no)"
if /i "%XY%"=="yes" %_ROSBE_BASEDIR%\ssvn update
if /i "%XY%"=="no" goto :ExitSVN

:ExitSVN
title ReactOS Build Environment %_ROSBE_VERSION%
