#ifndef _EXPLORER_PRECOMP__H_
#define _EXPLORER_PRECOMP__H_

#define WIN7_COMPAT_MODE 0
#define WIN7_DEBUG_MODE 0

#if WIN7_DEBUG_MODE && !WIN7_COMPAT_MODE
#undef WIN7_COMPAT_MODE
#define WIN7_COMPAT_MODE 1
#endif

#include <stdio.h>
#include <tchar.h>

#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H

#define COBJMACROS

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <wingdi.h>
#include <winnls.h>
#include <wincon.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <uxtheme.h>
#include <strsafe.h>

#include <undocuser.h>
#include <shlwapi_undoc.h>
#include <shlobj_undoc.h>
#include <shlguid_undoc.h>
#include <undocshell.h>

#include <rosctrls.h>
#include <shellutils.h>

#include "tmschema.h"
#include "resource.h"

#include <wine/debug.h>

WINE_DEFAULT_DEBUG_CHANNEL(explorernew);

#define ASSERT(cond) \
    do if (!(cond)) { \
        Win32DbgPrint(__FILE__, __LINE__, "ASSERTION %s FAILED!\n", #cond); \
        } while (0)

extern HINSTANCE hExplorerInstance;
extern HANDLE hProcessHeap;
extern HKEY hkExplorer;

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

/*
 * explorer.c
 */

static inline 
LONG
SetWindowStyle(IN HWND hWnd,
               IN LONG dwStyleMask,
               IN LONG dwStyle)
{
    return SHSetWindowBits(hWnd, GWL_STYLE, dwStyleMask, dwStyle);
}

static inline
LONG
SetWindowExStyle(IN HWND hWnd,
                 IN LONG dwStyleMask,
                 IN LONG dwStyle)
{
    return SHSetWindowBits(hWnd, GWL_EXSTYLE, dwStyleMask, dwStyle);
}

HMENU
LoadPopupMenu(IN HINSTANCE hInstance,
              IN LPCWSTR lpMenuName);

HMENU
FindSubMenu(IN HMENU hMenu,
            IN UINT uItem,
            IN BOOL fByPosition);

BOOL
GetCurrentLoggedOnUserName(OUT LPWSTR szBuffer,
                           IN DWORD dwBufferSize);

BOOL
FormatMenuString(IN HMENU hMenu,
                 IN UINT uPosition,
                 IN UINT uFlags,
                 ...);

BOOL
GetExplorerRegValueSet(IN HKEY hKey,
                       IN LPCWSTR lpSubKey,
                       IN LPCWSTR lpValue);

/*
 *  rshell.c
 */

VOID InitRSHELL(VOID);
HRESULT WINAPI _CStartMenu_CreateInstance(REFIID riid, void **ppv);
HANDLE WINAPI _SHCreateDesktop(IShellDesktopTray *ShellDesk);
BOOL WINAPI _SHDesktopMessageLoop(HANDLE hDesktop);
DWORD WINAPI _WinList_Init(void);
void WINAPI _ShellDDEInit(BOOL bInit);
HRESULT WINAPI _CBandSiteMenu_CreateInstance(REFIID riid, void **ppv);
HRESULT WINAPI _CBandSite_CreateInstance(LPUNKNOWN pUnkOuter, REFIID riid, void **ppv);

/*
 * traywnd.c
 */

#define TWM_OPENSTARTMENU (WM_USER + 260)

extern const GUID IID_IShellDesktopTray;

#define INTERFACE ITrayWindow
DECLARE_INTERFACE_(ITrayWindow, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD_(HRESULT, QueryInterface) (THIS_ REFIID riid, void** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;
    /*** ITrayWindow methods ***/
    STDMETHOD_(HRESULT, Open) (THIS) PURE;
    STDMETHOD_(HRESULT, Close) (THIS) PURE;
    STDMETHOD_(HWND, GetHWND) (THIS) PURE;
    STDMETHOD_(BOOL, IsSpecialHWND) (THIS_ HWND hWnd) PURE;
    STDMETHOD_(BOOL, IsHorizontal) (THIS) PURE;
    STDMETHOD_(HWND, DisplayProperties) (THIS) PURE;
    STDMETHOD_(BOOL, ExecContextMenuCmd) (THIS_ UINT uiCmd) PURE;
    STDMETHOD_(BOOL, Lock) (THIS_ BOOL bLock) PURE;
};
#undef INTERFACE

#if defined(COBJMACROS)
/*** IUnknown methods ***/
#define ITrayWindow_QueryInterface(p,a,b)   (p)->lpVtbl->QueryInterface(p,a,b)
#define ITrayWindow_AddRef(p)               (p)->lpVtbl->AddRef(p)
#define ITrayWindow_Release(p)              (p)->lpVtbl->Release(p)
/*** ITrayWindow methods ***/
#define ITrayWindow_Open(p)                 (p)->lpVtbl->Open(p)
#define ITrayWindow_Close(p)                (p)->lpVtbl->Close(p)
#define ITrayWindow_GetHWND(p)              (p)->lpVtbl->GetHWND(p)
#define ITrayWindow_IsSpecialHWND(p,a)      (p)->lpVtbl->IsSpecialHWND(p,a)
#define ITrayWindow_IsHorizontal(p)         (p)->lpVtbl->IsHorizontal(p)
#define ITrayWindow_DisplayProperties(p)    (p)->lpVtbl->DisplayProperties(p)
#define ITrayWindow_ExecContextMenuCmd(p,a) (p)->lpVtbl->ExecContextMenuCmd(p,a)
#define ITrayWindow_Lock(p,a)               (p)->lpVtbl->Lock(p,a)
#endif

BOOL
RegisterTrayWindowClass(VOID);

VOID
UnregisterTrayWindowClass(VOID);

HRESULT CreateTrayWindow(ITrayWindow ** ppTray);

VOID
TrayProcessMessages(IN OUT ITrayWindow *Tray);

VOID
TrayMessageLoop(IN OUT ITrayWindow *Tray);

/*
 * settings.c
 */

/* Structure to hold non-default options*/
typedef struct _TASKBAR_SETTINGS
{
    BOOL bLock;
    BOOL bAutoHide;
    BOOL bAlwaysOnTop;
    BOOL bGroupButtons;
    BOOL bShowQuickLaunch;
    BOOL bShowClock;
    BOOL bShowSeconds;
    BOOL bHideInactiveIcons;
} TASKBAR_SETTINGS, *PTASKBAR_SETTINGS;

extern TASKBAR_SETTINGS TaskBarSettings;

VOID
LoadTaskBarSettings(VOID);

VOID
SaveTaskBarSettings(VOID);

BOOL
LoadSettingDword(IN LPCWSTR pszKeyName,
                 IN LPCWSTR pszValueName,
                 OUT DWORD &dwValue);
BOOL
SaveSettingDword(IN LPCWSTR pszKeyName,
                 IN LPCWSTR pszValueName,
                 IN DWORD dwValue);

/*
 * shellservice.cpp
 */
HRESULT InitShellServices(HDPA * phdpa);
HRESULT ShutdownShellServices(HDPA hdpa);

/*
 * startup.cpp
 */

int
ProcessStartupItems(VOID);

/*
 * trayprop.h
 */

VOID
DisplayTrayProperties(IN HWND hwndOwner);

/*
 * desktop.cpp
 */
HANDLE
DesktopCreateWindow(IN OUT ITrayWindow *Tray);

VOID
DesktopDestroyShellWindow(IN HANDLE hDesktop);


/*
 * notifyiconscust.cpp
 */
VOID
ShowCustomizeNotifyIcons(HINSTANCE, HWND);

/*
 * taskband.cpp
 */

extern const GUID CLSID_ITaskBand;  /* Internal Task Band CLSID */
HRESULT CTaskBand_CreateInstance(IN ITrayWindow *Tray, HWND hWndStartButton, REFIID riid, void **ppv);

/*
 * tbsite.cpp
 */

#define INTERFACE ITrayBandSite
DECLARE_INTERFACE_(ITrayBandSite, IUnknown)
{
    /*** IUnknown methods ***/
    STDMETHOD_(HRESULT, QueryInterface) (THIS_ REFIID riid, void** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef) (THIS) PURE;
    STDMETHOD_(ULONG, Release) (THIS) PURE;
    /*** IBandSiteStreamCallback ***/
    STDMETHOD_(HRESULT, OnLoad)(THIS_ IStream *pStm, REFIID riid, PVOID *pvObj) PURE;
    STDMETHOD_(HRESULT, OnSave)(THIS_ IUnknown *pUnk, IStream *pStm) PURE;
    /*** ITrayBandSite methods ***/
    STDMETHOD_(HRESULT, IsTaskBand) (THIS_ IUnknown *punk) PURE;
    STDMETHOD_(HRESULT, ProcessMessage) (THIS_ HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT *plResult) PURE;
    STDMETHOD_(HRESULT, AddContextMenus) (THIS_ HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags, IContextMenu **ppcm) PURE;
    STDMETHOD_(HRESULT, Lock) (THIS_ BOOL bLock) PURE;
};
#undef INTERFACE

#if defined(COBJMACROS)
/*** IUnknown methods ***/
#define ITrayBandSite_QueryInterface(p,a,b)             (p)->lpVtbl->QueryInterface(p,a,b)
#define ITrayBandSite_AddRef(p)                         (p)->lpVtbl->AddRef(p)
#define ITrayBandSite_Release(p)                        (p)->lpVtbl->Release(p)
/*** IBandSiteStreamCallback methods ***/
#define ITrayBandSite_OnLoad(p,a,b,c)                   (p)->lpVtbl->OnLoad(p,a,b,c)
#define ITrayBandSite_OnSave(p,a,b)                     (p)->lpVtbl->OnSave(p,a,b)
/*** ITrayBandSite methods ***/
#define ITrayBandSite_IsTaskBand(p,a)                   (p)->lpVtbl->IsTaskBand(p,a)
#define ITrayBandSite_ProcessMessage(p,a,b,c,d,e)       (p)->lpVtbl->ProcessMessage(p,a,b,c,d,e)
#define ITrayBandSite_AddContextMenus(p,a,b,c,d,e,f)    (p)->lpVtbl->AddContextMenus(p,a,b,c,d,e,f)
#define ITrayBandSite_Lock(p,a)                         (p)->lpVtbl->Lock(p,a)
#endif

HRESULT CTrayBandSite_CreateInstance(IN ITrayWindow *tray, IN IDeskBand* pTaskBand, OUT ITrayBandSite** pBandSite);

/*
 * startmnu.cpp
 */

HRESULT StartMenuBtnCtxMenuCreator(ITrayWindow * TrayWnd, IN HWND hWndOwner, IContextMenu ** ppCtxMenu);

IMenuPopup*
CreateStartMenu(IN ITrayWindow *Tray,
                OUT IMenuBand **ppMenuBand,
                IN HBITMAP hbmBanner OPTIONAL,
                IN BOOL bSmallIcons);

/*
 * startmnucust.cpp
 */
VOID 
ShowCustomizeClassic(HINSTANCE, HWND);

/*
* startmnusite.cpp
*/

HRESULT 
CreateStartMenuSite(IN OUT ITrayWindow *Tray, const IID & riid, PVOID * ppv);

/*
 * trayntfy.c
 */

/* TrayClockWnd */
#define TCWM_GETMINIMUMSIZE (WM_USER + 0x100)
#define TCWM_UPDATETIME     (WM_USER + 0x101)

/* TrayNotifyWnd */
#define TNWM_GETMINIMUMSIZE (WM_USER + 0x100)
#define TNWM_UPDATETIME     (WM_USER + 0x101)
#define TNWM_SHOWCLOCK      (WM_USER + 0x102)
#define TNWM_SHOWTRAY       (WM_USER + 0x103)
#define TNWM_CHANGETRAYPOS  (WM_USER + 0x104)

#define NTNWM_REALIGN   (0x1)

class CTrayNotifyWnd;

BOOL
RegisterTrayNotifyWndClass(VOID);

VOID
UnregisterTrayNotifyWndClass(VOID);

HWND
CreateTrayNotifyWnd(IN OUT ITrayWindow *TrayWindow, IN BOOL bHideClock, CTrayNotifyWnd** ppTrayNotify);

BOOL
TrayNotify_NotifyIconCmd(CTrayNotifyWnd* pTrayNotify, IN WPARAM wParam, IN LPARAM lParam);

BOOL
TrayNotify_GetClockRect(CTrayNotifyWnd* pTrayNotify, OUT PRECT rcClock);

/*
 * taskswnd.c
 */

#define TSWM_ENABLEGROUPING     (WM_USER + 1)
#define TSWM_UPDATETASKBARPOS   (WM_USER + 2)

BOOL
RegisterTaskSwitchWndClass(VOID);

VOID
UnregisterTaskSwitchWndClass(VOID);

HWND
CreateTaskSwitchWnd(IN HWND hWndParent,
IN OUT ITrayWindow *Tray);

HRESULT
Tray_OnStartMenuDismissed(ITrayWindow* Tray);

HRESULT
IsSameObject(IN IUnknown *punk1, IN IUnknown *punk2);

#endif /* _EXPLORER_PRECOMP__H_ */
