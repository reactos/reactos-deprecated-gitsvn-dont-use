@ECHO OFF

REM Copyright (C) 2006 Hervé Poussineau (hpoussin@reactos.org)

SET WINE_TMPFILE1=tmpfile1.wine
SET WINE_TMPFILE2=tmpfile2.wine
IF "%WINE_RESULTFILE%" == "" SET WINE_RESULTFILE=con

IF "%1" == "" GOTO help
IF "%1" == "download" GOTO download
IF "%1" == "process" GOTO process
IF "%1" == "internal_analyseline" GOTO internal_analyseline

:help
ECHO Syntax:
ECHO %0 download {wine_lib} [{wine_lib} ...]
ECHO %0 process {wine_lib} [{wine_lib} ...]
GOTO :eof

:download
IF "%2" == "" GOTO help
SET CVSROOT=:pserver:cvs@cvs.winehq.org:/home/wine
ECHO *** Password is "cvs" ***
cvs.exe login
:startdownload
ECHO Downloading %2 ...
>NUL 2>NUL cvs.exe -z 3 checkout wine/dlls/%2
SHIFT
IF NOT "%2" == "" GOTO startdownload
>NUL 2>NUL cvs.exe logout
GOTO :eof

:process
SET WINE_BATCH=%0
IF "%2" == "" GOTO help
:startprocess
IF NOT EXIST wine\dlls\%2\makefile.in (
	ECHO WARNING: Did you forget "%WINE_BATCH% download %2"?
	GOTO :processnext
)
>NUL COPY /Y wine\dlls\%2\makefile.in %WINE_TMPFILE1%

>%WINE_RESULTFILE% ECHO ^<module name="%2" type="win32dll" baseaddress="${BASEADDRESS_%2}" installbase="system32" installname="%2.dll" allowwarnings="true"^>
>>%WINE_RESULTFILE% ECHO 	^<importlibrary definition="%2.spec.def" /^>
>>%WINE_RESULTFILE% ECHO 	^<include base="%2"^>.^</include^>
>>%WINE_RESULTFILE% ECHO 	^<include base="ReactOS"^>include/reactos/wine^</include^>
>>%WINE_RESULTFILE% ECHO 	^<define name="__REACTOS__" /^>
>>%WINE_RESULTFILE% ECHO 	^<define name="__USE_W32API" /^>
>>%WINE_RESULTFILE% ECHO 	^<define name="_WIN32_IE"^>0x600^</define^>
>>%WINE_RESULTFILE% ECHO 	^<define name="_WIN32_WINNT"^>0x501^</define^>
>>%WINE_RESULTFILE% ECHO 	^<define name="WINVER"^>0x501^</define^>
>>%WINE_RESULTFILE% ECHO 	^<library^>wine^</library^>
SET WINE_FULL_LINE=
SET WINE_END_PREC_LINE=
SET WINE_VARTYPE=0
FOR /F "eol=# delims=" %%l IN (%WINE_TMPFILE1%) DO (
	CALL %WINE_BATCH% internal_analyseline %%l
)
>>%WINE_RESULTFILE% ECHO 	^<file^>%2.spec^</file^>
>>%WINE_RESULTFILE% ECHO ^</module^>

:processnext
SHIFT
IF NOT "%2" == "" GOTO startprocess
DEL %WINE_TMPFILE1% %WINE_TMPFILE2%
GOTO :eof

:internal_analyseline
SET WINE_LINE=%*
SET WINE_LINE=%WINE_LINE:~21%
SET WINE_LINE=%WINE_LINE:-luuid=%
SET WINE_START_LINE1=%WINE_LINE:~0,1%
SET WINE_START_LINE2=%WINE_LINE:~0,2%
SET WINE_END_LINE=%WINE_LINE:~-1%
IF NOT "%WINE_END_PREC_LINE%" == "\" (
	IF NOT "%WINE_VARTYPE%" == "0" (
		FOR /F "tokens=2* delims= " %%h IN ("%WINE_FULL_LINE%") DO (
			FOR %%j IN (%%i) DO (
				IF "%WINE_VARTYPE%" == "1" (
					>>%WINE_RESULTFILE% ECHO 	^<file^>%%j^</file^>
				) ELSE (
					>>%WINE_RESULTFILE% ECHO 	^<library^>%%j^</library^>
				)
			)
		)
	)
	SET WINE_FULL_LINE=
	IF "%2" == "C_SRCS" (
		SET WINE_VARTYPE=1
	) ELSE IF "%2" == "RC_SRCS" (
		SET WINE_VARTYPE=1
	) ELSE IF "%2" == "IMPORTS" (
		SET WINE_VARTYPE=2
	) ELSE (
		SET WINE_VARTYPE=0
	)
)
IF "%WINE_END_LINE%" == "\" (
	>%WINE_TMPFILE2% ECHO %WINE_FULL_LINE%%WINE_LINE:~0,-1%
	<%WINE_TMPFILE2% SET /P WINE_FULL_LINE=
) ELSE (
	>%WINE_TMPFILE2% ECHO %WINE_FULL_LINE%%WINE_LINE%
	<%WINE_TMPFILE2% SET /P WINE_FULL_LINE=
)
SET WINE_END_PREC_LINE=%WINE_END_LINE%
