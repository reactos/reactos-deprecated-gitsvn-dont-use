/*
 * Trash virtual folder support. The trashing engine is implemented in trash.c
 *
 * Copyright (C) 2006 Mikolaj Zalewski
 * Copyright (C) 2009 Andrew Hill
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include <precomp.h>

#include <mmsystem.h>
#include <ntquery.h>

#define MAX_PROPERTY_SHEET_PAGE 32

WINE_DEFAULT_DEBUG_CHANNEL(CRecycleBin);

typedef struct
{
    int column_name_id;
    const GUID *fmtId;
    DWORD pid;
    int pcsFlags;
    int fmt;
    int cxChars;
} columninfo;

static const columninfo RecycleBinColumns[] =
{
    {IDS_SHV_COLUMN_NAME,        &FMTID_Storage,   PID_STG_NAME,       SHCOLSTATE_TYPE_STR | SHCOLSTATE_ONBYDEFAULT,  LVCFMT_LEFT,  30},
    {IDS_SHV_COLUMN_DELFROM, &FMTID_Displaced, PID_DISPLACED_FROM, SHCOLSTATE_TYPE_STR | SHCOLSTATE_ONBYDEFAULT,  LVCFMT_LEFT,  30},
    {IDS_SHV_COLUMN_DELDATE, &FMTID_Displaced, PID_DISPLACED_DATE, SHCOLSTATE_TYPE_DATE | SHCOLSTATE_ONBYDEFAULT, LVCFMT_LEFT,  20},
    {IDS_SHV_COLUMN_SIZE,        &FMTID_Storage,   PID_STG_SIZE,       SHCOLSTATE_TYPE_INT | SHCOLSTATE_ONBYDEFAULT,  LVCFMT_RIGHT, 20},
    {IDS_SHV_COLUMN_TYPE,        &FMTID_Storage,   PID_STG_STORAGETYPE, SHCOLSTATE_TYPE_INT | SHCOLSTATE_ONBYDEFAULT,  LVCFMT_LEFT,  20},
    {IDS_SHV_COLUMN_MODIFIED,        &FMTID_Storage,   PID_STG_WRITETIME,  SHCOLSTATE_TYPE_DATE | SHCOLSTATE_ONBYDEFAULT, LVCFMT_LEFT,  20},
    /*    {"creation time",  &FMTID_Storage,   PID_STG_CREATETIME, SHCOLSTATE_TYPE_DATE,                        LVCFMT_LEFT,  20}, */
    /*    {"attribs",        &FMTID_Storage,   PID_STG_ATTRIBUTES, SHCOLSTATE_TYPE_STR,                         LVCFMT_LEFT,  20},       */
};

#define COLUMN_NAME    0
#define COLUMN_DELFROM 1
#define COLUMN_DATEDEL 2
#define COLUMN_SIZE    3
#define COLUMN_TYPE    4
#define COLUMN_MTIME   5

#define COLUMNS_COUNT  6

/*
 * Recycle Bin folder
 */

HRESULT CRecyclerExtractIcon_CreateInstance(LPCITEMIDLIST pidl, REFIID riid, LPVOID * ppvOut)
{
    CComPtr<IDefaultExtractIconInit> initIcon;
    HRESULT hr = SHCreateDefaultExtractIcon(IID_PPV_ARG(IDefaultExtractIconInit, &initIcon));
    if (FAILED_UNEXPECTEDLY(hr))
        return hr;

    /* FIXME: This is completely unimplemented */
    initIcon->SetNormalIcon(swShell32Name, 0);

    return initIcon->QueryInterface(riid, ppvOut);
}

class CRecycleBinEnum :
    public CEnumIDListBase
{
    private:
    public:
        CRecycleBinEnum();
        ~CRecycleBinEnum();
        HRESULT WINAPI Initialize(DWORD dwFlags);
        static BOOL WINAPI CBEnumRecycleBin(IN PVOID Context, IN HANDLE hDeletedFile);
        BOOL WINAPI CBEnumRecycleBin(IN HANDLE hDeletedFile);

        BEGIN_COM_MAP(CRecycleBinEnum)
        COM_INTERFACE_ENTRY_IID(IID_IEnumIDList, IEnumIDList)
        END_COM_MAP()
};

class CRecycleBinItemContextMenu :
    public CComObjectRootEx<CComMultiThreadModelNoCS>,
    public IContextMenu2
{
    private:
        LPITEMIDLIST                        apidl;
    public:
        CRecycleBinItemContextMenu();
        ~CRecycleBinItemContextMenu();
        HRESULT WINAPI Initialize(LPCITEMIDLIST pidl);

        // IContextMenu
        virtual HRESULT WINAPI QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags);
        virtual HRESULT WINAPI InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi);
        virtual HRESULT WINAPI GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *lpReserved, LPSTR lpszName, UINT uMaxNameLen);

        // IContextMenu2
        virtual HRESULT WINAPI HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

        BEGIN_COM_MAP(CRecycleBinItemContextMenu)
        COM_INTERFACE_ENTRY_IID(IID_IContextMenu, IContextMenu)
        COM_INTERFACE_ENTRY_IID(IID_IContextMenu2, IContextMenu2)
        END_COM_MAP()
};

typedef struct
{
    PIDLRecycleStruct *pFileDetails;
    HANDLE hDeletedFile;
    BOOL bFound;
} SEARCH_CONTEXT, *PSEARCH_CONTEXT;

BOOL WINAPI CBSearchRecycleBin(IN PVOID Context, IN HANDLE hDeletedFile)
{
    PSEARCH_CONTEXT pContext = (PSEARCH_CONTEXT)Context;

    PDELETED_FILE_DETAILS_W pFileDetails;
    DWORD dwSize;
    BOOL ret;

    if (!GetDeletedFileDetailsW(hDeletedFile,
                                0,
                                NULL,
                                &dwSize) &&
            GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        ERR("GetDeletedFileDetailsW failed\n");
        return FALSE;
    }

    pFileDetails = (DELETED_FILE_DETAILS_W *)SHAlloc(dwSize);
    if (!pFileDetails)
    {
        ERR("No memory\n");
        return FALSE;
    }

    if (!GetDeletedFileDetailsW(hDeletedFile,
                                dwSize,
                                pFileDetails,
                                NULL))
    {
        ERR("GetDeletedFileDetailsW failed\n");
        SHFree(pFileDetails);
        return FALSE;
    }

    ret = memcmp(pFileDetails, pContext->pFileDetails, dwSize);
    if (!ret)
    {
        pContext->hDeletedFile = hDeletedFile;
        pContext->bFound = TRUE;
    }
    else
        CloseRecycleBinHandle(hDeletedFile);

    SHFree(pFileDetails);
    return ret;
}

