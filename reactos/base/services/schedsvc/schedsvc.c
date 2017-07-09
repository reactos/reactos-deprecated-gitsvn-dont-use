/*
 *  ReactOS Services
 *  Copyright (C) 2015 ReactOS Team
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
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS Services
 * FILE:             base/services/schedsvc/schedsvc.c
 * PURPOSE:          Scheduling service
 * PROGRAMMER:       Eric Kohl <eric.kohl@reactos.org>
 */

/* INCLUDES *****************************************************************/

#include "precomp.h"

WINE_DEFAULT_DEBUG_CHANNEL(schedsvc);

/* GLOBALS ******************************************************************/

static WCHAR ServiceName[] = L"Schedule";

static SERVICE_STATUS_HANDLE ServiceStatusHandle;
static SERVICE_STATUS ServiceStatus;

static BOOL bStopService = FALSE;

/* FUNCTIONS *****************************************************************/

static VOID
UpdateServiceStatus(DWORD dwState)
{
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = dwState;

    if (dwState == SERVICE_PAUSED || dwState == SERVICE_RUNNING)
        ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP |
                                           SERVICE_ACCEPT_SHUTDOWN |
                                           SERVICE_ACCEPT_PAUSE_CONTINUE;
    else
        ServiceStatus.dwControlsAccepted = 0;

    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;

    if (dwState == SERVICE_START_PENDING ||
        dwState == SERVICE_STOP_PENDING ||
        dwState == SERVICE_PAUSE_PENDING ||
        dwState == SERVICE_CONTINUE_PENDING)
        ServiceStatus.dwWaitHint = 10000;
    else
        ServiceStatus.dwWaitHint = 0;

    SetServiceStatus(ServiceStatusHandle,
                     &ServiceStatus);
}


static DWORD WINAPI
ServiceControlHandler(DWORD dwControl,
                      DWORD dwEventType,
                      LPVOID lpEventData,
                      LPVOID lpContext)
{
    TRACE("ServiceControlHandler() called\n");

    switch (dwControl)
    {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            TRACE("  SERVICE_CONTROL_STOP/SERVICE_CONTROL_SHUTDOWN received\n");
            UpdateServiceStatus(SERVICE_STOP_PENDING);
            /* Stop listening to incoming RPC messages */
            RpcMgmtStopServerListening(NULL);
            bStopService = TRUE;
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_PAUSE:
            TRACE("  SERVICE_CONTROL_PAUSE received\n");
            UpdateServiceStatus(SERVICE_PAUSED);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_CONTINUE:
            TRACE("  SERVICE_CONTROL_CONTINUE received\n");
            UpdateServiceStatus(SERVICE_RUNNING);
            return ERROR_SUCCESS;

        case SERVICE_CONTROL_INTERROGATE:
            TRACE("  SERVICE_CONTROL_INTERROGATE received\n");
            SetServiceStatus(ServiceStatusHandle,
                             &ServiceStatus);
            return ERROR_SUCCESS;

        default:
            TRACE("  Control %lu received\n", dwControl);
            return ERROR_CALL_NOT_IMPLEMENTED;
    }
}


static
DWORD
ServiceInit(PHANDLE phEvent)
{
    HANDLE hThread;
    DWORD dwError;

    /* Initialize the job list */
    InitializeListHead(&JobListHead);

    /* Initialize the job list lock */
    RtlInitializeResource(&JobListLock);

    /* Initialize the start list */
    InitializeListHead(&StartListHead);

    /* Initialize the start list lock */
    RtlInitializeResource(&StartListLock);

    /* Load stored jobs from the registry */
    dwError = LoadJobs();
    if (dwError != ERROR_SUCCESS)
        return dwError;

    /* Start the RPC thread */
    hThread = CreateThread(NULL,
                           0,
                           (LPTHREAD_START_ROUTINE)RpcThreadRoutine,
                           NULL,
                           0,
                           NULL);
    if (!hThread)
    {
        ERR("Could not create the RPC thread\n");
        return GetLastError();
    }

    CloseHandle(hThread);

    /* Create the scheduler event */
    *phEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (*phEvent == NULL)
    {
        ERR("Could not create the scheduler event\n");
        return GetLastError();
    }

    return ERROR_SUCCESS;
}


VOID WINAPI
SchedServiceMain(DWORD argc, LPTSTR *argv)
{
    HANDLE hEvent = NULL;
    DWORD dwError;

    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    TRACE("SchedServiceMain() called\n");

    ServiceStatusHandle = RegisterServiceCtrlHandlerExW(ServiceName,
                                                        ServiceControlHandler,
                                                        NULL);
    if (!ServiceStatusHandle)
    {
        ERR("RegisterServiceCtrlHandlerExW() failed! (Error %lu)\n", GetLastError());
        return;
    }

    UpdateServiceStatus(SERVICE_START_PENDING);

    dwError = ServiceInit(&hEvent);
    if (dwError != ERROR_SUCCESS)
    {
        ERR("Service stopped (dwError: %lu\n", dwError);
        UpdateServiceStatus(SERVICE_STOPPED);
        return;
    }

    UpdateServiceStatus(SERVICE_RUNNING);

    for (;;)
    {
        /* Leave the loop, if the service has to be stopped */
        if (bStopService)
            break;

        /* Wait for the next timeout */
        WaitForSingleObject(hEvent, 5000);
        TRACE("Service running!\n");
    }

    /* Close the scheduler event handle */
    CloseHandle(hEvent);

    /* Stop the service */
    UpdateServiceStatus(SERVICE_STOPPED);
}


BOOL WINAPI
DllMain(HINSTANCE hinstDLL,
        DWORD fdwReason,
        LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hinstDLL);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}
