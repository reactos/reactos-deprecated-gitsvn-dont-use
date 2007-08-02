@ECHO OFF

REM Copyright (C) 2006-2007 Hervé Poussineau (hpoussin@reactos.org)

SET WINE_TMPFILE1=tmpfile1.wine

IF "%1" == "" GOTO help
IF "%1" == "download" GOTO download
IF "%1" == "createrbuild" GOTO createrbuild
IF "%1" == "link" GOTO link
IF "%1" == "merge" GOTO merge
IF "%1" == "make" GOTO make
IF "%1" == "fullprocessing" GOTO fullprocessing

:help
ECHO Syntax:
ECHO %0 download {wine_lib} [{wine_lib} ...]
ECHO %0 createrbuild {wine_lib} [{wine_lib} ...]
ECHO %0 link {path_to_reactos} {wine_lib}
ECHO %0 merge {path_to_reactos} {wine_lib} [{wine_lib} ...]
ECHO %0 make {path_to_reactos} {wine_lib} [{wine_lib} ...]
ECHO %0 fullprocessing {path_to_reactos} {wine_lib} [{wine_lib} ...]
GOTO :eof

REM ****************************************************************************

:download
IF "%2" == "" GOTO help
SET CVSROOT=:pserver:cvs:cvs@cvs.winehq.org:/home/wine
cvs.exe login >NUL
IF ERRORLEVEL 2 (
	ECHO Error when executing cvs.exe. Try to download the lastest version at
	ECHO http://ftp.gnu.org/non-gnu/cvs/binary/stable/x86-woe/
)
:startdownload
ECHO Downloading %2 ...
IF EXIST "wine\dlls\%2\Makefile.in" goto download_update
>NUL 2>NUL cvs.exe -z 3 checkout wine/dlls/%2
IF EXIST "wine\dlls\%2\Makefile.in" goto download_next

ECHO Error when downloading %2. Are you sure it does exist?
COLOR 00
GOTO :eof
:download_update
>NUL 2>NUL cvs.exe -z 3 update -PA wine/dlls/%2
:download_next
SHIFT
IF NOT "%2" == "" GOTO startdownload
>NUL 2>NUL cvs.exe logout
GOTO :eof

REM ****************************************************************************

:createrbuild
SETLOCAL ENABLEEXTENSIONS
SET WINE_BATCH=%0
IF "%2" == "" GOTO help
:startcreaterbuild
SET WINE_RELATIVE_WINE=wine\dlls\%2
CALL :internal_createrbuild %2
SET WINE_RELATIVE_WINE=wine\dlls\%2\tests
IF EXIST "%WINE_RELATIVE_WINE%" CALL :internal_createrbuild %2_winetest
SHIFT
IF NOT "%2" == "" GOTO startcreaterbuild
GOTO :eof

:internal_createrbuild
REM Syntax: internal_createrbuild {module_name}
REM Relative path to Wine DLL source code must be in %WINE_RELATIVE_WINE%
REM Name of the script must be in %WINE_BATCH%
REM .rbuild file is printed to standard output
SETLOCAL ENABLEDELAYEDEXPANSION
SET WINE_MODULE_NAME=%1

IF NOT EXIST "%WINE_RELATIVE_WINE%\Makefile.in" (
	ECHO WARNING: Did you forget "%WINE_BATCH% download %WINE_MODULE_NAME%"?
	GOTO :eof
)
IF "%WINE_MODULE_NAME:~-4%" == ".ocx" (
	SET WINE_INSTALL_NAME=%WINE_MODULE_NAME%
	SET WINE_SHORT_NAME=%WINE_MODULE_NAME:~0,-4%
	SET WINE_MODULE_TYPE=win32ocx
	SET WINE_INSTALL_BASE=system32
	SET WINE_IS_EXE=0
) ELSE IF "%WINE_MODULE_NAME:~-9%" == "_winetest" (
	SET WINE_INSTALL_NAME=%WINE_MODULE_NAME%.exe
	SET WINE_SHORT_NAME=%WINE_MODULE_NAME%
	SET WINE_MODULE_TYPE=win32cui
	SET WINE_INSTALL_BASE=bin
	SET WINE_IS_EXE=1
) ELSE (
	SET WINE_INSTALL_NAME=%WINE_MODULE_NAME%.dll
	SET WINE_SHORT_NAME=%WINE_MODULE_NAME%
	SET WINE_MODULE_TYPE=win32dll
	SET WINE_INSTALL_BASE=system32
	SET WINE_IS_EXE=0
)