static PIDLRecycleStruct * _ILGetRecycleStruct(LPCITEMIDLIST pidl)
{
    LPPIDLDATA pdata = _ILGetDataPointer(pidl);

    if (pdata && pdata->type == 0x00)
        return (PIDLRecycleStruct*) & (pdata->u.crecycle);

    return NULL;
}

CRecycleBinEnum::CRecycleBinEnum()
{
}

CRecycleBinEnum::~CRecycleBinEnum()
{
}

HRESULT WINAPI CRecycleBinEnum::Initialize(DWORD dwFlags)
{
    static LPCWSTR szDrive = L"C:\\";

    if (dwFlags & SHCONTF_NONFOLDERS)
    {
        TRACE("Starting Enumeration\n");

        if (!EnumerateRecycleBinW(szDrive /* FIXME */ , CBEnumRecycleBin, (PVOID)this))
        {
            WARN("Error: EnumerateCRecycleBinW failed\n");
            return E_FAIL;
        }
    }
    else
    {
        // do nothing
    }
    return S_OK;
}

static LPITEMIDLIST _ILCreateRecycleItem(PDELETED_FILE_DETAILS_W pFileDetails)
{
    PIDLDATA tmp;
    LPITEMIDLIST pidl;
    PIDLRecycleStruct * p;
    int size0 = (char*)&tmp.u.crecycle.szName - (char*)&tmp.u.crecycle;
    int size = size0;

    tmp.type = 0x00;
    size += (wcslen(pFileDetails->FileName) + 1) * sizeof(WCHAR);

    pidl = (LPITEMIDLIST)SHAlloc(size + 4);
    if (!pidl)
        return pidl;

    pidl->mkid.cb = size + 2;
    memcpy(pidl->mkid.abID, &tmp, 2 + size0);

    p = &((PIDLDATA*)pidl->mkid.abID)->u.crecycle;
    RtlCopyMemory(p, pFileDetails, sizeof(DELETED_FILE_DETAILS_W));
    wcscpy(p->szName, pFileDetails->FileName);
    *(WORD*)((char*)pidl + (size + 2)) = 0;
    return pidl;
}

BOOL WINAPI CRecycleBinEnum::CBEnumRecycleBin(IN PVOID Context, IN HANDLE hDeletedFile)
{
    return static_cast<CRecycleBinEnum *>(Context)->CBEnumRecycleBin(hDeletedFile);
}

BOOL WINAPI CRecycleBinEnum::CBEnumRecycleBin(IN HANDLE hDeletedFile)
{
    PDELETED_FILE_DETAILS_W pFileDetails;
    DWORD dwSize;
    LPITEMIDLIST pidl = NULL;
    BOOL ret;

    if (!GetDeletedFileDetailsW(hDeletedFile,
                                0,
                                NULL,
                                &dwSize) &&
            GetLastError() != ERROR_INSUFFICIENT_BUFFER)
    {
        ERR("GetDeletedFileDetailsW failed\n");
        return FALSE;
    }

    pFileDetails = (DELETED_FILE_DETAILS_W *)SHAlloc(dwSize);
    if (!pFileDetails)
    {
        ERR("No memory\n");
        return FALSE;
    }

    if (!GetDeletedFileDetailsW(hDeletedFile,
                                dwSize,
                                pFileDetails,
                                NULL))
    {
        ERR("GetDeletedFileDetailsW failed\n");
        SHFree(pFileDetails);
        return FALSE;
    }

    pidl = _ILCreateRecycleItem(pFileDetails);
    if (!pidl)
    {
        SHFree(pFileDetails);
        return FALSE;
    }

    ret = AddToEnumList(pidl);

    if (!ret)
        SHFree(pidl);
    SHFree(pFileDetails);
    TRACE("Returning %d\n", ret);
    CloseRecycleBinHandle(hDeletedFile);
    return ret;
}

/**************************************************************************
* IContextMenu2 Bitbucket Item Implementation
*/

CRecycleBinItemContextMenu::CRecycleBinItemContextMenu()
{
    apidl = NULL;
}

CRecycleBinItemContextMenu::~CRecycleBinItemContextMenu()
{
    ILFree(apidl);
}

HRESULT WINAPI CRecycleBinItemContextMenu::Initialize(LPCITEMIDLIST pidl)
{
    apidl = ILClone(pidl);
    if (apidl == NULL)
        return E_OUTOFMEMORY;
    return S_OK;
}

HRESULT WINAPI CRecycleBinItemContextMenu::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    WCHAR szBuffer[30] = {0};
    ULONG Count = 1;

    TRACE("(%p)->(hmenu=%p indexmenu=%x cmdfirst=%x cmdlast=%x flags=%x )\n", this, hMenu, indexMenu, idCmdFirst, idCmdLast, uFlags);

    if (LoadStringW(shell32_hInstance, IDS_RESTORE, szBuffer, sizeof(szBuffer) / sizeof(WCHAR)))
    {
        szBuffer[(sizeof(szBuffer)/sizeof(WCHAR))-1] = L'\0';
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count, MFT_STRING, szBuffer, MFS_ENABLED);
        Count++;
    }

    if (LoadStringW(shell32_hInstance, IDS_CUT, szBuffer, sizeof(szBuffer) / sizeof(WCHAR)))
    {
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count++, MFT_SEPARATOR, NULL, MFS_ENABLED);
        szBuffer[(sizeof(szBuffer)/sizeof(WCHAR))-1] = L'\0';
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count++, MFT_STRING, szBuffer, MFS_ENABLED);
    }

    if (LoadStringW(shell32_hInstance, IDS_DELETE, szBuffer, sizeof(szBuffer) / sizeof(WCHAR)))
    {
        szBuffer[(sizeof(szBuffer)/sizeof(WCHAR))-1] = L'\0';
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count++, MFT_SEPARATOR, NULL, MFS_ENABLED);
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count++, MFT_STRING, szBuffer, MFS_ENABLED);
    }

    if (LoadStringW(shell32_hInstance, IDS_PROPERTIES, szBuffer, sizeof(szBuffer) / sizeof(WCHAR)))
    {
        szBuffer[(sizeof(szBuffer)/sizeof(WCHAR))-1] = L'\0';
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count++, MFT_SEPARATOR, NULL, MFS_ENABLED);
        _InsertMenuItemW(hMenu, indexMenu++, TRUE, idCmdFirst + Count, MFT_STRING, szBuffer, MFS_DEFAULT);
    }

    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, Count);
}

