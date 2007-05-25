#include <windows.h>

HWND GetConsoleHwnd(void)
{
    WCHAR szNewTitle[MAX_PATH];
    WCHAR szOldTitle[MAX_PATH];
    HWND hwnd = NULL;

    if (GetConsoleTitleW(szOldTitle, MAX_PATH))
    {
        wsprintfW(szNewTitle,
                  L"%d/%d",
                  GetTickCount(),
                  GetCurrentProcessId());

        if (SetConsoleTitleW(szNewTitle))
        {
            Sleep(40);

            hwnd = FindWindowW(NULL, szNewTitle);

            SetConsoleTitleW(szOldTitle);
        }
    }

    return hwnd;
}


int main()
{
    HWND hwnd;
    FLASHWINFO fwi = {0,};

    hwnd = GetConsoleHwnd();
    if (hwnd)
    {
        fwi.cbSize = sizeof(FLASHWINFO);
        fwi.hwnd = hwnd;
        fwi.dwFlags = FLASHW_TRAY | FLASHW_TIMERNOFG;
        fwi.uCount = 1;

        FlashWindowEx(&fwi);
    }

    return 0;
}
