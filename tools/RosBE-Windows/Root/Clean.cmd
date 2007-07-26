::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See COPYING in the top level directory
:: FILE:        Root/Clean.cmd
:: PURPOSE:     Clean the ReactOS source directory.
:: COPYRIGHT:   Copyright 2007 Daniel Reimer <reimer.daniel@freenet.de>
::                             Peter Ward <dralnix@gmail.com>
::
::
@echo off

title Cleaning...

if "%1" == "" (
    ::
    : Check if we have something to clean, if so, clean it.
    ::
    if exist "%CD%\obj-i386\." (
        echo Cleaning ReactOS source directory...

        ::
        : Run ReactOS's make clean.
        ::
        "%_MINGWMAKE%" clean 1> NUL 2> NUL

        ::
        : Remove directories/makefile.auto created by the build.
        ::
        if exist "%CD%\obj-i386\." (
            rd /s /q "%CD%\obj-i386" 1> NUL 2> NUL
        )
        if exist "%CD%\output-i386\." (
            rd /s /q "%CD%\output-i386" 1> NUL 2> NUL
        )
        if exist "%CD%\reactos\." (
            rd /s /q "%CD%\reactos" 1> NUL 2> NUL
        )
        if exist "%CD%\makefile.auto" (
            del "%CD%\makefile.auto" 1> NUL 2> NUL
        )

        ::
        : Now delete auto-generated/copied files not removed by make clean.
        ::
        del /f "%CD%\media\inf\syssetup.inf" 1> NUL 2> NUL
        del /f "%CD%\include\roscfg.h" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\ole32\nodrop.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\ole32\drag_move.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\ole32\drag_copy.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\ole32\drag_link.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idc_divideropen.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idc_copy.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idc_divider.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_view_small.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_std_large.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_view_large.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idi_tt_info_sm.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idc_movebutton.cur" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idi_tt_warn_sm.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idi_dragarrow.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_hist_small.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_hist_large.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idt_check.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idi_tt_error_sm.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comctl32\idb_std_small.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\msi\instabsent.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\msi\instlocal.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\msi\instadvert.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\network.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\fontpics.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\pd32_nocollate.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\pd32_collate.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\folder.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\cdrom.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\floppy.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\pd32_landscape.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\folder2.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\800.bmp" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\pd32_portrait.ico" 1> NUL 2> NUL
        del /f "%CD%\dll\win32\comdlg32\hdisk.ico" 1> NUL 2> NUL

        echo Done cleaning ReactOS source directory.
    ) else (
        echo There is nothing to clean.
    )
    title ReactOS Build Environment %_VER%
    goto :EOC
)
if "%1" == "logs" (
    ::
    : Check if we have any logs to clean, if so, clean them.
    ::
    if exist "%_ROSBELOGDIR%\*.txt" (
        echo Cleaning build logs...

        del /f "%_ROSBELOGDIR%\*.txt" 1> NUL 2> NUL

        echo Done cleaning build logs.
    ) else (
        echo There are no logs to clean.
    )
    goto :EOC
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOC
)

:EOC
title ReactOS Build Environment %_VER%