SET WINE_HAS_DLLMAIN=0
SET WINE_HAS_DLLINSTALL=0
SET WINE_HAS_DLLREGISTERSERVER=0
IF EXIST "%WINE_RELATIVE_WINE%\%WINE_MODULE_NAME%.spec" (
	FOR /F "usebackq tokens=3,4 delims= " %%l IN ("%WINE_RELATIVE_WINE%\%WINE_MODULE_NAME%.spec") DO (
		IF NOT "%%l" == "" CALL :internal_analysespec %%l
		IF NOT "%%m" == "" CALL :internal_analysespec %%m
	)
)
>NUL COPY /Y "%WINE_RELATIVE_WINE%\Makefile.in" %WINE_TMPFILE1%

SET WINE_OPTION1=
SET WINE_OPTION2=
IF "%WINE_HAS_DLLMAIN%" == "0" (
	IF "%WINE_IS_EXE%" == "0" (
		SET WINE_OPTION1= entrypoint="0"
	)
)
IF "%WINE_IS_EXE%" == "0" (
	SET WINE_LOWER=abcdefghijklmnopqrstuvwxyz0123456789_.
	SET WINE_UPPER=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.
	SET WINE_LINE=%WINE_SHORT_NAME%
	SET WINE_UPPERCASE=
	FOR /L %%i IN (0, 1, 13) DO (
		FOR /L %%j IN (0, 1, 37) DO (
			IF "!WINE_LOWER:~%%j,1!" == "!WINE_LINE:~%%i,1!" (
				SET WINE_UPPERCASE=!WINE_UPPERCASE!!WINE_UPPER:~%%j,1!
			)
		)
	)
	SET WINE_OPTION2= baseaddress="${BASEADDRESS_!WINE_UPPERCASE!}"
)
ECHO ^<module name="%WINE_SHORT_NAME%" type="%WINE_MODULE_TYPE%"%WINE_OPTION2% installbase="%WINE_INSTALL_BASE%" installname="%WINE_INSTALL_NAME%" allowwarnings="true"%WINE_OPTION1%^>
IF "%WINE_HAS_DLLINSTALL%" == "1" (
	IF "%WINE_HAS_DLLREGISTERSERVER%" == "1" (
		ECHO 	^<autoregister infsection="OleControlDlls" type="Both" /^>
	) ELSE (
		ECHO 	^<autoregister infsection="OleControlDlls" type="DllInstall" /^>
	)
) ELSE (
	IF "%WINE_HAS_DLLREGISTERSERVER%" == "1" (
		ECHO 	^<autoregister infsection="OleControlDlls" type="DllRegisterServer" /^>
	)
)
IF EXIST "%WINE_RELATIVE_WINE%\%WINE_MODULE_NAME%.spec" (
	ECHO 	^<importlibrary definition="%WINE_MODULE_NAME%.spec.def" /^>
)
ECHO 	^<include base="%WINE_SHORT_NAME%"^>.^</include^>
IF "%WINE_IS_EXE%" == "0" (
	ECHO 	^<include base="ReactOS"^>include/reactos/wine^</include^>
	ECHO 	^<define name="__REACTOS__" /^>
	ECHO 	^<define name="__WINESRC__" /^>
)
ECHO 	^<define name="__USE_W32API" /^>
ECHO 	^<define name="_WIN32_IE"^>0x600^</define^>
ECHO 	^<define name="_WIN32_WINNT"^>0x501^</define^>
ECHO 	^<define name="WINVER"^>0x501^</define^>
ECHO 	^<library^>wine^</library^>
SET WINE_FULL_LINE=
SET WINE_END_PREC_LINE=
SET WINE_VARTYPE=0
SET WINE_HAS_NTDLL=
SET WINE_HAS_IDL=
FOR /F "eol=# delims=" %%l IN (%WINE_TMPFILE1%) DO (
	CALL :internal_analyseline %%l
)
IF NOT "%WINE_HAS_IDL%" == "" (
	ECHO 	^<include base="%WINE_MODULE_NAME%" root="intermediate"^>.^</include^>
)
IF "%WINE_IS_EXE%" == "1" (
	ECHO 	^<file^>testlist.c^</file^>
)
IF EXIST "%WINE_RELATIVE_WINE%\%WINE_MODULE_NAME%.spec" (
	ECHO 	^<file^>%WINE_MODULE_NAME%.spec^</file^>
)
ECHO ^</module^>
DEL %WINE_TMPFILE1%
GOTO :eof

