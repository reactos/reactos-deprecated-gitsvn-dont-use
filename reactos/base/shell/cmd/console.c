/*
 *  CONSOLE.C - console input/output functions.
 *
 *
 *  History:
 *
 *    20-Jan-1999 (Eric Kohl)
 *        started
 *
 *    03-Apr-2005 (Magnus Olsen <magnus@greatlord.com>)
 *        Remove all hardcoded strings in En.rc
 *
 *    01-Jul-2005 (Brandon Turner <turnerb7@msu.edu>)
 *        Added ConPrintfPaging and ConOutPrintfPaging
 *
 *    02-Feb-2007 (Paolo Devoti <devotip at gmail.com>)
 *        Fixed ConPrintfPaging
 */

#include "precomp.h"

#define OUTPUT_BUFFER_SIZE  4096

/* Cache codepage for text streams */
UINT InputCodePage;
UINT OutputCodePage;


BOOL IsConsoleHandle(HANDLE hHandle)
{
    DWORD dwMode;

    /* Check whether the handle may be that of a console... */
    if ((GetFileType(hHandle) & ~FILE_TYPE_REMOTE) != FILE_TYPE_CHAR)
        return FALSE;

    /*
     * It may be. Perform another test... The idea comes from the
     * MSDN description of the WriteConsole API:
     *
     * "WriteConsole fails if it is used with a standard handle
     *  that is redirected to a file. If an application processes
     *  multilingual output that can be redirected, determine whether
     *  the output handle is a console handle (one method is to call
     *  the GetConsoleMode function and check whether it succeeds).
     *  If the handle is a console handle, call WriteConsole. If the
     *  handle is not a console handle, the output is redirected and
     *  you should call WriteFile to perform the I/O."
     */
    return GetConsoleMode(hHandle, &dwMode);
}



/********************* Console STREAM IN utility functions ********************/

VOID ConInDisable(VOID)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwMode;

    GetConsoleMode(hInput, &dwMode);
    dwMode &= ~ENABLE_PROCESSED_INPUT;
    SetConsoleMode(hInput, dwMode);
}

VOID ConInEnable(VOID)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD dwMode;

    GetConsoleMode(hInput, &dwMode);
    dwMode |= ENABLE_PROCESSED_INPUT;
    SetConsoleMode(hInput, dwMode);
}

VOID ConInFlush(VOID)
{
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}

VOID ConInKey(PINPUT_RECORD lpBuffer)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD  dwRead;

    if (hInput == INVALID_HANDLE_VALUE)
        WARN ("Invalid input handle!!!\n");

    do
    {
        ReadConsoleInput(hInput, lpBuffer, 1, &dwRead);
        if ((lpBuffer->EventType == KEY_EVENT) &&
            (lpBuffer->Event.KeyEvent.bKeyDown != FALSE))
            break;
    }
    while (TRUE);
}

VOID ConInString(LPTSTR lpInput, DWORD dwLength)
{
    DWORD dwOldMode;
    DWORD dwRead = 0;
    HANDLE hFile;

    LPTSTR p;
    PCHAR pBuf;

#ifdef _UNICODE
    pBuf = (PCHAR)cmd_alloc(dwLength - 1);
#else
    pBuf = lpInput;
#endif
    ZeroMemory(lpInput, dwLength * sizeof(TCHAR));
    hFile = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(hFile, &dwOldMode);

    SetConsoleMode(hFile, ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);

    ReadFile(hFile, (PVOID)pBuf, dwLength - 1, &dwRead, NULL);

#ifdef _UNICODE
    MultiByteToWideChar(InputCodePage, 0, pBuf, dwRead, lpInput, dwLength - 1);
    cmd_free(pBuf);
#endif
    for (p = lpInput; *p; p++)
    {
        if (*p == _T('\x0d'))
        {
            *p = _T('\0');
            break;
        }
    }

    SetConsoleMode(hFile, dwOldMode);
}



/******************** Console STREAM OUT utility functions ********************/

