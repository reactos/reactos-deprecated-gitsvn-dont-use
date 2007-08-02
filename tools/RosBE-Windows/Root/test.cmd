::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/test.cmd
:: PURPOSE:     First step in the direction of a Multiple Source Tree "Compatible" RosBE.
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
grep \"%XY%\" "%ROSBEBASEDIR%\srclist.xml"|cutz dir > "%ROSBEBASEDIR%\aaa.tmp"
set /P dir=< "%ROSBEBASEDIR%\aaa.tmp"
del "%ROSBEBASEDIR%\aaa.tmp"
cd /D %dir%
goto :END

:ADD
echo Choose your Shortcut:
SET /P CUT=
echo Choose your Source Folder:
SET /P DIR=
echo ^<property name="%CUT%" value="%DIR%" /^> > "%ROSBEBASEDIR%\aaa.tmp"
copy "%ROSBEBASEDIR%\srclist.xml" + "%ROSBEBASEDIR%\aaa.tmp" "%ROSBEBASEDIR%\srclist2.xml"
del "%ROSBEBASEDIR%\srclist.xml"
ren "%ROSBEBASEDIR%\srclist2.xml" "srclist.xml"
goto :END

:REM
echo Choose your Shortcut:
SET /P CUTREM=
grep -v \"%CUTREM%\" "%ROSBEBASEDIR%\srclist.xml" > "%ROSBEBASEDIR%\srclist2.xml"
del "%ROSBEBASEDIR%\srclist.xml"
ren "%ROSBEBASEDIR%\srclist2.xml" "srclist.xml"
goto :END

:END
title ReactOS Build Environment %_VER%
