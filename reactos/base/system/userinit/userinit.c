/*
 *  ReactOS applications
 *  Copyright (C) 2001, 2002 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
/*
 * COPYRIGHT:   See COPYING in the top level directory
 * PROJECT:     ReactOS Userinit Logon Application
 * FILE:        base/system/userinit/userinit.c
 * PROGRAMMERS: Thomas Weidenmueller (w3seek@users.sourceforge.net)
 *              Herv� Poussineau (hpoussin@reactos.org)
 */

#include "userinit.h"

#define CMP_MAGIC  0x01234567

/* GLOBALS ******************************************************************/

HINSTANCE hInstance;


/* FUNCTIONS ****************************************************************/

LONG
ReadRegSzKey(
    IN HKEY hKey,
    IN LPCWSTR pszKey,
    OUT LPWSTR *pValue)
{
    LONG rc;
    DWORD dwType;
    DWORD cbData = 0;
    LPWSTR Value;

    TRACE("(%p, %s, %p)\n", hKey, debugstr_w(pszKey), pValue);

    rc = RegQueryValueExW(hKey, pszKey, NULL, &dwType, NULL, &cbData);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegQueryValueEx(%s) failed with error %lu\n", debugstr_w(pszKey), rc);
        return rc;
    }
    if (dwType != REG_SZ)
    {
        WARN("Wrong registry data type (%u vs %u)\n", dwType, REG_SZ);
        return ERROR_FILE_NOT_FOUND;
    }
    Value = (WCHAR*) HeapAlloc(GetProcessHeap(), 0, cbData + sizeof(WCHAR));
    if (!Value)
    {
        WARN("No memory\n");
        return ERROR_NOT_ENOUGH_MEMORY;
    }
    rc = RegQueryValueExW(hKey, pszKey, NULL, NULL, (LPBYTE)Value, &cbData);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegQueryValueEx(%s) failed with error %lu\n", debugstr_w(pszKey), rc);
        HeapFree(GetProcessHeap(), 0, Value);
        return rc;
    }
    /* NULL-terminate the string */
    Value[cbData / sizeof(WCHAR)] = L'\0';

    *pValue = Value;
    return ERROR_SUCCESS;
}

static BOOL
IsConsoleShell(VOID)
{
    HKEY ControlKey = NULL;
    LPWSTR SystemStartOptions = NULL;
    LPWSTR CurrentOption, NextOption; /* Pointers into SystemStartOptions */
    LONG rc;
    BOOL ret = FALSE;

    TRACE("()\n");

    rc = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        REGSTR_PATH_CURRENT_CONTROL_SET,
        0,
        KEY_QUERY_VALUE,
        &ControlKey);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegOpenKeyEx() failed with error %lu\n", rc);
        goto cleanup;
    }

    rc = ReadRegSzKey(ControlKey, L"SystemStartOptions", &SystemStartOptions);
    if (rc != ERROR_SUCCESS)
    {
        WARN("ReadRegSzKey() failed with error %lu\n", rc);
        goto cleanup;
    }

    /* Check for CONSOLE switch in SystemStartOptions */
    CurrentOption = SystemStartOptions;
    while (CurrentOption)
    {
        NextOption = wcschr(CurrentOption, L' ');
        if (NextOption)
            *NextOption = L'\0';
        if (_wcsicmp(CurrentOption, L"CONSOLE") == 0)
        {
            TRACE("Found 'CONSOLE' boot option\n");
            ret = TRUE;
            goto cleanup;
        }
        CurrentOption = NextOption ? NextOption + 1 : NULL;
    }

cleanup:
    if (ControlKey != NULL)
        RegCloseKey(ControlKey);
    HeapFree(GetProcessHeap(), 0, SystemStartOptions);
    TRACE("IsConsoleShell() returning %d\n", ret);
    return ret;
}