static VOID ConWrite(DWORD nStdHandle, TCHAR *str, DWORD len)
{
    DWORD dwNumBytes = 0;
    HANDLE hOutput = GetStdHandle(nStdHandle);
    PVOID p;

    /* If we don't write anything, just return */
    if (!str || len == 0)
        return;

    /* Check whether we are writing to a console and if so, write to it */
    if (IsConsoleHandle(hOutput))
    {
        WriteConsole(hOutput, str, len, &dwNumBytes, NULL);
        return;
    }

    /* We're writing to a file or pipe instead of the console. Convert the
     * string from TCHARs to the desired output format, if the two differ */
    if (bUnicodeOutput)
    {
#ifndef _UNICODE
        WCHAR *buffer = cmd_alloc(len * sizeof(WCHAR));
        if (!buffer)
        {
            error_out_of_memory();
            return;
        }
        len = (DWORD)MultiByteToWideChar(OutputCodePage, 0, str, (INT)len, buffer, (INT)len);
        str = (PVOID)buffer;
#endif
        /*
         * Find any newline character in the buffer,
         * write the part BEFORE the newline, then write
         * a carriage-return + newline, and then write
         * the remaining part of the buffer.
         *
         * This fixes output in files and serial console.
         */
        while (len > 0)
        {
            /* Loop until we find a \r or \n character */
            // FIXME: What about the pair \r\n ?
            p = str;
            while (len > 0 && *(PWCHAR)p != L'\r' && *(PWCHAR)p != L'\n')
            {
                /* Advance one character */
                p = (PVOID)((PWCHAR)p + 1);
                len--;
            }

            /* Write everything up to \r or \n */
            dwNumBytes = ((PWCHAR)p - (PWCHAR)str) * sizeof(WCHAR);
            WriteFile(hOutput, str, dwNumBytes, &dwNumBytes, NULL);

            /* If we hit \r or \n ... */
            if (len > 0 && (*(PWCHAR)p == L'\r' || *(PWCHAR)p == L'\n'))
            {
                /* ... send a carriage-return + newline sequence and skip \r or \n */
                WriteFile(hOutput, L"\r\n", 2 * sizeof(WCHAR), &dwNumBytes, NULL);
                str = (PVOID)((PWCHAR)p + 1);
                len--;
            }
        }

#ifndef _UNICODE
        cmd_free(buffer);
#endif
    }
    else
    {
#ifdef _UNICODE
        CHAR *buffer = cmd_alloc(len * MB_LEN_MAX * sizeof(CHAR));
        if (!buffer)
        {
            error_out_of_memory();
            return;
        }
        len = WideCharToMultiByte(OutputCodePage, 0, str, len, buffer, len * MB_LEN_MAX, NULL, NULL);
        str = (PVOID)buffer;
#endif
        /*
         * Find any newline character in the buffer,
         * write the part BEFORE the newline, then write
         * a carriage-return + newline, and then write
         * the remaining part of the buffer.
         *
         * This fixes output in files and serial console.
         */
        while (len > 0)
        {
            /* Loop until we find a \r or \n character */
            // FIXME: What about the pair \r\n ?
            p = str;
            while (len > 0 && *(PCHAR)p != '\r' && *(PCHAR)p != '\n')
            {
                /* Advance one character */
                p = (PVOID)((PCHAR)p + 1);
                len--;
            }

            /* Write everything up to \r or \n */
            dwNumBytes = ((PCHAR)p - (PCHAR)str) * sizeof(CHAR);
            WriteFile(hOutput, str, dwNumBytes, &dwNumBytes, NULL);

            /* If we hit \r or \n ... */
            if (len > 0 && (*(PCHAR)p == '\r' || *(PCHAR)p == '\n'))
            {
                /* ... send a carriage-return + newline sequence and skip \r or \n */
                WriteFile(hOutput, "\r\n", 2, &dwNumBytes, NULL);
                str = (PVOID)((PCHAR)p + 1);
                len--;
            }
        }

#ifdef _UNICODE
        cmd_free(buffer);
#endif
    }
}

VOID ConPuts(DWORD nStdHandle, LPTSTR szText)
{
    ConWrite(nStdHandle, szText, (DWORD)_tcslen(szText));
}

VOID ConResPuts(DWORD nStdHandle, UINT resID)
{
    TCHAR szMsg[RC_STRING_MAX_SIZE];
    LoadString(CMD_ModuleHandle, resID, szMsg, ARRAYSIZE(szMsg));
    ConPuts(nStdHandle, szMsg);
}

VOID ConOutChar(TCHAR c)
{
    ConWrite(STD_OUTPUT_HANDLE, &c, 1);
}

VOID ConErrChar(TCHAR c)
{
    ConWrite(STD_ERROR_HANDLE, &c, 1);
}

