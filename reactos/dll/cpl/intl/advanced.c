#include "intl.h"
#include <wingdi.h>

#include <debug.h>

typedef struct CPStruct
{
    UINT CodePage;
    DWORD Flags;
    WCHAR Name[MAX_PATH];
    struct CPStruct *NextItem;
} CPAGE, *PCPAGE;

#define CODEPAGE_INSTALLED      0x00000001
#define CODEPAGE_NOT_REMOVEABLE 0x00000002
#define CODEPAGE_INSTALL        0x00000004
#define CODEPAGE_REMOVE         0x00000008

static PCPAGE PCPage = NULL;
static BOOL bSpain = FALSE;
static HWND hLangList;

static BOOL
GetSupportedCP(
    HINF hInf)
{
    WCHAR szSection[MAX_PATH];
    INFCONTEXT Context, Context2;
    PCPAGE pCodePage;
    CPINFOEX cpInfEx;
    UINT uiCodePage;

    if (!SetupFindFirstLine(hInf,
                            L"CodePages",
                            NULL,
                            &Context))
        return FALSE;

    for (;;)
    {
        if (SetupGetIntField(&Context, 0, (PINT)&uiCodePage))
        {
            pCodePage = HeapAlloc(GetProcessHeap(), 0, sizeof(CPAGE));
            if (pCodePage == NULL)
                return FALSE;

            pCodePage->CodePage = uiCodePage;
            pCodePage->Flags = 0;
            (pCodePage->Name)[0] = UNICODE_NULL;

            if (GetCPInfoExW(uiCodePage, 0, &cpInfEx))
            {
                wcscpy(pCodePage->Name, cpInfEx.CodePageName);
            }
            else
            {
                SetupGetStringFieldW(&Context, 1, pCodePage->Name, MAX_PATH, NULL);
            }

            if (wcslen(pCodePage->Name) != 0)
            {
                pCodePage->NextItem = PCPage;
                PCPage = pCodePage;

                wsprintf(szSection, L"CODEPAGE_REMOVE_%d", uiCodePage);

                if ((uiCodePage == GetACP()) ||
                    (uiCodePage == GetOEMCP()) ||
                    (!SetupFindFirstLineW(hInf, szSection, L"AddReg", &Context2)))
                {
                    pCodePage->Flags |= CODEPAGE_NOT_REMOVEABLE;
                }
            }
            else
            {
                HeapFree(GetProcessHeap(), 0, pCodePage);
            }
        }

        if (!SetupFindNextLine(&Context, &Context))
            break;
    }

    return TRUE;
}

static BOOL CALLBACK
InstalledCPProc(PWSTR lpStr)
{
    PCPAGE lpCP;
    UINT uiCP;

    lpCP = PCPage;
    uiCP = _wtol(lpStr);

    for (;;)
    {
        if (!lpCP)
            break;

        if (lpCP->CodePage == uiCP)
        {
            lpCP->Flags |= CODEPAGE_INSTALLED;
            break;
        }

        lpCP = lpCP->NextItem;
    }

    return TRUE;
}