HRESULT WINAPI CRecycleBinItemContextMenu::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
    SEARCH_CONTEXT Context;
    static LPCWSTR szDrive = L"C:\\";

    TRACE("(%p)->(invcom=%p verb=%p wnd=%p)\n", this, lpcmi, lpcmi->lpVerb, lpcmi->hwnd);

    if (lpcmi->lpVerb == MAKEINTRESOURCEA(1) || lpcmi->lpVerb == MAKEINTRESOURCEA(5))
    {
        Context.pFileDetails = _ILGetRecycleStruct(apidl);
        Context.bFound = FALSE;

        EnumerateRecycleBinW(szDrive, CBSearchRecycleBin, (PVOID)&Context);
        if (!Context.bFound)
            return E_FAIL;

        if (lpcmi->lpVerb == MAKEINTRESOURCEA(1))
        {
            /* restore file */
            if (RestoreFile(Context.hDeletedFile))
                return S_OK;
            else
                return E_FAIL;
        }
        else
        {
            DeleteFileHandleToRecycleBin(Context.hDeletedFile);
            return E_NOTIMPL;
        }
    }
    else if (lpcmi->lpVerb == MAKEINTRESOURCEA(3))
    {
        FIXME("implement cut\n");
        return E_NOTIMPL;
    }
    else if (lpcmi->lpVerb == MAKEINTRESOURCEA(7))
    {
        FIXME("implement properties\n");
        return E_NOTIMPL;
    }

    return S_OK;
}

HRESULT WINAPI CRecycleBinItemContextMenu::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *lpReserved, LPSTR lpszName, UINT uMaxNameLen)
{
    TRACE("(%p)->(idcom=%lx flags=%x %p name=%p len=%x)\n", this, idCommand, uFlags, lpReserved, lpszName, uMaxNameLen);

    return E_FAIL;
}

HRESULT WINAPI CRecycleBinItemContextMenu::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    TRACE("CRecycleBin_IContextMenu2Item_HandleMenuMsg (%p)->(msg=%x wp=%lx lp=%lx)\n", this, uMsg, wParam, lParam);

    return E_NOTIMPL;
}

/**************************************************************************
* registers clipboardformat once
*/
void CRecycleBin::SF_RegisterClipFmt()
{
    TRACE ("(%p)\n", this);

    if (!cfShellIDList)
        cfShellIDList = RegisterClipboardFormatW(CFSTR_SHELLIDLIST);
}

CRecycleBin::CRecycleBin()
{
    pidl = NULL;
    iIdEmpty = 0;
    cfShellIDList = 0;
    SF_RegisterClipFmt();
    fAcceptFmt = FALSE;
}

CRecycleBin::~CRecycleBin()
{
    /*    InterlockedDecrement(&objCount);*/
    SHFree(pidl);
}

/*************************************************************************
 * RecycleBin IPersistFolder2 interface
 */

HRESULT WINAPI CRecycleBin::GetClassID(CLSID *pClassID)
{
    TRACE("(%p, %p)\n", this, pClassID);
    if (pClassID == NULL)
        return E_INVALIDARG;
    memcpy(pClassID, &CLSID_RecycleBin, sizeof(CLSID));
    return S_OK;
}

HRESULT WINAPI CRecycleBin::Initialize(LPCITEMIDLIST pidl)
{
    TRACE("(%p, %p)\n", this, pidl);

    SHFree((LPVOID)this->pidl);
    this->pidl = ILClone(pidl);
    if (this->pidl == NULL)
        return E_OUTOFMEMORY;
    return S_OK;
}

HRESULT WINAPI CRecycleBin::GetCurFolder(LPITEMIDLIST *ppidl)
{
    TRACE("\n");
    *ppidl = ILClone(pidl);
    return S_OK;
}

/*************************************************************************
 * RecycleBin IShellFolder2 interface
 */

HRESULT WINAPI CRecycleBin::ParseDisplayName(HWND hwnd, LPBC pbc,
        LPOLESTR pszDisplayName, ULONG *pchEaten, PIDLIST_RELATIVE *ppidl,
        ULONG *pdwAttributes)
{
    FIXME("stub\n");
    return E_NOTIMPL;
}


PDELETED_FILE_DETAILS_W
UnpackDetailsFromPidl(LPCITEMIDLIST pidl)
{
    return (PDELETED_FILE_DETAILS_W)&pidl->mkid.abID;
}

HRESULT WINAPI CRecycleBin::EnumObjects(HWND hwndOwner, DWORD dwFlags, LPENUMIDLIST *ppEnumIDList)
{
    return ShellObjectCreatorInit<CRecycleBinEnum>(dwFlags, IID_PPV_ARG(IEnumIDList, ppEnumIDList));
}

HRESULT WINAPI CRecycleBin::BindToObject(PCUIDLIST_RELATIVE pidl, LPBC pbc, REFIID riid, void **ppv)
{
    FIXME("(%p, %p, %p, %s, %p) - stub\n", this, pidl, pbc, debugstr_guid(&riid), ppv);
    return E_NOTIMPL;
}

HRESULT WINAPI CRecycleBin::BindToStorage(PCUIDLIST_RELATIVE pidl, LPBC pbc, REFIID riid, void **ppv)
{
    FIXME("(%p, %p, %p, %s, %p) - stub\n", this, pidl, pbc, debugstr_guid(&riid), ppv);
    return E_NOTIMPL;
}

HRESULT WINAPI CRecycleBin::CompareIDs(LPARAM lParam, PCUIDLIST_RELATIVE pidl1, PCUIDLIST_RELATIVE pidl2)
{
    PIDLRecycleStruct* pData1 = _ILGetRecycleStruct(pidl1);
    PIDLRecycleStruct* pData2 = _ILGetRecycleStruct(pidl2);
    LPWSTR pName1, pName2;

    if(!pData1 || !pData2 || LOWORD(lParam) >= COLUMNS_COUNT)
        return E_INVALIDARG;

    SHORT result;
    LONGLONG diff;
    switch (LOWORD(lParam))
    {
        case 0: /* Name */
            pName1 = PathFindFileNameW(pData1->szName);
            pName2 = PathFindFileNameW(pData2->szName);
            result = wcsicmp(pName1, pName2);
            break;
        case 1: /* Orig. Location */
            result = wcsicmp(pData1->szName, pData2->szName);
            break;
        case 2: /* Date Deleted */
            result = CompareFileTime(&pData1->DeletionTime, &pData2->DeletionTime);
            break;
        case 3: /* Size */
            diff = pData1->FileSize.QuadPart - pData2->FileSize.QuadPart;
            return MAKE_COMPARE_HRESULT(diff);
        case 4: /* Type */
            pName1 = PathFindExtensionW(pData1->szName);
            pName2 = PathFindExtensionW(pData2->szName);
            result = wcsicmp(pName1, pName2);
            break;
        case 5: /* Modified */
            result = CompareFileTime(&pData1->LastModification, &pData2->LastModification);
            break;
    }
    return MAKE_COMPARE_HRESULT(result);
}

