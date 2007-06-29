///////////////////////////////////////////////////////////////////////////////
//
// (c) 2000 by Volkmar Uhlig, volkmar@ira.uka.de, www.uhlig-langert.de/volkmar
//
// all code can be reused and modified
//
//
//


#define WINVER 0x401

#include "winsock2.h"
#include <stdio.h>
#include "servicemgr.h"

#define SERVICE_NAME	"VMWareGateway"
#define DISPLAY_NAME	"VMWareGateway"

#define SERVER_PORT  567
#define BUFFER_SIZE  1024

#define PIPE_NAME "\\\\.\\pipe\\vmwaredebug"

const char* errServerBusy = "Server is currently connected\n";


// event numbers/index into event array
#define LISTEN_EVENT	0
#define IO_EVENT		1
#define PIPE_CONNECT	2
#define PIPE_READ		3
#define SERVICE_EVENT	4
#define MAX_EVENTS		5

// verbose output - traces a lot
BOOL verbose = FALSE;

PSECURITY_DESCRIPTOR
CreateVmwareGatewaySD(VOID)
{
	static SID_IDENTIFIER_AUTHORITY LocalSystemAuthority = {SECURITY_NT_AUTHORITY};
	static SID_IDENTIFIER_AUTHORITY WorldAuthority = {SECURITY_WORLD_SID_AUTHORITY};
	PSID LocalSystemSid = NULL;
	PSID AdministratorsSid = NULL;
	PSID EveryoneSid = NULL;
	PACL Dacl;
	DWORD DaclSize;
	PSECURITY_DESCRIPTOR pSD = NULL;

	/* create the SYSTEM, Administrators and Everyone SIDs */
	if (!AllocateAndInitializeSid(&LocalSystemAuthority,
	                              1,
	                              SECURITY_LOCAL_SYSTEM_RID,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              &LocalSystemSid) ||
	    !AllocateAndInitializeSid(&LocalSystemAuthority,
	                              2,
	                              SECURITY_BUILTIN_DOMAIN_RID,
	                              DOMAIN_ALIAS_RID_ADMINS,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              &AdministratorsSid) ||
	    !AllocateAndInitializeSid(&WorldAuthority,
	                              1,
	                              SECURITY_WORLD_RID,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              0,
	                              &EveryoneSid))
	{
		printf("Failed initializing the SIDs for the default security descriptor (0x%p, 0x%p, 0x%p)\n",
		        LocalSystemSid, AdministratorsSid, EveryoneSid);
		goto Cleanup;
	}

	/* allocate the security descriptor and DACL */
	DaclSize = sizeof(ACL) +
	           ((GetLengthSid(LocalSystemSid) +
	             GetLengthSid(AdministratorsSid) +
	             GetLengthSid(EveryoneSid)) +
	            (3 * FIELD_OFFSET(ACCESS_ALLOWED_ACE,
	                              SidStart)));

	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LMEM_FIXED,
	                                       (SIZE_T)DaclSize + sizeof(SECURITY_DESCRIPTOR));
	if (pSD == NULL)
	{
		printf("Failed to allocate the default security descriptor and ACL\n");
		goto Cleanup;
	}

	if (!InitializeSecurityDescriptor(pSD,
	                                  SECURITY_DESCRIPTOR_REVISION))
	{
		printf("Failed to initialize the default security descriptor\n");
		goto Cleanup;
	}

	/* initialize and build the DACL */
	Dacl = (PACL)((ULONG_PTR)pSD + sizeof(SECURITY_DESCRIPTOR));
	if (!InitializeAcl(Dacl,
	                   (DWORD)DaclSize,
	                   ACL_REVISION))
	{
		printf("Failed to initialize the DACL of the default security descriptor\n");
		goto Cleanup;
	}

	/* add the SYSTEM Ace */
	if (!AddAccessAllowedAce(Dacl,
	                         ACL_REVISION,
	                         FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_GENERIC_EXECUTE,
	                         LocalSystemSid))
	{
		printf("Failed to add the SYSTEM ACE\n");
		goto Cleanup;
	}

	/* add the Administrators Ace */
	if (!AddAccessAllowedAce(Dacl,
	                         ACL_REVISION,
	                         FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_GENERIC_EXECUTE,
	                         AdministratorsSid))
	{
		printf("Failed to add the Administrators ACE\n");
		goto Cleanup;
	}

	/* add the Everyone Ace */
	if (!AddAccessAllowedAce(Dacl,
	                         ACL_REVISION,
	                         FILE_GENERIC_READ | FILE_GENERIC_WRITE | FILE_GENERIC_EXECUTE,
	                         EveryoneSid))
	{
		printf("Failed to add the Everyone ACE\n");
		goto Cleanup;
	}

	/* set the DACL */
	if (!SetSecurityDescriptorDacl(pSD,
	                               TRUE,
	                               Dacl,
	                               FALSE))
	{
		printf("Failed to set the DACL of the default security descriptor\n");

Cleanup:
		if (pSD != NULL)
		{
			LocalFree((HLOCAL)pSD);
			pSD = NULL;
		}
	}

	if (LocalSystemSid != NULL)
	{
		FreeSid(LocalSystemSid);
	}
	if (AdministratorsSid != NULL)
	{
		FreeSid(AdministratorsSid);
	}
	if (EveryoneSid != NULL)
	{
		FreeSid(EveryoneSid);
	}

	return pSD;
}

