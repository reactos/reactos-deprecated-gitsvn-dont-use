/*
 *    ShellView
 *
 *    Copyright 1998,1999    <juergen.schmied@debitel.net>
 *
 * This is the view visualizing the data provided by the shellfolder.
 * No direct access to data from pidls should be done from here.
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
 *
 * FIXME: CheckToolbar: handle the "new folder" and "folder up" button
 */

/*
TODO:
- Load/Save the view state from/into the stream provided by the ShellBrowser.
- When editing starts on item, set edit text to for editing value.
- Fix shell view to handle view mode popup exec.
- The background context menu should have a pidl just like foreground menus. This
   causes crashes when dynamic handlers try to use the NULL pidl.
- Reorder of columns doesn't work - might be bug in comctl32
*/

#include "precomp.h"

#include <atlwin.h>
#include <rosctrls.h>

WINE_DEFAULT_DEBUG_CHANNEL(shell);

#undef SV_CLASS_NAME

static const WCHAR SV_CLASS_NAME[] = {'S', 'H', 'E', 'L', 'L', 'D', 'L', 'L', '_', 'D', 'e', 'f', 'V', 'i', 'e', 'w', 0};

typedef struct
{
    BOOL    bIsAscending;
    INT     nHeaderID;
    INT     nLastHeaderID;
} LISTVIEW_SORT_INFO, *LPLISTVIEW_SORT_INFO;

#define SHV_CHANGE_NOTIFY WM_USER + 0x1111

class CDefView :
    public CWindowImpl<CDefView, CWindow, CControlWinTraits>,
    public CComObjectRootEx<CComMultiThreadModelNoCS>,
    public IShellView2,
    public IFolderView,
    public IShellFolderView,
    public IOleCommandTarget,
    public IDropTarget,
    public IDropSource,
    public IViewObject,
    public IServiceProvider
{
    private:
        CComPtr<IShellFolder>     m_pSFParent;
        CComPtr<IShellFolder2>    m_pSF2Parent;
        CComPtr<IShellBrowser>    m_pShellBrowser;
        CComPtr<ICommDlgBrowser>  m_pCommDlgBrowser;
        CComPtr<IShellFolderViewDual> m_pShellFolderViewDual;
        CListView                 m_ListView;
        HWND                      m_hWndParent;
        FOLDERSETTINGS            m_FolderSettings;
        HMENU                     m_hMenu;                /* Handle to the menu bar of the browser */
        HMENU                     m_hMenuArrangeModes;    /* Handle to the popup menu with the arrange modes */
        HMENU                     m_hMenuViewModes;       /* Handle to the popup menu with the view modes */
        HMENU                     m_hContextMenu;         /* Handle to the open context menu */
        BOOL                      m_bmenuBarInitialized;
        UINT                      m_uState;
        UINT                      m_cidl;
        PCUITEMID_CHILD          *m_apidl;
        PIDLIST_ABSOLUTE          m_pidlParent;
        LISTVIEW_SORT_INFO        m_sortInfo;
        ULONG                     m_hNotify;            /* Change notification handle */
        HACCEL                    m_hAccel;
        DWORD                     m_dwAspects;
        DWORD                     m_dwAdvf;
        CComPtr<IAdviseSink>      m_pAdvSink;
        // for drag and drop
        CComPtr<IDataObject>      m_pSourceDataObject;
        CComPtr<IDropTarget>      m_pCurDropTarget;     /* The sub-item, which is currently dragged over */
        CComPtr<IDataObject>      m_pCurDataObject;     /* The dragged data-object */
        LONG                      m_iDragOverItem;      /* Dragged over item's index, iff m_pCurDropTarget != NULL */
        UINT                      m_cScrollDelay;       /* Send a WM_*SCROLL msg every 250 ms during drag-scroll */
        POINT                     m_ptLastMousePos;     /* Mouse position at last DragOver call */
        POINT                     m_ptFirstMousePos;    /* Mouse position when the drag operation started */
        //
        CComPtr<IContextMenu>     m_pCM;

        BOOL                      m_isEditing;

        CLSID m_Category;
        BOOL m_Destroyed;
    private:

        HRESULT _MergeToolbar();

    public:
        CDefView();
        ~CDefView();
        HRESULT WINAPI Initialize(IShellFolder *shellFolder);
        HRESULT IncludeObject(PCUITEMID_CHILD pidl);
        HRESULT OnDefaultCommand();
        HRESULT OnStateChange(UINT uFlags);
        void UpdateStatusbar();
        void CheckToolbar();
        void SetStyle(DWORD dwAdd, DWORD dwRemove);
        BOOL CreateList();
        void UpdateListColors();
        BOOL InitList();
        static INT CALLBACK ListViewCompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lpData);

        PCUITEMID_CHILD _PidlByItem(int i);
        PCUITEMID_CHILD _PidlByItem(LVITEM& lvItem);
        int LV_FindItemByPidl(PCUITEMID_CHILD pidl);
        BOOLEAN LV_AddItem(PCUITEMID_CHILD pidl);
        BOOLEAN LV_DeleteItem(PCUITEMID_CHILD pidl);
        BOOLEAN LV_RenameItem(PCUITEMID_CHILD pidlOld, PCUITEMID_CHILD pidlNew);
        BOOLEAN LV_ProdItem(PCUITEMID_CHILD pidl);
        static INT CALLBACK fill_list(LPVOID ptr, LPVOID arg);
        HRESULT FillList();
        HRESULT FillFileMenu();
        HRESULT FillEditMenu();
        HRESULT FillViewMenu();
        HRESULT FillArrangeAsMenu(HMENU hmenuArrange);
        HRESULT CheckViewMode(HMENU hmenuView);
        UINT GetSelections();
        HRESULT OpenSelectedItems();
        void OnDeactivate();
        void DoActivate(UINT uState);
        HRESULT drag_notify_subitem(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
        HRESULT InvokeContextMenuCommand(UINT uCommand);
        LRESULT OnExplorerCommand(UINT uCommand, BOOL bUseSelection);

        // *** IOleWindow methods ***
        virtual HRESULT STDMETHODCALLTYPE GetWindow(HWND *lphwnd);
        virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode);

        // *** IShellView methods ***
        virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator(MSG *pmsg);
        virtual HRESULT STDMETHODCALLTYPE EnableModeless(BOOL fEnable);
        virtual HRESULT STDMETHODCALLTYPE UIActivate(UINT uState);
        virtual HRESULT STDMETHODCALLTYPE Refresh();
        virtual HRESULT STDMETHODCALLTYPE CreateViewWindow(IShellView *psvPrevious, LPCFOLDERSETTINGS pfs, IShellBrowser *psb, RECT *prcView, HWND *phWnd);
        virtual HRESULT STDMETHODCALLTYPE DestroyViewWindow();
        virtual HRESULT STDMETHODCALLTYPE GetCurrentInfo(LPFOLDERSETTINGS pfs);
        virtual HRESULT STDMETHODCALLTYPE AddPropertySheetPages(DWORD dwReserved, LPFNSVADDPROPSHEETPAGE pfn, LPARAM lparam);
        virtual HRESULT STDMETHODCALLTYPE SaveViewState();
        virtual HRESULT STDMETHODCALLTYPE SelectItem(PCUITEMID_CHILD pidlItem, SVSIF uFlags);
        virtual HRESULT STDMETHODCALLTYPE GetItemObject(UINT uItem, REFIID riid, void **ppv);

        // *** IShellView2 methods ***
        virtual HRESULT STDMETHODCALLTYPE GetView(SHELLVIEWID *view_guid, ULONG view_type);
        virtual HRESULT STDMETHODCALLTYPE CreateViewWindow2(LPSV2CVW2_PARAMS view_params);
        virtual HRESULT STDMETHODCALLTYPE HandleRename(LPCITEMIDLIST new_pidl);
        virtual HRESULT STDMETHODCALLTYPE SelectAndPositionItem(LPCITEMIDLIST item, UINT flags, POINT *point);

        // *** IShellView3 methods ***
        virtual HRESULT STDMETHODCALLTYPE CreateViewWindow3(IShellBrowser *psb, IShellView *psvPrevious, SV3CVW3_FLAGS view_flags, FOLDERFLAGS mask, FOLDERFLAGS flags, FOLDERVIEWMODE mode, const SHELLVIEWID *view_id, RECT *prcView, HWND *hwnd);

        // *** IFolderView methods ***
        virtual HRESULT STDMETHODCALLTYPE GetCurrentViewMode(UINT *pViewMode);
        virtual HRESULT STDMETHODCALLTYPE SetCurrentViewMode(UINT ViewMode);
        virtual HRESULT STDMETHODCALLTYPE GetFolder(REFIID riid, void **ppv);
        virtual HRESULT STDMETHODCALLTYPE Item(int iItemIndex, PITEMID_CHILD *ppidl);
        virtual HRESULT STDMETHODCALLTYPE ItemCount(UINT uFlags, int *pcItems);
        virtual HRESULT STDMETHODCALLTYPE Items(UINT uFlags, REFIID riid, void **ppv);
        virtual HRESULT STDMETHODCALLTYPE GetSelectionMarkedItem(int *piItem);
        virtual HRESULT STDMETHODCALLTYPE GetFocusedItem(int *piItem);
        virtual HRESULT STDMETHODCALLTYPE GetItemPosition(PCUITEMID_CHILD pidl, POINT *ppt);
        virtual HRESULT STDMETHODCALLTYPE GetSpacing(POINT *ppt);
        virtual HRESULT STDMETHODCALLTYPE GetDefaultSpacing(POINT *ppt);
        virtual HRESULT STDMETHODCALLTYPE GetAutoArrange();
        virtual HRESULT STDMETHODCALLTYPE SelectItem(int iItem, DWORD dwFlags);
        virtual HRESULT STDMETHODCALLTYPE SelectAndPositionItems(UINT cidl, PCUITEMID_CHILD_ARRAY apidl, POINT *apt, DWORD dwFlags);

        // *** IShellFolderView methods ***
        virtual HRESULT STDMETHODCALLTYPE Rearrange(LPARAM sort);
        virtual HRESULT STDMETHODCALLTYPE GetArrangeParam(LPARAM *sort);
        virtual HRESULT STDMETHODCALLTYPE ArrangeGrid();
        virtual HRESULT STDMETHODCALLTYPE AutoArrange();
        virtual HRESULT STDMETHODCALLTYPE AddObject(PITEMID_CHILD pidl, UINT *item);
        virtual HRESULT STDMETHODCALLTYPE GetObject(PITEMID_CHILD *pidl, UINT item);
        virtual HRESULT STDMETHODCALLTYPE RemoveObject(PITEMID_CHILD pidl, UINT *item);
        virtual HRESULT STDMETHODCALLTYPE GetObjectCount(UINT *count);
        virtual HRESULT STDMETHODCALLTYPE SetObjectCount(UINT count, UINT flags);
        virtual HRESULT STDMETHODCALLTYPE UpdateObject(PITEMID_CHILD pidl_old, PITEMID_CHILD pidl_new, UINT *item);
        virtual HRESULT STDMETHODCALLTYPE RefreshObject(PITEMID_CHILD pidl, UINT *item);
        virtual HRESULT STDMETHODCALLTYPE SetRedraw(BOOL redraw);
        virtual HRESULT STDMETHODCALLTYPE GetSelectedCount(UINT *count);
        virtual HRESULT STDMETHODCALLTYPE GetSelectedObjects(PCUITEMID_CHILD **pidl, UINT *items);
        virtual HRESULT STDMETHODCALLTYPE IsDropOnSource(IDropTarget *drop_target);
        virtual HRESULT STDMETHODCALLTYPE GetDragPoint(POINT *pt);
        virtual HRESULT STDMETHODCALLTYPE GetDropPoint(POINT *pt);
        virtual HRESULT STDMETHODCALLTYPE MoveIcons(IDataObject *obj);
        virtual HRESULT STDMETHODCALLTYPE SetItemPos(PCUITEMID_CHILD pidl, POINT *pt);
        virtual HRESULT STDMETHODCALLTYPE IsBkDropTarget(IDropTarget *drop_target);
        virtual HRESULT STDMETHODCALLTYPE SetClipboard(BOOL move);
        virtual HRESULT STDMETHODCALLTYPE SetPoints(IDataObject *obj);
        virtual HRESULT STDMETHODCALLTYPE GetItemSpacing(ITEMSPACING *spacing);
        virtual HRESULT STDMETHODCALLTYPE SetCallback(IShellFolderViewCB *new_cb, IShellFolderViewCB **old_cb);
        virtual HRESULT STDMETHODCALLTYPE Select(UINT flags);
        virtual HRESULT STDMETHODCALLTYPE QuerySupport(UINT *support);
        virtual HRESULT STDMETHODCALLTYPE SetAutomationObject(IDispatch *disp);

        // *** IOleCommandTarget methods ***
        virtual HRESULT STDMETHODCALLTYPE QueryStatus(const GUID *pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[  ], OLECMDTEXT *pCmdText);
        virtual HRESULT STDMETHODCALLTYPE Exec(const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut);

        // *** IDropTarget methods ***
        virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
        virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
        virtual HRESULT STDMETHODCALLTYPE DragLeave();
        virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

        // *** IDropSource methods ***
        virtual HRESULT STDMETHODCALLTYPE QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState);
        virtual HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);

        // *** IViewObject methods ***
        virtual HRESULT STDMETHODCALLTYPE Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
                                               HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
                                               BOOL ( STDMETHODCALLTYPE *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
        virtual HRESULT STDMETHODCALLTYPE GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect,
                DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet);
        virtual HRESULT STDMETHODCALLTYPE Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze);
        virtual HRESULT STDMETHODCALLTYPE Unfreeze(DWORD dwFreeze);
        virtual HRESULT STDMETHODCALLTYPE SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
        virtual HRESULT STDMETHODCALLTYPE GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);

        // *** IServiceProvider methods ***
        virtual HRESULT STDMETHODCALLTYPE QueryService(REFGUID guidService, REFIID riid, void **ppvObject);

        // Message handlers
        LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnGetShellBrowser(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnNCCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnNCDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnChangeNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnCustomItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
        LRESULT OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

        static ATL::CWndClassInfo& GetWndClassInfo()
        {
            static ATL::CWndClassInfo wc =
            {
                {   sizeof(WNDCLASSEX), CS_PARENTDC, StartWindowProc,
                    0, 0, NULL, NULL,
                    LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, SV_CLASS_NAME, NULL
                },
                NULL, NULL, IDC_ARROW, TRUE, 0, _T("")
            };
            return wc;
        }

        virtual WNDPROC GetWindowProc()
        {
            return WindowProc;
        }

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            CDefView *pThis;
            LRESULT  result;

            // Must hold a reference during message handling
            pThis = reinterpret_cast<CDefView *>(hWnd);
            pThis->AddRef();
            result = CWindowImpl<CDefView, CWindow, CControlWinTraits>::WindowProc(hWnd, uMsg, wParam, lParam);
            pThis->Release();
            return result;
        }

        BEGIN_MSG_MAP(CDefView)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
        MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
        MESSAGE_HANDLER(WM_NCCREATE, OnNCCreate)
        MESSAGE_HANDLER(WM_NCDESTROY, OnNCDestroy)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_ACTIVATE, OnActivate)
        MESSAGE_HANDLER(WM_NOTIFY, OnNotify)
        MESSAGE_HANDLER(WM_COMMAND, OnCommand)
        MESSAGE_HANDLER(SHV_CHANGE_NOTIFY, OnChangeNotify)
        MESSAGE_HANDLER(WM_CONTEXTMENU, OnContextMenu)
        MESSAGE_HANDLER(WM_DRAWITEM, OnCustomItem)
        MESSAGE_HANDLER(WM_MEASUREITEM, OnCustomItem)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
        MESSAGE_HANDLER(WM_SYSCOLORCHANGE, OnSysColorChange)
        MESSAGE_HANDLER(CWM_GETISHELLBROWSER, OnGetShellBrowser)
        MESSAGE_HANDLER(WM_SETTINGCHANGE, OnSettingChange)
        MESSAGE_HANDLER(WM_INITMENUPOPUP, OnInitMenuPopup)
        END_MSG_MAP()

        BEGIN_COM_MAP(CDefView)
        // Windows returns E_NOINTERFACE for IOleWindow
        // COM_INTERFACE_ENTRY_IID(IID_IOleWindow, IOleWindow)
        COM_INTERFACE_ENTRY_IID(IID_IShellView, IShellView)
        COM_INTERFACE_ENTRY_IID(IID_CDefView, IShellView)
        COM_INTERFACE_ENTRY_IID(IID_IShellView2, IShellView2)
        COM_INTERFACE_ENTRY_IID(IID_IFolderView, IFolderView)
        COM_INTERFACE_ENTRY_IID(IID_IShellFolderView, IShellFolderView)
        COM_INTERFACE_ENTRY_IID(IID_IOleCommandTarget, IOleCommandTarget)
        COM_INTERFACE_ENTRY_IID(IID_IDropTarget, IDropTarget)
        COM_INTERFACE_ENTRY_IID(IID_IDropSource, IDropSource)
        COM_INTERFACE_ENTRY_IID(IID_IViewObject, IViewObject)
        COM_INTERFACE_ENTRY_IID(IID_IServiceProvider, IServiceProvider)
        END_COM_MAP()
};

