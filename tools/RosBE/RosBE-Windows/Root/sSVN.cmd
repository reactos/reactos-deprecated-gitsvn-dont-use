::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/sSVN.cmd
:: PURPOSE:     Integrated SVN Client.
:: COPYRIGHT:   Copyright 2009 Daniel Reimer <reimer.daniel@freenet.de>
::

@echo off
if not defined _ROSBE_DEBUG set _ROSBE_DEBUG=0
if %_ROSBE_DEBUG% == 1 (
    @echo on
)

setlocal enabledelayedexpansion

:: Receive the first parameter and decide what to do.
if "%1" == "" (
    echo No parameter specified. Try 'help [COMMAND]'.
    goto :EOC
)

:: These two are directly parsed to svn.
if /i "%1" == "update" (
    title SVN Updating...
    echo This might take a while, so please be patient.
    echo.
    set _ROSBE_SSVN_JOB=update
    goto :UP    
)

if /i "%1" == "cleanup" (
    title SVN Cleaning...

    svn.exe cleanup

    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "create" (
    title SVN Creating...
    if exist ".svn\." (
        echo ERROR: Folder already contains a repository.
        goto :EOC
    )
    rd /s /q "%_ROSBE_LOGDIR%" 1> NUL 2> NUL
    dir /b 2>nul | findstr "." >nul
    if errorlevel 1 (
        if "%ROS_ARCH%" == "amd64" (
            svn.exe checkout svn://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos .
        ) else (
            svn.exe checkout svn://svn.reactos.org/reactos/trunk/reactos .
        )
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rosapps" (
    title SVN RosApps Creating...
    if exist "modules\rosapps\.svn\." (
        echo ERROR: Folder already contains a RosApps repository.
        goto :EOC
    )
    if not exist "modules\rosapps\." (
        md modules\rosapps
    )
    cd modules\rosapps
    dir /b 2>nul | findstr "." >nul
    if errorlevel 1 (
        if "%ROS_ARCH%" == "amd64" (
            svn.exe checkout svn://svn.reactos.org/reactos/branches/ros-amd64-bringup/rosapps .
        ) else (
            svn.exe checkout svn://svn.reactos.org/reactos/trunk/rosapps .
        )
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    cd "%_ROSBE_ROSSOURCEDIR%"
    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rostests" (
    title SVN RosTests Creating...
    if exist "modules\rostests\.svn\." (
        echo ERROR: Folder already contains a RosTests repository.
        goto :EOC
    )
    if not exist "modules\rostests\." (
        md modules\rostests
    )
    cd modules\rostests
    dir /b 2>nul | findstr "." >nul
    if errorlevel 1 (
        if "%ROS_ARCH%" == "amd64" (
            svn.exe checkout svn://svn.reactos.org/reactos/branches/ros-amd64-bringup/rostests .
        ) else (
            svn.exe checkout svn://svn.reactos.org/reactos/trunk/rostests .
        )
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    cd "%_ROSBE_ROSSOURCEDIR%"
    goto :EOC
)

:: Output the revision of the local and online trees and tell the user if
:: its up to date or not.
if /i "%1" == "status" (
    title SVN Status
    echo This might take a while, so please be patient.
    echo.
    set _ROSBE_SSVN_JOB=status
    goto :UP
)

if not "%1" == "" (
    echo Unknown parameter specified. Try 'help ssvn'.
)

:UP
    for /f "usebackq tokens=2" %%i in (`"svn.exe info | find "Revision:""`) do set OFFSVN=%%i
    if "%ROS_ARCH%" == "amd64" (
        for /f "usebackq tokens=2" %%j in (`"svn.exe info svn://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos | find "Revision:""`) do set ONSVN=%%j
    ) else (
        for /f "usebackq tokens=2" %%j in (`"svn.exe info svn://svn.reactos.org/reactos/trunk/reactos | find "Revision:""`) do set ONSVN=%%j
    )

    echo Local Revision: !OFFSVN!
    echo Online HEAD Revision: !ONSVN!
    echo.

    if !OFFSVN! lss !ONSVN! (
        if "!_ROSBE_SSVN_JOB!" == "status" (
            echo Your tree is not up to date. Do you want to update it?
            set /p UP="Please enter 'yes' or 'no': "
            if /i "!UP!" == "yes" set _ROSBE_SSVN_JOB=update
        )
        if "!_ROSBE_SSVN_JOB!" == "update" (
            if not "%2" == "" (
                svn.exe update -r %2
                if exist "modules\rosapps\." (
                    cd modules\rosapps
                    echo Updating RosApps...
                    svn.exe update -r %2
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
                if exist "modules\rostests\." (
                    cd modules\rostests
                    echo Updating RosTests...
                    svn.exe update -r %2
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
            ) else (
                svn.exe update
                if exist "modules\rosapps\." (
                    cd modules\rosapps
                    echo Updating RosApps...
                    svn.exe update
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
                if exist "modules\rostests\." (
                    cd modules\rostests
                    echo Updating RosTests...
                    svn.exe update
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
            )
        )
        echo Do you want to see the changelog?
        set /p CL="Please enter 'yes' or 'no': "
        if /i "!CL!"=="yes" (
            svn.exe log -r !OFFSVN!:!ONSVN!
        )
    )
    if !OFFSVN! equ !ONSVN! (
        echo Your tree is up to date.
    )

goto EOC

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