static VOID
InitCodePagesList(HWND hwndDlg)
{
    PCPAGE pCodePage;
    INT ItemIndex;
    HWND hList;
    LV_COLUMN column;
    LV_ITEM item;
    RECT ListRect;
    HINF hIntlInf;

    hIntlInf = SetupOpenInfFileW(L"intl.inf", NULL, INF_STYLE_WIN4, NULL);
    if (hIntlInf == INVALID_HANDLE_VALUE)
        return;

    if (!SetupOpenAppendInfFile(NULL, hIntlInf, NULL))
    {
        SetupCloseInfFile(hIntlInf);
        hIntlInf = NULL;
        return;
    }

    if (!GetSupportedCP(hIntlInf))
        return;

    SetupCloseInfFile(hIntlInf);

    if (!EnumSystemCodePagesW(InstalledCPProc, CP_INSTALLED))
    {
        /* Hack: EnumSystemCodePages returns FALSE on successful completion! */
        /* return; */
    }

    hList = GetDlgItem(hwndDlg, IDC_CONV_TABLES);

    ZeroMemory(&column, sizeof(LV_COLUMN));
    column.mask = LVCF_FMT | LVCF_WIDTH;
    column.fmt = LVCFMT_LEFT;
    GetClientRect(hList, &ListRect);
    column.cx = ListRect.right - GetSystemMetrics(SM_CYHSCROLL);
    ListView_InsertColumn(hList, 0, &column);

    (VOID) ListView_SetExtendedListViewStyle(hList, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

    pCodePage = PCPage;

    for (;;)
    {
        if (pCodePage == NULL)
            break;

        ZeroMemory(&item, sizeof(LV_ITEM));
        item.mask      = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
        item.state     = 0;
        item.stateMask = LVIS_STATEIMAGEMASK;
        item.pszText   = pCodePage->Name;
        item.lParam    = (LPARAM)pCodePage;

        ItemIndex = ListView_InsertItem(hList, &item);
        if (ItemIndex != -1)
        {
            if (pCodePage->Flags & CODEPAGE_INSTALLED)
            {
                ListView_SetItemState(hList, ItemIndex,
                                      INDEXTOSTATEIMAGEMASK(LVIS_SELECTED),
                                      LVIS_STATEIMAGEMASK);
            }
            else
            {
                ListView_SetItemState(hList, ItemIndex,
                                      INDEXTOSTATEIMAGEMASK(LVIS_FOCUSED),
                                      LVIS_STATEIMAGEMASK);
            }
        }

        pCodePage = pCodePage->NextItem;
    }
}

static BOOL CALLBACK
LocalesEnumProc(PWSTR lpLocale)
{
    LCID lcid;
    WCHAR lang[255];
    INT index;
    BOOL bNoShow = FALSE;

    lcid = wcstoul(lpLocale, NULL, 16);

    if (lcid == MAKELCID(MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH), SORT_DEFAULT) ||
        lcid == MAKELCID(MAKELANGID(LANG_SPANISH, SUBLANG_SPANISH_MODERN), SORT_DEFAULT))
    {
        if (bSpain == FALSE)
        {
            LoadStringW(hApplet, IDS_SPAIN, lang, 255);
            bSpain = TRUE;
        }
        else
        {
            bNoShow = TRUE;
        }
    }
    else
    {
        GetLocaleInfoW(lcid, LOCALE_SLANGUAGE, lang, sizeof(lang)/sizeof(WCHAR));
    }

    if (bNoShow == FALSE)
    {
        index = SendMessageW(hLangList,
                             CB_ADDSTRING,
                             0,
                             (LPARAM)lang);

        SendMessageW(hLangList,
                     CB_SETITEMDATA,
                     index,
                     (LPARAM)lcid);
    }

    return TRUE;
}

static VOID
InitLanguagesList(
    HWND hwndDlg,
    PGLOBALDATA pGlobalData)
{
    WCHAR langSel[255];

    hLangList = GetDlgItem(hwndDlg, IDC_LANGUAGE_COMBO);

    bSpain = FALSE;
    EnumSystemLocalesW(LocalesEnumProc, LCID_SUPPORTED);

    /* Select current locale */
    GetLocaleInfoW(pGlobalData->SystemLCID, LOCALE_SLANGUAGE, langSel, sizeof(langSel)/sizeof(WCHAR));

    SendMessageW(hLangList, CB_SELECTSTRING, -1, (LPARAM)langSel);
}

static VOID
GetCurrentDPI(LPTSTR szDPI)
{
    DWORD dwType, dwSize, dwDPI, dwDefDPI = 0x00000060; // Default 96 DPI
    HKEY hKey;

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\FontDPI", 0, NULL,
                        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
    {
        wcscpy(szDPI, L"96");
        return;
    }

    dwType = REG_DWORD;
    dwSize = sizeof(DWORD);

    if (RegQueryValueExW(hKey, L"LogPixels", NULL, &dwType, (LPBYTE)&dwDPI, &dwSize) != ERROR_SUCCESS)
    {
        if (RegSetValueExW(hKey, L"LogPixels", 0, REG_DWORD, (LPBYTE)&dwDefDPI, sizeof(DWORD)) == ERROR_SUCCESS)
        {
            wcscpy(szDPI, L"96");
            RegCloseKey(hKey);
            return;
        }
    }
    else
    {
        wsprintf(szDPI, L"%d", dwDPI);
    }

    RegCloseKey(hKey);
}