/*menu items */
#define IDM_VIEW_FILES  (FCIDM_SHVIEWFIRST + 0x500)
#define IDM_VIEW_IDW    (FCIDM_SHVIEWFIRST + 0x501)
#define IDM_MYFILEITEM  (FCIDM_SHVIEWFIRST + 0x502)

#define ID_LISTVIEW     1

/*windowsx.h */
#define GET_WM_COMMAND_ID(wp, lp)       LOWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)     (HWND)(lp)
#define GET_WM_COMMAND_CMD(wp, lp)      HIWORD(wp)

typedef void (CALLBACK *PFNSHGETSETTINGSPROC)(LPSHELLFLAGSTATE lpsfs, DWORD dwMask);

CDefView::CDefView() :
    m_ListView(),
    m_hWndParent(NULL),
    m_hMenu(NULL),
    m_hMenuArrangeModes(NULL),
    m_hMenuViewModes(NULL),
    m_hContextMenu(NULL),
    m_bmenuBarInitialized(FALSE),
    m_uState(0),
    m_cidl(0),
    m_apidl(NULL),
    m_pidlParent(NULL),
    m_hNotify(0),
    m_hAccel(NULL),
    m_dwAspects(0),
    m_dwAdvf(0),
    m_iDragOverItem(0),
    m_cScrollDelay(0),
    m_isEditing(FALSE),
    m_Destroyed(FALSE)
{
    ZeroMemory(&m_FolderSettings, sizeof(m_FolderSettings));
    ZeroMemory(&m_sortInfo, sizeof(m_sortInfo));
    ZeroMemory(&m_ptLastMousePos, sizeof(m_ptLastMousePos));
    ZeroMemory(&m_Category, sizeof(m_Category));
}

CDefView::~CDefView()
{
    TRACE(" destroying IShellView(%p)\n", this);

    if (m_hWnd)
    {
        DestroyViewWindow();
    }

    SHFree(m_apidl);
}

HRESULT WINAPI CDefView::Initialize(IShellFolder *shellFolder)
{
    m_pSFParent = shellFolder;
    shellFolder->QueryInterface(IID_PPV_ARG(IShellFolder2, &m_pSF2Parent));

    return S_OK;
}

/**********************************************************
 *
 * ##### helperfunctions for communication with ICommDlgBrowser #####
 */
HRESULT CDefView::IncludeObject(PCUITEMID_CHILD pidl)
{
    HRESULT ret = S_OK;

    if (m_pCommDlgBrowser.p != NULL)
    {
        TRACE("ICommDlgBrowser::IncludeObject pidl=%p\n", pidl);
        ret = m_pCommDlgBrowser->IncludeObject(this, pidl);
        TRACE("--0x%08x\n", ret);
    }

    return ret;
}

HRESULT CDefView::OnDefaultCommand()
{
    HRESULT ret = S_FALSE;

    if (m_pCommDlgBrowser.p != NULL)
    {
        TRACE("ICommDlgBrowser::OnDefaultCommand\n");
        ret = m_pCommDlgBrowser->OnDefaultCommand(this);
        TRACE("-- returns %08x\n", ret);
    }

    return ret;
}

HRESULT CDefView::OnStateChange(UINT uFlags)
{
    HRESULT ret = S_FALSE;

    if (m_pCommDlgBrowser.p != NULL)
    {
        TRACE("ICommDlgBrowser::OnStateChange flags=%x\n", uFlags);
        ret = m_pCommDlgBrowser->OnStateChange(this, uFlags);
        TRACE("--\n");
    }

    return ret;
}
/**********************************************************
 *    set the toolbar of the filedialog buttons
 *
 * - activates the buttons from the shellbrowser according to
 *   the view state
 */
void CDefView::CheckToolbar()
{
    LRESULT result;

    TRACE("\n");

    if (m_pCommDlgBrowser != NULL)
    {
        m_pShellBrowser->SendControlMsg(FCW_TOOLBAR, TB_CHECKBUTTON,
                                      FCIDM_TB_SMALLICON, (m_FolderSettings.ViewMode == FVM_LIST) ? TRUE : FALSE, &result);
        m_pShellBrowser->SendControlMsg(FCW_TOOLBAR, TB_CHECKBUTTON,
                                      FCIDM_TB_REPORTVIEW, (m_FolderSettings.ViewMode == FVM_DETAILS) ? TRUE : FALSE, &result);
        m_pShellBrowser->SendControlMsg(FCW_TOOLBAR, TB_ENABLEBUTTON,
                                      FCIDM_TB_SMALLICON, TRUE, &result);
        m_pShellBrowser->SendControlMsg(FCW_TOOLBAR, TB_ENABLEBUTTON,
                                      FCIDM_TB_REPORTVIEW, TRUE, &result);
    }
}

void CDefView::UpdateStatusbar()
{
    WCHAR szFormat[MAX_PATH] = {0};
    WCHAR szObjects[MAX_PATH] = {0};
    UINT cSelectedItems;

    cSelectedItems = m_ListView.GetSelectedCount();
    if (cSelectedItems)
    {
        LoadStringW(shell32_hInstance, IDS_OBJECTS_SELECTED, szFormat, _countof(szFormat));
        StringCchPrintfW(szObjects, MAX_PATH, szFormat, cSelectedItems);
    }
    else
    {
        LoadStringW(shell32_hInstance, IDS_OBJECTS, szFormat, _countof(szFormat));
        StringCchPrintfW(szObjects, MAX_PATH, szFormat, m_ListView.GetItemCount());
    }
    m_pShellBrowser->SetStatusTextSB(szObjects);
}

/**********************************************************
 *
 * ##### helperfunctions for initializing the view #####
 */
/**********************************************************
 *    change the style of the listview control
 */
void CDefView::SetStyle(DWORD dwAdd, DWORD dwRemove)
{
    DWORD tmpstyle;

    TRACE("(%p)\n", this);

    tmpstyle = ::GetWindowLongPtrW(m_ListView, GWL_STYLE);
    ::SetWindowLongPtrW(m_ListView, GWL_STYLE, dwAdd | (tmpstyle & ~dwRemove));
}

/**********************************************************
* ShellView_CreateList()
*
* - creates the list view window
*/
BOOL CDefView::CreateList()
{
    DWORD dwStyle, dwExStyle;

    TRACE("%p\n", this);

    dwStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
              LVS_SHAREIMAGELISTS | LVS_EDITLABELS | LVS_AUTOARRANGE;
    dwExStyle = WS_EX_CLIENTEDGE;

    if (m_FolderSettings.fFlags & FWF_DESKTOP)
        dwStyle |= LVS_ALIGNLEFT;
    else
        dwStyle |= LVS_ALIGNTOP | LVS_SHOWSELALWAYS;

    switch (m_FolderSettings.ViewMode)
    {
        case FVM_ICON:
            dwStyle |= LVS_ICON;
            break;

        case FVM_DETAILS:
            dwStyle |= LVS_REPORT;
            break;

        case FVM_SMALLICON:
            dwStyle |= LVS_SMALLICON;
            break;

        case FVM_LIST:
            dwStyle |= LVS_LIST;
            break;

        default:
            dwStyle |= LVS_LIST;
            break;
    }

    if (m_FolderSettings.fFlags & FWF_AUTOARRANGE)
        dwStyle |= LVS_AUTOARRANGE;

    if (m_FolderSettings.fFlags & FWF_DESKTOP)
        m_FolderSettings.fFlags |= FWF_NOCLIENTEDGE | FWF_NOSCROLL;

    if (m_FolderSettings.fFlags & FWF_SINGLESEL)
        dwStyle |= LVS_SINGLESEL;

    if (m_FolderSettings.fFlags & FWF_NOCLIENTEDGE)
        dwExStyle &= ~WS_EX_CLIENTEDGE;

    RECT rcListView = {0,0,0,0};
    m_ListView.Create(m_hWnd, rcListView, L"FolderView", dwStyle, dwExStyle, ID_LISTVIEW);

    if (!m_ListView)
        return FALSE;

    m_sortInfo.bIsAscending = TRUE;
    m_sortInfo.nHeaderID = -1;
    m_sortInfo.nLastHeaderID = -1;

    UpdateListColors();

    /*  UpdateShellSettings(); */
    return TRUE;
}

void CDefView::UpdateListColors()
{
    if (m_FolderSettings.fFlags & FWF_DESKTOP)
    {
        /* Check if drop shadows option is enabled */
        BOOL bDropShadow = FALSE;
        DWORD cbDropShadow = sizeof(bDropShadow);

        /*
         * The desktop ListView always take the default desktop colours, by
         * remaining transparent and letting user32/win32k paint itself the
         * desktop background color, if any.
         */
        m_ListView.SetBkColor(CLR_NONE);

        SHGetValueW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced",
                     L"ListviewShadow", NULL, &bDropShadow, &cbDropShadow);
        if (bDropShadow)
        {
            /* Set the icon background transparent */
            m_ListView.SetTextBkColor(CLR_NONE);
            m_ListView.SetTextColor(RGB(255, 255, 255));
            m_ListView.SetExtendedListViewStyle(LVS_EX_TRANSPARENTSHADOWTEXT, LVS_EX_TRANSPARENTSHADOWTEXT);
        }
        else
        {
            /* Set the icon background as the same colour as the desktop */
            COLORREF crDesktop = GetSysColor(COLOR_DESKTOP);
            m_ListView.SetTextBkColor(crDesktop);
            if (GetRValue(crDesktop) + GetGValue(crDesktop) + GetBValue(crDesktop) > 128 * 3)
                m_ListView.SetTextColor(RGB(0, 0, 0));
            else
                m_ListView.SetTextColor(RGB(255, 255, 255));
            m_ListView.SetExtendedListViewStyle(0, LVS_EX_TRANSPARENTSHADOWTEXT);
        }
    }
}

