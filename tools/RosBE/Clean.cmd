:: Copyright (c) Peter Ward.  All rights reserved.
::      (Inspired by Dazzle from TinyKRNL)
::
::    Clean the ReactOS source directory.
@echo off

if "%1" == "" (
    ::
    : Check if we have something to clean, if so, clean it.
    ::
    if exist "%_ROSSOURCEDIR%\obj-i386\." (
        echo Cleaning ReactOS source directory...

        ::
        : Run ReactOS's make clean.
        ::
        if exist "%_ROSSOURCEDIR%\makefile.auto" (
            if not "%CD%" == "%_ROSSOURCEDIR%" (
                pushd "%_ROSSOURCEDIR%"
                "%_MINGWMAKE%" clean 1> NUL 2> NUL
                popd
            ) else (
                "%_MINGWMAKE%" clean 1> NUL 2> NUL
            )
        )

        ::
        : Remove directories/makefile.auto created by the build.
        ::
        if exist "%_ROSSOURCEDIR%\obj-i386\." (
            rd /s /q "%_ROSSOURCEDIR%\obj-i386" 1> NUL 2> NUL
        )
        if exist "%_ROSSOURCEDIR%\output-i386\." (
            rd /s /q "%_ROSSOURCEDIR%\output-i386" 1> NUL 2> NUL
        )
        if exist "%_ROSSOURCEDIR%\reactos\." (
            rd /s /q "%_ROSSOURCEDIR%\reactos" 1> NUL 2> NUL
        )
        if exist "%_ROSSOURCEDIR%\makefile.auto" (
            del "%_ROSSOURCEDIR%\makefile.auto" 1> NUL 2> NUL
        )

        ::
        : Now delete auto-generated/copied files not removed by make clean.
        ::
        del /f "%_ROSSOURCEDIR%\media\inf\syssetup.inf" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\include\roscfg.h" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\ole32\nodrop.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\ole32\drag_move.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\ole32\drag_copy.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\ole32\drag_link.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idc_divideropen.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idc_copy.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idc_divider.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_view_small.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_std_large.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_view_large.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idi_tt_info_sm.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idc_movebutton.cur" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idi_tt_warn_sm.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idi_dragarrow.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_hist_small.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_hist_large.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idt_check.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idi_tt_error_sm.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comctl32\idb_std_small.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\msi\instabsent.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\msi\instlocal.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\msi\instadvert.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\network.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\fontpics.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\pd32_nocollate.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\pd32_collate.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\folder.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\cdrom.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\floppy.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\pd32_landscape.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\folder2.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\800.bmp" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\pd32_portrait.ico" 1> NUL 2> NUL
        del /f "%_ROSSOURCEDIR%\dll\win32\comdlg32\hdisk.ico" 1> NUL 2> NUL

        echo Done cleaning ReactOS source directory.
    ) else (
        echo There is nothing to clean.
    )
    goto :EOF
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
    goto :EOF
)
if not "%1" == "" (
    echo Unknown parameter specified. Try 'help [COMMAND]'.
    goto :EOF
)