:internal_analyseline
SET WINE_LINE=%*
SET WINE_END_LINE=%WINE_LINE:~-1%
IF NOT "%WINE_END_PREC_LINE%" == "\" (
	IF NOT "%WINE_VARTYPE%" == "0" (
		FOR /F "tokens=2* delims= " %%h IN ("%WINE_FULL_LINE%") DO (
			FOR %%j IN (%%i) DO (
				IF "%WINE_VARTYPE%" == "1" (
					IF "!WINE_HAS_NTDLL!" == "" (
						ECHO 	^<library^>ntdll^</library^>
						SET WINE_HAS_NTDLL=1
					)
					ECHO 	^<file^>%%j^</file^>
				) ELSE IF "%WINE_VARTYPE%" == "2" (
					ECHO 	^<library^>%%j^</library^>
					IF "%%j" == "ntdll" SET WINE_HAS_NTDLL=1
				) ELSE (
					IF "%%j" == "-luuid" ECHO 	^<library^>uuid^</library^>
				)
			)
		)
	)
	SET WINE_FULL_LINE=
	IF "%1" == "C_SRCS" (
		SET WINE_VARTYPE=1
	) ELSE IF "%1" == "RC_SRCS" (
		SET WINE_VARTYPE=1
	) ELSE IF "%1" == "CTESTS" (
		SET WINE_VARTYPE=1
	) ELSE IF "%1" == "IDL_H_SRCS" (
		SET WINE_VARTYPE=1
		SET WINE_HAS_IDL=1
	) ELSE IF "%1" == "IMPORTS" (
		SET WINE_VARTYPE=2
	) ELSE IF "%1" == "DELAYIMPORTS" (
		SET WINE_VARTYPE=2
	) ELSE IF "%1" == "EXTRALIBS" (
		SET WINE_VARTYPE=3
	) ELSE (
		SET WINE_VARTYPE=0
	)
)
SET WINE_END_PREC_LINE=%WINE_END_LINE%
IF "%WINE_END_PREC_LINE%" == "\" (
	SET WINE_FULL_LINE=!WINE_FULL_LINE!!WINE_LINE:~0,-1!
) ELSE (
	SET WINE_FULL_LINE=!WINE_FULL_LINE!!WINE_LINE!
)
GOTO :eof

:internal_analysespec
SET WINE_LINE=%*
IF "%WINE_LINE:~0,7%" == "DllMain" SET WINE_HAS_DLLMAIN=1
IF "%WINE_LINE:~0,10%" == "DllInstall" SET WINE_HAS_DLLINSTALL=1
IF "%WINE_LINE:~0,17%" == "DllRegisterServer" SET WINE_HAS_DLLREGISTERSERVER=1
goto :eof


REM ****************************************************************************