/**********************************************************
* ShellView_InitList()
*
* - adds all needed columns to the shellview
*/
BOOL CDefView::InitList()
{
    SHELLDETAILS sd;
    WCHAR szTemp[50];
    HIMAGELIST big_icons, small_icons;

    TRACE("%p\n", this);

    m_ListView.DeleteAllItems();

    m_hMenuArrangeModes = CreateMenu();

    if (m_pSF2Parent)
    {
        for (int i = 0; 1; i++)
        {
            if (FAILED(m_pSF2Parent->GetDetailsOf(NULL, i, &sd)))
                break;
            StrRetToStrNW( szTemp, 50, &sd.str, NULL);
            m_ListView.InsertColumn(i, szTemp, sd.fmt, sd.cxChar * 8);

            InsertMenuW(m_hMenuArrangeModes, -1, MF_STRING, 0x30 + i,  szTemp);
        }

        InsertMenuW(m_hMenuArrangeModes, -1, MF_BYPOSITION | MF_SEPARATOR, 0, 0);
    }
    else
    {
        FIXME("no m_pSF2Parent\n");
    }

    Shell_GetImageLists(&big_icons, &small_icons);
    m_ListView.SetImageList(big_icons, LVSIL_NORMAL);
    m_ListView.SetImageList(small_icons, LVSIL_SMALL);

    return TRUE;
}

/*************************************************************************
 * ShellView_ListViewCompareItems
 *
 * Compare Function for the Listview (FileOpen Dialog)
 *
 * PARAMS
 *     lParam1       [I] the first ItemIdList to compare with
 *     lParam2       [I] the second ItemIdList to compare with
 *     lpData        [I] The column ID for the header Ctrl to process
 *
 * RETURNS
 *     A negative value if the first item should precede the second,
 *     a positive value if the first item should follow the second,
 *     or zero if the two items are equivalent
 */
INT CALLBACK CDefView::ListViewCompareItems(LPARAM lParam1, LPARAM lParam2, LPARAM lpData)
{
    PCUIDLIST_RELATIVE pidl1 = reinterpret_cast<PCUIDLIST_RELATIVE>(lParam1);
    PCUIDLIST_RELATIVE pidl2 = reinterpret_cast<PCUIDLIST_RELATIVE>(lParam2);
    CDefView *pThis = reinterpret_cast<CDefView*>(lpData);

    HRESULT hres = pThis->m_pSFParent->CompareIDs(pThis->m_sortInfo.nHeaderID, pidl1, pidl2);
    if (FAILED_UNEXPECTEDLY(hres))
        return 0;

    SHORT nDiff = HRESULT_CODE(hres);
    if (!pThis->m_sortInfo.bIsAscending)
        nDiff = -nDiff;
    return nDiff;
}

PCUITEMID_CHILD CDefView::_PidlByItem(int i)
{
    return reinterpret_cast<PCUITEMID_CHILD>(m_ListView.GetItemData(i));
}

PCUITEMID_CHILD CDefView::_PidlByItem(LVITEM& lvItem)
{
    return reinterpret_cast<PCUITEMID_CHILD>(lvItem.lParam);
}

/**********************************************************
*  LV_FindItemByPidl()
*/
int CDefView::LV_FindItemByPidl(PCUITEMID_CHILD pidl)
{
    int cItems = m_ListView.GetItemCount();

    for (int i = 0; i<cItems; i++)
    {
        PCUITEMID_CHILD currentpidl = _PidlByItem(i);
        HRESULT hr = m_pSFParent->CompareIDs(0, pidl, currentpidl);

        if (SUCCEEDED(hr) && !HRESULT_CODE(hr))
        {
            return i;
        }
    }
    return -1;
}

/**********************************************************
* LV_AddItem()
*/
BOOLEAN CDefView::LV_AddItem(PCUITEMID_CHILD pidl)
{
    LVITEMW lvItem;

    TRACE("(%p)(pidl=%p)\n", this, pidl);

    lvItem.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;    /*set the mask*/
    lvItem.iItem = m_ListView.GetItemCount();             /*add the item to the end of the list*/
    lvItem.iSubItem = 0;
    lvItem.lParam = reinterpret_cast<LPARAM>(ILClone(pidl)); /*set the item's data*/
    lvItem.pszText = LPSTR_TEXTCALLBACKW;                 /*get text on a callback basis*/
    lvItem.iImage = I_IMAGECALLBACK;                      /*get the image on a callback basis*/
    lvItem.stateMask = LVIS_CUT;

    if (m_ListView.InsertItem(&lvItem) == -1)
        return FALSE;
    else
        return TRUE;
}

/**********************************************************
* LV_DeleteItem()
*/
BOOLEAN CDefView::LV_DeleteItem(PCUITEMID_CHILD pidl)
{
    int nIndex;

    TRACE("(%p)(pidl=%p)\n", this, pidl);

    nIndex = LV_FindItemByPidl(pidl);

    return (-1 == m_ListView.DeleteItem(nIndex)) ? FALSE : TRUE;
}

/**********************************************************
* LV_RenameItem()
*/
BOOLEAN CDefView::LV_RenameItem(PCUITEMID_CHILD pidlOld, PCUITEMID_CHILD pidlNew)
{
    int nItem;
    LVITEMW lvItem;

    TRACE("(%p)(pidlold=%p pidlnew=%p)\n", this, pidlOld, pidlNew);

    nItem = LV_FindItemByPidl(pidlOld);

    if ( -1 != nItem )
    {
        lvItem.mask = LVIF_PARAM;        /* only the pidl */
        lvItem.iItem = nItem;
        lvItem.iSubItem = 0;
        m_ListView.GetItem(&lvItem);

        SHFree(reinterpret_cast<LPVOID>(lvItem.lParam));
        lvItem.mask = LVIF_PARAM|LVIF_IMAGE;
        lvItem.iItem = nItem;
        lvItem.iSubItem = 0;
        lvItem.lParam = reinterpret_cast<LPARAM>(ILClone(pidlNew));    /* set the item's data */
        lvItem.iImage = SHMapPIDLToSystemImageListIndex(m_pSFParent, pidlNew, 0);
        m_ListView.SetItem(&lvItem);
        m_ListView.Update(nItem);
        return TRUE;                    /* FIXME: better handling */
    }

    return FALSE;
}

/**********************************************************
* LV_ProdItem()
*/
BOOLEAN CDefView::LV_ProdItem(PCUITEMID_CHILD pidl)
{
    int nItem;
    LVITEMW lvItem;

    TRACE("(%p)(pidl=%p)\n", this, pidl);

    nItem = LV_FindItemByPidl(pidl);

    if (-1 != nItem)
    {
        lvItem.mask = LVIF_IMAGE;
        lvItem.iItem = nItem;
        lvItem.iSubItem = 0;
        lvItem.iImage = SHMapPIDLToSystemImageListIndex(m_pSFParent, pidl, 0);
        m_ListView.SetItem(&lvItem);
        m_ListView.Update(nItem);
        return TRUE;
    }

    return FALSE;
}

/**********************************************************
* ShellView_FillList()
*
* - gets the objectlist from the shellfolder
* - sorts the list
* - fills the list into the view
*/
INT CALLBACK CDefView::fill_list(LPVOID ptr, LPVOID arg)
{
    PITEMID_CHILD pidl = static_cast<PITEMID_CHILD>(ptr);
    CDefView *pThis = static_cast<CDefView *>(arg);

    /* in a commdlg This works as a filemask*/
    if (pThis->IncludeObject(pidl) == S_OK)
        pThis->LV_AddItem(pidl);

    SHFree(pidl);
    return TRUE;
}

HRESULT CDefView::FillList()
{
    CComPtr<IEnumIDList> pEnumIDList;
    PITEMID_CHILD pidl;
    DWORD         dwFetched;
    HRESULT       hRes;
    HDPA          hdpa;
    HKEY          hKey;
    DWORD         dFlags = SHCONTF_NONFOLDERS | SHCONTF_FOLDERS;

    TRACE("%p\n", this);

    /* determine if there is a setting to show all the hidden files/folders */
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Advanced", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        DWORD dataLength, flagVal;

        dataLength = sizeof(flagVal);
        if (RegQueryValueExW(hKey, L"Hidden", NULL, NULL, (LPBYTE)&flagVal, &dataLength) == ERROR_SUCCESS)
        {
            /* if the value is 1, then show all hidden files/folders */
            if (flagVal == 1)
            {
                dFlags |= SHCONTF_INCLUDEHIDDEN;
                m_ListView.SendMessageW(LVM_SETCALLBACKMASK, LVIS_CUT, 0);
            }
        }

        /* close the key */
        RegCloseKey(hKey);
    }

    /* get the itemlist from the shfolder */
    hRes = m_pSFParent->EnumObjects(m_hWnd, dFlags, &pEnumIDList);
    if (hRes != S_OK)
    {
        if (hRes == S_FALSE)
            return(NOERROR);
        return(hRes);
    }

    /* create a pointer array */
    hdpa = DPA_Create(16);
    if (!hdpa)
    {
        return(E_OUTOFMEMORY);
    }

    /* copy the items into the array*/
    while((S_OK == pEnumIDList->Next(1, &pidl, &dwFetched)) && dwFetched)
    {
        if (DPA_InsertPtr(hdpa, 0x7fff, pidl) == -1)
        {
            SHFree(pidl);
        }
    }

    /*turn the listview's redrawing off*/
    m_ListView.SetRedraw(FALSE);

    DPA_DestroyCallback( hdpa, fill_list, this);

    /* sort the array */
    if (m_pSF2Parent)
    {
        m_pSF2Parent->GetDefaultColumn(NULL, (ULONG*)&m_sortInfo.nHeaderID, NULL);
    }
    else
    {
        FIXME("no m_pSF2Parent\n");
    }
    m_sortInfo.bIsAscending = TRUE;
    m_sortInfo.nLastHeaderID = m_sortInfo.nHeaderID;
    m_ListView.SortItems(ListViewCompareItems, this);

    /*turn the listview's redrawing back on and force it to draw*/
    m_ListView.SetRedraw(TRUE);

    return S_OK;
}

LRESULT CDefView::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    m_ListView.UpdateWindow();
    bHandled = FALSE;
    return 0;
}

LRESULT CDefView::OnGetDlgCode(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return m_ListView.SendMessageW(uMsg, 0, 0);
}

LRESULT CDefView::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    if (!m_Destroyed)
    {
        m_Destroyed = TRUE;
        if (m_hMenu)
        {
            DestroyMenu(m_hMenu);
            m_hMenu = NULL;
        }
        RevokeDragDrop(m_hWnd);
        SHChangeNotifyDeregister(m_hNotify);
        m_hNotify = NULL;
        SHFree(m_pidlParent);
        m_pidlParent = NULL;
    }
    bHandled = FALSE;
    return 0;
}

LRESULT CDefView::OnEraseBackground(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    /* redirect to parent */
    if (m_FolderSettings.fFlags & (FWF_DESKTOP | FWF_TRANSPARENT))
        return SendMessageW(GetParent(), WM_ERASEBKGND, wParam, lParam);

    bHandled = FALSE;
    return 0;
}

LRESULT CDefView::OnSysColorChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    /* Update desktop labels color */
    UpdateListColors();

    /* Forward WM_SYSCOLORCHANGE to common controls */
    return m_ListView.SendMessageW(uMsg, 0, 0);
}

LRESULT CDefView::OnGetShellBrowser(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    return reinterpret_cast<LRESULT>(m_pShellBrowser.p);
}

LRESULT CDefView::OnNCCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    this->AddRef();
    bHandled = FALSE;
    return 0;
}

LRESULT CDefView::OnNCDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    this->Release();
    bHandled = FALSE;
    return 0;
}

/**********************************************************
*  ShellView_OnCreate()
*/
LRESULT CDefView::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    CComPtr<IDropTarget>     pdt;
    SHChangeNotifyEntry      ntreg;
    CComPtr<IPersistFolder2> ppf2;

    TRACE("%p\n", this);

    if (CreateList())
    {
        if (InitList())
        {
            FillList();
        }
    }

    if (SUCCEEDED(QueryInterface(IID_PPV_ARG(IDropTarget, &pdt))))
    {
        if (FAILED(RegisterDragDrop(m_hWnd, pdt)))
            ERR("Registering Drag Drop Failed");
    }

    /* register for receiving notifications */
    m_pSFParent->QueryInterface(IID_PPV_ARG(IPersistFolder2, &ppf2));
    if (ppf2)
    {
        ppf2->GetCurFolder(&m_pidlParent);
        ntreg.fRecursive = TRUE;
        ntreg.pidl = m_pidlParent;
        m_hNotify = SHChangeNotifyRegister(m_hWnd, SHCNRF_InterruptLevel | SHCNRF_ShellLevel, SHCNE_ALLEVENTS, SHV_CHANGE_NOTIFY, 1, &ntreg);
    }

    m_hAccel = LoadAcceleratorsW(shell32_hInstance, MAKEINTRESOURCEW(IDA_SHELLVIEW));

    UpdateStatusbar();

    return S_OK;
}

/**********************************************************
 *    #### Handling of the menus ####
 */

extern "C" DWORD WINAPI SHMenuIndexFromID(HMENU hMenu, UINT uID);

HMENU GetSubmenuByID(HMENU hmenu, UINT id)
{
    MENUITEMINFOW mii = {sizeof(mii), MIIM_SUBMENU};
    if (::GetMenuItemInfoW(hmenu, id, FALSE, &mii))
        return mii.hSubMenu;

    return NULL;
}

/* ReallyGetMenuItemID returns the id of an item even if it opens a submenu,
   GetMenuItemID returns -1 if the specified item opens a submenu */
