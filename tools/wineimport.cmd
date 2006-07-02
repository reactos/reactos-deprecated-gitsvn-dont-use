@ECHO OFF

REM Copyright (C) 2006 Hervé Poussineau (hpoussin@reactos.org)

SET WINE_TMPFILE1=tmpfile1.wine

IF "%1" == "" GOTO help
IF "%1" == "download" GOTO download
IF "%1" == "process" GOTO process
IF "%1" == "link" GOTO link
IF "%1" == "merge" GOTO merge
IF "%1" == "fullprocessing" GOTO fullprocessing

:help
ECHO Syntax:
ECHO %0 download {wine_lib} [{wine_lib} ...]
ECHO %0 process {wine_lib} [{wine_lib} ...]
ECHO %0 link {path_to_reactos} {wine_lib}
ECHO %0 merge {path_to_reactos} {wine_lib} [{wine_lib} ...]
ECHO %0 fullprocessing {path_to_reactos} {wine_lib} [{wine_lib} ...]
GOTO :eof

:download
IF "%2" == "" GOTO help
SET CVSROOT=:pserver:cvs:cvs@cvs.winehq.org:/home/wine
cvs.exe login >NUL
IF ERRORLEVEL 2 (
	ECHO Error when executing cvs.exe. Try to download the lastest version at
	ECHO http://ftp.gnu.org/non-gnu/cvs/binary/stable/x86-woe/
)
IF ERRORLEVEL 1 GOTO :eof
:startdownload
ECHO Downloading %2 ...
>NUL 2>NUL cvs.exe -z 3 checkout wine/dlls/%2
SHIFT
IF NOT "%2" == "" GOTO startdownload
>NUL 2>NUL cvs.exe logout
GOTO :eof

:process
SETLOCAL ENABLEEXTENSIONS
SETLOCAL ENABLEDELAYEDEXPANSION
SET WINE_BATCH=%0
IF "%2" == "" GOTO help
:startprocess
IF NOT EXIST wine\dlls\%2\makefile.in (
	ECHO WARNING: Did you forget "%WINE_BATCH% download %2"?
	GOTO :processnext
)

SET WINE_LOWER=abcdefghijklmnopqrstuvwxyz0123456789
SET WINE_UPPER=ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789
SET WINE_LINE=%2
SET WINE_UPPERCASE=
FOR /L %%i IN (0, 1, 8) DO (
	FOR /L %%j IN (0, 1, 35) DO (
		IF "!WINE_LOWER:~%%j,1!" == "!WINE_LINE:~%%i,1!" (
			SET WINE_UPPERCASE=!WINE_UPPERCASE!!WINE_UPPER:~%%j,1!
		)
	)
)

>NUL COPY /Y wine\dlls\%2\makefile.in %WINE_TMPFILE1%

ECHO ^<module name="%2" type="win32dll" baseaddress="${BASEADDRESS_!WINE_UPPERCASE!}" installbase="system32" installname="%2.dll" allowwarnings="true"^>
ECHO 	^<importlibrary definition="%2.spec.def" /^>
ECHO 	^<include base="%2"^>.^</include^>
ECHO 	^<include base="ReactOS"^>include/reactos/wine^</include^>
ECHO 	^<define name="__REACTOS__" /^>
ECHO 	^<define name="__WINESRC__" /^>
ECHO 	^<define name="__USE_W32API" /^>
ECHO 	^<define name="_WIN32_IE"^>0x600^</define^>
ECHO 	^<define name="_WIN32_WINNT"^>0x501^</define^>
ECHO 	^<define name="WINVER"^>0x501^</define^>
ECHO 	^<library^>wine^</library^>
ECHO 	^<library^>ntdll^</library^>
SET WINE_FULL_LINE=
SET WINE_END_PREC_LINE=
SET WINE_VARTYPE=0
FOR /F "eol=# delims=" %%l IN (%WINE_TMPFILE1%) DO (
	CALL :internal_analyseline %%l
)
ECHO 	^<file^>%2.spec^</file^>
ECHO ^</module^>

:processnext
SHIFT
IF NOT "%2" == "" GOTO startprocess
DEL %WINE_TMPFILE1%
GOTO :eof

