::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/Basedir.cmd
:: PURPOSE:     Temporarily change the Basedir in RosBE.
:: COPYRIGHT:   Copyright 2009 Colin Finck <colin@reactos.org>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

@cd /d "%_ROSBE_ROSSOURCEDIR%"