UINT ReallyGetMenuItemID(HMENU hmenu, int i)
{
    MENUITEMINFOW mii = {sizeof(mii), MIIM_ID};
    if (::GetMenuItemInfoW(hmenu, i, TRUE, &mii))
        return mii.wID;

    return UINT_MAX;
}

HRESULT CDefView::FillFileMenu()
{
    HMENU hFileMenu = GetSubmenuByID(m_hMenu, FCIDM_MENU_FILE);
    if (!hFileMenu)
        return E_FAIL;

    /* Cleanup the items added previously */
    for (int i = GetMenuItemCount(hFileMenu) - 1; i >= 0; i--)
    {
        UINT id = GetMenuItemID(hFileMenu, i);
        if (id < FCIDM_BROWSERFIRST || id > FCIDM_BROWSERLAST)
            DeleteMenu(hFileMenu, i, MF_BYPOSITION);
    }

    /* Store the context menu in m_pCM and keep it in order to invoke the selected command later on */
    HRESULT hr = GetItemObject(SVGIO_SELECTION, IID_PPV_ARG(IContextMenu, &m_pCM));
    if (FAILED_UNEXPECTEDLY(hr))
        return hr;

    IUnknown_SetSite(m_pCM, (IShellView *)this);

    HMENU hmenu = CreatePopupMenu();

    hr = m_pCM->QueryContextMenu(hmenu, 0, FCIDM_SHVIEWFIRST, FCIDM_SHVIEWLAST, 0);
    if (FAILED_UNEXPECTEDLY(hr))
        return hr;

    // TODO: filter or something

    Shell_MergeMenus(hFileMenu, hmenu, 0, 0, 0xFFFF, MM_ADDSEPARATOR | MM_SUBMENUSHAVEIDS);

    ::DestroyMenu(hmenu);

    return S_OK;
}

HRESULT CDefView::FillEditMenu()
{
    HMENU hEditMenu = GetSubmenuByID(m_hMenu, FCIDM_MENU_EDIT);
    if (!hEditMenu)
        return E_FAIL;

    HMENU hmenuContents = ::LoadMenuW(shell32_hInstance, L"MENU_003");
    if (!hmenuContents)
        return E_FAIL;

    Shell_MergeMenus(hEditMenu, hmenuContents, 0, 0, 0xFFFF, 0);

    ::DestroyMenu(hmenuContents);

    return S_OK;
}

HRESULT CDefView::FillViewMenu()
{
    HMENU hViewMenu = GetSubmenuByID(m_hMenu, FCIDM_MENU_VIEW);
    if (!hViewMenu)
        return E_FAIL;

    m_hMenuViewModes = ::LoadMenuW(shell32_hInstance, L"MENU_001");
    if (!m_hMenuViewModes)
        return E_FAIL;

    UINT i = SHMenuIndexFromID(hViewMenu, FCIDM_MENU_VIEW_SEP_OPTIONS);
    Shell_MergeMenus(hViewMenu, m_hMenuViewModes, i, 0, 0xFFFF, MM_ADDSEPARATOR | MM_DONTREMOVESEPS | MM_SUBMENUSHAVEIDS);

    return S_OK;
}

HRESULT CDefView::FillArrangeAsMenu(HMENU hmenuArrange)
{
    /* We only need to fill this once */
    if (GetMenuItemID(hmenuArrange, 0) == FCIDM_SHVIEW_AUTOARRANGE)
    {
        Shell_MergeMenus(hmenuArrange, m_hMenuArrangeModes, 0, 0, 0xFFFF,0);
    }

    /* Also check the menu item according to which we sort */
    CheckMenuRadioItem(hmenuArrange, 
                       0x30, 
                       0x100, 
                       m_sortInfo.nHeaderID + 0x30, 
                       MF_BYCOMMAND);

    return S_OK;
}

HRESULT CDefView::CheckViewMode(HMENU hmenuView)
{
    if (m_FolderSettings.ViewMode >= FVM_FIRST && m_FolderSettings.ViewMode <= FVM_LAST)
    {
        UINT iItemFirst = FCIDM_SHVIEW_BIGICON;
        UINT iItemLast = iItemFirst + FVM_LAST - FVM_FIRST;
        UINT iItem = iItemFirst + m_FolderSettings.ViewMode - FVM_FIRST;
        CheckMenuRadioItem(hmenuView, iItemFirst, iItemLast, iItem, MF_BYCOMMAND);
    }

    return S_OK;
}

/**********************************************************
*   ShellView_GetSelections()
*
* - fills the m_apidl list with the selected objects
*
* RETURNS
*  number of selected items
*/
UINT CDefView::GetSelections()
{
    SHFree(m_apidl);

    m_cidl = m_ListView.GetSelectedCount();
    m_apidl = static_cast<PCUITEMID_CHILD*>(SHAlloc(m_cidl * sizeof(PCUITEMID_CHILD)));
    if (!m_apidl)
    {
        m_cidl = 0;
        return 0;
    }

    TRACE("-- Items selected =%u\n", m_cidl);

    UINT i = 0;
    int lvIndex = -1;
    while ((lvIndex = m_ListView.GetNextItem(lvIndex,  LVNI_SELECTED)) > -1)
    {
        m_apidl[i] = _PidlByItem(lvIndex);
        i++;
        if (i == m_cidl)
             break;
        TRACE("-- selected Item found\n");
    }

    return m_cidl;
}

HRESULT CDefView::InvokeContextMenuCommand(UINT uCommand)
{
    CMINVOKECOMMANDINFO cmi;

    ZeroMemory(&cmi, sizeof(cmi));
    cmi.cbSize = sizeof(cmi);
    cmi.lpVerb = MAKEINTRESOURCEA(uCommand);
    cmi.hwnd = m_hWnd;

    if (GetKeyState(VK_SHIFT) & 0x8000)
        cmi.fMask |= CMIC_MASK_SHIFT_DOWN;

    if (GetKeyState(VK_CONTROL) & 0x8000)
        cmi.fMask |= CMIC_MASK_CONTROL_DOWN;

    HRESULT hr = m_pCM->InvokeCommand(&cmi);
    if (FAILED_UNEXPECTEDLY(hr))
        return hr;

    return S_OK;
}

/**********************************************************
 *    ShellView_OpenSelectedItems()
 */
HRESULT CDefView::OpenSelectedItems()
{
    HMENU hMenu;
    UINT uCommand;
    HRESULT hResult;

    m_cidl = m_ListView.GetSelectedCount();
    if (m_cidl == 0)
        return S_OK;

    hResult = OnDefaultCommand();
    if (hResult == S_OK)
        return hResult;

    hMenu = CreatePopupMenu();
    if (!hMenu) 
        return E_FAIL;

    hResult = GetItemObject(SVGIO_SELECTION, IID_PPV_ARG(IContextMenu, &m_pCM));
    if (FAILED_UNEXPECTEDLY(hResult))
        goto cleanup;

    IUnknown_SetSite(m_pCM, (IShellView *)this);

    hResult = m_pCM->QueryContextMenu(hMenu, 0, 0x20, 0x7fff, CMF_DEFAULTONLY);
    if (FAILED_UNEXPECTEDLY(hResult))
        goto cleanup;

    uCommand = GetMenuDefaultItem(hMenu, FALSE, 0);
    if (uCommand == (UINT)-1)
    {
        hResult = E_FAIL;
        goto cleanup;
    }

    InvokeContextMenuCommand(uCommand);

cleanup:
    
    if (hMenu)
        DestroyMenu(hMenu);

    if (m_pCM)
    {
        IUnknown_SetSite(m_pCM, NULL);
        m_pCM.Release();
    }

    return hResult;
}

/**********************************************************
 *    ShellView_DoContextMenu()
 */
LRESULT CDefView::OnContextMenu(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    WORD    x, y;
    UINT    uCommand;
    HRESULT hResult;

    x = LOWORD(lParam);
    y = HIWORD(lParam);

    TRACE("(%p)->(0x%08x 0x%08x) stub\n", this, x, y);

    m_hContextMenu = CreatePopupMenu();
    if (!m_hContextMenu) 
        return E_FAIL;

    m_cidl = m_ListView.GetSelectedCount();

    hResult = GetItemObject( m_cidl ? SVGIO_SELECTION : SVGIO_BACKGROUND, IID_PPV_ARG(IContextMenu, &m_pCM));
    if (FAILED_UNEXPECTEDLY(hResult))
        goto cleanup;

    IUnknown_SetSite(m_pCM, (IShellView *)this);

    hResult = m_pCM->QueryContextMenu(m_hContextMenu, 0, FCIDM_SHVIEWFIRST, FCIDM_SHVIEWLAST, CMF_NORMAL);
    if (FAILED_UNEXPECTEDLY(hResult))
        goto cleanup;

    uCommand = TrackPopupMenu(m_hContextMenu,
                              TPM_LEFTALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
                              x, y, 0, m_hWnd, NULL);
    if (uCommand == 0)
        goto cleanup;

    if (uCommand == FCIDM_SHVIEW_OPEN && OnDefaultCommand() == S_OK)
        goto cleanup;

    InvokeContextMenuCommand(uCommand);

cleanup:
    if (m_pCM)
    {
        IUnknown_SetSite(m_pCM, NULL);
        m_pCM.Release();
    }

    if (m_hContextMenu)
    {
        DestroyMenu(m_hContextMenu);
        m_hContextMenu = NULL;
    }

    return 0;
}

LRESULT CDefView::OnExplorerCommand(UINT uCommand, BOOL bUseSelection)
{
    HRESULT hResult;
    HMENU hMenu;

    hMenu = CreatePopupMenu();
    if (!hMenu) 
        return 0;

    hResult = GetItemObject( bUseSelection ? SVGIO_SELECTION : SVGIO_BACKGROUND, IID_PPV_ARG(IContextMenu, &m_pCM));
    if (FAILED_UNEXPECTEDLY( hResult))
        goto cleanup;

    IUnknown_SetSite(m_pCM, (IShellView *)this);

    hResult = m_pCM->QueryContextMenu(hMenu, 0, FCIDM_SHVIEWFIRST, FCIDM_SHVIEWLAST, CMF_NORMAL);
    if (FAILED_UNEXPECTEDLY( hResult))
        goto cleanup;

    InvokeContextMenuCommand(uCommand);

cleanup:
    if (m_pCM)
    {
        IUnknown_SetSite(m_pCM, NULL);
        m_pCM.Release();
    }

    if (hMenu)
        DestroyMenu(hMenu);

    return 0;
}

/**********************************************************
 *    ##### message handling #####
 */

/**********************************************************
*  ShellView_OnSize()
*/
LRESULT CDefView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    WORD wWidth, wHeight;

    wWidth  = LOWORD(lParam);
    wHeight = HIWORD(lParam);

    TRACE("%p width=%u height=%u\n", this, wWidth, wHeight);

    /* Resize the ListView to fit our window */
    if (m_ListView)
    {
        ::MoveWindow(m_ListView, 0, 0, wWidth, wHeight, TRUE);
    }

    return 0;
}

/**********************************************************
* ShellView_OnDeactivate()
*
* NOTES
*  internal
*/
void CDefView::OnDeactivate()
{
    TRACE("%p\n", this);

    if (m_uState != SVUIA_DEACTIVATE)
    {
        // TODO: cleanup menu after deactivation

        m_uState = SVUIA_DEACTIVATE;
    }
}

void CDefView::DoActivate(UINT uState)
{
    TRACE("%p uState=%x\n", this, uState);

    /*don't do anything if the state isn't really changing */
    if (m_uState == uState)
    {
        return;
    }

    if (uState == SVUIA_DEACTIVATE)
    {
        OnDeactivate();
    }
    else
    {
        if(m_hMenu && !m_bmenuBarInitialized)
        {
            FillEditMenu();
            FillViewMenu();
            m_pShellBrowser->SetMenuSB(m_hMenu, 0, m_hWnd);
            m_bmenuBarInitialized = TRUE;
        }

        if (SVUIA_ACTIVATE_FOCUS == uState)
        {
            m_ListView.SetFocus();
        }
    }

    m_uState = uState;
    TRACE("--\n");
}

/**********************************************************
* ShellView_OnActivate()
*/
LRESULT CDefView::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    DoActivate(SVUIA_ACTIVATE_FOCUS);
    return 0;
}

/**********************************************************
*  ShellView_OnSetFocus()
*
*/
LRESULT CDefView::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    TRACE("%p\n", this);

    /* Tell the browser one of our windows has received the focus. This
    should always be done before merging menus (OnActivate merges the
    menus) if one of our windows has the focus.*/

    m_pShellBrowser->OnViewWindowActive(this);
    DoActivate(SVUIA_ACTIVATE_FOCUS);

    /* Set the focus to the listview */
    m_ListView.SetFocus();

    /* Notify the ICommDlgBrowser interface */
    OnStateChange(CDBOSC_SETFOCUS);

    return 0;
}

/**********************************************************
* ShellView_OnKillFocus()
*/
LRESULT CDefView::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    TRACE("(%p) stub\n", this);

    DoActivate(SVUIA_ACTIVATE_NOFOCUS);
    /* Notify the ICommDlgBrowser */
    OnStateChange(CDBOSC_KILLFOCUS);

    return 0;
}

