/*
 * PROJECT:     RosBE - ReactOS Build Environment for Windows
 * LICENSE:     GPL - See COPYING in the top level directory
 * FILE:        Tools/getdate.c
 * PURPOSE:     Returns System Date
 * COPYRIGHT:   Copyright 2007 Peter Ward <dralnix@gmail.com>
 *
 */


#include <windows.h>
#include <stdio.h>

int main(void)
{
    SYSTEMTIME LocalSystemTime;
    char CurrentDate[20];

    GetSystemTime(&LocalSystemTime);
    GetDateFormat(LOCALE_USER_DEFAULT,
                  0,
                  &LocalSystemTime,
                  "MM/dd/yyyy",
                  CurrentDate,
                  20);

    printf("%s", CurrentDate);

    return 0;
}
