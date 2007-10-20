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
"%_ROSBE_BASEDIR%\RosBE.cmd"
