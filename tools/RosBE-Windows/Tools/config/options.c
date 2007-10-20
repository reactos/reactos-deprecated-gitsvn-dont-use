/*
 * RosBE Options Dialog (options.c)
 *
 * Copyright 2007 by Maarten Bosma
 *                   Pierre Schweitzer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program ; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <windows.h>
#include <stdio.h>
#include <shlobj.h>
#include <wchar.h>
#include "resources.h"

#define MINGWVERSION L"\\4.1.3"

typedef LPITEMIDLIST __stdcall (CALLBACK* ILCREATEFROMPATHW)(LPCWSTR path);

// note: do not change the order - theses are the color under winxp they might differ in another OSes
WCHAR *Colors[] = { L"black", L"dark blue", L"dark green", L"turquoise", L"dark red", L"purple",
    L"ochar", L"light grey", L"dark grey", L"light blue", L"light green",
    L"cyan", L"light red", L"magenta", L"yellow", L"white"
};

// note: do not change the order - it matches to previous order
COLORREF ColorsRGB[] = { 0x00000000, 0x00800000, 0x00008000, 0x00808000, 0x00000080, 0x00800080,
    0x00008080, 0x00c0c0c0, 0x00808080, 0x00ff0000, 0x0000ff00,
    0x00ffff00, 0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
};

HINSTANCE hInstance;

INT
WriteSettings(HWND hwnd)
{
    int foreground, background;
    BOOL showtime, writelog, useccache, strip;
    WCHAR logpath[MAX_PATH], mingwpath[MAX_PATH], checkmgw[MAX_PATH], checklog[MAX_PATH];
    WCHAR msgerror[256];
    HANDLE hFile;
    FILE *pFile;

    showtime = SendMessage(GetDlgItem(hwnd, ID_SHOWBUILDTIME), BM_GETCHECK, 0, 0) == BST_CHECKED;
    writelog = SendMessage(GetDlgItem(hwnd, ID_SAVELOGS), BM_GETCHECK, 0, 0) == BST_CHECKED;
    useccache = SendMessage(GetDlgItem(hwnd, ID_USECCACHE), BM_GETCHECK, 0, 0);
    strip = SendMessage(GetDlgItem(hwnd, ID_STRIP), BM_GETCHECK, 0, 0);
    foreground = SendMessage(GetDlgItem(hwnd, IDC_FONT), CB_GETCURSEL, 0, 0);
    background = SendMessage(GetDlgItem(hwnd, IDC_BACK), CB_GETCURSEL, 0, 0);
    GetDlgItemText(hwnd, ID_LOGDIR, logpath, MAX_PATH);
    GetDlgItemText(hwnd, ID_MGWDIR, mingwpath, MAX_PATH);

    if (writelog)
    {
        GetCurrentDirectory(MAX_PATH, checklog);
        if (SetCurrentDirectory(logpath))
        {
            SetCurrentDirectory(checklog);
        }
        else
        {
            if (!CreateDirectory(logpath, NULL))
            {
                LoadString(hInstance, MSG_DIREFAILED, msgerror, 256);
                MessageBox(NULL, msgerror, NULL, MB_ICONERROR);
                return FALSE;
            }
        }
    }

    wcscpy(checkmgw, mingwpath);
    wcscat(checkmgw, L"\\bin\\gcc.exe");
    hFile = CreateFile(checkmgw, (INT)NULL, (INT)NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        LoadString(hInstance, MSG_NOGCCFOUND, msgerror, 256);
        MessageBox(NULL, msgerror, NULL, MB_ICONERROR);
        return FALSE;
    }
    else
    {
        CloseHandle(hFile);
    }

    pFile = fopen("options.cmd", "w");
    if (pFile)
    {
        fprintf(pFile, "REM This file has been automatically created by RosBE Options Dialog\n\n");
        fprintf(pFile, "color %X%X\n", background, foreground);
        fprintf(pFile, "set _ROSBE_SHOWTIME=%d\n", showtime);
        fprintf(pFile, "set _ROSBE_USECCACHE=%d\n", useccache);
        fprintf(pFile, "set _ROSBE_STRIP=%d\n", strip);
        fprintf(pFile, "set _ROSBE_WRITELOG=%d\n", writelog);
        fprintf(pFile, "set _ROSBE_LOGPATH=%S\n", logpath);
        fprintf(pFile, "set _ROSBE_MINGWPATH=%S\n", mingwpath);
        fclose(pFile);
        return TRUE;
    }
    else
    {
        LoadString(hInstance, MSG_FILEFAILED, msgerror, 256);
        MessageBox(NULL, msgerror, NULL, MB_ICONERROR);
        return FALSE;
    }
}

INT_PTR CALLBACK
DlgProc(HWND Dlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    WCHAR defaultmingwpath[MAX_PATH];
    static HICON hIcon;

    switch (Msg)
    {
        case WM_INITDIALOG:
        {
            hIcon = LoadImage( hInstance,
                               MAKEINTRESOURCE(ID_OPTICON),
                               IMAGE_ICON,
                               GetSystemMetrics(SM_CXSMICON),
                               GetSystemMetrics(SM_CYSMICON),
                               0);
            if(hIcon)
                SendMessage(Dlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

            for(unsigned int i = 0; i < sizeof(Colors) / sizeof(char *); i++)
            {
                SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_ADDSTRING, 0, (LPARAM) (Colors[i]));
                SendMessage(GetDlgItem(Dlg, IDC_FONT), CB_ADDSTRING, 0, (LPARAM) (Colors[i]));
            }
            SendMessage(GetDlgItem(Dlg, IDC_FONT), CB_SETCURSEL, 0xA, 0);
            SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_SETCURSEL, 0, 0);
            GetCurrentDirectory(MAX_PATH, defaultmingwpath);
            wcscat(defaultmingwpath, MINGWVERSION);
            SetDlgItemText(Dlg, ID_MGWDIR, defaultmingwpath);

            return TRUE;
        }

        case WM_COMMAND:
        {
            if (wParam == ID_CANCEL)
            {
                PostMessage(Dlg, WM_CLOSE, 0, 0);
            }
            else if (wParam == ID_OK)
            {
                if (WriteSettings(Dlg))
                PostMessage(Dlg, WM_CLOSE, 0, 0);
            }
            else if (wParam == ID_BROWSE)
            {
                WCHAR Path[MAX_PATH];
                BROWSEINFO PathInfo;
                ZeroMemory(&PathInfo, sizeof(BROWSEINFO));
                PathInfo.hwndOwner = Dlg;
                PathInfo.lpszTitle = L"Please choose a directory where the the logs should be stored:";
                LPITEMIDLIST pidl = SHBrowseForFolder(&PathInfo);
                if (pidl && SHGetPathFromIDList(pidl, Path))
                {
                    SetDlgItemText(Dlg, ID_LOGDIR, Path);
                    EnableWindow(GetDlgItem(Dlg, ID_OK), TRUE);
                }
            }
            else if (wParam == ID_BROWSEMGW)
            {
                WCHAR MGWPath[MAX_PATH];
                BROWSEINFO PathInfo;
                HINSTANCE hDLL;
                ILCREATEFROMPATHW ILCreateFromPathW;
                ZeroMemory(&PathInfo, sizeof(BROWSEINFO));
                hDLL = LoadLibrary(L"shell32.dll");
                if (hDLL)
                {
                    ILCreateFromPathW = (ILCREATEFROMPATHW)GetProcAddress(hDLL, "ILCreateFromPathW");
                    if (ILCreateFromPathW)
                    {
                        GetDlgItemText(Dlg, ID_MGWDIR, MGWPath, MAX_PATH);
                        PathInfo.pidlRoot = ILCreateFromPathW(MGWPath);
                    }
                    FreeLibrary(hDLL);
                }
                PathInfo.hwndOwner = Dlg;
                PathInfo.lpszTitle = L"Please choose the directory where MingW is located:";
                LPITEMIDLIST pidl = SHBrowseForFolder(&PathInfo);
                if (pidl && SHGetPathFromIDList(pidl, MGWPath))
                {
                    SetDlgItemText(Dlg, ID_MGWDIR, MGWPath);
                    EnableWindow(GetDlgItem(Dlg, ID_OK), TRUE);
                }
            }
            else if (wParam == ID_SAVELOGS)
            {
                BOOL WriteLogSet = SendMessage(GetDlgItem(Dlg, ID_SAVELOGS), BM_GETCHECK, 0,
                                               0) == BST_CHECKED;
                EnableWindow(GetDlgItem(Dlg, ID_BROWSE), WriteLogSet);
                EnableWindow(GetDlgItem(Dlg, ID_LOGDIR), WriteLogSet);
            }
            else if (wParam == ID_STRIP)
            {
                EnableWindow(GetDlgItem(Dlg, ID_OK), TRUE);
            }
            else if (wParam == ID_USECCACHE)
            {
                EnableWindow(GetDlgItem(Dlg, ID_OK), TRUE);
            }
            else if ((wParam == ID_SHOWBUILDTIME) || ((LOWORD(wParam) == IDC_FONT) && (HIWORD(wParam) == CBN_SELCHANGE)) || ((LOWORD(wParam) == IDC_BACK) && (HIWORD(wParam) == CBN_SELCHANGE)))
            {
                RECT rcWnd;
                GetClientRect(GetDlgItem(Dlg, ID_EXAMPLE), &rcWnd);
                InvalidateRect(GetDlgItem(Dlg, ID_EXAMPLE), &rcWnd, FALSE);
                UpdateWindow(GetDlgItem(Dlg, ID_EXAMPLE));
                EnableWindow(GetDlgItem(Dlg, ID_OK), TRUE);
            }
            return FALSE;
        }

        case WM_CTLCOLORSTATIC :
        {
            if((HWND)lParam == GetDlgItem(Dlg, ID_EXAMPLE))
            {
                SetTextColor((HDC)wParam, ColorsRGB[SendMessage(GetDlgItem(Dlg, IDC_FONT), CB_GETCURSEL, 0, 0)]);
                SetBkColor((HDC)wParam, ColorsRGB[SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_GETCURSEL, 0, 0)]);
                return (LONG)CreateSolidBrush(ColorsRGB[SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_GETCURSEL, 0, 0)]);
            }
            break;
        }

        case WM_DESTROY:
        {
            DestroyIcon(hIcon);
        }

        case WM_CLOSE:
        {
            EndDialog(Dlg, 0);
            return TRUE;
        }
    }
    return FALSE;
}

int WINAPI
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdline, int cmdshow)
{
    hInstance = hInst;

    DialogBox(hInst, MAKEINTRESOURCE(ID_DIALOG), 0, DlgProc);
    return 0;
}