// the central server loop
void Server()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	PSECURITY_DESCRIPTOR sd;
	SECURITY_ATTRIBUTES sa;
	wVersionRequested = MAKEWORD( 2, 2 );

	// socket and pipe handles
	HANDLE pipe;
	SOCKET s, conn;
	WSAEVENT hEvents[MAX_EVENTS];
	OVERLAPPED ovRead, ovConnect;

	// copy buffers
	char buffersocket[BUFFER_SIZE];
	char bufferpipe[BUFFER_SIZE];
	DWORD dwBytesSocket, dwBytesPipe;
	
	// booleans of connection states
	BOOL pipeConnected = FALSE;
	BOOL socketConnected = FALSE;

	// socket stuff
	sockaddr_in sockaddr;
	int err;
	int addrlen;

	// init network
	if (err = WSAStartup(wVersionRequested, &wsaData)) {
		printf("Error initializing network\n");
		return;
	}

	// create a suitable security descriptor
	sd = CreateVmwareGatewaySD();
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = sd;
	sa.bInheritHandle = FALSE;

	// first create the named pipe
	pipe = CreateNamedPipe(PIPE_NAME, 
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		0,
		1,
		100,
		100,
		NMPWAIT_WAIT_FOREVER,
		&sa);
	if (sd != NULL)
		LocalFree((HLOCAL)sd);
	
	if (pipe == INVALID_HANDLE_VALUE) {
		printf("Could not create named pipe (%d)\n", GetLastError());
		exit(0);
	}

	hEvents[SERVICE_EVENT] = hServiceEvent;

	// now create the listen socket to wait for incoming requests
	hEvents[LISTEN_EVENT] = WSACreateEvent();
	hEvents[IO_EVENT] = WSACreateEvent();
	ovConnect.hEvent = hEvents[PIPE_CONNECT] = CreateEvent(NULL, TRUE, FALSE, NULL);
	ovRead.hEvent = hEvents[PIPE_READ] = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (hEvents[LISTEN_EVENT] == WSA_INVALID_EVENT || 
		hEvents[IO_EVENT] == WSA_INVALID_EVENT ||
		hEvents[PIPE_CONNECT] == NULL ||
		hEvents[PIPE_READ] == NULL) 
	{
		printf("Error creating net event\n");
		return;
	}

	// async connect to named pipe
	err = ConnectNamedPipe(pipe, &ovConnect);

	// listen socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Could not initialize socket (%d)\n", GetLastError());
		return;
	}
	sockaddr.sin_family=AF_INET;
	sockaddr.sin_port=htons(SERVER_PORT);
	sockaddr.sin_addr.s_addr = ADDR_ANY;
	
	err = bind(s, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
	if (err) {
		printf("Could not bind socket (%d)\n", GetLastError());
		return;
	}
	err = listen(s, 5);
	WSAEventSelect(s, hEvents[LISTEN_EVENT], FD_ACCEPT);


	// central loop
	while(1)
	{
		DWORD numEvent = WSAWaitForMultipleEvents(MAX_EVENTS, hEvents, 
			FALSE, WSA_INFINITE, TRUE);
		//printf("Event: %d\n", numEvent);
		switch(numEvent) {
		case LISTEN_EVENT:
			printf("incoming telnet request ");
			if (!socketConnected) {
				addrlen = sizeof(sockaddr);
				conn = accept(s, (struct sockaddr*)&sockaddr, &addrlen);
				if (conn == INVALID_SOCKET) {
					printf("error connecting\n");		
				}
				else {
					//err = listen(s, 5); relisten???
					hEvents[IO_EVENT] = WSACreateEvent();
					WSAEventSelect(conn, hEvents[IO_EVENT], FD_READ|FD_CLOSE);
					socketConnected = TRUE;
					printf("accepted\n");
				}
			}
			else {
				addrlen = sizeof(sockaddr);
				SOCKET tmp = accept(s, (struct sockaddr*)&sockaddr, &addrlen);
				send(tmp, errServerBusy, strlen(errServerBusy), 0);
				closesocket(tmp);
				printf("denied\n");
			}
				
			WSAResetEvent(hEvents[LISTEN_EVENT]);
			break;

		case IO_EVENT: 
			{
				WSANETWORKEVENTS NetEvents;
				WSAEnumNetworkEvents(conn, hEvents[IO_EVENT], &NetEvents);
				switch(NetEvents.lNetworkEvents) 
				{
				case FD_READ:
					dwBytesSocket = recv(conn, buffersocket, sizeof(buffersocket), 0);
					if (verbose) {
						buffersocket[dwBytesSocket]=0;
						printf("%s", buffersocket);
					}
					if (pipeConnected) {
						DWORD dwWritten;
						WriteFile(pipe, buffersocket, dwBytesSocket, &dwWritten, NULL);
					}
					break;
				case FD_CLOSE:
					/* connection closed by host */
					closesocket(conn);
					socketConnected = FALSE;
					listen(s, 5);
					printf("telnet session closed\n");
					break;
				}

			} break;
		case PIPE_CONNECT:
			if (!GetOverlappedResult(pipe, &ovConnect, &dwBytesPipe, FALSE)) 
			{
				printf("error pipe connect (%d)\n", GetLastError());
				break;
			}
			printf("pipe connected\n");
			ResetEvent(hEvents[PIPE_CONNECT]);
			ResetEvent(hEvents[PIPE_READ]);
			pipeConnected = TRUE;

			ovRead.Offset = 0;
			ovRead.OffsetHigh = 0;
			ReadFile(pipe, bufferpipe, sizeof(bufferpipe), &dwBytesPipe, &ovRead);
			break;
		
		case PIPE_READ:
			if (GetOverlappedResult(pipe, &ovRead, &dwBytesPipe, FALSE))
			{
				if (verbose) {
					bufferpipe[dwBytesPipe] = 0;
					printf(bufferpipe);
				}
				if (socketConnected)
				{
					send(conn, bufferpipe, dwBytesPipe, 0);
				}
				ResetEvent(hEvents[PIPE_READ]);
			}
			else
			{
				DWORD err = GetLastError();
				switch (err) {
				case ERROR_BROKEN_PIPE: /* disconnect and reconnect pipe */
					DisconnectNamedPipe(pipe);
					ConnectNamedPipe(pipe, &ovConnect);
					ResetEvent(hEvents[PIPE_CONNECT]);
					ResetEvent(hEvents[PIPE_READ]);
					pipeConnected = FALSE;
					printf("pipe disconnected\n");
					continue;break;
				}
			}
			ovRead.Offset = 0;
			ovRead.OffsetHigh = 0;
			ReadFile(pipe, bufferpipe, sizeof(bufferpipe), &dwBytesPipe, &ovRead);
			break;
		}
	}
	WSACleanup();
}