/**********************************************************
* ShellView_OnCommand()
*
* NOTES
*    the CmdID's are the ones from the context menu
*/
LRESULT CDefView::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    DWORD dwCmdID;
    DWORD dwCmd;
    HWND  hwndCmd;
    int   nCount; 

    dwCmdID = GET_WM_COMMAND_ID(wParam, lParam);
    dwCmd = GET_WM_COMMAND_CMD(wParam, lParam);
    hwndCmd = GET_WM_COMMAND_HWND(wParam, lParam);

    TRACE("(%p)->(0x%08x 0x%08x %p) stub\n", this, dwCmdID, dwCmd, hwndCmd);

    switch (dwCmdID)
    {
        case FCIDM_SHVIEW_SMALLICON:
            m_FolderSettings.ViewMode = FVM_SMALLICON;
            SetStyle (LVS_SMALLICON, LVS_TYPEMASK);
            CheckToolbar();
            break;

        case FCIDM_SHVIEW_BIGICON:
            m_FolderSettings.ViewMode = FVM_ICON;
            SetStyle (LVS_ICON, LVS_TYPEMASK);
            CheckToolbar();
            break;

        case FCIDM_SHVIEW_LISTVIEW:
            m_FolderSettings.ViewMode = FVM_LIST;
            SetStyle (LVS_LIST, LVS_TYPEMASK);
            CheckToolbar();
            break;

        case FCIDM_SHVIEW_REPORTVIEW:
            m_FolderSettings.ViewMode = FVM_DETAILS;
            SetStyle (LVS_REPORT, LVS_TYPEMASK);
            CheckToolbar();
            break;

            /* the menu-ID's for sorting are 0x30... see shrec.rc */
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
            m_sortInfo.nHeaderID = dwCmdID - 0x30;
            m_sortInfo.bIsAscending = TRUE;
            m_sortInfo.nLastHeaderID = m_sortInfo.nHeaderID;
            m_ListView.SortItems(ListViewCompareItems, this);
            break;

        case FCIDM_SHVIEW_SNAPTOGRID:
        case FCIDM_SHVIEW_AUTOARRANGE:
            //FIXME
            break;
        case FCIDM_SHVIEW_SELECTALL:
            m_ListView.SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
            break;

        case FCIDM_SHVIEW_INVERTSELECTION:
            nCount = m_ListView.GetItemCount();
            for (int i=0; i < nCount; i++)
                m_ListView.SetItemState(i, m_ListView.GetItemState(i, LVIS_SELECTED) ? 0 : LVIS_SELECTED, LVIS_SELECTED);
            break;

        case FCIDM_SHVIEW_REFRESH:
            Refresh();
            break;

        case FCIDM_SHVIEW_DELETE:
        case FCIDM_SHVIEW_CUT:
        case FCIDM_SHVIEW_COPY:
        case FCIDM_SHVIEW_RENAME:
            return OnExplorerCommand(dwCmdID, TRUE);

        case FCIDM_SHVIEW_INSERT:
        case FCIDM_SHVIEW_UNDO:
        case FCIDM_SHVIEW_INSERTLINK:
        case FCIDM_SHVIEW_NEWFOLDER:
            return OnExplorerCommand(dwCmdID, FALSE);
        default:
            /* WM_COMMAND messages from the file menu are routed to the CDefView so as to let m_pCM handle the command */
            if (m_pCM)
            {
                InvokeContextMenuCommand(dwCmdID);
            }
    }

    return 0;
}

/**********************************************************
* ShellView_OnNotify()
*/

LRESULT CDefView::OnNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    UINT                                CtlID;
    LPNMHDR                                lpnmh;
    LPNMLISTVIEW                        lpnmlv;
    NMLVDISPINFOW                        *lpdi;
    PCUITEMID_CHILD                     pidl;
    BOOL                                unused;

    CtlID = wParam;
    lpnmh = (LPNMHDR)lParam;
    lpnmlv = (LPNMLISTVIEW)lpnmh;
    lpdi = (NMLVDISPINFOW *)lpnmh;

    TRACE("%p CtlID=%u lpnmh->code=%x\n", this, CtlID, lpnmh->code);

    switch (lpnmh->code)
    {
        case NM_SETFOCUS:
            TRACE("-- NM_SETFOCUS %p\n", this);
            OnSetFocus(0, 0, 0, unused);
            break;

        case NM_KILLFOCUS:
            TRACE("-- NM_KILLFOCUS %p\n", this);
            OnDeactivate();
            /* Notify the ICommDlgBrowser interface */
            OnStateChange(CDBOSC_KILLFOCUS);
            break;

        case NM_CUSTOMDRAW:
            TRACE("-- NM_CUSTOMDRAW %p\n", this);
            return CDRF_DODEFAULT;

        case NM_RELEASEDCAPTURE:
            TRACE("-- NM_RELEASEDCAPTURE %p\n", this);
            break;

        case NM_CLICK:
            TRACE("-- NM_CLICK %p\n", this);
            break;

        case NM_RCLICK:
            TRACE("-- NM_RCLICK %p\n", this);
            break;

        case NM_DBLCLK:
            TRACE("-- NM_DBLCLK %p\n", this);
            OpenSelectedItems();
            break;

        case NM_RETURN:
            TRACE("-- NM_RETURN %p\n", this);
            OpenSelectedItems();
            break;

        case HDN_ENDTRACKW:
            TRACE("-- HDN_ENDTRACKW %p\n", this);
            /*nColumn1 = m_ListView.GetColumnWidth(0);
            nColumn2 = m_ListView.GetColumnWidth(1);*/
            break;

        case LVN_DELETEITEM:
            TRACE("-- LVN_DELETEITEM %p\n", this);

            /*delete the pidl because we made a copy of it*/
            SHFree(reinterpret_cast<LPVOID>(lpnmlv->lParam));

            break;

        case LVN_DELETEALLITEMS:
            TRACE("-- LVN_DELETEALLITEMS %p\n", this);
            return FALSE;

        case LVN_INSERTITEM:
            TRACE("-- LVN_INSERTITEM (STUB)%p\n", this);
            break;

        case LVN_ITEMACTIVATE:
            TRACE("-- LVN_ITEMACTIVATE %p\n", this);
            OnStateChange(CDBOSC_SELCHANGE);  /* the browser will get the IDataObject now */
            break;

        case LVN_COLUMNCLICK:
            m_sortInfo.nHeaderID = lpnmlv->iSubItem;
            if (m_sortInfo.nLastHeaderID == m_sortInfo.nHeaderID)
                m_sortInfo.bIsAscending = !m_sortInfo.bIsAscending;
            else
                m_sortInfo.bIsAscending = TRUE;
            m_sortInfo.nLastHeaderID = m_sortInfo.nHeaderID;

            m_ListView.SortItems(ListViewCompareItems, this);
            break;

        case LVN_GETDISPINFOA:
        case LVN_GETDISPINFOW:
            TRACE("-- LVN_GETDISPINFO %p\n", this);
            pidl = _PidlByItem(lpdi->item);

            if (lpdi->item.mask & LVIF_TEXT)    /* text requested */
            {
                if (m_pSF2Parent)
                {
                    SHELLDETAILS sd;
                    if (FAILED_UNEXPECTEDLY(m_pSF2Parent->GetDetailsOf(pidl, lpdi->item.iSubItem, &sd)))
                        break;

                    if (lpnmh->code == LVN_GETDISPINFOA)
                    {
                        /* shouldn't happen */
                        NMLVDISPINFOA *lpdiA = (NMLVDISPINFOA *)lpnmh;
                        StrRetToStrNA( lpdiA->item.pszText, lpdiA->item.cchTextMax, &sd.str, NULL);
                        TRACE("-- text=%s\n", lpdiA->item.pszText);
                    }
                    else /* LVN_GETDISPINFOW */
                    {
                        StrRetToStrNW( lpdi->item.pszText, lpdi->item.cchTextMax, &sd.str, NULL);
                        TRACE("-- text=%s\n", debugstr_w(lpdi->item.pszText));
                    }
                }
                else
                {
                    FIXME("no m_pSF2Parent\n");
                }
            }
            if(lpdi->item.mask & LVIF_IMAGE)    /* image requested */
            {
                lpdi->item.iImage = SHMapPIDLToSystemImageListIndex(m_pSFParent, pidl, 0);
            }
            if(lpdi->item.mask & LVIF_STATE)
            {
                ULONG attributes = SFGAO_HIDDEN;
                if (SUCCEEDED(m_pSFParent->GetAttributesOf(1, &pidl, &attributes)))
                {
                    if (attributes & SFGAO_HIDDEN)
                    {
                        lpdi->item.state |= LVIS_CUT;
                    }
                }
            }
            lpdi->item.mask |= LVIF_DI_SETITEM;
            break;

        case LVN_ITEMCHANGED:
            TRACE("-- LVN_ITEMCHANGED %p\n", this);
            OnStateChange(CDBOSC_SELCHANGE);  /* the browser will get the IDataObject now */
            UpdateStatusbar();
            break;

        case LVN_BEGINDRAG:
        case LVN_BEGINRDRAG:
            TRACE("-- LVN_BEGINDRAG\n");

            if (GetSelections())
            {
                CComPtr<IDataObject> pda;
                DWORD dwAttributes = SFGAO_CANLINK;
                DWORD dwEffect = DROPEFFECT_COPY | DROPEFFECT_MOVE;

                if (SUCCEEDED(m_pSFParent->GetUIObjectOf(m_hWnd, m_cidl, m_apidl, IID_NULL_PPV_ARG(IDataObject, &pda))))
                {
                    LPNMLISTVIEW params = (LPNMLISTVIEW)lParam;

                    if (SUCCEEDED(m_pSFParent->GetAttributesOf(m_cidl, m_apidl, &dwAttributes)))
                    {
                        if (dwAttributes & SFGAO_CANLINK)
                        {
                            dwEffect |= DROPEFFECT_LINK;
                        }
                    }
                    
                    CComPtr<IAsyncOperation> piaso;
                    if (SUCCEEDED(pda->QueryInterface(IID_PPV_ARG(IAsyncOperation, &piaso))))
                    {
                        piaso->SetAsyncMode(TRUE);
                    }

                    DWORD dwEffect2;

                    m_pSourceDataObject = pda;
                    m_ptFirstMousePos = params->ptAction;
                    ClientToScreen(&m_ptFirstMousePos);

                    HIMAGELIST big_icons, small_icons;
                    Shell_GetImageLists(&big_icons, &small_icons);
                    PCUITEMID_CHILD pidl = _PidlByItem(params->iItem);
                    int iIcon = SHMapPIDLToSystemImageListIndex(m_pSFParent, pidl, 0);
                    POINT ptItem;
                    m_ListView.GetItemPosition(params->iItem, &ptItem);

                    ImageList_BeginDrag(big_icons, iIcon, params->ptAction.x - ptItem.x, params->ptAction.y - ptItem.y);

                    DoDragDrop(pda, this, dwEffect, &dwEffect2);

                    m_pSourceDataObject.Release();
                }
            }
            break;

        case LVN_BEGINLABELEDITW:
        {
            DWORD dwAttr = SFGAO_CANRENAME;
            pidl = _PidlByItem(lpdi->item);

            TRACE("-- LVN_BEGINLABELEDITW %p\n", this);

            m_pSFParent->GetAttributesOf(1, &pidl, &dwAttr);
            if (SFGAO_CANRENAME & dwAttr)
            {
                m_isEditing = TRUE;
                return FALSE;
            }
            return TRUE;
        }

        case LVN_ENDLABELEDITW:
        {
            TRACE("-- LVN_ENDLABELEDITW %p\n", this);

            m_isEditing = FALSE;

            if (lpdi->item.pszText)
            {
                HRESULT hr;
                LVITEMW lvItem;

                pidl = _PidlByItem(lpdi->item);
                PITEMID_CHILD pidlNew;
                hr = m_pSFParent->SetNameOf(0, pidl, lpdi->item.pszText, SHGDN_INFOLDER, &pidlNew);

                if (SUCCEEDED(hr) && pidlNew)
                {
                    lvItem.mask = LVIF_PARAM|LVIF_IMAGE;
                    lvItem.iItem = lpdi->item.iItem;
                    lvItem.iSubItem = 0;
                    lvItem.lParam = reinterpret_cast<LPARAM>(pidlNew);
                    lvItem.iImage = SHMapPIDLToSystemImageListIndex(m_pSFParent, pidlNew, 0);
                    m_ListView.SetItem(&lvItem);
                    m_ListView.Update(lpdi->item.iItem);
                    return TRUE;
                }
            }

            return FALSE;
        }

        default:
            TRACE("-- %p WM_COMMAND %x unhandled\n", this, lpnmh->code);
            break;
    }

    return 0;
}

/*
 * This is just a quick hack to make the desktop work correctly.
 * ITranslateShellChangeNotify's IsChildID is undocumented, but most likely the way that
 * a folder should know if it should update upon a change notification.
 * It is exported by merged folders at a minimum.
 */
static BOOL ILIsParentOrSpecialParent(PCIDLIST_ABSOLUTE pidl1, PCIDLIST_ABSOLUTE pidl2)
{
    if (!pidl1 || !pidl2)
        return FALSE;
    if (ILIsParent(pidl1, pidl2, TRUE))
        return TRUE;

    if (_ILIsDesktop(pidl1))
    {
        PIDLIST_ABSOLUTE deskpidl;
        SHGetFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, &deskpidl);
        if (ILIsParent(deskpidl, pidl2, TRUE))
        {
            ILFree(deskpidl);
            return TRUE;
        }
        ILFree(deskpidl);
        SHGetFolderLocation(NULL, CSIDL_COMMON_DESKTOPDIRECTORY, NULL, 0, &deskpidl);
        if (ILIsParent(deskpidl, pidl2, TRUE))
        {
            ILFree(deskpidl);
            return TRUE;
        }
        ILFree(deskpidl);
    }
    return FALSE;
}