HRESULT WINAPI CRecycleBin::CreateViewObject(HWND hwndOwner, REFIID riid, void **ppv)
{
    CComPtr<IShellView> pShellView;
    HRESULT hr = E_NOINTERFACE;

    TRACE("(%p, %p, %s, %p)\n", this, hwndOwner, debugstr_guid(&riid), ppv);

    if (!ppv)
        return hr;

    *ppv = NULL;

    if (IsEqualIID (riid, IID_IDropTarget))
    {
        hr = this->QueryInterface (IID_IDropTarget, ppv);
    }
    else if (IsEqualIID (riid, IID_IContextMenu) || IsEqualIID (riid, IID_IContextMenu2))
    {
        hr = this->QueryInterface(riid, ppv);
    }
    else if (IsEqualIID (riid, IID_IShellView))
    {
        SFV_CREATE sfvparams = {sizeof(SFV_CREATE), this};
        hr = SHCreateShellFolderView(&sfvparams, (IShellView**)ppv);
    }
    else
        return hr;
    TRACE ("-- (%p)->(interface=%p)\n", this, ppv);
    return hr;

}

HRESULT WINAPI CRecycleBin::GetAttributesOf(UINT cidl, PCUITEMID_CHILD_ARRAY apidl,
        SFGAOF *rgfInOut)
{
    TRACE("(%p, %d, {%p, ...}, {%x})\n", this, cidl, apidl ? apidl[0] : NULL, (unsigned int)*rgfInOut);
    *rgfInOut &= SFGAO_FOLDER|SFGAO_DROPTARGET|SFGAO_HASPROPSHEET|SFGAO_CANLINK;
    return S_OK;
}

HRESULT WINAPI CRecycleBin::GetUIObjectOf(HWND hwndOwner, UINT cidl, PCUITEMID_CHILD_ARRAY apidl,
        REFIID riid, UINT *prgfInOut, void **ppv)
{
    LPVOID pObj = NULL;
    HRESULT hr = E_INVALIDARG;

    TRACE ("(%p)->(%p,%u,apidl=%p, %p %p)\n", this,
           hwndOwner, cidl, apidl, prgfInOut, ppv);

    if (!ppv)
        return hr;

    *ppv = NULL;

    if ((IsEqualIID (riid, IID_IContextMenu) || IsEqualIID(riid, IID_IContextMenu2)) && (cidl >= 1))
    {
        hr = ShellObjectCreatorInit<CRecycleBinItemContextMenu>(apidl[0], riid, &pObj);
    }
    else if (IsEqualIID (riid, IID_IDropTarget) && (cidl == 1))
    {
        IDropTarget * pDt = NULL;
        hr = QueryInterface(IID_PPV_ARG(IDropTarget, &pDt));
        pObj = pDt;
    }
    else if((IsEqualIID(riid, IID_IExtractIconA) || IsEqualIID(riid, IID_IExtractIconW)) && (cidl == 1))
    {
        hr = CRecyclerExtractIcon_CreateInstance(apidl[0], riid, &pObj);
    }
    else
        hr = E_NOINTERFACE;

    if (SUCCEEDED(hr) && !pObj)
        hr = E_OUTOFMEMORY;

    *ppv = pObj;
    TRACE ("(%p)->hr=0x%08x\n", this, hr);
    return hr;
}

HRESULT WINAPI CRecycleBin::GetDisplayNameOf(PCUITEMID_CHILD pidl, SHGDNF uFlags, STRRET *pName)
{
    PIDLRecycleStruct *pFileDetails;
    LPWSTR pFileName;

    TRACE("(%p, %p, %x, %p)\n", this, pidl, (unsigned int)uFlags, pName);

    pFileDetails = _ILGetRecycleStruct(pidl);
    if (!pFileDetails)
    {
        pName->cStr[0] = 0;
        pName->uType = STRRET_CSTR;
        return E_INVALIDARG;
    }

    pFileName = wcsrchr(pFileDetails->szName, L'\\');
    if (!pFileName)
    {
        pName->cStr[0] = 0;
        pName->uType = STRRET_CSTR;
        return E_UNEXPECTED;
    }

    pName->pOleStr = StrDupW(pFileName + 1);
    if (pName->pOleStr == NULL)
        return E_OUTOFMEMORY;

    pName->uType = STRRET_WSTR;
    return S_OK;
}

HRESULT WINAPI CRecycleBin::SetNameOf(HWND hwnd, PCUITEMID_CHILD pidl, LPCOLESTR pszName,
                                      SHGDNF uFlags, PITEMID_CHILD *ppidlOut)
{
    TRACE("\n");
    return E_FAIL; /* not supported */
}

HRESULT WINAPI CRecycleBin::GetDefaultSearchGUID(GUID *pguid)
{
    FIXME("stub\n");
    return E_NOTIMPL;
}

HRESULT WINAPI CRecycleBin::EnumSearches(IEnumExtraSearch **ppEnum)
{
    FIXME("stub\n");
    *ppEnum = NULL;
    return E_NOTIMPL;
}

HRESULT WINAPI CRecycleBin::GetDefaultColumn(DWORD dwReserved, ULONG *pSort, ULONG *pDisplay)
{
    TRACE("(%p, %x, %p, %p)\n", this, (unsigned int)dwReserved, pSort, pDisplay);
    if (pSort)
        *pSort = 0;
    if (pDisplay)
        *pDisplay = 0;
    return S_OK;
}

HRESULT WINAPI CRecycleBin::GetDefaultColumnState(UINT iColumn, SHCOLSTATEF *pcsFlags)
{
    TRACE("(%p, %d, %p)\n", this, iColumn, pcsFlags);
    if (iColumn >= COLUMNS_COUNT)
        return E_INVALIDARG;
    *pcsFlags = RecycleBinColumns[iColumn].pcsFlags;
    return S_OK;
}

HRESULT WINAPI CRecycleBin::GetDetailsEx(PCUITEMID_CHILD pidl, const SHCOLUMNID *pscid, VARIANT *pv)
{
    FIXME("stub\n");
    return E_NOTIMPL;
}