:link
IF "%3" == "" GOTO help
IF NOT "%4" == "" GOTO help
IF NOT EXIST "%2\ReactOS.rbuild" (
	ECHO %2\ReactOS.rbuild doesn't exit.
	GOTO :help
)
SET WINE_RELATIVE_WINE=wine\dlls\%3
SET WINE_RELATIVE_ROS=%2\dll\win32\%3
IF NOT EXIST "%WINE_RELATIVE_WINE%" (
	ECHO %WINE_RELATIVE_WINE% doesn't exist.
	ECHO Did you forget "%WINE_BATCH% download %3"?
	GOTO :eof
)
IF NOT EXIST "%WINE_RELATIVE_WINE%\%3.rbuild" (
	ECHO %WINE_RELATIVE_WINE%\%3.rbuild doesn't exist.
	ECHO Did you forget "%WINE_BATCH% createrbuild %3 >%WINE_RELATIVE_WINE%\%3.rbuild"?
	GOTO :eof
)
>NUL 2>NUL junction.exe "%WINE_RELATIVE_ROS%.wine" "%WINE_RELATIVE_WINE%\%3"
IF ERRORLEVEL 1 (
	ECHO Error when executing junction.exe. Try to download the lastest version at
	ECHO http://www.sysinternals.com/Utilities/Junction.html
)
GOTO :eof

REM ****************************************************************************

:merge
IF "%3" == "" GOTO help
IF NOT EXIST "%2\ReactOS.rbuild" (
	ECHO %2\ReactOS.rbuild doesn't exit.
	GOTO :help
)
SET WINE_ROS_DIR=%2
:mergenext
SHIFT
IF "%2" == "" GOTO :eof
SET WINE_RELATIVE_WINE=wine\dlls\%2
SET WINE_RELATIVE_ROS=%WINE_ROS_DIR%\dll\win32\%2
CALL :internal_merge %2
SET WINE_RELATIVE_WINE=wine\dlls\%2\tests
SET WINE_RELATIVE_ROS=%WINE_ROS_DIR%\modules\rostests\winetests\%2
IF EXIST "%WINE_RELATIVE_WINE%\." (
	IF EXIST "%WINE_ROS_DIR%\modules\rostests\winetests\." (
		CALL :internal_create_testlist
		CALL :internal_merge %2
	) ELSE (
		ECHO WARNING: tests for %2 will NOT be updated!
	)
)
GOTO mergenext