/**********************************************************
* ShellView_OnChange()
*/
LRESULT CDefView::OnChangeNotify(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    PCIDLIST_ABSOLUTE *Pidls = reinterpret_cast<PCIDLIST_ABSOLUTE*>(wParam);
    BOOL bParent0 = ILIsParentOrSpecialParent(m_pidlParent, Pidls[0]);
    BOOL bParent1 = ILIsParentOrSpecialParent(m_pidlParent, Pidls[1]);

    TRACE("(%p)(%p,%p,0x%08x)\n", this, Pidls[0], Pidls[1], lParam);

    switch (lParam &~ SHCNE_INTERRUPT)
    {
        case SHCNE_MKDIR:
        case SHCNE_CREATE:
            if (bParent0)
            {
                if (LV_FindItemByPidl(ILFindLastID(Pidls[0])) == -1)
                {
                    LV_AddItem(ILFindLastID(Pidls[0]));
                }
                else
                {
                    LV_ProdItem(ILFindLastID(Pidls[0]));
                }
            }
            break;

        case SHCNE_RMDIR:
        case SHCNE_DELETE:
            if (bParent0)
                LV_DeleteItem(ILFindLastID(Pidls[0]));
            break;

        case SHCNE_RENAMEFOLDER:
        case SHCNE_RENAMEITEM:
            if (bParent0 && bParent1)
                LV_RenameItem(ILFindLastID(Pidls[0]), ILFindLastID(Pidls[1]));
            else if (bParent0)
                LV_DeleteItem(ILFindLastID(Pidls[0]));
            else if (bParent1)
                LV_AddItem(ILFindLastID(Pidls[1]));
            break;

        case SHCNE_UPDATEITEM:
            if (bParent0)
                LV_RenameItem(ILFindLastID(Pidls[0]), ILFindLastID(Pidls[0]));
            break;

        case SHCNE_UPDATEDIR:
            Refresh();
            break;
    }
    return TRUE;
}

/**********************************************************
*  CDefView::OnCustomItem
*/
LRESULT CDefView::OnCustomItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    if (!m_pCM.p)
    {
        /* no menu */
        ERR("no menu!!!\n");
        return FALSE;
    }

    LRESULT result;
    HRESULT hres = SHForwardContextMenuMsg(m_pCM, uMsg, wParam, lParam, &result, TRUE);
    if (SUCCEEDED(hres))
        return TRUE;
    else
        return FALSE;
}

LRESULT CDefView::OnSettingChange(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    /* Wallpaper setting affects drop shadows effect */
    if (wParam == SPI_SETDESKWALLPAPER || wParam == 0)
        UpdateListColors();

    return S_OK;
}

/**********************************************************
*  CDefView::OnInitMenuPopup
*/
LRESULT CDefView::OnInitMenuPopup(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    HMENU hmenu = (HMENU) wParam;
    int nPos = LOWORD(lParam);
    UINT  menuItemId;

    OnCustomItem(uMsg, wParam, lParam, bHandled);

    HMENU hViewMenu = GetSubmenuByID(m_hMenu, FCIDM_MENU_VIEW);

    /* Lets try to find out what the hell wParam is */
    if (hmenu == GetSubMenu(m_hMenu, nPos))
        menuItemId = ReallyGetMenuItemID(m_hMenu, nPos);
    else if (hViewMenu && hmenu == GetSubMenu(hViewMenu, nPos))
        menuItemId = ReallyGetMenuItemID(hViewMenu, nPos);
    else if (m_hContextMenu && hmenu == GetSubMenu(m_hContextMenu, nPos))
        menuItemId = ReallyGetMenuItemID(m_hContextMenu, nPos);
    else
        return FALSE;

    switch (menuItemId)
    {
    case FCIDM_MENU_FILE:
        FillFileMenu();
        break;
    case FCIDM_MENU_VIEW:
    case FCIDM_SHVIEW_VIEW:
        CheckViewMode(hmenu);
        break;
    case FCIDM_SHVIEW_ARRANGE:
        FillArrangeAsMenu(hmenu);
        break;
    }

    return FALSE;
}

/**********************************************************
*
*
*  The INTERFACE of the IShellView object
*
*
**********************************************************
*/

/**********************************************************
*  ShellView_GetWindow
*/
HRESULT WINAPI CDefView::GetWindow(HWND *phWnd)
{
    TRACE("(%p)\n", this);

    *phWnd = m_hWnd;

    return S_OK;
}

HRESULT WINAPI CDefView::ContextSensitiveHelp(BOOL fEnterMode)
{
    FIXME("(%p) stub\n", this);

    return E_NOTIMPL;
}

/**********************************************************
* IShellView_TranslateAccelerator
*
* FIXME:
*  use the accel functions
*/
HRESULT WINAPI CDefView::TranslateAccelerator(LPMSG lpmsg)
{
    if (m_isEditing)
        return S_FALSE;

    if (lpmsg->message >= WM_KEYFIRST && lpmsg->message <= WM_KEYLAST)
    {
        if (::TranslateAcceleratorW(m_hWnd, m_hAccel, lpmsg) != 0)
            return S_OK;

        TRACE("-- key=0x04%lx\n", lpmsg->wParam) ;
    }

    return m_pShellBrowser->TranslateAcceleratorSB(lpmsg, 0);
}

