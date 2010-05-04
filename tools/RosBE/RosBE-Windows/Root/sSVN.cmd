::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GNU General Public License v2. (see LICENSE.txt)
:: FILE:        Root/sSVN.cmd
:: PURPOSE:     Integrated SVN Client.
:: COPYRIGHT:   Copyright 2010 Daniel Reimer <reimer.daniel@freenet.de>
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
        if not "%2" == "" (
            if "%ROS_ARCH%" == "amd64" (
                svn.exe checkout -r %2 http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos .
            ) else (
                svn.exe checkout -r %2 http://svn.reactos.org/reactos/trunk/reactos .
            )
        ) else (
            if "%ROS_ARCH%" == "amd64" (
                svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos .
            ) else (
                svn.exe checkout http://svn.reactos.org/reactos/trunk/reactos .
            )
        )
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rosapps" (
    if not "%2" == "" (
        if not exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\." (
            md "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
        )
        if exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\.svn\." (
            title SVN RosApps Updating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
            svn.exe update -r %2
        ) else (
            title SVN RosApps Creating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
            dir /b 2>nul | findstr "." >nul
            if errorlevel 1 (
                if "%ROS_ARCH%" == "amd64" (
                    svn.exe checkout -r %2 http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rosapps .
                ) else (
                    svn.exe checkout -r %2 http://svn.reactos.org/reactos/trunk/rosapps .
                )
            ) else (
                echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
            )
        )
    ) else (
        if not exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\." (
            md "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
        )
        if exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\.svn\." (
            title SVN RosApps Updating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
            svn.exe update
        ) else (
            title SVN RosApps Creating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
            dir /b 2>nul | findstr "." >nul
            if errorlevel 1 (
                if "%ROS_ARCH%" == "amd64" (
                    svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rosapps .
                ) else (
                    svn.exe checkout http://svn.reactos.org/reactos/trunk/rosapps .
                )
            ) else (
                echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
            )
        )
    )
    cd "%_ROSBE_ROSSOURCEDIR%"
    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rostests" (
    if not "%2" == "" (
        if not exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\." (
            md "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
        )
        if exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\.svn\." (
            title SVN RosTests Updating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
            svn.exe update -r %2
        ) else (
            title SVN RosTests Creating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
            dir /b 2>nul | findstr "." >nul
            if errorlevel 1 (
                if "%ROS_ARCH%" == "amd64" (
                    svn.exe checkout -r %2 http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rostests .
                ) else (
                    svn.exe checkout -r %2 http://svn.reactos.org/reactos/trunk/rostests .
                )
            ) else (
                echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
            )
        )
    ) else (
        if not exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\." (
            md "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
        )
        if exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\.svn\." (
            title SVN RosTests Updating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
            svn.exe update
        ) else (
            title SVN RosTests Creating...
            cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
            dir /b 2>nul | findstr "." >nul
            if errorlevel 1 (
                if "%ROS_ARCH%" == "amd64" (
                    svn.exe checkout http://svn.reactos.org/reactos/branches/ros-amd64-bringup/rostests .
                ) else (
                    svn.exe checkout http://svn.reactos.org/reactos/trunk/rostests .
                )
            ) else (
                echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
            )
        )
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
        for /f "usebackq tokens=2" %%j in (`"svn.exe info http://svn.reactos.org/reactos/branches/ros-amd64-bringup/reactos | find "Revision:""`) do set ONSVN=%%j
    ) else (
        for /f "usebackq tokens=2" %%j in (`"svn.exe info http://svn.reactos.org/reactos/trunk/reactos | find "Revision:""`) do set ONSVN=%%j
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
                if not "%_BUILDBOT_SVNSKIPMAINTRUNK%" == "1" (
                    svn.exe update -r %2
                )
                if exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\." (
                    cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
                    echo Updating RosApps...
                    svn.exe update -r %2
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
                if exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\." (
                    cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
                    echo Updating RosTests...
                    svn.exe update -r %2
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
            ) else (
                if not "%_BUILDBOT_SVNSKIPMAINTRUNK%" == "1" (
                    svn.exe update
                )
                if exist "%_ROSBE_ROSSOURCEDIR%\modules\rosapps\." (
                    cd "%_ROSBE_ROSSOURCEDIR%\modules\rosapps"
                    echo Updating RosApps...
                    svn.exe update
                    cd "%_ROSBE_ROSSOURCEDIR%"
                )
                if exist "%_ROSBE_ROSSOURCEDIR%\modules\rostests\." (
                    cd "%_ROSBE_ROSSOURCEDIR%\modules\rostests"
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
