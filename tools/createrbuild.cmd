@ECHO OFF
REM Creates a .rbuild file using files in the current directory
REM Copyright 2007 H. Poussineau
REM Usage:
REM 1) Copy this script to the root dir of the module
REM 2) Run "createrbuild.cmd >module.rbuild"
REM 3) Update 2nd line (path to project.dtd)
REM 4) Update 3rd line (module parameters)
REM 5) Delete "createrbuild.cmd" ;)

SETLOCAL ENABLEDELAYEDEXPANSION
IF "%1" == "" (
	ECHO ^<?xml version="1.0"?^>
	ECHO ^<!DOCTYPE module SYSTEM "../../../tools/rbuild/project.dtd"^>
	ECHO ^<module name="" type="" installbase="" installname=""^>
	SET LEVEL=1
) ELSE (
	SET LEVEL=%1
)
FOR /F %%F IN ('DIR /B /AD') DO (
	SET INDENT=
	FOR /L %%i IN (1, 1, %LEVEL%) DO (
		SET INDENT=!INDENT!	
	)
	ECHO !INDENT!^<directory name="%%F"^>
	SET /A NEWLEVEL=%LEVEL%+1
	PUSHD %%F
	CALL ..\%0 !NEWLEVEL!
	POPD
	SET INDENT=
	FOR /L %%i IN (1, 1, %LEVEL%) DO (
		SET INDENT=!INDENT!	
	)
	ECHO !INDENT!^</directory^>
)
FOR /F %%F IN ('DIR /B /A-D 2^>NUL') DO (
	SET INDENT=
	FOR /L %%i IN (1, 1, %LEVEL%) DO (
		SET INDENT=!INDENT!	
	)
	IF NOT %LEVEL% == 1 (
		ECHO !INDENT!^<file^>%%F^</file^>
	) ELSE IF NOT "%%F" == "%0" (
		ECHO !INDENT!^<file^>%%F^</file^>
	)
)
IF "%1" == "" (
	ECHO ^</module^>
)
