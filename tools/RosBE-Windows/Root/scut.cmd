::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/scut.cmd
:: PURPOSE:     Shortcut Creator, Remover and Switcher.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Choose your Source Folder...

if /I "%1"=="" (
    goto :DEF
)
if /I "%1"=="add" (
    goto :ADD
)
if /I "%1"=="remove" (
    goto :REM
)
) else (
    set XY=%1
    goto :RUN
)
goto :END

:DEF
echo Choose your Source Folder:
SET /P XY=
goto :RUN

:RUN
for /f "usebackq" %%i in (`"grep \"%XY%\" "%ROSBEBASEDIR%\srclist.xml"|cutz dir"`) do @SET dir=%%i
cd /D %dir%
goto :END

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
SET /P CUT=
:ADD2
echo Choose your Source Folder:
SET /P DIR=
:ADD23
echo ^<property name="%CUT%" value="%DIR%" /^> > "%ROSBEBASEDIR%\aaa.tmp"
copy "%ROSBEBASEDIR%\srclist.xml" + "%ROSBEBASEDIR%\aaa.tmp" "%ROSBEBASEDIR%\srclist2.xml"
del "%ROSBEBASEDIR%\srclist.xml"
ren "%ROSBEBASEDIR%\srclist2.xml" srclist.xml
goto :END

:REM
if not "%2" == "" (
    set CUTREM=%2
    goto :REM1
)
echo Choose your Shortcut:
SET /P CUTREM=
:REM1
grep -v \"%CUTREM%\" "%ROSBEBASEDIR%\srclist.xml" > "%ROSBEBASEDIR%\srclist2.xml"
del "%ROSBEBASEDIR%\srclist.xml"
ren "%ROSBEBASEDIR%\srclist2.xml" srclist.xml
goto :END

:END
title ReactOS Build Environment %_VER%
