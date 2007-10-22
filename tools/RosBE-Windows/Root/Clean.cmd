::
:: PROJECT:     RosBE - ReactOS Build Environment for Windows
:: LICENSE:     GPL - See LICENSE.txt in the top level directory.
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
    if exist "obj-i386\." (
        echo Cleaning ReactOS source directory...

        ::
        : Remove directories/makefile.auto created by the build.
        ::
        if exist "obj-i386\." (
            rd /s /q "obj-i386" 1> NUL 2> NUL
        )
        if exist "output-i386\." (
            rd /s /q "output-i386" 1> NUL 2> NUL
        )
        if exist "reactos\." (
            rd /s /q "reactos" 1> NUL 2> NUL
        )
        if exist "makefile.auto" (
            del "makefile.auto" 1> NUL 2> NUL
        )

        ::
        : Now delete auto-generated/copied files not removed by make clean.
        ::
        del /f "media\inf\syssetup.inf" 1> NUL 2> NUL
        del /f "include\roscfg.h" 1> NUL 2> NUL
        del /f "dll\win32\ole32\nodrop.cur" 1> NUL 2> NUL
        del /f "dll\win32\ole32\drag_move.cur" 1> NUL 2> NUL
        del /f "dll\win32\ole32\drag_copy.cur" 1> NUL 2> NUL
        del /f "dll\win32\ole32\drag_link.cur" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idc_divideropen.cur" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idc_copy.cur" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idc_divider.cur" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_view_small.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_std_large.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_view_large.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idi_tt_info_sm.ico" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idc_movebutton.cur" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idi_tt_warn_sm.ico" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idi_dragarrow.ico" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_hist_small.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_hist_large.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idt_check.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idi_tt_error_sm.ico" 1> NUL 2> NUL
        del /f "dll\win32\comctl32\idb_std_small.bmp" 1> NUL 2> NUL
        del /f "dll\win32\msi\instabsent.bmp" 1> NUL 2> NUL
        del /f "dll\win32\msi\instlocal.bmp" 1> NUL 2> NUL
        del /f "dll\win32\msi\instadvert.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\network.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\fontpics.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\pd32_nocollate.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\pd32_collate.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\folder.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\cdrom.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\floppy.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\pd32_landscape.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\folder2.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\800.bmp" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\pd32_portrait.ico" 1> NUL 2> NUL
        del /f "dll\win32\comdlg32\hdisk.ico" 1> NUL 2> NUL

        del /f "dll\ntdll\napi.S" 1> NUL 2> NUL
        del /f "dll\ntdll\i386\napi.S" 1> NUL 2> NUL
        rmdir /q "dll\ntdll\i386" 1> NUL 2> NUL
        del /f "dll\win32\gdi32\misc\win32k.S" 1> NUL 2> NUL
        del /f "dll\win32\gdi32\misc\i386\win32k.S" 1> NUL 2> NUL
        rmdir /q "dll\win32\gdi32\misc\i386" 1> NUL 2> NUL
        del /f "dll\win32\kernel32\errcodes.rc" 1> NUL 2> NUL
        del /f "dll\win32\user32\misc\win32k.S" 1> NUL 2> NUL
        del /f "dll\win32\user32\misc\i386\win32k.S" 1> NUL 2> NUL
        rmdir /q "dll\win32\user32\misc\i386" 1> NUL 2> NUL
        del /f "include\reactos\bugcodes.h" 1> NUL 2> NUL
        del /f "include\reactos\buildno.h" 1> NUL 2> NUL
        del /f "include\reactos\errcodes.h" 1> NUL 2> NUL
        del /f "lib\win32ksys\win32k.S" 1> NUL 2> NUL
        del /f "lib\win32ksys\i386\win32k.S" 1> NUL 2> NUL
        rmdir /q "lib\win32ksys\i386" 1> NUL 2> NUL
        del /f "ntoskrnl\bugcodes.rc" 1> NUL 2> NUL
        del /f "ntoskrnl\ex\zw.S" 1> NUL 2> NUL
        del /f "ntoskrnl\ex\i386\zw.S" 1> NUL 2> NUL
        del /f "ntoskrnl\include\internal\napi.h" 1> NUL 2> NUL
        del /f "subsystems\win32\win32k\dib\dib16gen.c" 1> NUL 2> NUL
        del /f "subsystems\win32\win32k\dib\dib32gen.c" 1> NUL 2> NUL
        del /f "subsystems\win32\win32k\dib\dib8gen.c" 1> NUL 2> NUL
        del /f "subsystems\win32\win32k\include\napi.h" 1> NUL 2> NUL

        echo Done cleaning ReactOS source directory.
    ) else (
        echo ERROR: There is nothing to clean.
    )
    goto :EOC
)
if "%1" == "logs" (
    ::
    : Check if we have any logs to clean, if so, clean them.
    ::
    if exist "%_ROSBE_LOGDIR%\*.txt" (
        echo Cleaning build logs...

        del /f "%_ROSBE_LOGDIR%\*.txt" 1> NUL 2> NUL

        echo Done cleaning build logs.
    ) else (
        echo ERROR: There are no logs to clean.
    )
    goto :EOC
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOC
)

:EOC
title ReactOS Build Environment %_ROSBE_VERSION%
