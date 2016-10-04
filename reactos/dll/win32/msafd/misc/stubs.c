/*
 * COPYRIGHT:   See COPYING in the top level directory
 * PROJECT:     ReactOS Ancillary Function Driver DLL
 * FILE:        dll/win32/msafd/misc/stubs.c
 * PURPOSE:     Stubs
 * PROGRAMMERS: Casper S. Hornstrup (chorns@users.sourceforge.net)
 * REVISIONS:
 *   CSH 01/09-2000 Created
 */

#include <msafd.h>

#include <wine/debug.h>
WINE_DEFAULT_DEBUG_CHANNEL(msafd);

INT
WSPAPI
WSPCancelBlockingCall(
    OUT LPINT lpErrno)
{
    UNIMPLEMENTED;

    return 0;
}


BOOL
WSPAPI
WSPGetOverlappedResult(
    IN  SOCKET s,
    IN  LPWSAOVERLAPPED lpOverlapped,
    OUT LPDWORD lpcbTransfer,
    IN  BOOL fWait,
    OUT LPDWORD lpdwFlags,
    OUT LPINT lpErrno)
{
    UNIMPLEMENTED;

    return FALSE;
}


BOOL
WSPAPI
WSPGetQOSByName(
    IN      SOCKET s,
    IN OUT  LPWSABUF lpQOSName,
    OUT     LPQOS lpQOS,
    OUT     LPINT lpErrno)
{
    UNIMPLEMENTED;

    return FALSE;
}


SOCKET
WSPAPI
WSPJoinLeaf(
    IN  SOCKET s,
    IN  CONST SOCKADDR *name,
    IN  INT namelen,
    IN  LPWSABUF lpCallerData,
    OUT LPWSABUF lpCalleeData,
    IN  LPQOS lpSQOS,
    IN  LPQOS lpGQOS,
    IN  DWORD dwFlags,
    OUT LPINT lpErrno)
{
    UNIMPLEMENTED;

    return (SOCKET)0;
}

/* EOF */
