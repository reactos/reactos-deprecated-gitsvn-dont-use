::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/options.cmd
:: PURPOSE:     Starts options.exe and reboots RosBE at the End.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

title Options

"%_ROSBE_BASEDIR%\options.exe"
ECHO Press any Button to reset RosBE with the new Settings.
pause
"%_ROSBE_BASEDIR%\RosBE.cmd"
