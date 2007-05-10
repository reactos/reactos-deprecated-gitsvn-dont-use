/*
 * RosBE Options Dialog (options.c)
 *
 * Copyright 2007 by Maarten Bosma
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
#include "resources.h"

// note: do not change the order - theses are the color under winxp they might differ in another OSes
WCHAR *Colors[] = { L"black", L"dark blue", L"green", L"turquoise", L"dark red", L"purple",
    L"ochar", L"light grey", L"dark grey", L"light blue", L"light green",
    L"cyan", L"light red", L"magenta", L"yellow", L"white"
};

int
WriteSettings(HWND hwnd)
{
    int foreground, background;
    BOOL showtime, writelog;
    WCHAR logpath[MAX_PATH];

    showtime = SendMessage(GetDlgItem(hwnd, ID_SHOWBUILDTIME), BM_GETCHECK, 0, 0) == BST_CHECKED;
    writelog = SendMessage(GetDlgItem(hwnd, ID_SAVELOGS), BM_GETCHECK, 0, 0) == BST_CHECKED;
    foreground = SendMessage(GetDlgItem(hwnd, IDC_BACK), CB_GETCURSEL, 0, 0);
    background = SendMessage(GetDlgItem(hwnd, IDC_FONT), CB_GETCURSEL, 0, 0);
    GetDlgItemText(hwnd, ID_LOGDIR, logpath, MAX_PATH);

    FILE *pFile = fopen("options.cmd", "w");

    fprintf(pFile, "REM This file has been automatically created by RosBE Options Dialog\n\n");
    fprintf(pFile, "color %X%X\n", background, foreground);
    fprintf(pFile, "set ROSBE_SHOWTIME=%d\n", showtime);
    fprintf(pFile, "set ROSBE_WRITELOG=%d\n", writelog);
    fprintf(pFile, "set ROSBE_LOGPATH=%S\n", logpath);

    fclose(pFile);
}

INT_PTR CALLBACK
DlgProc(HWND Dlg, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    switch (Msg)
    {
        case WM_INITDIALOG:
        {
            for(i = 0; i < sizeof(Colors) / sizeof(char *); i++)
            {
                SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_ADDSTRING, 0, (LPARAM) (Colors[i]));
                SendMessage(GetDlgItem(Dlg, IDC_FONT), CB_ADDSTRING, 0, (LPARAM) (Colors[i]));
            }
            SendMessage(GetDlgItem(Dlg, IDC_BACK), CB_SETCURSEL, 0, 0);
            SendMessage(GetDlgItem(Dlg, IDC_FONT), CB_SETCURSEL, 0xA, 0);
            EnableWindow(GetDlgItem(Dlg, ID_BROWSE), FALSE);
            EnableWindow(GetDlgItem(Dlg, ID_LOGDIR), FALSE);
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
                WriteSettings(Dlg);
                PostMessage(Dlg, WM_CLOSE, 0, 0);
            }
            else if (wParam == ID_BROWSE)
            {
                WCHAR Path[MAX_PATH];
                BROWSEINFO PathInfo = { 0 };
                PathInfo.hwndOwner = Dlg;
                PathInfo.lpszTitle = L"Please choose a directory where the the logs should be stored:";
                LPITEMIDLIST pidl = SHBrowseForFolder(&PathInfo);
                if (pidl && SHGetPathFromIDList(pidl, Path))
                    SetDlgItemText(Dlg, ID_LOGDIR, Path);
            }
            else if (wParam == ID_SAVELOGS)
            {
                BOOL WriteLogSet = SendMessage(GetDlgItem(Dlg, ID_SAVELOGS), BM_GETCHECK, 0,
                                               0) == BST_CHECKED;
                EnableWindow(GetDlgItem(Dlg, ID_BROWSE), WriteLogSet);
                EnableWindow(GetDlgItem(Dlg, ID_LOGDIR), WriteLogSet);
            }
            return FALSE;
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
    DialogBox(hInst, MAKEINTRESOURCE(ID_DIALOG), 0, DlgProc);
    return 0;
}