:internal_merge
IF NOT EXIST "%WINE_RELATIVE_WINE%" GOTO :eof
2>NUL MKDIR "%WINE_RELATIVE_ROS%"
ATTRIB -R "%WINE_RELATIVE_ROS%\*" >NUL
COPY /Y "%WINE_RELATIVE_WINE%\*.*" "%WINE_RELATIVE_ROS%" >NUL
SET WINE_FILES_DELETED=
FOR /F "delims=" %%f IN ('DIR /B "%WINE_RELATIVE_ROS%\*.*"') DO (
	IF "%%f" == ".cvsignore" (
		svn.exe delete --force "%WINE_RELATIVE_ROS%\%%f" 2>NUL
		DEL /Q "%WINE_RELATIVE_ROS%\%%f" 2>NUL
	) ELSE IF "%%f" == "Makefile.in" (
		svn.exe delete --force "%WINE_RELATIVE_ROS%\%%f" 2>NUL
		DEL /Q "%WINE_RELATIVE_ROS%\%%f" 2>NUL
	) ELSE IF NOT EXIST "%WINE_RELATIVE_WINE%\%%f" (
		SET WINE_FILE=%%f
		IF NOT "!WINE_FILE:~-9!" == "_ros.diff" (
			svn.exe delete "%WINE_RELATIVE_ROS%\%%f" 2>NUL
			IF ERRORLEVEL 2 GOTO :helpsvn
			IF EXIST "%WINE_RELATIVE_ROS%\%%f" DEL /Q "%WINE_RELATIVE_ROS%\%%f"
			IF EXIST "%WINE_RELATIVE_ROS%\%%f" RD /S /Q "%WINE_RELATIVE_ROS%\%%f"
			SET WINE_FILES_DELETED=!WINE_FILES_DELETED! %%f
		)
	) ELSE (
		svn.exe add "%WINE_RELATIVE_ROS%\%%f" 2>NUL
		IF ERRORLEVEL 2 GOTO :helpsvn
		svn.exe propset svn:eol-style native "%WINE_RELATIVE_ROS%\%%f" >NUL 2>NUL
		SET >NUL
	)
)
SET WINE_PATCH_FILE=%1_ros.diff
IF EXIST "%WINE_RELATIVE_ROS%\%WINE_PATCH_FILE%" (
	svn.exe add "%WINE_RELATIVE_ROS%\%WINE_PATCH_FILE%" 2>NUL
	IF ERRORLEVEL 2 GOTO :helpsvn
	svn.exe propset svn:eol-style native "%WINE_RELATIVE_ROS%\%WINE_PATCH_FILE%" >NUL 2>NUL
	PUSHD "%WINE_RELATIVE_ROS%"
	patch.exe -p0 -N < "%WINE_PATCH_FILE%"
	POPD
	IF ERRORLEVEL 1 (
		ECHO Error when executing patch.exe. Try to download the lastest version at
		ECHO http://gnuwin32.sourceforge.net/packages/patch.htm
		GOTO :eof
	)
	FOR %%f IN (%WINE_FILES_DELETED%) DO (
		IF EXIST "%WINE_RELATIVE_ROS%\%%f" (
			REN "%WINE_RELATIVE_ROS%\%%f" "%WINE_TMPFILE1%"
			svn.exe revert -q "%WINE_RELATIVE_ROS%\%%f" 2>NUL
			DEL "%WINE_RELATIVE_ROS%\%%f"
			REN "%WINE_RELATIVE_ROS%\%WINE_TMPFILE1%" "%%f"
		)
	)
)
GOTO :eof

:internal_create_testlist
SET WINE_TESTLIST=%WINE_RELATIVE_WINE%\testlist.c
SET WINE_FULL_LINE=
SET WINE_END_PREC_LINE=
SET WINE_VARTYPE=0
SET WINE_TESTS=
FOR /F "eol=# delims=" %%l IN (%WINE_RELATIVE_WINE%\Makefile.in) DO (
	CALL :internal_findtest %%l
)
SETLOCAL DISABLEDELAYEDEXPANSION
>%WINE_TESTLIST% ECHO /* Automatically generated file; DO NOT EDIT!! */
SETLOCAL ENABLEDELAYEDEXPANSION
>>%WINE_TESTLIST% ECHO.
>>%WINE_TESTLIST% ECHO #define WIN32_LEAN_AND_MEAN
>>%WINE_TESTLIST% ECHO #include ^<windows.h^>
>>%WINE_TESTLIST% ECHO.
>>%WINE_TESTLIST% ECHO #define STANDALONE
>>%WINE_TESTLIST% ECHO #include "wine/test.h"
>>%WINE_TESTLIST% ECHO.
FOR %%t IN (%WINE_TESTS%) DO (
	>>%WINE_TESTLIST% ECHO extern void func_%%t(void^);
)
>>%WINE_TESTLIST% ECHO.
>>%WINE_TESTLIST% ECHO const struct test winetest_testlist[] =
>>%WINE_TESTLIST% ECHO {
FOR %%t IN (%WINE_TESTS%) DO (
	>>%WINE_TESTLIST% ECHO     { "%%t", func_%%t },
)
>>%WINE_TESTLIST% ECHO     { 0, 0 }
>>%WINE_TESTLIST% ECHO };
GOTO :eof