static
VOID
SaveFontSubstitutionSettings(
    HWND hwnd,
    PGLOBALDATA pGlobalData)
{
    WCHAR szDefCP[5 + 1], szSection[MAX_PATH], szDPI[3 + 1];
    HINF hFontInf;
    UINT Count;

    GetLocaleInfoW(MAKELCID(pGlobalData->SystemLCID, SORT_DEFAULT), LOCALE_IDEFAULTCODEPAGE, szDefCP, sizeof(szDefCP) / sizeof(WCHAR));
    GetCurrentDPI(szDPI);

    wsprintf(szSection, L"Font.CP%s.%s", szDefCP, szDPI);

    hFontInf = SetupOpenInfFileW(L"font.inf", NULL, INF_STYLE_WIN4, NULL);
    if (hFontInf == INVALID_HANDLE_VALUE)
        return;

    if (!SetupOpenAppendInfFile(NULL, hFontInf, NULL))
    {
        SetupCloseInfFile(hFontInf);
        return;
    }

    Count = (UINT)SetupGetLineCount(hFontInf, szSection);
    if (Count <= 0)
        return;

    if (!SetupInstallFromInfSectionW(hwnd, hFontInf, szSection, SPINST_REGISTRY & ~SPINST_FILES,
                                     NULL, NULL, 0, NULL, NULL, NULL, NULL))
    {
        MessageBoxW(hwnd, L"Unable to install a new language for programs don't support unicode!",
                   NULL, MB_ICONERROR | MB_OK);
    }

    SetupCloseInfFile(hFontInf);
}


static
VOID
SaveFontLinkingSettings(
    HWND hwnd,
    PGLOBALDATA pGlobalData)
{
    /* TODO */
}


static
VOID
SaveSystemSettings(
    PGLOBALDATA pGlobalData)
{
    WCHAR ACPPage[9];
    WCHAR OEMPage[9];
    HKEY langKey;
    DWORD ret;
    WCHAR value[5];
    DWORD valuesize;

    ret = GetLocaleInfoW(MAKELCID(pGlobalData->SystemLCID, SORT_DEFAULT), LOCALE_IDEFAULTCODEPAGE, OEMPage, sizeof(OEMPage)/sizeof(WCHAR));
    if (ret == 0)
    {
        PrintErrorMsgBox(IDS_ERROR_OEM_CODE_PAGE);
        return;
    }

    ret = GetLocaleInfoW(MAKELCID(pGlobalData->SystemLCID, SORT_DEFAULT), LOCALE_IDEFAULTANSICODEPAGE, ACPPage, sizeof(ACPPage)/sizeof(WCHAR));
    if (ret == 0)
    {
        PrintErrorMsgBox(IDS_ERROR_ANSI_CODE_PAGE);
        return;
    }

    /* Set codepages */
    ret = RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\NLS\\CodePage", &langKey);
    if (ret != ERROR_SUCCESS)
    {
        PrintErrorMsgBox(IDS_ERROR_NLS_CODE_REG);
        return;
    }

    RegSetValueExW(langKey, L"OEMCP", 0, REG_SZ, (BYTE *)OEMPage, (wcslen(OEMPage) +1 ) * sizeof(WCHAR));
    RegSetValueExW(langKey, L"ACP", 0, REG_SZ, (BYTE *)ACPPage, (wcslen(ACPPage) +1 ) * sizeof(WCHAR));

    RegCloseKey(langKey);


    wsprintf(value, L"%04hX", LANGIDFROMLCID(pGlobalData->SystemLCID));
    valuesize = (wcslen(value) + 1) * sizeof(WCHAR);

    /* Set language */
    ret = RegOpenKeyW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\NLS\\Language", &langKey);
    if (ret != ERROR_SUCCESS)
    {
        PrintErrorMsgBox(IDS_ERROR_NLS_KEY_REG);
        return;
    }

    RegSetValueExW(langKey, L"Default", 0, REG_SZ, (BYTE *)value, valuesize);
    RegCloseKey(langKey);
}