static BOOL
GetShell(
    OUT WCHAR *CommandLine, /* must be at least MAX_PATH long */
    IN HKEY hRootKey)
{
    HKEY hKey;
    DWORD Type, Size;
    WCHAR Shell[MAX_PATH];
    BOOL ConsoleShell = IsConsoleShell();
    LONG rc;

    TRACE("(%p, %p)\n", CommandLine, hRootKey);

    rc = RegOpenKeyExW(hRootKey, L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon",
                       0, KEY_QUERY_VALUE, &hKey);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegOpenKeyEx() failed with error %lu\n", rc);
        return FALSE;
    }

    Size = sizeof(Shell);
    rc = RegQueryValueExW(hKey,
                          ConsoleShell ? L"ConsoleShell" : L"Shell",
                          NULL,
                          &Type,
                          (LPBYTE)Shell,
                          &Size);
    RegCloseKey(hKey);

    if (rc != ERROR_SUCCESS)
    {
        WARN("RegQueryValueEx() failed with error %lu\n", rc);
        return FALSE;
    }

    if ((Type == REG_SZ) || (Type == REG_EXPAND_SZ))
    {
        TRACE("Found command line %s\n", debugstr_w(Shell));
        wcscpy(CommandLine, Shell);
        return TRUE;
    }
    else
    {
        WARN("Wrong type %lu (expected %u or %u)\n", Type, REG_SZ, REG_EXPAND_SZ);
        return FALSE;
    }
}

static VOID
StartAutoApplications(
    IN INT clsid)
{
    WCHAR szPath[MAX_PATH] = {0};
    HRESULT hResult;
    HANDLE hFind;
    WIN32_FIND_DATAW findData;
    SHELLEXECUTEINFOW ExecInfo;
    size_t len;

    TRACE("(%d)\n", clsid);

    hResult = SHGetFolderPathW(NULL, clsid, NULL, SHGFP_TYPE_CURRENT, szPath);
    len = wcslen(szPath);
    if (!SUCCEEDED(hResult) || len == 0)
    {
        WARN("SHGetFolderPath() failed with error %lu\n", GetLastError());
        return;
    }

    wcscat(szPath, L"\\*");
    hFind = FindFirstFileW(szPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        WARN("FindFirstFile(%s) failed with error %lu\n", debugstr_w(szPath), GetLastError());
        return;
    }

    do
    {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (findData.nFileSizeHigh || findData.nFileSizeLow))
        {
            ZeroMemory(&ExecInfo, sizeof(ExecInfo));
            ExecInfo.cbSize = sizeof(ExecInfo);
            wcscpy(&szPath[len+1], findData.cFileName);
            ExecInfo.lpVerb = L"open";
            ExecInfo.lpFile = szPath;
            ExecInfo.lpDirectory = NULL;
            TRACE("Executing %s in directory %s\n",
                debugstr_w(findData.cFileName), debugstr_w(szPath));
            ShellExecuteExW(&ExecInfo);
        }
    } while (FindNextFileW(hFind, &findData));
    FindClose(hFind);
}

static BOOL
TryToStartShell(
    IN LPCWSTR Shell)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    WCHAR ExpandedShell[MAX_PATH];

    TRACE("(%s)\n", debugstr_w(Shell));

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNORMAL;
    ZeroMemory(&pi, sizeof(pi));

    ExpandEnvironmentStringsW(Shell, ExpandedShell, ARRAYSIZE(ExpandedShell));

    if (!CreateProcessW(NULL,
                        ExpandedShell,
                        NULL,
                        NULL,
                        FALSE,
                        NORMAL_PRIORITY_CLASS,
                        NULL,
                        NULL,
                        &si,
                        &pi))
    {
        WARN("CreateProcess() failed with error %lu\n", GetLastError());
        return FALSE;
    }

    StartAutoApplications(CSIDL_STARTUP);
    StartAutoApplications(CSIDL_COMMON_STARTUP);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return TRUE;
}

