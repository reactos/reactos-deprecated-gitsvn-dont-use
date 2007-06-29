///////////////////////////////////////////////////////////////////////////////
//
// (c) 2000 by Volkmar Uhlig, volkmar@ira.uka.de, www.uhlig-langert.de/volkmar
//
// all code can be reused and modified
//
//
//

#include <windows.h>
#include <stdio.h>
#include "servicemgr.h"

extern void Server();

SERVICE_STATUS			ssStatus;
SERVICE_STATUS_HANDLE	sshStatusHandle;

HANDLE hServiceEvent = NULL;

BOOL ReportStatusToSCMgr(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwCheckPoint, DWORD dwWaitHint) 
{
	BOOL fResult;
	if (dwCurrentState==SERVICE_START_PENDING)
		ssStatus.dwControlsAccepted=0;
	else
		ssStatus.dwControlsAccepted=SERVICE_ACCEPT_STOP; // | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ssStatus.dwCurrentState=dwCurrentState;
	ssStatus.dwWin32ExitCode=dwWin32ExitCode;
	ssStatus.dwCheckPoint=dwCheckPoint;
	ssStatus.dwWaitHint=dwWaitHint;
	if (!(fResult = SetServiceStatus(sshStatusHandle, &ssStatus))) {
		SetEvent(hServiceEvent);
	}
	return fResult;
}


void RegisterService(LPCSTR lpszBinaryPathName, LPCSTR lpszDependencies)
{
	printf("Create %s Service for executable %s\n", GetServiceName(), lpszBinaryPathName);
	SC_HANDLE schService, schSCManager;
	schSCManager=OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schSCManager!=NULL) {
		schService=CreateService(
			schSCManager,
			GetServiceName(FALSE),
			GetServiceName(TRUE),
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			(char*)lpszBinaryPathName,
			NULL,	// no load ordering
			NULL,	// no tag id
			lpszDependencies,	
			NULL,	// Local system account
			NULL);	// no pwd
		if (schService==NULL) 
			printf("CreateService Error: %ld", GetLastError());
		else
			printf("CreateService SUCCESS\n");
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}

void UnregisterService() {
	SC_HANDLE schService, schSCManager;
	schSCManager=OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	schService=OpenService(schSCManager,
		GetServiceName(FALSE),
		SERVICE_ALL_ACCESS);
	if (DeleteService(schService)) 
		printf("Service %s successfully unregistered.\n", GetServiceName());
	else
		printf("Error while unregistering service %s: %ld\n", GetServiceName(), GetLastError());
	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);
}

VOID service_ctrl(DWORD dwCtrlCode) {
	DWORD dwState = SERVICE_RUNNING;
	switch(dwCtrlCode) {
	case SERVICE_CONTROL_STOP:
		dwState = SERVICE_STOP_PENDING;
		ReportStatusToSCMgr(
			SERVICE_STOP_PENDING,
			NO_ERROR,
			1,
			3000);
		SetEvent(hServiceEvent);
		return;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	default:
		break;
	}
	ReportStatusToSCMgr(dwState, NO_ERROR, 0, 0);
}

VOID service_main(DWORD dwArgs, LPTSTR *lpszArgv) 
{
	DWORD dwWait;
	sshStatusHandle=RegisterServiceCtrlHandler(GetServiceName(),
		(LPHANDLER_FUNCTION)service_ctrl);
	if (!sshStatusHandle) goto cleanup;
	ssStatus.dwServiceType=SERVICE_WIN32_OWN_PROCESS;
	ssStatus.dwServiceSpecificExitCode=0;
	if(!ReportStatusToSCMgr(SERVICE_START_PENDING,	//service state
		NO_ERROR,		// exit code
		1,				// checkpoint
		3000))			// wait hint
		goto cleanup;
	hServiceEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hServiceEvent == (HANDLE)NULL) goto cleanup;
	if (!ReportStatusToSCMgr(SERVICE_START_PENDING, NO_ERROR, 2, 3000)) goto cleanup;

	// starting...
	if (CreateThread(NULL, 4000, LPTHREAD_START_ROUTINE(Server), NULL, 0, NULL)==NULL) goto cleanup;
	if (!ReportStatusToSCMgr(SERVICE_RUNNING, NO_ERROR, 0, 0)) goto cleanup;

	dwWait = WaitForSingleObject( hServiceEvent, INFINITE);

cleanup:
	CloseHandle(hServiceEvent);

	if (sshStatusHandle!=NULL)
		ReportStatusToSCMgr(SERVICE_STOPPED, 0, 0, 0);

	return;
}
