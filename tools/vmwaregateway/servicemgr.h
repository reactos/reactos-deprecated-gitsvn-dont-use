///////////////////////////////////////////////////////////////////////////////
//
// (c) 2000 by Volkmar Uhlig, volkmar@ira.uka.de, www.uhlig-langert.de/volkmar
//
// all code can be reused and modified
//
//
//


VOID service_ctrl(DWORD dwCtrlCode);
VOID service_main(DWORD dwArgs, LPTSTR *lpszArgv);

void RegisterService(LPCSTR lpszBinaryPathName, LPCSTR lpszDependencies);
void UnregisterService();

LPCSTR GetServiceName(BOOL bLongName = FALSE);

extern HANDLE hServiceEvent;