static HRESULT FormatDateTime(LPWSTR buffer, int size, FILETIME * ft)
{
    FILETIME lft;
    SYSTEMTIME time;
    int ret;

    FileTimeToLocalFileTime(ft, &lft);
    FileTimeToSystemTime(&lft, &time);

    ret = GetDateFormatW(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &time, NULL, buffer, size);
    if (ret > 0 && ret < size)
    {
        /* Append space + time without seconds */
        buffer[ret-1] = ' ';
        GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &time, NULL, &buffer[ret], size - ret);
    }

    return (ret != 0 ? E_FAIL : S_OK);
}

HRESULT WINAPI CRecycleBin::GetDetailsOf(PCUITEMID_CHILD pidl, UINT iColumn, LPSHELLDETAILS pDetails)
{
    PIDLRecycleStruct * pFileDetails;
    WCHAR buffer[MAX_PATH];
    WCHAR szTypeName[100];
    LPWSTR pszBackslash;
    UINT Length;

    TRACE("(%p, %p, %d, %p)\n", this, pidl, iColumn, pDetails);
    if (iColumn >= COLUMNS_COUNT)
        return E_FAIL;
    pDetails->fmt = RecycleBinColumns[iColumn].fmt;
    pDetails->cxChar = RecycleBinColumns[iColumn].cxChars;
    if (pidl == NULL)
        return SHSetStrRet(&pDetails->str, RecycleBinColumns[iColumn].column_name_id);

    if (iColumn == COLUMN_NAME)
        return GetDisplayNameOf(pidl, SHGDN_NORMAL, &pDetails->str);

    pFileDetails = _ILGetRecycleStruct(pidl);
    switch (iColumn)
    {
        case COLUMN_DATEDEL:
            FormatDateTime(buffer, MAX_PATH, &pFileDetails->DeletionTime);
            break;
        case COLUMN_DELFROM:
            pszBackslash = wcsrchr(pFileDetails->szName, L'\\');
            Length = (pszBackslash - pFileDetails->szName);
            memcpy((LPVOID)buffer, pFileDetails->szName, Length * sizeof(WCHAR));
            buffer[Length] = L'\0';
            break;
        case COLUMN_SIZE:
            StrFormatKBSizeW(pFileDetails->FileSize.QuadPart, buffer, MAX_PATH);
            break;
        case COLUMN_MTIME:
            FormatDateTime(buffer, MAX_PATH, &pFileDetails->LastModification);
            break;
        case COLUMN_TYPE:
            // FIXME: We should in fact use a UNICODE version of _ILGetFileType
            szTypeName[0] = L'\0';
            wcscpy(buffer, PathFindExtensionW(pFileDetails->szName));
            if (!( HCR_MapTypeToValueW(buffer, buffer, _countof(buffer), TRUE) &&
                    HCR_MapTypeToValueW(buffer, szTypeName, _countof(szTypeName), FALSE )))
            {
                /* load localized file string */
                szTypeName[0] = '\0';
                if(LoadStringW(shell32_hInstance, IDS_ANY_FILE, szTypeName, _countof(szTypeName)))
                {
                    szTypeName[63] = '\0';
                    StringCchPrintfW(buffer, _countof(buffer), szTypeName, PathFindExtensionW(pFileDetails->szName));
                }
            }
            return SHSetStrRet(&pDetails->str, szTypeName);
        default:
            return E_FAIL;
    }

    return SHSetStrRet(&pDetails->str, buffer);
}

HRESULT WINAPI CRecycleBin::MapColumnToSCID(UINT iColumn, SHCOLUMNID *pscid)
{
    TRACE("(%p, %d, %p)\n", this, iColumn, pscid);
    if (iColumn >= COLUMNS_COUNT)
        return E_INVALIDARG;
    pscid->fmtid = *RecycleBinColumns[iColumn].fmtId;
    pscid->pid = RecycleBinColumns[iColumn].pid;
    return S_OK;
}

BOOL CRecycleBin::RecycleBinIsEmpty()
{
    CComPtr<IEnumIDList> spEnumFiles;
    HRESULT hr = EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &spEnumFiles);
    if (FAILED(hr))
        return TRUE;
    CComHeapPtr<ITEMIDLIST> spPidl;
    ULONG itemcount;
    return spEnumFiles->Next(1, &spPidl, &itemcount) != S_OK;
    }

/*************************************************************************
 * RecycleBin IContextMenu interface
 */

HRESULT WINAPI CRecycleBin::QueryContextMenu(HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    WCHAR szBuffer[100];
    MENUITEMINFOW mii;
    int id = 1;

    TRACE("QueryContextMenu %p %p %u %u %u %u\n", this, hMenu, indexMenu, idCmdFirst, idCmdLast, uFlags );

    if (!hMenu)
        return E_INVALIDARG;

    memset(&mii, 0, sizeof(mii));
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE | MIIM_ID | MIIM_STATE;
    mii.fState = RecycleBinIsEmpty() ? MFS_DISABLED : MFS_ENABLED;
    szBuffer[0] = L'\0';
    LoadStringW(shell32_hInstance, IDS_EMPTY_BITBUCKET, szBuffer, sizeof(szBuffer) / sizeof(WCHAR));
    mii.dwTypeData = szBuffer;
    mii.cch = wcslen(mii.dwTypeData);
    mii.wID = idCmdFirst + id++;
    mii.fType = MFT_STRING;
    iIdEmpty = 1;

    if (!InsertMenuItemW(hMenu, indexMenu, TRUE, &mii))
        return E_FAIL;

    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, id);
}

HRESULT WINAPI CRecycleBin::InvokeCommand(LPCMINVOKECOMMANDINFO lpcmi)
{
    HRESULT hr;
    LPSHELLBROWSER lpSB;
    IShellView * lpSV = NULL;

    TRACE("%p %p verb %p\n", this, lpcmi, lpcmi->lpVerb);

    if (LOWORD(lpcmi->lpVerb) == iIdEmpty)
    {
        // FIXME
        // path & flags
        hr = SHEmptyRecycleBinW(lpcmi->hwnd, L"C:\\", 0);
        TRACE("result %x\n", hr);
        if (hr != S_OK)
            return hr;

        lpSB = (LPSHELLBROWSER)SendMessageA(lpcmi->hwnd, CWM_GETISHELLBROWSER, 0, 0);
        if (lpSB && SUCCEEDED(lpSB->QueryActiveShellView(&lpSV)))
            lpSV->Refresh();
    }
    return S_OK;
}

HRESULT WINAPI CRecycleBin::GetCommandString(UINT_PTR idCommand, UINT uFlags, UINT *lpReserved, LPSTR lpszName, UINT uMaxNameLen)
{
    FIXME("%p %lu %u %p %p %u\n", this, idCommand, uFlags, lpReserved, lpszName, uMaxNameLen);

    return E_NOTIMPL;
}