static BOOL
StartShell(VOID)
{
    WCHAR Shell[MAX_PATH];
    WCHAR szMsg[RC_STRING_MAX_SIZE];
    DWORD Type, Size;
    DWORD Value = 0;
    LONG rc;
    HKEY hKey;

    TRACE("()\n");

    /* Safe Mode shell run */
    rc = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                       L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot\\Option",
                       0, KEY_QUERY_VALUE, &hKey);
    if (rc == ERROR_SUCCESS)
    {
        Size = sizeof(Value);
        rc = RegQueryValueExW(hKey, L"UseAlternateShell", NULL,
                              &Type, (LPBYTE)&Value, &Size);
        RegCloseKey(hKey);

        if (rc == ERROR_SUCCESS)
        {
            if (Type == REG_DWORD)
            {
                if (Value)
                {
                    /* Safe Mode Alternate Shell required */
                    rc = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                                       L"SYSTEM\\CurrentControlSet\\Control\\SafeBoot",
                                       0, KEY_READ, &hKey);
                    if (rc == ERROR_SUCCESS)
                    {
                        Size = sizeof(Shell);
                        rc = RegQueryValueExW(hKey, L"AlternateShell", NULL,
                                              &Type, (LPBYTE)Shell, &Size);
                        RegCloseKey(hKey);

                        if (rc == ERROR_SUCCESS)
                        {
                            if ((Type == REG_SZ) || (Type == REG_EXPAND_SZ))
                            {
                                TRACE("Key located - %s\n", debugstr_w(Shell));

                                /* Try to run alternate shell */
                                if (TryToStartShell(Shell))
                                {
                                    TRACE("Alternate shell started (Safe Mode)\n");
                                    return TRUE;
                                }
                            }
                            else
                            {
                                WARN("Wrong type %lu (expected %u or %u)\n",
                                     Type, REG_SZ, REG_EXPAND_SZ);
                            }
                        }
                        else
                        {
                            WARN("Alternate shell in Safe Mode required but not specified.");
                        }
                    }
                }
            }
            else
            {
                WARN("Wrong type %lu (expected %u)\n", Type, REG_DWORD);
            }
        }
    }

    /* Try to run shell in user key */
    if (GetShell(Shell, HKEY_CURRENT_USER) && TryToStartShell(Shell))
    {
        TRACE("Started shell from HKEY_CURRENT_USER\n");
        return TRUE;
    }

    /* Try to run shell in local machine key */
    if (GetShell(Shell, HKEY_LOCAL_MACHINE) && TryToStartShell(Shell))
    {
        TRACE("Started shell from HKEY_LOCAL_MACHINE\n");
        return TRUE;
    }

    /* Try default shell */
    if (IsConsoleShell())
    {
        if (GetSystemDirectoryW(Shell, ARRAYSIZE(Shell) - 8))
            wcscat(Shell, L"\\cmd.exe");
        else
            wcscpy(Shell, L"cmd.exe");
    }
    else
    {
        if (GetWindowsDirectoryW(Shell, ARRAYSIZE(Shell) - 13))
            wcscat(Shell, L"\\explorer.exe");
        else
            wcscpy(Shell, L"explorer.exe");
    }

    if (!TryToStartShell(Shell))
    {
        WARN("Failed to start default shell %s\n", debugstr_w(Shell));
        LoadStringW(GetModuleHandle(NULL), IDS_SHELL_FAIL, szMsg, ARRAYSIZE(szMsg));
        MessageBoxW(NULL, szMsg, NULL, MB_OK);
        return FALSE;
    }
    return TRUE;
}

const WCHAR g_RegColorNames[][32] = {
    L"Scrollbar",             /* 00 = COLOR_SCROLLBAR */
    L"Background",            /* 01 = COLOR_DESKTOP */
    L"ActiveTitle",           /* 02 = COLOR_ACTIVECAPTION  */
    L"InactiveTitle",         /* 03 = COLOR_INACTIVECAPTION */
    L"Menu",                  /* 04 = COLOR_MENU */
    L"Window",                /* 05 = COLOR_WINDOW */
    L"WindowFrame",           /* 06 = COLOR_WINDOWFRAME */
    L"MenuText",              /* 07 = COLOR_MENUTEXT */
    L"WindowText",            /* 08 = COLOR_WINDOWTEXT */
    L"TitleText",             /* 09 = COLOR_CAPTIONTEXT */
    L"ActiveBorder",          /* 10 = COLOR_ACTIVEBORDER */
    L"InactiveBorder",        /* 11 = COLOR_INACTIVEBORDER */
    L"AppWorkSpace",          /* 12 = COLOR_APPWORKSPACE */
    L"Hilight",               /* 13 = COLOR_HIGHLIGHT */
    L"HilightText",           /* 14 = COLOR_HIGHLIGHTTEXT */
    L"ButtonFace",            /* 15 = COLOR_BTNFACE */
    L"ButtonShadow",          /* 16 = COLOR_BTNSHADOW */
    L"GrayText",              /* 17 = COLOR_GRAYTEXT */
    L"ButtonText",            /* 18 = COLOR_BTNTEXT */
    L"InactiveTitleText",     /* 19 = COLOR_INACTIVECAPTIONTEXT */
    L"ButtonHilight",         /* 20 = COLOR_BTNHIGHLIGHT */
    L"ButtonDkShadow",        /* 21 = COLOR_3DDKSHADOW */
    L"ButtonLight",           /* 22 = COLOR_3DLIGHT */
    L"InfoText",              /* 23 = COLOR_INFOTEXT */
    L"InfoWindow",            /* 24 = COLOR_INFOBK */
    L"ButtonAlternateFace",   /* 25 = COLOR_ALTERNATEBTNFACE */
    L"HotTrackingColor",      /* 26 = COLOR_HOTLIGHT */
    L"GradientActiveTitle",   /* 27 = COLOR_GRADIENTACTIVECAPTION */
    L"GradientInactiveTitle", /* 28 = COLOR_GRADIENTINACTIVECAPTION */
    L"MenuHilight",           /* 29 = COLOR_MENUHILIGHT */
    L"MenuBar"                /* 30 = COLOR_MENUBAR */
};

