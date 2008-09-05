/*
 * iphlpapi dll implementation -- Setting and storing route information
 *
 * These are stubs for functions that set routing information on the target
 * operating system.  They are grouped here because their implementation will
 * vary widely by operating system.
 *
 * Copyright (C) 2004 Art Yerkes
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"
#include "iphlpapi_private.h"

#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
# include <arpa/nameser.h>
#endif
#ifdef HAVE_RESOLV_H
# include <resolv.h>
#endif

#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "resinfo.h"
#include "iphlpapi.h"
#include "wine/debug.h"

typedef struct _NAME_SERVER_LIST_PRIVATE {
    UINT NumServers;
    UINT CurrentName;
    PIP_ADDRESS_STRING AddrString;
} NAME_SERVER_LIST_PRIVATE, *PNAME_SERVER_LIST_PRIVATE;

NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap (
	HANDLE	Heap,
	ULONG	Flags,
	ULONG	Size
	);

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHeap (
	HANDLE	Heap,
	ULONG	Flags,
	PVOID	Address
	);

NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeToMultiByteN (
	PCHAR	MbString,
	ULONG	MbSize,
	PULONG	ResultSize,
	PWCHAR	UnicodeString,
	ULONG	UnicodeSize
	);

typedef VOID (*EnumNameServersFunc)( PWCHAR Interface,
				     PWCHAR NameServer,
				     PVOID Data );
typedef VOID (*EnumInterfacesFunc)( HKEY ChildKeyHandle,
				    PWCHAR ChildKeyName,
				    PVOID Data );

/*
 * EnumInterfaces
 *
 * Call the enumeration function for each name server.
 */

static void EnumInterfaces( PVOID Data, EnumInterfacesFunc cb ) {
  HKEY RegHandle;
  HKEY ChildKeyHandle = 0;
  PWCHAR RegKeyToEnumerate =
      L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces";
  PWCHAR ChildKeyName = 0;
  DWORD CurrentInterface;

  if (OpenChildKeyRead(HKEY_LOCAL_MACHINE,RegKeyToEnumerate,&RegHandle)) {
    return;
  }

  for (CurrentInterface = 0; TRUE; CurrentInterface++) {
      ChildKeyName = GetNthChildKeyName( RegHandle, CurrentInterface );
      if (!ChildKeyName) break;
      if (OpenChildKeyRead(RegHandle,ChildKeyName,
			   &ChildKeyHandle) == 0) {
	  cb( ChildKeyHandle, ChildKeyName, Data );
	  RegCloseKey( ChildKeyHandle );
      }
      ConsumeChildKeyName( ChildKeyName );
  }
}

/*
 * EnumNameServers
 */

static void EnumNameServers( HANDLE RegHandle, PWCHAR Interface,
			     PVOID Data, EnumNameServersFunc cb ) {
    PWCHAR NameServerString =
	QueryRegistryValueString(RegHandle, L"NameServer");
    /* Now, count the non-empty comma separated */
    if (NameServerString) {
	DWORD ch;
	DWORD LastNameStart = 0;
	for (ch = 0; NameServerString[ch]; ch++) {
	    if (NameServerString[ch] == ',') {
		if (ch - LastNameStart > 0) { /* Skip empty entries */
		    PWCHAR NameServer =
			malloc(ch - LastNameStart + 1);
		    if (NameServer) {
			memcpy(NameServer,NameServerString + LastNameStart,
			       (ch - LastNameStart));
			NameServer[ch - LastNameStart] = 0;
			cb( Interface, NameServer, Data );
			free(NameServer);
		    }
		}
		LastNameStart = ch + 1; /* The first one after the comma */
	    }
	}
	if (ch - LastNameStart > 0) { /* A last name? */
	    PWCHAR NameServer = malloc(ch - LastNameStart + 1);
	    memcpy(NameServer,NameServerString + LastNameStart,
		   (ch - LastNameStart));
	    NameServer[ch - LastNameStart] = 0;
	    cb( Interface, NameServer, Data );
	    free(NameServer);
	}
	ConsumeRegValueString(NameServerString);
    }
}

static void CreateNameServerListEnumNamesFuncCount( PWCHAR Interface,
						    PWCHAR Server,
						    PVOID _Data ) {
    PNAME_SERVER_LIST_PRIVATE Data = (PNAME_SERVER_LIST_PRIVATE)_Data;
    Data->NumServers++;
}

static void CreateNameServerListEnumIfFuncCount( HKEY RegHandle,
						 PWCHAR InterfaceName,
						 PVOID _Data ) {
    PNAME_SERVER_LIST_PRIVATE Data = (PNAME_SERVER_LIST_PRIVATE)_Data;
    EnumNameServers(RegHandle,InterfaceName,Data,
		    CreateNameServerListEnumNamesFuncCount);
}