LPCSTR GetServiceName(BOOL bLongName)
{
	if (bLongName)
		return DISPLAY_NAME;
	else
		return SERVICE_NAME;
}

void ShowHelp() {
	printf("Named pipe <-> telnet server for Windows NT/2000 (use on your own risk :)\n"
		"Volkmar Uhlig (volkmar@ira.uka.de), www.uhlig-langert.de/volkmar\n\n"
		"Commands:\n"
		"\t/R register as a service\n"
		"\t/U unregister service\n"
		"\t/T testmode\n"
		"\t/V verbose mode\n"
		"\nThe pipe name is \"%s\" and the telnet port is %d.\n", PIPE_NAME, SERVER_PORT);
}
		
BOOL CheckArg(int argc, char * argv[], char * arg)
{
	for (int i=1; i<argc; i++)
		if (_stricmp(argv[i], arg) == 0) return TRUE;
	return FALSE;
}

int main(int argc, char* argv[])
{
	char modulename[512];
	GetModuleFileName(NULL, modulename, sizeof(modulename));
	if (argc > 1) {
		if (CheckArg(argc, argv, "/v")) 
		{
			verbose = TRUE;
			printf("verbose mode\n");
		}

		if (CheckArg(argc, argv, "/r")) 
		{
			RegisterService(modulename, "\0\0");
			return 0;
		}
		
		if (CheckArg(argc, argv, "/u")) 
		{
			UnregisterService();
			return 0;
		}
		
		if (CheckArg(argc, argv, "/t")) {
			printf("test mode - press Ctrl+C to stop program\n");
			hServiceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			Server();
			CloseHandle(hServiceEvent);
			return 0;
		}
		
		ShowHelp();
	}
	else {
		SERVICE_TABLE_ENTRY dispatchTable[] = 
		{
			{	TEXT(SERVICE_NAME), (LPSERVICE_MAIN_FUNCTION)service_main},
			{	NULL, NULL	}
		};
		StartServiceCtrlDispatcher(dispatchTable);
	}
	return 0;
}
