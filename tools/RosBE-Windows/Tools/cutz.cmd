::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Tools/cutz.cmd
:: PURPOSE:     Bypass a stupid bug in cut.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off
if "%1" == "svn" (
    cut -d " " -f 2
)
