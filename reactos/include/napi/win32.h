#ifndef __INCLUDE_NAPI_WIN32_H
#define __INCLUDE_NAPI_WIN32_H

typedef struct _W32THREAD
{
  PVOID MessageQueue;
  FAST_MUTEX WindowListLock;
  LIST_ENTRY WindowListHead;
  struct _KBDTABLES* KeyboardLayout;
  struct _DESKTOP_OBJECT* Desktop;
  DWORD MessagePumpHookValue;
} __attribute__((packed)) W32THREAD, *PW32THREAD;

typedef struct _W32PROCESS
{
  FAST_MUTEX ClassListLock;
  LIST_ENTRY ClassListHead;
  FAST_MUTEX MenuListLock;
  LIST_ENTRY MenuListHead;
  FAST_MUTEX PrivateFontListLock;
  LIST_ENTRY PrivateFontListHead;
  FAST_MUTEX CursorIconListLock;
  LIST_ENTRY CursorIconListHead;
  struct _KBDTABLES* KeyboardLayout;
  struct _WINSTATION_OBJECT* WindowStation;
  WORD GDIObjects;
  WORD UserObjects;
  BOOL CreatedWindowOrDC;
  BOOL ManualGuiCheck;
} W32PROCESS, *PW32PROCESS;

PW32THREAD STDCALL
PsGetWin32Thread(VOID);
PW32PROCESS STDCALL
PsGetWin32Process(VOID);

#endif /* __INCLUDE_NAPI_WIN32_H */