/*************************************************************************
 * RecycleBin IShellPropSheetExt interface
 */

HRESULT WINAPI CRecycleBin::AddPages(LPFNSVADDPROPSHEETPAGE pfnAddPage, LPARAM lParam)
{
    FIXME("%p %p %lu\n", this, pfnAddPage, lParam);

    return E_NOTIMPL;
}

HRESULT WINAPI CRecycleBin::ReplacePage(EXPPS uPageID, LPFNSVADDPROPSHEETPAGE pfnReplaceWith, LPARAM lParam)
{
    FIXME("%p %lu %p %lu\n", this, uPageID, pfnReplaceWith, lParam);

    return E_NOTIMPL;
}

/*************************************************************************
 * RecycleBin IShellExtInit interface
 */

HRESULT WINAPI CRecycleBin::Initialize(LPCITEMIDLIST pidlFolder, IDataObject *pdtobj, HKEY hkeyProgID)
{
    TRACE("%p %p %p %p\n", this, pidlFolder, pdtobj, hkeyProgID );
    return S_OK;
}

BOOL
TRASH_CanTrashFile(LPCWSTR wszPath)
{
    LONG ret;
    DWORD dwNukeOnDelete, dwType, VolSerialNumber, MaxComponentLength;
    DWORD FileSystemFlags, dwSize, dwDisposition;
    HKEY hKey;
    WCHAR szBuffer[10];
    WCHAR szKey[150] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Bitbucket\\Volume\\";

    if (wszPath[1] != L':')
    {
        /* path is UNC */
        return FALSE;
    }

    if (GetDriveTypeW(wszPath) != DRIVE_FIXED)
    {
        /* no bitbucket on removable media */
        return FALSE;
    }

    if (!GetVolumeInformationW(wszPath, NULL, 0, &VolSerialNumber, &MaxComponentLength, &FileSystemFlags, NULL, 0))
    {
        ERR("GetVolumeInformationW failed with %u\n", GetLastError());
        return FALSE;
    }

    swprintf(szBuffer, L"%04X-%04X", LOWORD(VolSerialNumber), HIWORD(VolSerialNumber));
    wcscat(szKey, szBuffer);

    if (RegCreateKeyExW(HKEY_CURRENT_USER, szKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, &dwDisposition) != ERROR_SUCCESS)
    {
        ERR("RegCreateKeyExW failed\n");
        return FALSE;
    }

    if (dwDisposition  & REG_CREATED_NEW_KEY)
    {
        /* per default move to bitbucket */
        dwNukeOnDelete = 0;
        RegSetValueExW(hKey, L"NukeOnDelete", 0, REG_DWORD, (LPBYTE)&dwNukeOnDelete, sizeof(DWORD));
        /* per default unlimited size */
        dwSize = -1;
        RegSetValueExW(hKey, L"MaxCapacity", 0, REG_DWORD, (LPBYTE)&dwSize, sizeof(DWORD));
        RegCloseKey(hKey);
        return TRUE;
    }
    else
    {
        dwSize = sizeof(dwNukeOnDelete);
        ret = RegQueryValueExW(hKey, L"NukeOnDelete", NULL, &dwType, (LPBYTE)&dwNukeOnDelete, &dwSize);
        if (ret != ERROR_SUCCESS)
        {
            if (ret ==  ERROR_FILE_NOT_FOUND)
            {
                /* restore key and enable bitbucket */
                dwNukeOnDelete = 0;
                RegSetValueExW(hKey, L"NukeOnDelete", 0, REG_DWORD, (LPBYTE)&dwNukeOnDelete, sizeof(DWORD));
            }
            RegCloseKey(hKey);
            return TRUE;
        }
        else if (dwNukeOnDelete)
        {
            /* do not delete to bitbucket */
            RegCloseKey(hKey);
            return FALSE;
        }
        /* FIXME
         * check if bitbucket is full
         */
        RegCloseKey(hKey);
        return TRUE;
    }
}

BOOL
TRASH_TrashFile(LPCWSTR wszPath)
{
    TRACE("(%s)\n", debugstr_w(wszPath));
    return DeleteFileToRecycleBin(wszPath);
}

/*************************************************************************
 * SHUpdateCRecycleBinIcon                                [SHELL32.@]
 *
 * Undocumented
 */
EXTERN_C HRESULT WINAPI SHUpdateRecycleBinIcon(void)
{
    FIXME("stub\n");



    return S_OK;
}

/****************************************************************************
 * IDropTarget implementation
 */
BOOL CRecycleBin::QueryDrop(DWORD dwKeyState, LPDWORD pdwEffect)
{
    /* TODO on shift we should delete, we should update the cursor manager to show this. */

    DWORD dwEffect = DROPEFFECT_COPY;

    *pdwEffect = DROPEFFECT_NONE;

    if (fAcceptFmt) { /* Does our interpretation of the keystate ... */
        *pdwEffect = KeyStateToDropEffect (dwKeyState);

        if (*pdwEffect == DROPEFFECT_NONE)
            *pdwEffect = dwEffect;

        /* ... matches the desired effect ? */
        if (dwEffect & *pdwEffect) {
            return TRUE;
        }
    }
    return FALSE;
}

HRESULT WINAPI CRecycleBin::DragEnter(IDataObject *pDataObject,
                                    DWORD dwKeyState, POINTL pt, DWORD *pdwEffect)
{
    TRACE("Recycle bin drag over (%p)\n", this);
    /* The recycle bin accepts pretty much everything, and sets a CSIDL flag. */
    fAcceptFmt = TRUE;

    QueryDrop(dwKeyState, pdwEffect);
    return S_OK;
}

HRESULT WINAPI CRecycleBin::DragOver(DWORD dwKeyState, POINTL pt,
                                   DWORD *pdwEffect)
{
    TRACE("(%p)\n", this);

    if (!pdwEffect)
        return E_INVALIDARG;

    QueryDrop(dwKeyState, pdwEffect);

    return S_OK;
}

HRESULT WINAPI CRecycleBin::DragLeave()
{
    TRACE("(%p)\n", this);

    fAcceptFmt = FALSE;

    return S_OK;
}