HRESULT WINAPI CDefView::EnableModeless(BOOL fEnable)
{
    FIXME("(%p) stub\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::UIActivate(UINT uState)
{
    // CHAR szName[MAX_PATH];
    LRESULT lResult;
    int nPartArray[1] = { -1};

    TRACE("(%p)->(state=%x) stub\n", this, uState);

    /* don't do anything if the state isn't really changing */
    if (m_uState == uState)
    {
        return S_OK;
    }

    /* OnActivate handles the menu merging and internal state */
    DoActivate(uState);

    /* only do This if we are active */
    if (uState != SVUIA_DEACTIVATE)
    {

        /*
            GetFolderPath is not a method of IShellFolder
            IShellFolder_GetFolderPath( m_pSFParent, szName, sizeof(szName) );
        */
        /* set the number of parts */
        m_pShellBrowser->SendControlMsg(FCW_STATUS, SB_SETPARTS, 1, (LPARAM)nPartArray, &lResult);

        /* set the text for the parts */
        /*
            m_pShellBrowser->SendControlMsg(FCW_STATUS, SB_SETTEXTA, 0, (LPARAM)szName, &lResult);
        */
    }

    return S_OK;
}

HRESULT WINAPI CDefView::Refresh()
{
    TRACE("(%p)\n", this);

    m_ListView.DeleteAllItems();
    FillList();

    return S_OK;
}

HRESULT WINAPI CDefView::CreateViewWindow(IShellView *lpPrevView, LPCFOLDERSETTINGS lpfs, IShellBrowser *psb, RECT *prcView, HWND *phWnd)
{
    return CreateViewWindow3(psb, lpPrevView, SV3CVW3_DEFAULT,
        (FOLDERFLAGS)lpfs->fFlags, (FOLDERFLAGS)lpfs->fFlags, (FOLDERVIEWMODE)lpfs->ViewMode, NULL, prcView, phWnd);
}

HRESULT WINAPI CDefView::DestroyViewWindow()
{
    TRACE("(%p)\n", this);

    /* Make absolutely sure all our UI is cleaned up */
    UIActivate(SVUIA_DEACTIVATE);

    if (m_hAccel)
    {
        // "Accelerator tables loaded from resources are freed automatically when the application terminates." -- MSDN
        m_hAccel = NULL;
    }

    if (m_hMenuViewModes)
    {
        DestroyMenu(m_hMenuViewModes);
        m_hMenuViewModes = NULL;
    }

    if (m_hMenu)
    {
        DestroyMenu(m_hMenu);
        m_hMenu = NULL;
    }

    if (m_ListView)
    {
        m_ListView.DestroyWindow();
    }

    if (m_hWnd)
    {
        DestroyWindow();
    }

    m_pShellBrowser.Release();
    m_pCommDlgBrowser.Release();

    return S_OK;
}

HRESULT WINAPI CDefView::GetCurrentInfo(LPFOLDERSETTINGS lpfs)
{
    TRACE("(%p)->(%p) vmode=%x flags=%x\n", this, lpfs,
          m_FolderSettings.ViewMode, m_FolderSettings.fFlags);

    if (!lpfs)
        return E_INVALIDARG;

    *lpfs = m_FolderSettings;
    return S_OK;
}

HRESULT WINAPI CDefView::AddPropertySheetPages(DWORD dwReserved, LPFNADDPROPSHEETPAGE lpfn, LPARAM lparam)
{
    FIXME("(%p) stub\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::SaveViewState()
{
    FIXME("(%p) stub\n", this);

    return S_OK;
}

HRESULT WINAPI CDefView::SelectItem(PCUITEMID_CHILD pidl, UINT uFlags)
{
    int i;

    TRACE("(%p)->(pidl=%p, 0x%08x) stub\n", this, pidl, uFlags);

    i = LV_FindItemByPidl(pidl);
    if (i == -1)
        return S_OK;

    if(uFlags & SVSI_ENSUREVISIBLE)
        m_ListView.EnsureVisible(i, FALSE);

    LVITEMW lvItem = {0};
    lvItem.mask = LVIF_STATE;
    lvItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;

    while (m_ListView.GetItem(&lvItem))
    {
        if (lvItem.iItem == i)
        {
            if (uFlags & SVSI_SELECT)
                lvItem.state |= LVIS_SELECTED;
            else
                lvItem.state &= ~LVIS_SELECTED;

            if (uFlags & SVSI_FOCUSED)
                lvItem.state &= ~LVIS_FOCUSED;
        }
        else
        {
            if (uFlags & SVSI_DESELECTOTHERS)
                lvItem.state &= ~LVIS_SELECTED;
        }

        m_ListView.SetItem(&lvItem);
        lvItem.iItem++;
    }

    if((uFlags & SVSI_EDIT) == SVSI_EDIT)
        m_ListView.EditLabel(i);

    return S_OK;
}

HRESULT WINAPI CDefView::GetItemObject(UINT uItem, REFIID riid, LPVOID *ppvOut)
{
    HRESULT hr = E_NOINTERFACE;

    TRACE("(%p)->(uItem=0x%08x,\n\tIID=%s, ppv=%p)\n", this, uItem, debugstr_guid(&riid), ppvOut);

    *ppvOut = NULL;

    switch (uItem)
    {
        case SVGIO_BACKGROUND:
            if (IsEqualIID(riid, IID_IContextMenu))
            {
                if (!ppvOut)
                    hr = E_OUTOFMEMORY;

                hr = CDefViewBckgrndMenu_CreateInstance(m_pSF2Parent, riid, ppvOut);
                if (FAILED_UNEXPECTEDLY(hr))
                    return hr;

            }
            else if (IsEqualIID(riid, IID_IDispatch))
            {
                if (m_pShellFolderViewDual == NULL)
                {
                    hr = CDefViewDual_Constructor(riid, (LPVOID*)&m_pShellFolderViewDual);
                    if (FAILED_UNEXPECTEDLY(hr))
                        return hr;
                }
                hr = m_pShellFolderViewDual->QueryInterface(riid, ppvOut);
            }
            break;

        case SVGIO_SELECTION:
            GetSelections();
            hr = m_pSFParent->GetUIObjectOf(m_hWnd, m_cidl, m_apidl, riid, 0, ppvOut);
            if (FAILED_UNEXPECTEDLY(hr))
                return hr;
            break;
    }

    TRACE("-- (%p)->(interface=%p)\n", this, *ppvOut);

    return hr;
}

HRESULT STDMETHODCALLTYPE CDefView::GetCurrentViewMode(UINT *pViewMode)
{
    TRACE("(%p)->(%p), stub\n", this, pViewMode);

    if (!pViewMode)
        return E_INVALIDARG;

    *pViewMode = m_FolderSettings.ViewMode;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::SetCurrentViewMode(UINT ViewMode)
{
    DWORD dwStyle;
    TRACE("(%p)->(%u), stub\n", this, ViewMode);

    /* It's not redundant to check FVM_AUTO because it's a (UINT)-1 */
    if (((INT)ViewMode < FVM_FIRST || (INT)ViewMode > FVM_LAST) && ((INT)ViewMode != FVM_AUTO))
        return E_INVALIDARG;

    /* Windows before Vista uses LVM_SETVIEW and possibly
       LVM_SETEXTENDEDLISTVIEWSTYLE to set the style of the listview,
       while later versions seem to accomplish this through other
       means. */
    switch (ViewMode)
    {
        case FVM_ICON:
            dwStyle = LVS_ICON;
            break;
        case FVM_DETAILS:
            dwStyle = LVS_REPORT;
            break;
        case FVM_SMALLICON:
            dwStyle = LVS_SMALLICON;
            break;
        case FVM_LIST:
            dwStyle = LVS_LIST;
            break;
        default:
        {
            FIXME("ViewMode %d not implemented\n", ViewMode);
            dwStyle = LVS_LIST;
            break;
        }
    }

    SetStyle(dwStyle, LVS_TYPEMASK);

    /* This will not necessarily be the actual mode set above.
       This mimics the behavior of Windows XP. */
    m_FolderSettings.ViewMode = ViewMode;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetFolder(REFIID riid, void **ppv)
{
    if (m_pSFParent == NULL)
        return E_FAIL;

    return m_pSFParent->QueryInterface(riid, ppv);
}

HRESULT STDMETHODCALLTYPE CDefView::Item(int iItemIndex, PITEMID_CHILD *ppidl)
{
    PCUITEMID_CHILD pidl = _PidlByItem(iItemIndex);
    if (pidl)
    {
        *ppidl = ILClone(pidl);
        return S_OK;
    }

    *ppidl = 0;
    return E_INVALIDARG;
}

HRESULT STDMETHODCALLTYPE CDefView::ItemCount(UINT uFlags, int *pcItems)
{
    TRACE("(%p)->(%u %p)\n", this, uFlags, pcItems);

    if (uFlags != SVGIO_ALLVIEW)
        FIXME("some flags unsupported, %x\n", uFlags & ~SVGIO_ALLVIEW);

    *pcItems = m_ListView.GetItemCount();

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::Items(UINT uFlags, REFIID riid, void **ppv)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetSelectionMarkedItem(int *piItem)
{
    TRACE("(%p)->(%p)\n", this, piItem);

    *piItem = m_ListView.GetSelectionMark();

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetFocusedItem(int *piItem)
{
    TRACE("(%p)->(%p)\n", this, piItem);

    *piItem = m_ListView.GetNextItem(-1, LVNI_FOCUSED);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetItemPosition(PCUITEMID_CHILD pidl, POINT *ppt)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetSpacing(POINT *ppt)
{
    TRACE("(%p)->(%p)\n", this, ppt);

    if (!m_ListView) 
        return S_FALSE;

    if (ppt)
    {
        SIZE spacing;
        m_ListView.GetItemSpacing(spacing);

        ppt->x = spacing.cx;
        ppt->y = spacing.cy;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetDefaultSpacing(POINT *ppt)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetAutoArrange()
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SelectItem(int iItem, DWORD dwFlags)
{
    LVITEMW lvItem;

    TRACE("(%p)->(%d, %x)\n", this, iItem, dwFlags);

    lvItem.state = 0;
    lvItem.stateMask = LVIS_SELECTED;

    if (dwFlags & SVSI_ENSUREVISIBLE)
        m_ListView.EnsureVisible(iItem, 0);

    /* all items */
    if (dwFlags & SVSI_DESELECTOTHERS)
        m_ListView.SetItemState(-1, 0, LVIS_SELECTED);

    /* this item */
    if (dwFlags & SVSI_SELECT)
        lvItem.state |= LVIS_SELECTED;

    if (dwFlags & SVSI_FOCUSED)
        lvItem.stateMask |= LVIS_FOCUSED;

    m_ListView.SetItemState(iItem, lvItem.state, lvItem.stateMask);

    if ((dwFlags & SVSI_EDIT) == SVSI_EDIT)
        m_ListView.EditLabel(iItem);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::SelectAndPositionItems(UINT cidl, PCUITEMID_CHILD_ARRAY apidl, POINT *apt, DWORD dwFlags)
{
    return E_NOTIMPL;
}

/**********************************************************
 * IShellView2 implementation
 */

HRESULT STDMETHODCALLTYPE CDefView::GetView(SHELLVIEWID *view_guid, ULONG view_type)
{
    FIXME("(%p)->(%p, %lu) stub\n", this, view_guid, view_type);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::CreateViewWindow2(LPSV2CVW2_PARAMS view_params)
{
    return CreateViewWindow3(view_params->psbOwner, view_params->psvPrev,
        SV3CVW3_DEFAULT, (FOLDERFLAGS)view_params->pfs->fFlags, (FOLDERFLAGS)view_params->pfs->fFlags,
        (FOLDERVIEWMODE)view_params->pfs->ViewMode, view_params->pvid, view_params->prcView, &view_params->hwndView);
}

HRESULT STDMETHODCALLTYPE CDefView::CreateViewWindow3(IShellBrowser *psb, IShellView *psvPrevious, SV3CVW3_FLAGS view_flags, FOLDERFLAGS mask, FOLDERFLAGS flags, FOLDERVIEWMODE mode, const SHELLVIEWID *view_id, RECT *prcView, HWND *hwnd)
{
    OLEMENUGROUPWIDTHS omw = { { 0, 0, 0, 0, 0, 0 } };

    *hwnd = NULL;

    TRACE("(%p)->(shlview=%p shlbrs=%p rec=%p hwnd=%p vmode=%x flags=%x)\n", this, psvPrevious, psb, prcView, hwnd, mode, flags);
    if (prcView != NULL)
        TRACE("-- left=%i top=%i right=%i bottom=%i\n", prcView->left, prcView->top, prcView->right, prcView->bottom);
    
    /* Validate the Shell Browser */
    if (psb == NULL || m_hWnd)
        return E_UNEXPECTED;

    if (view_flags != SV3CVW3_DEFAULT)
        FIXME("unsupported view flags 0x%08x\n", view_flags);

    /* Set up the member variables */
    m_pShellBrowser = psb;
    m_FolderSettings.ViewMode = mode;
    m_FolderSettings.fFlags = mask & flags;

    if (view_id)
    {
        if (IsEqualIID(*view_id, VID_LargeIcons))
            m_FolderSettings.ViewMode = FVM_ICON;
        else if (IsEqualIID(*view_id, VID_SmallIcons))
            m_FolderSettings.ViewMode = FVM_SMALLICON;
        else if (IsEqualIID(*view_id, VID_List))
            m_FolderSettings.ViewMode = FVM_LIST;
        else if (IsEqualIID(*view_id, VID_Details))
            m_FolderSettings.ViewMode = FVM_DETAILS;
        else if (IsEqualIID(*view_id, VID_Thumbnails))
            m_FolderSettings.ViewMode = FVM_THUMBNAIL;
        else if (IsEqualIID(*view_id, VID_Tile))
            m_FolderSettings.ViewMode = FVM_TILE;
        else if (IsEqualIID(*view_id, VID_ThumbStrip))
            m_FolderSettings.ViewMode = FVM_THUMBSTRIP;
        else
            FIXME("Ignoring unrecognized VID %s\n", debugstr_guid(view_id));
    }

    /* Get our parent window */
    m_pShellBrowser->GetWindow(&m_hWndParent);

    /* Try to get the ICommDlgBrowserInterface, adds a reference !!! */
    m_pCommDlgBrowser = NULL;
    if (SUCCEEDED(m_pShellBrowser->QueryInterface(IID_PPV_ARG(ICommDlgBrowser, &m_pCommDlgBrowser))))
    {
        TRACE("-- CommDlgBrowser\n");
    }

    Create(m_hWndParent, prcView, NULL, WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_TABSTOP, 0, 0U);
    if (m_hWnd == NULL)
        return E_FAIL;

    *hwnd = m_hWnd;

    CheckToolbar();

    if (!*hwnd)
        return E_FAIL;

    SetWindowPos(HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    UpdateWindow();

    if (!m_hMenu)
    {
        m_hMenu = CreateMenu();
        m_pShellBrowser->InsertMenusSB(m_hMenu, &omw);
        TRACE("-- after fnInsertMenusSB\n");
    }

    _MergeToolbar();

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::HandleRename(LPCITEMIDLIST new_pidl)
{
    FIXME("(%p)->(%p) stub\n", this, new_pidl);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SelectAndPositionItem(LPCITEMIDLIST item, UINT flags, POINT *point)
{
    FIXME("(%p)->(%p, %u, %p) stub\n", this, item, flags, point);
    return E_NOTIMPL;
}

/**********************************************************
 * IShellFolderView implementation
 */
HRESULT STDMETHODCALLTYPE CDefView::Rearrange(LPARAM sort)
{
    FIXME("(%p)->(%ld) stub\n", this, sort);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetArrangeParam(LPARAM *sort)
{
    FIXME("(%p)->(%p) stub\n", this, sort);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::ArrangeGrid()
{
    FIXME("(%p) stub\n", this);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::AutoArrange()
{
    FIXME("(%p) stub\n", this);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::AddObject(PITEMID_CHILD pidl, UINT *item)
{
    FIXME("(%p)->(%p %p) stub\n", this, pidl, item);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetObject(PITEMID_CHILD *pidl, UINT item)
{
    TRACE("(%p)->(%p %d)\n", this, pidl, item);
    return Item(item, pidl);
}

HRESULT STDMETHODCALLTYPE CDefView::RemoveObject(PITEMID_CHILD pidl, UINT *item)
{

    TRACE("(%p)->(%p %p)\n", this, pidl, item);

    if (pidl)
    {
        *item = LV_FindItemByPidl(ILFindLastID(pidl));
        m_ListView.DeleteItem(*item);
    }
    else
    {
        *item = 0;
        m_ListView.DeleteAllItems();
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetObjectCount(UINT *count)
{
    TRACE("(%p)->(%p)\n", this, count);
    *count = m_ListView.GetItemCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::SetObjectCount(UINT count, UINT flags)
{
    FIXME("(%p)->(%d %x) stub\n", this, count, flags);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::UpdateObject(PITEMID_CHILD pidl_old, PITEMID_CHILD pidl_new, UINT *item)
{
    FIXME("(%p)->(%p %p %p) stub\n", this, pidl_old, pidl_new, item);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::RefreshObject(PITEMID_CHILD pidl, UINT *item)
{
    FIXME("(%p)->(%p %p) stub\n", this, pidl, item);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SetRedraw(BOOL redraw)
{
    TRACE("(%p)->(%d)\n", this, redraw);
    m_ListView.SetRedraw(redraw);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::GetSelectedCount(UINT *count)
{
    FIXME("(%p)->(%p) stub\n", this, count);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetSelectedObjects(PCUITEMID_CHILD **pidl, UINT *items)
{
    TRACE("(%p)->(%p %p)\n", this, pidl, items);

    *items = GetSelections();

    if (*items)
    {
        *pidl = static_cast<PCUITEMID_CHILD *>(LocalAlloc(0, *items * sizeof(PCUITEMID_CHILD)));
        if (!*pidl)
        {
            return E_OUTOFMEMORY;
        }
        
        /* it's documented that caller shouldn't PIDLs, only array itself */
        memcpy(*pidl, m_apidl, *items * sizeof(PCUITEMID_CHILD));
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::IsDropOnSource(IDropTarget *drop_target)
{
    FIXME("(%p)->(%p) stub\n", this, drop_target);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetDragPoint(POINT *pt)
{
    FIXME("(%p)->(%p) stub\n", this, pt);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetDropPoint(POINT *pt)
{
    FIXME("(%p)->(%p) stub\n", this, pt);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::MoveIcons(IDataObject *obj)
{
    TRACE("(%p)->(%p)\n", this, obj);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SetItemPos(PCUITEMID_CHILD pidl, POINT *pt)
{
    FIXME("(%p)->(%p %p) stub\n", this, pidl, pt);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::IsBkDropTarget(IDropTarget *drop_target)
{
    FIXME("(%p)->(%p) stub\n", this, drop_target);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SetClipboard(BOOL move)
{
    FIXME("(%p)->(%d) stub\n", this, move);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SetPoints(IDataObject *obj)
{
    FIXME("(%p)->(%p) stub\n", this, obj);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::GetItemSpacing(ITEMSPACING *spacing)
{
    FIXME("(%p)->(%p) stub\n", this, spacing);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::SetCallback(IShellFolderViewCB  *new_cb, IShellFolderViewCB **old_cb)
{
    FIXME("(%p)->(%p %p) stub\n", this, new_cb, old_cb);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::Select(UINT flags)
{
    FIXME("(%p)->(%d) stub\n", this, flags);
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CDefView::QuerySupport(UINT *support)
{
    TRACE("(%p)->(%p)\n", this, support);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::SetAutomationObject(IDispatch *disp)
{
    FIXME("(%p)->(%p) stub\n", this, disp);
    return E_NOTIMPL;
}
 
/**********************************************************
 * ISVOleCmdTarget_QueryStatus (IOleCommandTarget)
 */
HRESULT WINAPI CDefView::QueryStatus(const GUID *pguidCmdGroup, ULONG cCmds, OLECMD *prgCmds, OLECMDTEXT *pCmdText)
{
    FIXME("(%p)->(%p(%s) 0x%08x %p %p\n",
          this, pguidCmdGroup, debugstr_guid(pguidCmdGroup), cCmds, prgCmds, pCmdText);

    if (!prgCmds)
        return E_INVALIDARG;

    for (UINT i = 0; i < cCmds; i++)
    {
        FIXME("\tprgCmds[%d].cmdID = %d\n", i, prgCmds[i].cmdID);
        prgCmds[i].cmdf = 0;
    }

    return OLECMDERR_E_UNKNOWNGROUP;
}

/**********************************************************
 * ISVOleCmdTarget_Exec (IOleCommandTarget)
 *
 * nCmdID is the OLECMDID_* enumeration
 */
HRESULT WINAPI CDefView::Exec(const GUID *pguidCmdGroup, DWORD nCmdID, DWORD nCmdexecopt, VARIANT *pvaIn, VARIANT *pvaOut)
{
    FIXME("(%p)->(\n\tTarget GUID:%s Command:0x%08x Opt:0x%08x %p %p)\n",
          this, debugstr_guid(pguidCmdGroup), nCmdID, nCmdexecopt, pvaIn, pvaOut);

    if (!pguidCmdGroup)
        return OLECMDERR_E_UNKNOWNGROUP;

    if (IsEqualCLSID(*pguidCmdGroup, m_Category))
    {
        if (nCmdID == FCIDM_SHVIEW_AUTOARRANGE)
        {
            if (V_VT(pvaIn) != VT_INT_PTR)
                return OLECMDERR_E_NOTSUPPORTED;

            TPMPARAMS params;
            params.cbSize = sizeof(params);
            params.rcExclude = *(RECT*) V_INTREF(pvaIn);

            if (m_hMenuViewModes)
            {
                /* Duplicate all but the last two items of the view modes menu */
                HMENU hmenuViewPopup = CreatePopupMenu();
                Shell_MergeMenus(hmenuViewPopup, m_hMenuViewModes, 0, 0, 0xFFFF, 0);
                DeleteMenu(hmenuViewPopup, GetMenuItemCount(hmenuViewPopup) - 1, MF_BYPOSITION);
                DeleteMenu(hmenuViewPopup, GetMenuItemCount(hmenuViewPopup) - 1, MF_BYPOSITION);
                CheckViewMode(hmenuViewPopup);
                TrackPopupMenuEx(hmenuViewPopup, TPM_LEFTALIGN | TPM_TOPALIGN, params.rcExclude.left, params.rcExclude.bottom, m_hWndParent, &params);
                ::DestroyMenu(hmenuViewPopup);
            }

            // pvaOut is VT_I4 with value 0x403 (cmd id of the new mode maybe?)
            V_VT(pvaOut) = VT_I4;
            V_I4(pvaOut) = 0x403;
        }
    }

    if (IsEqualIID(*pguidCmdGroup, CGID_Explorer) &&
            (nCmdID == 0x29) &&
            (nCmdexecopt == 4) && pvaOut)
        return S_OK;

    if (IsEqualIID(*pguidCmdGroup, CGID_ShellDocView) &&
            (nCmdID == 9) &&
            (nCmdexecopt == 0))
        return 1;

    return OLECMDERR_E_UNKNOWNGROUP;
}

/**********************************************************
 * ISVDropTarget implementation
 */

/******************************************************************************
 * drag_notify_subitem [Internal]
 *
 * Figure out the shellfolder object, which is currently under the mouse cursor
 * and notify it via the IDropTarget interface.
 */

#define SCROLLAREAWIDTH 20

HRESULT CDefView::drag_notify_subitem(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    LONG lResult;
    HRESULT hr;
    RECT clientRect;

    /* Map from global to client coordinates and query the index of the listview-item, which is
     * currently under the mouse cursor. */
    LVHITTESTINFO htinfo = {{pt.x, pt.y}, LVHT_ONITEM};
    ScreenToClient(&htinfo.pt);
    lResult = m_ListView.HitTest(&htinfo);

    /* Send WM_*SCROLL messages every 250 ms during drag-scrolling */
    ::GetClientRect(m_ListView, &clientRect);
    if (htinfo.pt.x == m_ptLastMousePos.x && htinfo.pt.y == m_ptLastMousePos.y &&
            (htinfo.pt.x < SCROLLAREAWIDTH || htinfo.pt.x > clientRect.right - SCROLLAREAWIDTH ||
             htinfo.pt.y < SCROLLAREAWIDTH || htinfo.pt.y > clientRect.bottom - SCROLLAREAWIDTH ))
    {
        m_cScrollDelay = (m_cScrollDelay + 1) % 5; /* DragOver is called every 50 ms */
        if (m_cScrollDelay == 0)
        {
            /* Mouse did hover another 250 ms over the scroll-area */
            if (htinfo.pt.x < SCROLLAREAWIDTH)
                m_ListView.SendMessageW(WM_HSCROLL, SB_LINEUP, 0);

            if (htinfo.pt.x > clientRect.right - SCROLLAREAWIDTH)
                m_ListView.SendMessageW(WM_HSCROLL, SB_LINEDOWN, 0);

            if (htinfo.pt.y < SCROLLAREAWIDTH)
                m_ListView.SendMessageW(WM_VSCROLL, SB_LINEUP, 0);

            if (htinfo.pt.y > clientRect.bottom - SCROLLAREAWIDTH)
                m_ListView.SendMessageW(WM_VSCROLL, SB_LINEDOWN, 0);
        }
    }
    else
    {
        m_cScrollDelay = 0; /* Reset, if the cursor is not over the listview's scroll-area */
    }

    m_ptLastMousePos = htinfo.pt;

    /* We need to check if we drag the selection over itself */
    if (lResult != -1 && m_pSourceDataObject.p != NULL)
    {
        PCUITEMID_CHILD pidl = _PidlByItem(lResult);

        for (UINT i = 0; i < m_cidl; i++)
        {
            if (pidl == m_apidl[i])
            {
                /* The item that is being draged is hovering itself. */
                lResult = -1;
                break;
            }
        }
    }

    /* If we are still over the previous sub-item, notify it via DragOver and return. */
    if (m_pCurDropTarget && lResult == m_iDragOverItem)
        return m_pCurDropTarget->DragOver(grfKeyState, pt, pdwEffect);

    /* We've left the previous sub-item, notify it via DragLeave and Release it. */
    if (m_pCurDropTarget)
    {
        PCUITEMID_CHILD pidl = _PidlByItem(m_iDragOverItem);
        if (pidl)
            SelectItem(pidl, 0);

        m_pCurDropTarget->DragLeave();
        m_pCurDropTarget.Release();
    }

    m_iDragOverItem = lResult;

    if (lResult == -1)
    {
        /* We are not above one of the listview's subitems. Bind to the parent folder's
         * DropTarget interface. */
        hr = m_pSFParent->CreateViewObject(NULL, IID_PPV_ARG(IDropTarget,&m_pCurDropTarget));
    }
    else
    {
        /* Query the relative PIDL of the shellfolder object represented by the currently
         * dragged over listview-item ... */
        PCUITEMID_CHILD pidl = _PidlByItem(lResult);

        /* ... and bind m_pCurDropTarget to the IDropTarget interface of an UIObject of this object */
        hr = m_pSFParent->GetUIObjectOf(m_ListView, 1, &pidl, IID_NULL_PPV_ARG(IDropTarget, &m_pCurDropTarget));
    }

    /* If anything failed, m_pCurDropTarget should be NULL now, which ought to be a save state. */
    if (FAILED(hr))
    {
        *pdwEffect = DROPEFFECT_NONE;
        return hr;
    }

    if (m_iDragOverItem != -1)
    {
        SelectItem(m_iDragOverItem, SVSI_SELECT);
    }

    /* Notify the item just entered via DragEnter. */
    return m_pCurDropTarget->DragEnter(m_pCurDataObject, grfKeyState, pt, pdwEffect);
}

HRESULT WINAPI CDefView::DragEnter(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    /* Get a hold on the data object for later calls to DragEnter on the sub-folders */
    m_pCurDataObject = pDataObject;

    HRESULT hr = drag_notify_subitem(grfKeyState, pt, pdwEffect);    
    if (SUCCEEDED(hr))
    {
        POINT ptClient = {pt.x, pt.y};
        ScreenToClient(&ptClient);
        ImageList_DragEnter(m_hWnd, ptClient.x, ptClient.y);
    }

    return hr;
}

HRESULT WINAPI CDefView::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    POINT ptClient = {pt.x, pt.y};
    ScreenToClient(&ptClient);
    ImageList_DragMove(ptClient.x, ptClient.y);
    return drag_notify_subitem(grfKeyState, pt, pdwEffect);
}

HRESULT WINAPI CDefView::DragLeave()
{
    ImageList_DragLeave(m_hWnd);

    if (m_pCurDropTarget)
    {
        m_pCurDropTarget->DragLeave();
        m_pCurDropTarget.Release();
    }

    if (m_pCurDataObject != NULL)
    {
        m_pCurDataObject.Release();
    }

    m_iDragOverItem = 0;

    return S_OK;
}

HRESULT WINAPI CDefView::Drop(IDataObject* pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    ERR("GetKeyState(VK_LBUTTON): %d\n", GetKeyState(VK_LBUTTON));

    ImageList_DragLeave(m_hWnd);
    ImageList_EndDrag();

    if ((m_iDragOverItem == -1 || m_pCurDropTarget == NULL) && 
        (*pdwEffect & DROPEFFECT_MOVE) && 
        /*(GetKeyState(VK_LBUTTON) != 0) &&*/
        (m_pSourceDataObject.p) && 
        (SHIsSameObject(pDataObject, m_pSourceDataObject)))
    {
        if (m_pCurDropTarget)
        {
            m_pCurDropTarget->DragLeave();
            m_pCurDropTarget.Release();
        }

        /* Restore the selection */
        m_ListView.SetItemState(-1, 0, LVIS_SELECTED);
        for (UINT i = 0 ; i < m_cidl; i++)
            SelectItem(m_apidl[i], SVSI_SELECT);

        /* Reposition the items */
        int lvIndex = -1;
        while ((lvIndex = m_ListView.GetNextItem(lvIndex,  LVNI_SELECTED)) > -1)
        {
            POINT ptItem;
            if (m_ListView.GetItemPosition(lvIndex, &ptItem))
            {
                ptItem.x += pt.x - m_ptFirstMousePos.x;
                ptItem.y += pt.y - m_ptFirstMousePos.y;
                m_ListView.SetItemPosition(lvIndex, &ptItem);
            }
        }
    }
    else if (m_pCurDropTarget)
    {
        m_pCurDropTarget->Drop(pDataObject, grfKeyState, pt, pdwEffect);
        m_pCurDropTarget.Release();
    }

    m_pCurDataObject.Release();    
    m_iDragOverItem = 0;
    return S_OK;
}

/**********************************************************
 * ISVDropSource implementation
 */

HRESULT WINAPI CDefView::QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
{
    TRACE("(%p)\n", this);

    if (fEscapePressed)
        return DRAGDROP_S_CANCEL;
    else if (!(grfKeyState & MK_LBUTTON) && !(grfKeyState & MK_RBUTTON))
        return DRAGDROP_S_DROP;
    else
        return S_OK;
}

HRESULT WINAPI CDefView::GiveFeedback(DWORD dwEffect)
{
    TRACE("(%p)\n", this);

    return DRAGDROP_S_USEDEFAULTCURSORS;
}

/**********************************************************
 * ISVViewObject implementation
 */

HRESULT WINAPI CDefView::Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd, HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds, BOOL (CALLBACK *pfnContinue)(ULONG_PTR dwContinue), ULONG_PTR dwContinue)
{
    FIXME("Stub: this=%p\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd, HDC hicTargetDevice, LOGPALETTE **ppColorSet)
{
    FIXME("Stub: this=%p\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze)
{
    FIXME("Stub: this=%p\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::Unfreeze(DWORD dwFreeze)
{
    FIXME("Stub: this=%p\n", this);

    return E_NOTIMPL;
}

HRESULT WINAPI CDefView::SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink)
{
    FIXME("partial stub: %p %08x %08x %p\n", this, aspects, advf, pAdvSink);

    /* FIXME: we set the AdviseSink, but never use it to send any advice */
    m_pAdvSink = pAdvSink;
    m_dwAspects = aspects;
    m_dwAdvf = advf;

    return S_OK;
}

HRESULT WINAPI CDefView::GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink)
{
    TRACE("this=%p pAspects=%p pAdvf=%p ppAdvSink=%p\n", this, pAspects, pAdvf, ppAdvSink);

    if (ppAdvSink)
    {
        *ppAdvSink = m_pAdvSink;
        m_pAdvSink.p->AddRef();
    }

    if (pAspects)
        *pAspects = m_dwAspects;

    if (pAdvf)
        *pAdvf = m_dwAdvf;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CDefView::QueryService(REFGUID guidService, REFIID riid, void **ppvObject)
{
    if (IsEqualIID(guidService, SID_IShellBrowser))
        return m_pShellBrowser->QueryInterface(riid, ppvObject);
    else if(IsEqualIID(guidService, SID_IFolderView))
        return QueryInterface(riid, ppvObject);

    return E_NOINTERFACE;
}

HRESULT CDefView::_MergeToolbar()
{
    CComPtr<IExplorerToolbar> ptb;
    HRESULT hr = S_OK;

    hr = IUnknown_QueryService(m_pShellBrowser, IID_IExplorerToolbar, IID_PPV_ARG(IExplorerToolbar, &ptb));
    if (FAILED(hr))
        return hr;

    m_Category = CGID_DefViewFrame;

    hr = ptb->SetCommandTarget(static_cast<IOleCommandTarget*>(this), &m_Category, 0);
    if (FAILED(hr))
        return hr;

    if (hr == S_FALSE)
        return S_OK;

#if 0
    hr = ptb->AddButtons(&m_Category, buttonsCount, buttons);
    if (FAILED(hr))
        return hr;
#endif

    return S_OK;
}

HRESULT CDefView_CreateInstance(IShellFolder *pFolder, REFIID riid, LPVOID * ppvOut)
{
    return ShellObjectCreatorInit<CDefView>(pFolder, riid, ppvOut);
}

HRESULT WINAPI SHCreateShellFolderViewEx(
    LPCSFV psvcbi,    /* [in] shelltemplate struct */
    IShellView **ppsv) /* [out] IShellView pointer */
{
    CComPtr<IShellView> psv;
    HRESULT hRes;

    TRACE("sf=%p pidl=%p cb=%p mode=0x%08x parm=%p\n",
      psvcbi->pshf, psvcbi->pidl, psvcbi->pfnCallback,
      psvcbi->fvm, psvcbi->psvOuter);

    *ppsv = NULL;
    hRes = CDefView_CreateInstance(psvcbi->pshf, IID_PPV_ARG(IShellView, &psv));
    if (FAILED_UNEXPECTEDLY(hRes))
        return hRes;

    *ppsv = psv.Detach();
    return hRes;
}

HRESULT WINAPI SHCreateShellFolderView(const SFV_CREATE *pcsfv,
                        IShellView **ppsv)
{
    CComPtr<IShellView> psv;
    HRESULT hRes;

    *ppsv = NULL;
    if (!pcsfv || pcsfv->cbSize != sizeof(*pcsfv))
        return E_INVALIDARG;

    TRACE("sf=%p outer=%p callback=%p\n",
      pcsfv->pshf, pcsfv->psvOuter, pcsfv->psfvcb);

    hRes = CDefView_CreateInstance(pcsfv->pshf, IID_PPV_ARG(IShellView, &psv));
    if (FAILED(hRes))
        return hRes;

    *ppsv = psv.Detach();
    return hRes;
}