static void CreateNameServerListEnumNamesFunc( PWCHAR Interface,
					       PWCHAR Server,
					       PVOID _Data ) {
    PNAME_SERVER_LIST_PRIVATE Data = (PNAME_SERVER_LIST_PRIVATE)_Data;
    RtlUnicodeToMultiByteN((PCHAR)&Data->AddrString[Data->CurrentName],
			   sizeof(Data->AddrString[0]),
			   NULL,
			   Server,
			   wcslen(Server));
    Data->CurrentName++;
}

static void CreateNameServerListEnumIfFunc( HKEY RegHandle,
					    PWCHAR InterfaceName,
					    PVOID _Data ) {
    PNAME_SERVER_LIST_PRIVATE Data = (PNAME_SERVER_LIST_PRIVATE)_Data;
    EnumNameServers(RegHandle,InterfaceName,Data,
		    CreateNameServerListEnumNamesFunc);
}

static int CountNameServers( PNAME_SERVER_LIST_PRIVATE PrivateData ) {
    EnumInterfaces(PrivateData,CreateNameServerListEnumIfFuncCount);
    return PrivateData->NumServers;
}

static void MakeNameServerList( PNAME_SERVER_LIST_PRIVATE PrivateData ) {
    EnumInterfaces(PrivateData,CreateNameServerListEnumIfFunc);
}

PIPHLP_RES_INFO getResInfo() {
    DWORD i, ServerCount, ExtraServer;
    HKEY hKey;
    LONG errCode;
    PWCHAR Str;
    IP_ADDR_STRING AddrString;
    NAME_SERVER_LIST_PRIVATE PrivateNSEnum = { 0 };
    PIPHLP_RES_INFO ResInfo;
    struct sockaddr_in *AddrList;

    ServerCount = CountNameServers( &PrivateNSEnum );

    errCode = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
			    "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\"
			    "Parameters", 0, KEY_READ, &hKey);
    if (errCode != ERROR_SUCCESS) {
	RegCloseKey( hKey );
	return NULL;
    }

    Str = QueryRegistryValueString( hKey, L"NameServer" );

    /* If NameServer is empty */
    if (*Str == L'\0')
    {
        /* Then use DhcpNameServer */
        Str = QueryRegistryValueString( hKey, L"DhcpNameServer" );
    }

    ExtraServer = Str ? 1 : 0;

    ServerCount += ExtraServer;

    PrivateNSEnum.NumServers = ServerCount;
    PrivateNSEnum.AddrString =
	(PIP_ADDRESS_STRING)
	RtlAllocateHeap( GetProcessHeap(), 0,
			 ServerCount * sizeof(IP_ADDRESS_STRING) );

    ResInfo =
	(PIPHLP_RES_INFO)RtlAllocateHeap
	( GetProcessHeap(), 0,
	  sizeof(IPHLP_RES_INFO) +
	  (ServerCount * sizeof(struct sockaddr_in)) );

    if( !ResInfo ) {
	RtlFreeHeap( GetProcessHeap(), 0, PrivateNSEnum.AddrString );
	RegCloseKey( hKey );
	return NULL;
    }

    ResInfo->riCount = ServerCount;
    AddrList = (struct sockaddr_in *)
	(((PCHAR)ResInfo) + sizeof(IPHLP_RES_INFO));
    ResInfo->riAddressList = AddrList;

    MakeNameServerList( &PrivateNSEnum );

    if( ExtraServer ) {
	ULONG ResultSize;

	for( ResultSize = 0; Str[ResultSize]; ResultSize++ )
	    ((PCHAR)&AddrString)[ResultSize] = Str[ResultSize];

	((PCHAR)&AddrString)[ResultSize] = 0;
	ResInfo->riAddressList[0].sin_family = AF_INET;
	ResInfo->riAddressList[0].sin_addr.s_addr =
	    inet_addr( (PCHAR)&AddrString );
	ResInfo->riAddressList[0].sin_port = 0;
	ConsumeRegValueString( Str );
    }

    for( i = ExtraServer; i < ServerCount; i++ ) {
	/* Hmm seems that dns servers are always AF_INET but ... */
	ResInfo->riAddressList[i].sin_family = AF_INET;
	ResInfo->riAddressList[i].sin_addr.s_addr =
	    inet_addr( (PCHAR)&PrivateNSEnum.AddrString[i - ExtraServer] );
	ResInfo->riAddressList[i].sin_port = 0;
    }

    RtlFreeHeap( GetProcessHeap(), 0, PrivateNSEnum.AddrString );
    RegCloseKey( hKey );

    return ResInfo;
}

VOID disposeResInfo( PIPHLP_RES_INFO InfoPtr ) {
    RtlFreeHeap( GetProcessHeap(), 0, InfoPtr );
}