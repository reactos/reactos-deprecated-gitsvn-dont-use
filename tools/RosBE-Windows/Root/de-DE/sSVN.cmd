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
    echo Unbekannter Parameter. Versuchen Sie 'help [BEFEHL]'.
    goto :ExitSVN
)
::
:: These two are directly parsed to svn.
::
if "%1" == "update" (
    title Aktualisiert...
    if not "%2" == "" (
        svn update -r %2
    ) else (
        svn update
    )
goto :ExitSVN
)
if "%1" == "cleanup" (
    title Aufräumen...
    svn cleanup
    goto :ExitSVN
)
::
:: Check if the Folder is empty. If not, output an error.
::

if "%1" == "create" (
    title Erstellt...
    if exist ".svn\." (
        echo Dieser Ordner enthält bereits einen Source Code. Anwendung wird beendet.
        goto :ExitSVN
    )
    dir /b 2>nul|findstr "." >nul
    if errorlevel 1 (
        svn checkout svn://svn.reactos.org/reactos/trunk/reactos
    ) else (
        echo Dieser Ordner ist nicht leer. Fortsetzen ist riskant und kann Fehler verursachen. Anwendung wird beendet.
    )
    goto :ExitSVN
)
::
:: Output the rev of your and the Online Tree and tell the User if
:: its Up to Date or not.
::
if "%1" == "status" (
    title Status
    for /f "usebackq" %%i IN (`svnversion .`) DO @set OFFSVN=%%i
    for /f "usebackq" %%j IN (`"svn info svn://svn.reactos.org/reactos/trunk/reactos|find "Revision:"|cutz svn"`) DO @set ONSVN=%%j
    call :UP
    goto :ExitSVN
)

if not "%1" == "" (
    echo Unbekannter Parameter. Versuchen Sie 'help [BEFEHL]'.
    goto :ExitSVN
)

:UP
echo Derzeitige Offline Revision: %OFFSVN%
echo Online HEAD Revision: %ONSVN%
echo.
if %OFFSVN% LSS %ONSVN% (
    echo Ihr Code ist nicht aktuell. Wollen Sie ihn aktualisieren?
    goto :UP2
)
if %OFFSVN% EQU %ONSVN% (
    echo Ihr Code ist aktuell.
    goto :ExitSVN
)

:UP2
SET /P XY="(yes), (no)"
if /I "%XY%"=="yes" %ROSBEBASEDIR%\ssvn update
if /I "%XY%"=="no" goto :ExitSVN

:ExitSVN
title ReactOS Build Environment %_VER%