VOID ConPrintfV(DWORD nStdHandle, LPTSTR szFormat, va_list arg_ptr)
{
    TCHAR szOut[OUTPUT_BUFFER_SIZE];
    DWORD len;

    len = (DWORD)_vstprintf(szOut, szFormat, arg_ptr);
    ConWrite(nStdHandle, szOut, len);
}

VOID ConPrintf(DWORD nStdHandle, LPTSTR szFormat, ...)
{
    va_list arg_ptr;

    va_start(arg_ptr, szFormat);
    ConPrintfV(nStdHandle, szFormat, arg_ptr);
    va_end(arg_ptr);
}

VOID ConResPrintf(DWORD nStdHandle, UINT resID, ...)
{
    TCHAR szMsg[RC_STRING_MAX_SIZE];
    va_list arg_ptr;

    va_start(arg_ptr, resID);
    LoadString(CMD_ModuleHandle, resID, szMsg, ARRAYSIZE(szMsg));
    ConPrintfV(nStdHandle, szMsg, arg_ptr);
    va_end(arg_ptr);
}

VOID ConFormatMessage(DWORD nStdHandle, DWORD MessageId, ...)
{
    DWORD ret;
    LPTSTR text;
    va_list arg_ptr;

    va_start(arg_ptr, MessageId);
    ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                        NULL,
                        MessageId,
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (LPTSTR)&text,
                        0,
                        &arg_ptr);
    va_end(arg_ptr);

    if (ret > 0)
    {
        ConPuts(nStdHandle, text);
        LocalFree(text);
    }
    else
    {
        ConResPrintf(nStdHandle, STRING_CONSOLE_ERROR, MessageId);
    }
}



/************************** Console PAGER functions ***************************/

BOOL ConPrintfVPaging(DWORD nStdHandle, BOOL NewPage, LPTSTR szFormat, va_list arg_ptr)
{
    INT len;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    TCHAR szOut[OUTPUT_BUFFER_SIZE];
    DWORD dwWritten;
    HANDLE hOutput = GetStdHandle(nStdHandle);

    /* Used to count number of lines since last pause */
    static int LineCount = 0;

    /* Used to see how big the screen is */
    int ScreenLines = 0;

    /* Chars since start of line */
    int CharSL;

    int from = 0, i = 0;

    if (NewPage != FALSE)
        LineCount = 0;

    /* Reset LineCount and return if no string has been given */
    if (szFormat == NULL)
        return TRUE;

    /* Get the size of the visual screen that can be printed to */
    if (!IsConsoleHandle(hOutput) || !GetConsoleScreenBufferInfo(hOutput, &csbi))
    {
        /* We assume it's a file handle */
        ConPrintfV(nStdHandle, szFormat, arg_ptr);
        return TRUE;
    }

    /*
     * Get the number of lines currently displayed on screen, minus 1
     * to account for the "press any key..." prompt from PagePrompt().
     */
    ScreenLines = (csbi.srWindow.Bottom - csbi.srWindow.Top);
    CharSL = csbi.dwCursorPosition.X;

    /* Make sure the user doesn't have the screen too small */
    if (ScreenLines < 4)
    {
        ConPrintfV(nStdHandle, szFormat, arg_ptr);
        return TRUE;
    }

    len = _vstprintf(szOut, szFormat, arg_ptr);

    while (i < len)
    {
        /* Search until the end of a line is reached */
        if (szOut[i++] != _T('\n') && ++CharSL < csbi.dwSize.X)
            continue;

        LineCount++;
        CharSL=0;

        if (LineCount >= ScreenLines)
        {
            WriteConsole(hOutput, &szOut[from], i-from, &dwWritten, NULL);
            from = i;

            /* Prompt the user */
            if (PagePrompt() != PROMPT_YES)
                return FALSE;

            // TODO: Recalculate 'ScreenLines' in case the user redimensions
            // the window during the prompt.

            /* Reset the number of lines being printed */
            LineCount = 0;
        }
    }

    WriteConsole(hOutput, &szOut[from], i-from, &dwWritten, NULL);

    return TRUE;
}

BOOL ConOutPrintfPaging(BOOL NewPage, LPTSTR szFormat, ...)
{
    BOOL bRet;
    va_list arg_ptr;

    va_start(arg_ptr, szFormat);
    bRet = ConPrintfVPaging(STD_OUTPUT_HANDLE, NewPage, szFormat, arg_ptr);
    va_end(arg_ptr);
    return bRet;
}

VOID ConOutResPaging(BOOL NewPage, UINT resID)
{
    TCHAR szMsg[RC_STRING_MAX_SIZE];
    LoadString(CMD_ModuleHandle, resID, szMsg, ARRAYSIZE(szMsg));
    ConOutPrintfPaging(NewPage, szMsg);
}