LRESULT
ListViewCustomDraw(
    LPARAM lParam)
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

    switch (lplvcd->nmcd.dwDrawStage)
    {
        case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;

        case CDDS_ITEMPREPAINT:
            if (((PCPAGE)lplvcd->nmcd.lItemlParam)->Flags & CODEPAGE_NOT_REMOVEABLE)
            {
                lplvcd->clrText   = GetSysColor(COLOR_GRAYTEXT);
                lplvcd->clrTextBk = GetSysColor(COLOR_WINDOW);
                return CDRF_NEWFONT;
            }
            else
            {
                lplvcd->clrText   = GetSysColor(COLOR_WINDOWTEXT);
                lplvcd->clrTextBk = GetSysColor(COLOR_WINDOW);
                return CDRF_NEWFONT;
            }
            break;
    }

    return CDRF_DODEFAULT;
}


/* Property page dialog callback */
INT_PTR CALLBACK
AdvancedPageProc(HWND hwndDlg,
                 UINT uMsg,
                 WPARAM wParam,
                 LPARAM lParam)
{
    PGLOBALDATA pGlobalData;

    pGlobalData = (PGLOBALDATA)GetWindowLongPtr(hwndDlg, DWLP_USER);

    switch (uMsg)
    {
        case WM_INITDIALOG:
            pGlobalData = (PGLOBALDATA)((LPPROPSHEETPAGE)lParam)->lParam;
            SetWindowLongPtr(hwndDlg, DWLP_USER, (LONG_PTR)pGlobalData);

            InitLanguagesList(hwndDlg, pGlobalData);
            InitCodePagesList(hwndDlg);
            break;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDC_LANGUAGE_COMBO:
                    if (HIWORD(wParam) == CBN_SELCHANGE)
                    {
                        LCID lcid;
                        INT iIndex;

                        iIndex = SendMessage(hLangList, CB_GETCURSEL, 0, 0);
                        if (iIndex == CB_ERR)
                            break;

                        lcid = SendMessage(hLangList, CB_GETITEMDATA, iIndex, 0);
                        if (lcid == (LCID)CB_ERR)
                            break;

                        pGlobalData->SystemLCID = lcid;

                        PropSheet_Changed(GetParent(hwndDlg), hwndDlg);
                    }
                    break;

                case IDC_APPLY_CUR_USER_DEF_PROFILE:
                    if (HIWORD(wParam) == BN_CLICKED)
                    {
                        if (SendDlgItemMessageW(hwndDlg, IDC_APPLY_CUR_USER_DEF_PROFILE, BM_GETCHECK, 0, 0))
                        {
                            ResourceMessageBox(hwndDlg,
                                               MB_OK | MB_ICONWARNING,
                                               IDS_APPLY_DEFAULT_TITLE,
                                               IDS_APPLY_DEFAULT_TEXT);
                            pGlobalData->bApplyToDefaultUser = TRUE;
                        }
                        else
                        {
                            pGlobalData->bApplyToDefaultUser = FALSE;
                        }

                        PropSheet_Changed(GetParent(hwndDlg), hwndDlg);
                    }
                    break;
            }
            break;

        case WM_NOTIFY:
            if (((LPNMHDR)lParam)->code == PSN_APPLY)
            {
                PropSheet_UnChanged(GetParent(hwndDlg), hwndDlg);

                SaveSystemSettings(pGlobalData);
                SaveFontSubstitutionSettings(hwndDlg, pGlobalData);
                SaveFontLinkingSettings(hwndDlg, pGlobalData);
            }
            else if (((LPNMHDR)lParam)->idFrom == IDC_CONV_TABLES &&
                     ((LPNMHDR)lParam)->code == NM_CUSTOMDRAW)
            {
                SetWindowLong(hwndDlg,
                              DWL_MSGRESULT,
                              (LONG)ListViewCustomDraw(lParam));
                return TRUE;
            }
            break;
    }

    return FALSE;
}

/* EOF */