static COLORREF
StrToColorref(
    IN LPWSTR lpszCol)
{
    BYTE rgb[3];

    TRACE("(%s)\n", debugstr_w(lpszCol));

    rgb[0] = (BYTE)wcstoul(lpszCol, &lpszCol, 10);
    rgb[1] = (BYTE)wcstoul(lpszCol, &lpszCol, 10);
    rgb[2] = (BYTE)wcstoul(lpszCol, &lpszCol, 10);
    return RGB(rgb[0], rgb[1], rgb[2]);
}

static VOID
SetUserSysColors(VOID)
{
    HKEY hKey;
    INT i;
    WCHAR szColor[25];
    DWORD Type, Size;
    COLORREF crColor;
    LONG rc;

    TRACE("()\n");

    rc = RegOpenKeyExW(HKEY_CURRENT_USER, REGSTR_PATH_COLORS,
                       0, KEY_QUERY_VALUE, &hKey);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegOpenKeyEx() failed with error %lu\n", rc);
        return;
    }

    for (i = 0; i < ARRAYSIZE(g_RegColorNames); i++)
    {
        Size = sizeof(szColor);
        rc = RegQueryValueExW(hKey, g_RegColorNames[i], NULL, &Type,
                              (LPBYTE)szColor, &Size);
        if (rc == ERROR_SUCCESS && Type == REG_SZ)
        {
            crColor = StrToColorref(szColor);
            SetSysColors(1, &i, &crColor);
        }
        else
        {
            WARN("RegQueryValueEx(%s) failed with error %lu\n",
                debugstr_w(g_RegColorNames[i]), rc);
        }
    }

    RegCloseKey(hKey);
}

static VOID
SetUserWallpaper(VOID)
{
    HKEY hKey;
    DWORD Type, Size;
    WCHAR szWallpaper[MAX_PATH + 1];
    LONG rc;

    TRACE("()\n");

    rc = RegOpenKeyExW(HKEY_CURRENT_USER, REGSTR_PATH_DESKTOP,
                       0, KEY_QUERY_VALUE, &hKey);
    if (rc != ERROR_SUCCESS)
    {
        WARN("RegOpenKeyEx() failed with error %lu\n", rc);
        return;
    }

    Size = sizeof(szWallpaper);
    rc = RegQueryValueExW(hKey,
                          L"Wallpaper",
                          NULL,
                          &Type,
                          (LPBYTE)szWallpaper,
                          &Size);
    RegCloseKey(hKey);

    if (rc == ERROR_SUCCESS && Type == REG_SZ)
    {
        ExpandEnvironmentStringsW(szWallpaper, szWallpaper, ARRAYSIZE(szWallpaper));
        TRACE("Using wallpaper %s\n", debugstr_w(szWallpaper));

        /* Load and change the wallpaper */
        SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, szWallpaper, SPIF_SENDCHANGE);
    }
    else
    {
        /* Remove the wallpaper */
        TRACE("No wallpaper set in registry (error %lu)\n", rc);
        SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_SENDCHANGE);
    }
}