/************************** Console SCREEN functions **************************/

VOID SetCursorXY(SHORT x, SHORT y)
{
    COORD coPos;

    coPos.X = x;
    coPos.Y = y;
    SetConsoleCursorPosition(GetStdHandle (STD_OUTPUT_HANDLE), coPos);
}

VOID GetCursorXY(PSHORT x, PSHORT y)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    *x = csbi.dwCursorPosition.X;
    *y = csbi.dwCursorPosition.Y;
}

SHORT GetCursorX(VOID)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}

SHORT GetCursorY(VOID)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}

VOID SetCursorType(BOOL bInsert, BOOL bVisible)
{
    CONSOLE_CURSOR_INFO cci;

    cci.dwSize = bInsert ? 10 : 99;
    cci.bVisible = bVisible;

    SetConsoleCursorInfo(GetStdHandle (STD_OUTPUT_HANDLE), &cci);
}

VOID GetScreenSize(PSHORT maxx, PSHORT maxy)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        csbi.dwSize.X = 80;
        csbi.dwSize.Y = 25;
    }

    if (maxx) *maxx = csbi.dwSize.X;
    if (maxy) *maxy = csbi.dwSize.Y;
}




#ifdef INCLUDE_CMD_COLOR

BOOL ConGetDefaultAttributes(PWORD pwDefAttr)
{
    BOOL Success;
    HANDLE hConsole;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    /* Do not modify *pwDefAttr if we fail, in which case use default attributes */

    hConsole = CreateFile(_T("CONOUT$"), GENERIC_READ|GENERIC_WRITE,
                          FILE_SHARE_READ|FILE_SHARE_WRITE, NULL,
                          OPEN_EXISTING, 0, NULL);
    if (hConsole == INVALID_HANDLE_VALUE)
        return FALSE; // No default console

    Success = GetConsoleScreenBufferInfo(hConsole, &csbi);
    if (Success)
        *pwDefAttr = csbi.wAttributes;

    CloseHandle(hConsole);
    return Success;
}

#endif


BOOL ConSetTitle(IN LPCTSTR lpConsoleTitle)
{
    /* Now really set the console title */
    return SetConsoleTitle(lpConsoleTitle);
}

#ifdef INCLUDE_CMD_BEEP
VOID ConRingBell(HANDLE hOutput)
{
#if 0
    /* Emit an error beep sound */
    if (IsConsoleHandle(hOutput))
        Beep(800, 200);
    else if (IsTTYHandle(hOutput))
        ConOutPuts(_T("\a")); // BEL character 0x07
    else
#endif
        MessageBeep(-1);
}
#endif

#ifdef INCLUDE_CMD_CLS
VOID ConClearScreen(HANDLE hOutput)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD coPos;
    DWORD dwWritten;

    if (GetConsoleScreenBufferInfo(hOutput, &csbi))
    {
        coPos.X = 0;
        coPos.Y = 0;
        FillConsoleOutputAttribute(hOutput, csbi.wAttributes,
                                   csbi.dwSize.X * csbi.dwSize.Y,
                                   coPos, &dwWritten);
        FillConsoleOutputCharacter(hOutput, _T(' '),
                                   csbi.dwSize.X * csbi.dwSize.Y,
                                   coPos, &dwWritten);
        SetConsoleCursorPosition(hOutput, coPos);
    }
    else
    {
        ConOutChar(_T('\f'));
    }
}
#endif

#ifdef INCLUDE_CMD_COLOR
BOOL ConSetScreenColor(HANDLE hOutput, WORD wColor, BOOL bFill)
{
    DWORD dwWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD coPos;

    /* Foreground and Background colors can't be the same */
    if ((wColor & 0x0F) == (wColor & 0xF0) >> 4)
        return FALSE;

    /* Fill the whole background if needed */
    if (bFill)
    {
        GetConsoleScreenBufferInfo(hOutput, &csbi);

        coPos.X = 0;
        coPos.Y = 0;
        FillConsoleOutputAttribute(hOutput,
                                   wColor & 0x00FF,
                                   csbi.dwSize.X * csbi.dwSize.Y,
                                   coPos,
                                   &dwWritten);
    }

    /* Set the text attribute */
    SetConsoleTextAttribute(hOutput, wColor & 0x00FF);
    return TRUE;
}
#endif

/* EOF */
