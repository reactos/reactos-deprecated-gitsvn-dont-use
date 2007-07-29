::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/sv.cmd
:: PURPOSE:     Integrated SVN Client, status step 2.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

echo Recent Offline Revision: %OFFSVN%
echo Online HEAD Revision: %ONSVN%
del "%ROSBEBASEDIR%\onsvn.tmp"
echo.
if %OFFSVN% LSS %ONSVN% (
    echo Your Tree is not Up to date. Do you want to update it?
    goto :UP
)
if %OFFSVN% EQU %ONSVN% (
echo Your Tree is Up to date.
)
goto :EOF

:UP
SET /P XY=(yes), (no)

if /I "%XY%"=="yes" call %ROSBEBASEDIR%\ssvn update
if /I "%XY%"=="no" goto :ExitSVN