static VOID
SetUserSettings(VOID)
{
    TRACE("()\n");

    UpdatePerUserSystemParameters(1, TRUE);
    SetUserSysColors();
    SetUserWallpaper();
}

typedef DWORD (WINAPI *PCMP_REPORT_LOGON)(DWORD, DWORD);

static VOID
NotifyLogon(VOID)
{
    HINSTANCE hModule;
    PCMP_REPORT_LOGON CMP_Report_LogOn;

    TRACE("()\n");

    hModule = LoadLibraryW(L"setupapi.dll");
    if (!hModule)
    {
        WARN("LoadLibrary() failed with error %lu\n", GetLastError());
        return;
    }

    CMP_Report_LogOn = (PCMP_REPORT_LOGON)GetProcAddress(hModule, "CMP_Report_LogOn");
    if (CMP_Report_LogOn)
        CMP_Report_LogOn(CMP_MAGIC, GetCurrentProcessId());
    else
        WARN("GetProcAddress() failed\n");

    FreeLibrary(hModule);
}

static BOOL
StartInstaller(VOID)
{
    WCHAR Shell[MAX_PATH];
    WCHAR szMsg[RC_STRING_MAX_SIZE];

    if (GetWindowsDirectoryW(Shell, ARRAYSIZE(Shell) - 12))
        wcscat(Shell, L"\\reactos.exe");
    else
        wcscpy(Shell, L"reactos.exe");

    if (!TryToStartShell(Shell))
    {
        WARN("Failed to start the installer: %s\n", debugstr_w(Shell));
        LoadStringW(GetModuleHandle(NULL), IDS_INSTALLER_FAIL, szMsg, ARRAYSIZE(szMsg));
        MessageBoxW(NULL, szMsg, NULL, MB_OK);
        return FALSE;
    }
    return TRUE;
}

/* Used to get the shutdown privilege */
static BOOL
EnablePrivilege(LPCWSTR lpszPrivilegeName, BOOL bEnablePrivilege)
{
    BOOL   Success;
    HANDLE hToken;
    TOKEN_PRIVILEGES tp;

    Success = OpenProcessToken(GetCurrentProcess(),
                               TOKEN_ADJUST_PRIVILEGES,
                               &hToken);
    if (!Success) return Success;

    Success = LookupPrivilegeValueW(NULL,
                                    lpszPrivilegeName,
                                    &tp.Privileges[0].Luid);
    if (!Success) goto Quit;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = (bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0);

    Success = AdjustTokenPrivileges(hToken, FALSE, &tp, 0, NULL, NULL);

Quit:
    CloseHandle(hToken);
    return Success;
}


int WINAPI
wWinMain(IN HINSTANCE hInst,
         IN HINSTANCE hPrevInstance,
         IN LPWSTR lpszCmdLine,
         IN int nCmdShow)
{
    BOOL bIsLiveCD, Success = TRUE;
    STATE State;

    hInstance = hInst;

    bIsLiveCD = IsLiveCD();

Restart:
    SetUserSettings();

    if (bIsLiveCD)
    {
        State.NextPage = LOCALEPAGE;
        State.Run = SHELL;
    }
    else
    {
        State.NextPage = DONE;
        State.Run = SHELL;
    }

    if (State.NextPage != DONE) // && bIsLiveCD
    {
        RunLiveCD(&State);
    }

    switch (State.Run)
    {
        case SHELL:
            Success = StartShell();
            if (Success)
                NotifyLogon();
            break;

        case INSTALLER:
            Success = StartInstaller();
            break;

        case REBOOT:
        {
            EnablePrivilege(SE_SHUTDOWN_NAME, TRUE);
            ExitWindowsEx(EWX_REBOOT, 0);
            EnablePrivilege(SE_SHUTDOWN_NAME, FALSE);
            Success = TRUE;
            break;
        }

        default:
            Success = FALSE;
            break;
    }

    /*
     * In LiveCD mode, go back to the main menu if we failed
     * to either start the shell or the installer.
     */
    if (bIsLiveCD && !Success)
        goto Restart;

    return 0;
}

/* EOF */