:internal_findtest
SET WINE_LINE=%*
SET WINE_END_LINE=%WINE_LINE:~-1%
IF NOT "%WINE_END_PREC_LINE%" == "\" (
	IF NOT "%WINE_VARTYPE%" == "0" (
		FOR /F "tokens=2* delims= " %%h IN ("%WINE_FULL_LINE%") DO (
			FOR %%j IN (%%i) DO (
				IF "%WINE_VARTYPE%" == "1" (
					SET WINE_CURRENT=%%j
					SET WINE_TESTS=!WINE_TESTS! !WINE_CURRENT:~0,-2!
				)
			)
		)
	)
	SET WINE_FULL_LINE=
	IF "%1" == "CTESTS" (
		SET WINE_VARTYPE=1
	) ELSE (
		SET WINE_VARTYPE=0
	)
)
SET WINE_END_PREC_LINE=%WINE_END_LINE%
IF "%WINE_END_PREC_LINE%" == "\" (
	SET WINE_FULL_LINE=!WINE_FULL_LINE!!WINE_LINE:~0,-1!
) ELSE (
	SET WINE_FULL_LINE=!WINE_FULL_LINE!!WINE_LINE!
)
GOTO :eof

:helpsvn
ECHO Error when executing svn.exe. Try to download the lastest version at
ECHO http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91
GOTO :eof

REM ****************************************************************************

:make
SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
IF "%3" == "" GOTO help
IF NOT EXIST "%2\ReactOS.rbuild" (
	ECHO %2\ReactOS.rbuild doesn't exit.
	GOTO :help
)
SET WINE_LIST=
SET WINE_ROS_DIR=%2
:make_filllist
SHIFT
IF "%2" == "" GOTO make_doit
SET WINE_MODULE_NAME=%2
IF "%WINE_MODULE_NAME:~-4%" == ".ocx" (
	SET WINE_MODULE_NAME=%WINE_MODULE_NAME:~0,-4%
)
SET WINE_LIST=%WINE_LIST% %WINE_MODULE_NAME%
GOTO make_filllist
:make_doit
PUSHD "%WINE_ROS_DIR%"
make.exe %WINE_LIST%
POPD
GOTO :eof

REM ****************************************************************************

:fullprocessing
SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
IF "%3" == "" GOTO help
SET WINE_BATCH=%0
SET WINE_LIST=
SET WINE_ROS_DIR=%2
:fullprocessing_filllist
SHIFT
IF "%2" == "" GOTO fullprocessing_download
SET WINE_LIST=%WINE_LIST% %2
GOTO fullprocessing_filllist
:fullprocessing_download
CALL :download download%WINE_LIST%
IF ERRORLEVEL 1 GOTO :eof
SET WINE_NEW_LIST=%WINE_LIST%
FOR %%m IN (%WINE_LIST%) DO (
	SET WINE_RELATIVE_WINE=wine\dlls\%%m
	>!WINE_RELATIVE_WINE!\%%m.rbuild CALL :internal_createrbuild %%m
	SET WINE_RELATIVE_WINE=wine\dlls\%%m\tests
	IF EXIST "!WINE_RELATIVE_WINE!\" (
		IF NOT EXIST "%WINE_ROS_DIR%\modules\rostests\winetests\." (
			ECHO Unable to find rostests module. Please checkout it.
			GOTO :eof
		)
		>!WINE_RELATIVE_WINE!\%%m.rbuild CALL :internal_createrbuild %%m_winetest
		SET WINE_NEW_LIST=!WINE_NEW_LIST! %%m_winetest
	)
	>NUL CALL :link link "%WINE_ROS_DIR%" %%m
)
CALL :merge merge "%WINE_ROS_DIR%" %WINE_LIST%
IF ERRORLEVEL 1 GOTO :eof
IF EXIST "%WINE_ROS_DIR%\makefile.auto" DEL "%WINE_ROS_DIR%\makefile.auto"
CALL :make make "%WINE_ROS_DIR%" %WINE_NEW_LIST%
IF ERRORLEVEL 1 GOTO :eof
ECHO Compilation successful. You should try to run ReactOS to see if
ECHO no visible regressions appeared before committing the changes.
FOR %%m IN (%WINE_NEW_LIST%) DO (
	REM svn.exe commit "%WINE_ROS_DIR%\dll\win32\%%m" -m "Autosyncing with Wine HEAD" --username LOGIN --password PASSWORD --non-interactive
)
