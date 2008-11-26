::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

::
:: Set Title
::
title Updating...

::
:: The Update Server.
::
set _ROSBE_URL=http://mitglied.lycos.de/reimerdaniel/rosbe

::
:: Save the recent dir to cd back there at the end.
::
set _ROSBE_OPATH=%~dp0
set _ROSBE_OPATH=%_ROSBE_OPATH:~0,-1%

if not exist "%_ROSBE_BASEDIR%\Tools\7z.exe" (
    cd /d "%_ROSBE_BASEDIR%\Tools"
    wget.exe -N --ignore-length --no-verbose %_ROSBE_URL%/7z.exe 1> NUL 2> NUL
    cd /d %_ROSBE_OPATH%
)

cd /d %_ROSBE_BASEDIR%

::
:: First check for a new Updater
::
setlocal enabledelayedexpansion
for %%F in (update.cmd) do set _ROSBE_UPDDATE=%%~tF
"Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/update.cmd 1> NUL 2> NUL
for %%F in (update.cmd) do set _ROSBE_UPDDATE2=%%~tF
if !_ROSBE_UPDDATE! NEQ !_ROSBE_UPDDATE2! (
    cls
    echo Updater got updated and needs to be restarted.
    goto :EOC
)
endlocal

::
:: Get to the Updates Subfolder.
::
if not exist "%APPDATA%\RosBE\Updates" mkdir "%APPDATA%\RosBE\Updates" 1> NUL 2> NUL
cd /d "%APPDATA%\RosBE\Updates"

::
:: Parse the args.
::
if "%1" == "" (
    set _ROSBE_MULTIUPD=1
    set _ROSBE_STATCOUNT=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    set /a _ROSBE_STATCOUNT+=1
    call :UPDCHECK
    goto :EOC
)
if /i "%1" == "reset" (
    del /F /Q "%APPDATA%\RosBE\Updates\*.*" 1> NUL 2> NUL
    del /F /Q "%APPDATA%\RosBE\Updates\tmp\*.*" 1> NUL 2> NUL
    goto :EOC
)
if /i "%1" == "nr" (
    set _ROSBE_STATCOUNT=%2
    call :UPDCHECK
    goto :EOC
)
if /i "%1" == "delete" (
    set _ROSBE_STATCOUNT=%2
    del /F /Q "%APPDATA%\RosBE\Updates\%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.*" 1> NUL 2> NUL
    del /F /Q "%APPDATA%\RosBE\Updates\tmp\%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.*" 1> NUL 2> NUL
    goto :EOC
)
if /i "%1" == "info" (
    set _ROSBE_STATCOUNT=%2
    call :UPDINFO
    goto :EOC
)
if /i "%1" == "status" (
    if not exist "tmp" mkdir tmp 1> NUL 2> NUL
    copy *.txt .\tmp\. 1> NUL 2> NUL
    set _ROSBE_STATCOUNT=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    set /a _ROSBE_STATCOUNT+=1
    call :STATUS
    goto :UPDFIN
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help update'.
    goto :EOC
)

:STATUS

cd tmp
if not exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt 1> NUL 2> NUL
    if exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
        set _ROSBE_UPDATES=%_ROSBE_UPDATES% %_ROSBE_STATCOUNT% 
    )
)
cd..
goto :EOF

:UPDINFO

cd tmp
if not exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt 1> NUL 2> NUL
    if exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
        type "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt"
    ) else (
        echo ERROR: This Update does not seem to exist or the Internet connection is not working correctly.
        goto :EOF
    )
)
cd..
del /F /Q tmp\*.* 1> NUL 2> NUL
goto :EOF

:UPDFIN

del /F /Q tmp\*.* 1> NUL 2> NUL
if not "%_ROSBE_UPDATES%" == "" (
    echo Following Updates available: %_ROSBE_UPDATES%
) else (
    echo RosBE is up to Date.
)
goto :EOC

:UPDCHECK

cd /d "%APPDATA%\RosBE\Updates"

if exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
    goto :EOF
)
if not exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt 1> NUL 2> NUL
)
if exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" (
    type "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt"
    echo.
    echo Install?
    setlocal enabledelayedexpansion
    set /p YESNO="(yes), (no)"
    if /i "!YESNO!"=="yes" (
        if not exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.7z" (
            "%_ROSBE_BASEDIR%\Tools\wget.exe" -N --ignore-length --no-verbose %_ROSBE_URL%/%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.7z 1> NUL 2> NUL
        )
        if exist "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.7z" (
            del /F /Q "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%\*.*" 1>NUL 2>NUL
            "%_ROSBE_BASEDIR%\Tools\7z.exe" x "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.7z"
            cd "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%"
            call "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.cmd"
            goto :EOF
        ) else (
            echo ERROR: This Update does not seem to exist or the Internet connection is not working correctly.
            goto :EOF
        )
    ) else if /i "!YESNO!"=="no" (
        echo Do you want to be asked again to install this update?
        set /p YESNO="(yes), (no)"
        if /i "!YESNO!"=="yes" (
            del "%_ROSBE_VERSION%-%_ROSBE_STATCOUNT%.txt" 1> NUL 2> NUL
        )
        goto :EOF
    )
    endlocal
) else (
    if not "%_ROSBE_MULTIUPD%" == "1" (
        echo ERROR: This Update does not seem to exist or the Internet connection is not working correctly.
        goto :EOF
    )
)
goto :EOF

:EOC

cd /d "%_ROSBE_OPATH%"
if defined _ROSBE_VERSION (
    title ReactOS Build Environment %_ROSBE_VERSION%
)

set _ROSBE_UPDFINISH=
set _ROSBE_OPATH=
set _ROSBE_UPDDATE=
set _ROSBE_UPDATES=
set _ROSBE_UPDDATE2=
set _ROSBE_MULTIUPD=
set _ROSBE_STATCOUNT=
