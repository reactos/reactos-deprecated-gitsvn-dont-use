::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/update.cmd
:: PURPOSE:     RosBE Updater.
:: COPYRIGHT:   Copyright 2008 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off

:: The Update Server.
set _ROSBE_URL=www.foo.bar

:: First check for a new Updater
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/update.cmd
"%_ROSBE_BASEDIR%\update.cmd"

:: PS1 Files.
if exist "%_ROSBE_BASEDIR%\Build.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Build.ps1
)
if exist "%_ROSBE_BASEDIR%\Clean.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Clean.ps1
)
if exist "%_ROSBE_BASEDIR%\Help.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Help.ps1
)
if exist "%_ROSBE_BASEDIR%\MinGW.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/MinGW.ps1
)
if exist "%_ROSBE_BASEDIR%\RosBE.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/RosBE.ps1
)
if exist "%_ROSBE_BASEDIR%\rosbe-gcc-env.ps1" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/rosbe-gcc-env.ps1
)

:: Options Files.
if exist "%_ROSBE_BASEDIR%\options.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/options.cmd
)

:: SVN Files.
if exist "%_ROSBE_BASEDIR%\sSVN.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/sSVN.cmd
)

:: SCut Files.
if exist "%_ROSBE_BASEDIR%\scut.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/scut.cmd
)

:: RelAddr2Line Files.
if exist "%_ROSBE_BASEDIR%\reladdr2line.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/reladdr2line.cmd
)

:: Other Tools Files.
if exist "%_ROSBE_BASEDIR%\Config.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Config.cmd
)
if exist "%_ROSBE_BASEDIR%\chdefdir.cmd" (
    "%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/chdefdir.cmd
)

:: Default Files.
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Build.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/ChangeLog.txt
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/chdefgcc.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Clean.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Help.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/LICENSE.TXT
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/MinGW.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/mingw.ico
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/MinGW.mac
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/README.pdf
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/RosBE.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/rosbe.ico
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/RosBE.mac
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/rosbe-gcc-env.cmd
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/TimeDate.cmd

:: Add Dates into Vars and load GCC and Tool SRC packages if needed.
if exist GCC.7z (
    for /f "usebackq" %%i in (`"forfiles /M GCC.7z /C "cmd /c echo @fdate""`) do set _ROSBE_GCCDATE=%%i
)
if exist Tools.7z (
    for /f "usebackq" %%i in (`"forfiles /M Tools.7z /C "cmd /c echo @fdate""`) do set _ROSBE_TOOLSDATE=%%i
)

"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/GCC.7z
"%_ROSBE_BASEDIR%\Tools\wget.exe" -N %_ROSBE_URL%/Tools.7z

:: Add the maybe Updated Dates to another Var.
for /f "usebackq" %%i in (`"forfiles /M GCC.7z /C "cmd /c echo @fdate""`) do set _ROSBE_GCCDATE2=%%i
for /f "usebackq" %%i in (`"forfiles /M Tools.7z /C "cmd /c echo @fdate""`) do set _ROSBE_TOOLSDATE2=%%i

:: Extract GCC.
if %_ROSBE_GCCDATE% NEQ %_ROSBE_GCCDATE2% (
    "%_ROSBE_BASEDIR%\Tools\7z.exe" x GCC.7z "%_ROSBE_BASEDIR%\4.1.3"
)
if %_ROSBE_TOOLSDATE% NEQ %_ROSBE_TOOLSDATE2% (
    "%_ROSBE_BASEDIR%\Tools\7z.exe" x Tools.7z "%TEMP%"
    :: Build the tools
    make -f %TEMP%\makefile
    copy %TEMP%\*.exe "%_ROSBE_BASEDIR%\Tools"
)

:: Unload Vars.
set _ROSBE_URL=
set _ROSBE_GCCDATE=
set _ROSBE_TOOLSDATE=
set _ROSBE_GCCDATE2=
set _ROSBE_TOOLSDATE2=
