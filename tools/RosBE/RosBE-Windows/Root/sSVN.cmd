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

set ROS_SVNURL=http://svn.reactos.org/reactos

if "%ROS_ARCH%" == "amd64" (
    set ROS_SVNURL=%ROS_SVNURL%/branches/ros-amd64-bringup
) else (
    if not defined ROS_BRANCH (
        set ROS_SVNURL=%ROS_SVNURL%/trunk
    ) else (
        set ROS_SVNURL=%ROS_SVNURL%/branches/%ROS_BRANCH%
    )
)

wget --spider --no-verbose %ROS_SVNURL%/reactos 1> NUL 2> NUL

if ERRORLEVEL 1 (
    wget --spider --no-verbose %ROS_SVNURL% 1> NUL 2> NUL
    if ERRORLEVEL 1 (
        echo The selected branch does not exist or the Internet Connection is down.
        goto :EOC
    ) else (
        set rsubfolder=
        set disapptest=1
    )
) else (
    set rsubfolder=/reactos
    set disapptest=0
)

set _ROSBE_SSVNSOURCEDIR=%CD%

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
            svn.exe checkout -r %2 %ROS_SVNURL%%rsubfolder% .
        ) else (
            svn.exe checkout %ROS_SVNURL%%rsubfolder% .
        )
    ) else (
        echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
    )
    goto :EOC
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rosapps" (
    if "%disapptest%" == "0" (
        if not "%2" == "" (
            if not exist "modules\rosapps\." (
                md "modules\rosapps"
            )
            if exist "modules\rosapps\.svn\." (
                title SVN RosApps Updating...
                cd "modules\rosapps"
                svn.exe update -r %2
            ) else (
                title SVN RosApps Creating...
                cd "modules\rosapps"
                dir /b 2>nul | findstr "." >nul
                if errorlevel 1 (
                    svn.exe checkout -r %2 %ROS_SVNURL%/rosapps .
                ) else (
                    echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
                )
            )
        ) else (
            if not exist "modules\rosapps\." (
                md "modules\rosapps"
            )
            if exist "modules\rosapps\.svn\." (
                title SVN RosApps Updating...
                cd "modules\rosapps"
                svn.exe update
            ) else (
                title SVN RosApps Creating...
                cd "modules\rosapps"
                dir /b 2>nul | findstr "." >nul
                if errorlevel 1 (
                    svn.exe checkout %ROS_SVNURL%/rosapps .
                ) else (
                    echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
                )
            )
        )
        cd "%_ROSBE_SSVNSOURCEDIR%"
        goto :EOC
    ) else (
        echo Rosapps and Rostests disabled.
    )
)

:: Check if the folder is empty. If not, output an error.
if /i "%1" == "rostests" (
    if "%disapptest%" == "0" (
        if not "%2" == "" (
            if not exist "modules\rostests\." (
                md "modules\rostests"
            )
            if exist "modules\rostests\.svn\." (
                title SVN RosTests Updating...
                cd "modules\rostests"
                svn.exe update -r %2
            ) else (
                title SVN RosTests Creating...
                cd "modules\rostests"
                dir /b 2>nul | findstr "." >nul
                if errorlevel 1 (
                    svn.exe checkout -r %2 %ROS_SVNURL%/rostests .
                ) else (
                    echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
                )
            )
        ) else (
            if not exist "modules\rostests\." (
                md "modules\rostests"
            )
            if exist "modules\rostests\.svn\." (
                title SVN RosTests Updating...
                cd "modules\rostests"
                svn.exe update
            ) else (
                title SVN RosTests Creating...
                cd "modules\rostests"
                dir /b 2>nul | findstr "." >nul
                if errorlevel 1 (
                    svn.exe checkout %ROS_SVNURL%/rostests .
                ) else (
                    echo ERROR: Folder is not empty. Continuing is dangerous and can cause errors. ABORTED
                )
            )
        )
        cd "%_ROSBE_SSVNSOURCEDIR%"
        goto :EOC
    ) else (
        echo Rosapps and Rostests disabled.
    )
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
goto :EOC

:UP
    for /f "usebackq tokens=4" %%i in (`"svn.exe info | find "Last Changed Rev:""`) do set OFFSVN=%%i
    for /f "usebackq tokens=4" %%j in (`"svn.exe info %ROS_SVNURL%%rsubfolder% | find "Last Changed Rev:""`) do set ONSVN=%%j

    echo Local Revision: !OFFSVN!
    echo Online HEAD Revision: !ONSVN!
    echo.

    if !OFFSVN! lss !ONSVN! (
        if "!_ROSBE_SSVN_JOB!" == "status" (
            echo Your tree is not up to date. Do you want to update it?
            set /p UP="Please enter 'yes' or 'no': "
            if /i "!UP!" == "yes" set _ROSBE_SSVN_JOB=update
        )
    )
    if !OFFSVN! equ !ONSVN! (
        echo Your tree is up to date.
        goto :EOC
    )

    if "!_ROSBE_SSVN_JOB!" == "update" (
        if not "%2" == "" (
            if "%2" == "!OFFSVN!" (
                echo Your Local Repository is currently %2
            )
            if "%2" LSS "!OFFSVN!" (
                echo Downgrading to %2 ...
            )
            if "%2" GTR "!OFFSVN!" (
                echo Updating to %2 ...
            )
            if not "%_BUILDBOT_SVNSKIPMAINTRUNK%" == "1" (
                svn.exe update -r %2
            ) else (
                echo Skipping ReactOS Trunk update.
            )
            if exist "modules\rosapps\." (
                cd "modules\rosapps"
                echo Updating RosApps...
                svn.exe update -r %2
                cd "%_ROSBE_SSVNSOURCEDIR%"
            )
            if exist "modules\rostests\." (
                cd "modules\rostests"
                echo Updating RosTests...
                svn.exe update -r %2
                cd "%_ROSBE_SSVNSOURCEDIR%"
            )
        ) else (
            if not "%_BUILDBOT_SVNSKIPMAINTRUNK%" == "1" (
                svn.exe update
            ) else (
                echo Skipping ReactOS Trunk update.
            )
            if exist "modules\rosapps\." (
                cd "modules\rosapps"
                echo Updating RosApps...
                svn.exe update
                cd "%_ROSBE_SSVNSOURCEDIR%"
            )
            if exist "modules\rostests\." (
                cd "modules\rostests"
                echo Updating RosTests...
                svn.exe update
                cd "%_ROSBE_SSVNSOURCEDIR%"
            )
        )
        echo Do you want to see the changelog?
        set /p CL="Please enter 'yes' or 'no': "
        if /i "!CL!"=="yes" (
            svn.exe log -r !OFFSVN!:!ONSVN!
        )
    )

goto :EOC

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
endlocal
