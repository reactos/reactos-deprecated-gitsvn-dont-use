::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/version.cmd
:: PURPOSE:     Display the current version of GCC, NASM, ld and make.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::
::
@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

ver

if not "%ROS_ARCH%" == "" (
    "%_ROSBE_TARGET_MINGWPATH%\bin\%ROS_PREFIX%-gcc" -v 2>&1 | find "gcc version"
    if "%ROS_PREFIX%" == "" (
        echo gcc target^: %_ROSBE_GCCTARGET%
    ) else (
        echo gcc target^: %ROS_PREFIX:~0,-11%
    )
    "%_ROSBE_TARGET_MINGWPATH%\bin\%ROS_PREFIX%-ld" -v
) else (
    gcc -v 2>&1 | find "gcc version"
    if "%ROS_PREFIX%" == "" (
        echo gcc target^: %_ROSBE_GCCTARGET%
    ) else (
        echo gcc target^: %ROS_PREFIX:~0,-11%
    )
    ld -v
)

if exist "%_ROSBE_HOST_MINGWPATH%\bin\nasm.exe" (
    nasm -v
) else (
    if exist "%_ROSBE_HOST_MINGWPATH%\bin\yasm.exe" (
        yasm --version | find "yasm 0"
    ) else (
        if "%_ROSBE_MODE%" == "RosBE" (
            echo ERROR: NASM or YASM is required to build ReactOS, none was found in the current MinGW/GCC.
        )
    )
)
if exist "%_ROSBE_HOST_MINGWPATH%\bin\bison.exe" (
    bison --version | find "GNU Bison"
) else (
    if "%_ROSBE_MODE%" == "RosBE" (
        echo WARNING: Bison will soon be required to build ReactOS, none was found in the current MinGW/GCC.
    )
)
if exist "%_ROSBE_HOST_MINGWPATH%\bin\flex.exe" (
    flex --version
) else (
    if "%_ROSBE_MODE%" == "RosBE" (
        echo WARNING: Flex will soon be required to build ReactOS, none was found in the current MinGW/GCC.
    )
)
mingw32-make -v | find "GNU Make"
