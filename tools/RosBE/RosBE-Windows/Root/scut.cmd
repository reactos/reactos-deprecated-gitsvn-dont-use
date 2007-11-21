::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows.
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/scut.cmd
:: PURPOSE:     Manages named shortcuts to ReactOS source directories.
:: COPYRIGHT:   Copyright 2007 Colin Finck <mail@colinfinck.de>
::                             Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
@echo off

:: Check if the parameter is an scut parameter, which can just be forwarded to the EXE file
if /i "%1" == "add"    goto paramcall
if /i "%1" == "rem"    goto paramcall
if /i "%1" == "edit"   goto paramcall
if /i "%1" == "def"    goto paramcall
if /i "%1" == "list"   goto paramcall
if /i "%1" == "/?"     goto paramcall
if /i "%1" == "-h"     goto paramcall
if /i "%1" == "--help" goto paramcall

:: It's a shortcut name, so change the directory
for /f "usebackq tokens=*" %%i in (`""%_ROSBE_BASEDIR%\Tools\scut.exe" %*"`) do (
	if /i not "%%i" == "Default" (
		if exist "%%i\." (
  		cd /d %%i
  		set _ROSBE_ROSSOURCEDIR=%%i
  	)
  )
)
goto :EOC

:paramcall
"%_ROSBE_BASEDIR%\Tools\scut.exe" %*

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