HRESULT WINAPI CRecycleBin::Drop(IDataObject *pDataObject,
                               DWORD dwKeyState, POINTL pt, DWORD *pdwEffect)
{
    TRACE("(%p) object dropped on recycle bin, effect %u\n", this, *pdwEffect);

    /* TODO: pdwEffect should be read and make the drop object be permanently deleted in the move case (shift held) */

    FORMATETC fmt;
    TRACE("(%p)->(DataObject=%p)\n", this, pDataObject);
    InitFormatEtc (fmt, cfShellIDList, TYMED_HGLOBAL);

    /* Handle cfShellIDList Drop objects here, otherwise send the approriate message to other software */
    if (SUCCEEDED(pDataObject->QueryGetData(&fmt)))
    {
        DWORD fMask = 0;

        if ((dwKeyState & MK_SHIFT) == MK_SHIFT)
            fMask |= CMIC_MASK_SHIFT_DOWN;

        DoDeleteAsync(pDataObject, fMask);
    }
    else
    {
        /*
         * TODO call SetData on the data object with format CFSTR_TARGETCLSID
         * set to the Recycle Bin's class identifier CLSID_RecycleBin.
         */
    }
    return S_OK;
}

HRESULT WINAPI DoDeleteDataObject(IDataObject *pda, DWORD fMask)
{
    TRACE("performing delete");
    HRESULT hr;

    STGMEDIUM medium;
    FORMATETC formatetc;
    InitFormatEtc(formatetc, RegisterClipboardFormatW(CFSTR_SHELLIDLIST), TYMED_HGLOBAL);
    hr = pda->GetData(&formatetc, &medium);
    if (FAILED(hr))
        return hr;

    /* lock the handle */
    LPIDA lpcida = (LPIDA)GlobalLock(medium.hGlobal);
    if (!lpcida)
    {
        ReleaseStgMedium(&medium);
        return E_FAIL;
    }

    /* convert the data into pidl */
    LPITEMIDLIST pidl;
    LPITEMIDLIST *apidl = _ILCopyCidaToaPidl(&pidl, lpcida);
    if (!apidl)
    {
        ReleaseStgMedium(&medium);
        return E_FAIL;
    }

    CComPtr<IShellFolder> psfDesktop;
    CComPtr<IShellFolder> psfFrom = NULL;

    /* Grab the desktop shell folder */
    hr = SHGetDesktopFolder(&psfDesktop);
    if (FAILED(hr))
    {
        ERR("SHGetDesktopFolder failed\n");
        SHFree(pidl);
        _ILFreeaPidl(apidl, lpcida->cidl);
        ReleaseStgMedium(&medium);
        return E_FAIL;
    }

    /* Find source folder, this is where the clipboard data was copied from */
    if (_ILIsDesktop(pidl))
    {
        psfFrom = psfDesktop;
    }
    else
    {
        hr = psfDesktop->BindToObject(pidl, NULL, IID_PPV_ARG(IShellFolder, &psfFrom));
        if (FAILED(hr))
        {
            ERR("no IShellFolder\n");
            SHFree(pidl);
            _ILFreeaPidl(apidl, lpcida->cidl);
            ReleaseStgMedium(&medium);
            return E_FAIL;
        }
    }

    STRRET strTemp;
    hr = psfFrom->GetDisplayNameOf(apidl[0], SHGDN_FORPARSING, &strTemp);
    if (FAILED(hr))
    {
        ERR("IShellFolder_GetDisplayNameOf failed with %x\n", hr);
        SHFree(pidl);
        _ILFreeaPidl(apidl, lpcida->cidl);
        ReleaseStgMedium(&medium);
        return hr;
    }

    WCHAR wszPath[MAX_PATH];
    hr = StrRetToBufW(&strTemp, apidl[0], wszPath, _countof(wszPath));
    if (FAILED(hr))
    {
        ERR("StrRetToBufW failed with %x\n", hr);
        SHFree(pidl);
        _ILFreeaPidl(apidl, lpcida->cidl);
        ReleaseStgMedium(&medium);
        return hr;
    }

    /* Only keep the base path */
    LPWSTR pwszFilename = PathFindFileNameW(wszPath);
    *pwszFilename = L'\0';

    /* Build paths list */
    LPWSTR pwszPaths = BuildPathsList(wszPath, lpcida->cidl, (LPCITEMIDLIST*) apidl, FALSE);
    if (!pwszPaths)
    {
        SHFree(pidl);
        _ILFreeaPidl(apidl, lpcida->cidl);
        ReleaseStgMedium(&medium);
        return E_FAIL;
    }

    /* Delete them */
    SHFILEOPSTRUCTW FileOp;
    ZeroMemory(&FileOp, sizeof(FileOp));
    FileOp.wFunc = FO_DELETE;
    FileOp.pFrom = pwszPaths;
    if ((fMask & CMIC_MASK_SHIFT_DOWN) == 0)
        FileOp.fFlags = FOF_ALLOWUNDO;

    if (SHFileOperationW(&FileOp) != 0)
    {
        ERR("SHFileOperation failed with 0x%x for %s\n", GetLastError(), debugstr_w(pwszPaths));
        hr = E_FAIL;
    }

    HeapFree(GetProcessHeap(), 0, pwszPaths);
    SHFree(pidl);
    _ILFreeaPidl(apidl, lpcida->cidl);
    ReleaseStgMedium(&medium);

    return hr;
}

struct DeleteThreadData {
    IStream *s;
    DWORD fMask;
};

DWORD WINAPI DoDeleteThreadProc(LPVOID lpParameter)
{
    DeleteThreadData *data = static_cast<DeleteThreadData*>(lpParameter);
    CoInitialize(NULL);
    IDataObject *pDataObject;
    HRESULT hr = CoGetInterfaceAndReleaseStream (data->s, IID_PPV_ARG(IDataObject, &pDataObject));
    if (SUCCEEDED(hr))
    {
        DoDeleteDataObject(pDataObject, data->fMask);
    }
    pDataObject->Release();
    CoUninitialize();
    HeapFree(GetProcessHeap(), 0, data);
    return 0;
}

void DoDeleteAsync(IDataObject *pda, DWORD fMask)
{
    DeleteThreadData *data = static_cast<DeleteThreadData*>(HeapAlloc(GetProcessHeap(), 0, sizeof(DeleteThreadData)));
    data->fMask = fMask;
    CoMarshalInterThreadInterfaceInStream(IID_IDataObject, pda, &data->s);
    SHCreateThread(DoDeleteThreadProc, data, NULL, NULL);
}

/*************************************************************************
 *              SHEmptyRecycleBinA (SHELL32.@)
 */
