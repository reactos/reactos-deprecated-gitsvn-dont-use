:: This file is entirely a HACK!!!
:: The ROS_* variables set here should later be set by the Options utility, so we can drop the translations from _ROSBE_* variables here

if "%_ROSBE_NOSTRIP%" == "1" (
    set ROS_BUILDNOSTRIP=yes
) else (
    set ROS_BUILDNOSTRIP=no
)

if "%_ROSBE_STRIP%" == "1" (
    set ROS_LEAN_AND_MEAN=yes
) else (
    set ROS_LEAN_AND_MEAN=no
)

:: Check if the user has chosen to use a different object or output path
:: and set it accordingly.
if defined _ROSBE_OBJPATH (
    if not exist "%_ROSBE_OBJPATH%\." (
        echo WARNING: The Object-Path specified doesn't seem to exist. Creating...
    )
    set ROS_INTERMEDIATE=%_ROSBE_OBJPATH%
    set ROS_TEMPORARY=%_ROSBE_OBJPATH%
)
if defined _ROSBE_OUTPATH (
    if not exist "%_ROSBE_OUTPATH%\." (
        echo WARNING: The Output-Path specified doesn't seem to exist. Creating...
    )
    set ROS_OUTPUT=%_ROSBE_OUTPATH%
)
