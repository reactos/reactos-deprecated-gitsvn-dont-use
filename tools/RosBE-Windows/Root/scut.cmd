::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/scut.cmd
:: PURPOSE:     Shortcut Creator, Remover and Switcher.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Choose your Source Folder...

::
:: Receive the first Parameter and decide what to do.
::
if /I "%1"=="" (
    goto :DEF
)
if /I "%1"=="add" (
    goto :ADD
)
if /I "%1"=="rem" (
    goto :REM
)
if /I "%1"=="def" (
    goto :DEF2
) else (
    set XY=%1
    goto :RUN
)
goto :END

::
:: Load Shortcut from XML and set it active.
::
:DEF
echo Choose your Source Folder:
SET /P XY=
:RUN
grep name=\"%XY%\" "%_ROSBE_BASEDIR%\srclist.xml"|cutz dir > "%_ROSBE_BASEDIR%\aaa.tmp"
test -s "%_ROSBE_BASEDIR%\aaa.tmp"
if errorlevel 1 (
    echo Shortcut with that name does not exist.
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :END
) else (
    set /P dir=<"%_ROSBE_BASEDIR%\aaa.tmp"
    call cd /D %dir%
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :END
)

::
:: Add new Shortcut to XML.
::
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
echo ^<property name="%CUT%" value="%DIR%" /^> > "%_ROSBE_BASEDIR%\aaa.tmp"
copy "%_ROSBE_BASEDIR%\srclist.xml" + "%_ROSBE_BASEDIR%\aaa.tmp" "%_ROSBE_BASEDIR%\srclist2.xml"
del "%_ROSBE_BASEDIR%\srclist.xml"
ren "%_ROSBE_BASEDIR%\srclist2.xml" srclist.xml
del "%_ROSBE_BASEDIR%\aaa.tmp"
goto :END

::
:: Remove Shortcut from XML.
::
:REM
if not "%2" == "" (
    set CUTREM=%2
    goto :REM1
)
echo Choose your Shortcut:
SET /P CUTREM=
:REM1
grep name=\"%CUTREM%\" "%_ROSBE_BASEDIR%\srclist.xml"|cutz dir > "%_ROSBE_BASEDIR%\aaa.tmp"
test -s "%_ROSBE_BASEDIR%\aaa.tmp"
if errorlevel 1 (
    echo Shortcut with that name does not exist.
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :END
) else (
    grep -v name=\"%CUTREM%\" "%_ROSBE_BASEDIR%\srclist.xml" > "%_ROSBE_BASEDIR%\srclist2.xml"
    del "%_ROSBE_BASEDIR%\srclist.xml"
    ren "%_ROSBE_BASEDIR%\srclist2.xml" srclist.xml
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :END
)

::
:: Set Default Shortcut.
::
:DEF2
if not "%2" == "" (
    set DEF=%2
    goto :DEF1
)
echo Choose your new Default Shortcut:
SET /P DEF=
:DEF1
grep name=\"%DEF%\" "%_ROSBE_BASEDIR%\srclist.xml"|cutz dir > "%_ROSBE_BASEDIR%\aaa.tmp"
test -s "%_ROSBE_BASEDIR%\aaa.tmp"
if errorlevel 1 (
    echo Shortcut with that name does not exist.
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :END
) else (
    sed "s/base=\".*\"/base=\"%DEF%\"/g" "%_ROSBE_BASEDIR%\srclist.xml" > "%_ROSBE_BASEDIR%\srclist2.xml"
    del "%_ROSBE_BASEDIR%\srclist.xml"
    ren "%_ROSBE_BASEDIR%\srclist2.xml" srclist.xml
    del "%_ROSBE_BASEDIR%\aaa.tmp"
    goto :PREEND
)

:PREEND
call "%_ROSBE_BASEDIR%\RosBE.cmd"

:END
title ReactOS Build Environment %_ROSBE_VERSION%
