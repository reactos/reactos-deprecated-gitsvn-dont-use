::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
:: FILE:        Root/rosbe-gcc-env.cmd
:: PURPOSE:     Set up the GCC 4.x.x build environment.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

::
:: Set up the GCC 4.x.x build environment.
::
set PATH=%_ROSBE_MINGWPATH%\bin;%_ROSBE_ORIGINALPATH%
set _ROSBE_GCCVERSION=
for /f "usebackq tokens=3" %%i in (`"gcc -v 2>&1 | find "gcc version""`) do set _ROSBE_GCCVERSION=%%i
set PATH=%_ROSBE_MINGWPATH%\bin;%_ROSBE_MINGWPATH%\libexec\gcc\mingw32\%_ROSBE_GCCVERSION%;%_ROSBE_ORIGINALPATH%
set _ROSBE_MINGWMAKE=%_ROSBE_MINGWPATH%\bin\mingw32-make.exe
if defined _ROSBE_OLDMODE (
    set C_INCLUDE_PATH=%_ROSBE_MINGWPATH%\include;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%\include
    set CPLUS_INCLUDE_PATH=%_ROSBE_MINGWPATH%\include;%_ROSBE_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%;%_ROSBE_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%\mingw32;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%\include
)
set HOST_CFLAGS=-I"%_ROSBE_MINGWPATH%\include" -I"%_ROSBE_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%\include"
set HOST_CPPFLAGS=-I"%_ROSBE_MINGWPATH%\include" -I"%_ROSBE_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%" -I"%_ROSBE_MINGWPATH%\include\c++\%_ROSBE_GCCVERSION%\mingw32" -I"%_ROSBE_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%\include"
set LIBRARY_PATH=%_ROSBE_MINGWPATH%\lib;%_ROSBE_MINGWPATH%\lib\gcc\mingw32\%_ROSBE_GCCVERSION%

::
:: Display the current version of GCC, NASM, ld and make.
::
gcc -v 2>&1 | find "gcc version"
nasm -v
ld -v
mingw32-make -v | find "GNU Make"