HRESULT WINAPI SHEmptyRecycleBinA(HWND hwnd, LPCSTR pszRootPath, DWORD dwFlags)
{
    LPWSTR szRootPathW = NULL;
    int len;
    HRESULT hr;

    TRACE("%p, %s, 0x%08x\n", hwnd, debugstr_a(pszRootPath), dwFlags);

    if (pszRootPath)
    {
        len = MultiByteToWideChar(CP_ACP, 0, pszRootPath, -1, NULL, 0);
        if (len == 0)
            return HRESULT_FROM_WIN32(GetLastError());
        szRootPathW = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
        if (!szRootPathW)
            return E_OUTOFMEMORY;
        if (MultiByteToWideChar(CP_ACP, 0, pszRootPath, -1, szRootPathW, len) == 0)
        {
            HeapFree(GetProcessHeap(), 0, szRootPathW);
            return HRESULT_FROM_WIN32(GetLastError());
        }
    }

    hr = SHEmptyRecycleBinW(hwnd, szRootPathW, dwFlags);
    HeapFree(GetProcessHeap(), 0, szRootPathW);

    return hr;
}

HRESULT WINAPI SHEmptyRecycleBinW(HWND hwnd, LPCWSTR pszRootPath, DWORD dwFlags)
{
    WCHAR szPath[MAX_PATH] = {0}, szBuffer[MAX_PATH];
    DWORD dwSize, dwType, count;
    LONG ret;
    IShellFolder *pDesktop, *pRecycleBin;
    PIDLIST_ABSOLUTE pidlRecycleBin;
    PITEMID_CHILD pidl;
    HRESULT hr = S_OK;
    LPENUMIDLIST penumFiles;
    STRRET StrRet;

    TRACE("%p, %s, 0x%08x\n", hwnd, debugstr_w(pszRootPath), dwFlags);

    if (!(dwFlags & SHERB_NOCONFIRMATION))
    {
        hr = SHGetDesktopFolder(&pDesktop);
        if (FAILED(hr))
            return hr;
        hr = SHGetFolderLocation(NULL, CSIDL_BITBUCKET, NULL, 0, &pidlRecycleBin);
        if (FAILED(hr))
        {
            pDesktop->Release();
            return hr;
        }
        hr = pDesktop->BindToObject(pidlRecycleBin, NULL, IID_PPV_ARG(IShellFolder, &pRecycleBin));
        CoTaskMemFree(pidlRecycleBin);
        pDesktop->Release();
        if (FAILED(hr))
            return hr;
        hr = pRecycleBin->EnumObjects(hwnd, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &penumFiles);
        if (FAILED(hr))
        {
            pRecycleBin->Release();
            return hr;
        }

        count = 0;
        if (hr != S_FALSE)
        {
            while (penumFiles->Next(1, &pidl, NULL) == S_OK)
            {
                count++;
                pRecycleBin->GetDisplayNameOf(pidl, SHGDN_NORMAL, &StrRet);
                StrRetToBuf(&StrRet, pidl, szBuffer, _countof(szBuffer));
                CoTaskMemFree(pidl);
            }
            penumFiles->Release();
        }
        pRecycleBin->Release();

        switch (count)
        {
            case 0:
                /* no files, don't need confirmation */
                break;

            case 1:
                /* we have only one item inside the bin, so show a message box with its name */
                if (ShellMessageBoxW(shell32_hInstance, hwnd, MAKEINTRESOURCEW(IDS_DELETEITEM_TEXT), MAKEINTRESOURCEW(IDS_EMPTY_BITBUCKET),
                                   MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2, szBuffer) == IDNO)
                {
                    return S_OK;
                }
                break;

            default:
                /* we have more than one item, so show a message box with the count of the items */
                StringCbPrintfW(szBuffer, sizeof(szBuffer), L"%u", count);
                if (ShellMessageBoxW(shell32_hInstance, hwnd, MAKEINTRESOURCEW(IDS_DELETEMULTIPLE_TEXT), MAKEINTRESOURCEW(IDS_EMPTY_BITBUCKET),
                                   MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2, szBuffer) == IDNO)
                {
                    return S_OK;
                }
                break;
        }
    }

    if (dwFlags & SHERB_NOPROGRESSUI)
    {
        ret = EmptyRecycleBinW(pszRootPath);
    }
    else
    {
       /* FIXME
        * show a progress dialog
        */
        ret = EmptyRecycleBinW(pszRootPath);
    }

    if (!ret)
        return HRESULT_FROM_WIN32(GetLastError());

    if (!(dwFlags & SHERB_NOSOUND))
    {
        dwSize = sizeof(szPath);
        ret = RegGetValueW(HKEY_CURRENT_USER,
                           L"AppEvents\\Schemes\\Apps\\Explorer\\EmptyRecycleBin\\.Current",
                           NULL,
                           RRF_RT_REG_SZ,
                           &dwType,
                           (PVOID)szPath,
                           &dwSize);
        if (ret != ERROR_SUCCESS)
            return S_OK;

        if (dwType != REG_EXPAND_SZ) /* type dismatch */
            return S_OK;

        szPath[(sizeof(szPath)/sizeof(WCHAR))-1] = L'\0';
        PlaySoundW(szPath, NULL, SND_FILENAME);
    }
    return S_OK;
}

HRESULT WINAPI SHQueryRecycleBinA(LPCSTR pszRootPath, LPSHQUERYRBINFO pSHQueryRBInfo)
{
    LPWSTR szRootPathW = NULL;
    int len;
    HRESULT hr;

    TRACE("%s, %p\n", debugstr_a(pszRootPath), pSHQueryRBInfo);

    if (pszRootPath)
    {
        len = MultiByteToWideChar(CP_ACP, 0, pszRootPath, -1, NULL, 0);
        if (len == 0)
            return HRESULT_FROM_WIN32(GetLastError());
        szRootPathW = (LPWSTR)HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
        if (!szRootPathW)
            return E_OUTOFMEMORY;
        if (MultiByteToWideChar(CP_ACP, 0, pszRootPath, -1, szRootPathW, len) == 0)
        {
            HeapFree(GetProcessHeap(), 0, szRootPathW);
            return HRESULT_FROM_WIN32(GetLastError());
        }
    }

    hr = SHQueryRecycleBinW(szRootPathW, pSHQueryRBInfo);
    HeapFree(GetProcessHeap(), 0, szRootPathW);

    return hr;
}

HRESULT WINAPI SHQueryRecycleBinW(LPCWSTR pszRootPath, LPSHQUERYRBINFO pSHQueryRBInfo)
{
    FIXME("%s, %p - stub\n", debugstr_w(pszRootPath), pSHQueryRBInfo);

    if (!(pszRootPath) || (pszRootPath[0] == 0) ||
        !(pSHQueryRBInfo) || (pSHQueryRBInfo->cbSize < sizeof(SHQUERYRBINFO)))
    {
        return E_INVALIDARG;
    }

    pSHQueryRBInfo->i64Size = 0;
    pSHQueryRBInfo->i64NumItems = 0;

    return S_OK;
}
