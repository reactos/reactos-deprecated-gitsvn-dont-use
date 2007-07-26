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
grep \"%XY%\" "%ROSBEBASEDIR%\srclist.xml"|cutz > "%ROSBEBASEDIR%\aaa.tmp"
set /P dir=< "%ROSBEBASEDIR%\aaa.tmp"
del "%ROSBEBASEDIR%\aaa.tmp"
cd %dir%

title ReactOS Build Environment %_VER%