:link
IF "%3" == "" GOTO help
IF NOT "%4" == "" GOTO help
IF NOT EXIST "%2\ReactOS.rbuild" (
	ECHO %2\ReactOS.rbuild doesn't exit.
	GOTO :help
)
IF NOT EXIST "wine\dlls\%3" (
	ECHO wine\dlls\%3 doesn't exist.
	ECHO Did you forget "%WINE_BATCH% download %3"?
	GOTO :eof
)
IF NOT EXIST "wine\dlls\%3\%3.rbuild" (
	ECHO wine\dlls\%3.rbuild doesn't exist.
	ECHO Did you forget "%WINE_BATCH% process %3 >wine\dlls\%3\%3.rbuild"?
	GOTO :eof
)
>NUL 2>NUL junction.exe "%2\dll\win32\%3.wine" "wine\dlls\%3"
IF ERRORLEVEL 1 (
	ECHO Error when executing junction.exe. Try to download the lastest version at
	ECHO http://www.sysinternals.com/Utilities/Junction.html
)
GOTO :eof

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
IF NOT EXIST "wine\dlls\%2" GOTO :mergenext
COPY /Y "wine\dlls\%2\*.*" "%WINE_ROS_DIR%\dll\win32\%2" >NUL
FOR /F "delims=" %%f IN ('DIR /B "%WINE_ROS_DIR%\dll\win32\%2\*.*"') DO (
	IF "%%f" == ".cvsignore" (
		DEL "%WINE_ROS_DIR%\dll\win32\%2\%%f"
	) ELSE IF "%%f" == "Makefile.in" (
		DEL "%WINE_ROS_DIR%\dll\win32\%2\%%f"
	) ELSE IF NOT EXIST "wine\dlls\%2\%%f" (
		SET WINE_FILE=%%f
		IF NOT "%WINE_FILE:~-9" == "_ros.diff" (
			svn.exe delete "%WINE_ROS_DIR%\dll\win32\%2\%%f" 2>NUL
			IF ERRORLEVEL 2 GOTO :helpsvn
		)
	) ELSE (
		svn.exe add "%WINE_ROS_DIR%\dll\win32\%2\%%f" 2>NUL
		IF ERRORLEVEL 2 GOTO :helpsvn
		svn.exe propset svn:eol-style native "%WINE_ROS_DIR%\dll\win32\%2\%%f" >NUL 2>NUL
	)
)
IF EXIST "%WINE_ROS_DIR%\dll\win32\%2\%2_ros.diff" (
	svn.exe add "%WINE_ROS_DIR%\dll\win32\%2\%2_ros.diff" 2>NUL
	IF ERRORLEVEL 2 GOTO :helpsvn
	svn.exe propset svn:eol-style native "%WINE_ROS_DIR%\dll\win32\%2\%2_ros.diff" >NUL 2>NUL
	PUSHD "%WINE_ROS_DIR%"
	patch.exe -p0 -N < "dll\win32\%2\%2_ros.diff"
	POPD
	IF ERRORLEVEL 1 (
		ECHO Error when executing patch.exe. Try to download the lastest version at
		ECHO http://unxutils.sourceforge.net/
		GOTO :eof
	)
)
GOTO :mergenext
:helpsvn
ECHO Error when executing svn.exe. Try to download the lastest version at
ECHO http://subversion.tigris.org/servlets/ProjectDocumentList?folderID=91
GOTO :eof

:fullprocessing
SETLOCAL ENABLEEXTENSIONS
IF "%3" == "" GOTO help
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
FOR %%m IN (%WINE_LIST%) DO (
	>wine\dlls\%%m\%%m.rbuild CALL :process process %%m
	CALL :link link "%WINE_ROS_DIR%" %%m
	IF ERRORLEVEL 1 GOTO :eof
)
CALL :merge merge "%WINE_ROS_DIR%" %WINE_LIST%
IF ERRORLEVEL 1 GOTO :eof
IF EXIST "%WINE_ROS_DIR%\makefile.auto" DEL "%WINE_ROS_DIR%\makefile.auto"
PUSHD "%WINE_ROS_DIR%"
make.exe %WINE_LIST%
POPD
GOTO :eof

:internal_analyseline
SET WINE_LINE=%*
SET WINE_END_LINE=%WINE_LINE:~-1%
IF NOT "%WINE_END_PREC_LINE%" == "\" (
	IF NOT "%WINE_VARTYPE%" == "0" (
		FOR /F "tokens=2* delims= " %%h IN ("%WINE_FULL_LINE%") DO (
			FOR %%j IN (%%i) DO (
				IF "%WINE_VARTYPE%" == "1" (
					ECHO 	^<file^>%%j^</file^>
				) ELSE IF "%WINE_VARTYPE%" == "2" (
					ECHO 	^<library^>%%j^</library^>
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
