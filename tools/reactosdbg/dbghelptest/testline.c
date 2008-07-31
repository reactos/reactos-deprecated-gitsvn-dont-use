#include <stdio.h>
#include <windows.h>
#include <dbghelp.h>

#define MAXNAMELEN 256
// PSDK and MSDN disagree on the name of this
#define SymMemberIndex info

#define SYMOPT_ALLOW_ZERO_ADDRESS 0x1000000

typedef struct StrIntPair {
    const char *name;
    int flag;
} StrIntPair_t;

static StrIntPair_t symInfoFlags[] = {
    { "SYMFLAG_CLR_TOKEN",      0x00040000 },
    { "SYMFLAG_CONSTANT",       0x00000100 },
    { "SYMFLAG_EXPORT",         0x00000200 },
    { "SYMFLAG_FORWARDER",      0x00000400 },
    { "SYMFLAG_FRAMEREL",       0x00000020 },
    { "SYMFLAG_FUNCTION",       0x00000800 },
    { "SYMFLAG_ILREL",          0x00010000 },
    { "SYMFLAG_LOCAL",          0x00000080 },
    { "SYMFLAG_METADATA",       0x00020000 },
    { "SYMFLAG_PARAMETER",      0x00000040 },
    { "SYMFLAG_REGISTER",       0x00000008 },
    { "SYMFLAG_REGREL",         0x00000010 },
    { "SYMFLAG_SLOT",           0x00008000 },
    { "SYMFLAG_THUNK",          0x00002000 },
    { "SYMFLAG_TLSREL",         0x00004000 },
    { "SYMFLAG_VALUEPRESENT",   0x00000001 },
    { "SYMFLAG_VIRTUAL",        0x00001000 },
    { }
};

void printFlags(StrIntPair_t *pairs, int flagValue)
{
    int beenThere = 0;
    while(pairs->name) {
	if (pairs->flag & flagValue) {
	    printf("%s%s", beenThere ? ", " : "", pairs->name);
	    beenThere = 1;
	}
	pairs++;
    }
}

void printDbgStruct(PSYMBOL_INFO symInfo)
{
    printf("-- SYMBOL INFO --\n");
    printf(" TypeIndex: ... %08x\n", symInfo->TypeIndex);
    printf(" Index: ....... %08x\n", symInfo->SymMemberIndex);
    printf(" Size: ........ %08x\n", symInfo->Size);
    printf(" Flags: ....... %08x -- ", symInfo->Flags);
    printFlags(symInfoFlags, symInfo->Flags); printf("\n");
    printf(" Value: ....... %08x\n", symInfo->Value);
    printf(" Address: ..... %08x\n", (ULONG)symInfo->Address);
    printf(" Register: .... %08x\n", symInfo->Register);
    printf(" Scope: ....... %08x\n", symInfo->Scope);
    printf(" Tag: ......... %08x\n", symInfo->Tag);
    printf(" Name: ........ %s\n", symInfo->Name);
    printf("-- SYMBOL INFO END --\n");
}

void printHlpSymbol(PIMAGEHLP_SYMBOL64 byNameSymbol)
{
	printf("== SYMBOL INFO ==\n");
	printf(" Address: ..... %08x\n", (ULONG)byNameSymbol->Address);
	printf(" Size: ........ %08x\n", byNameSymbol->Size);
	printf(" Name: ........ %s\n", byNameSymbol->Name);
	printf("== SYMBOL INFO END ==\n");
}

BOOL CALLBACK PrintSymbols(PSYMBOL_INFO symInfo, ULONG symbolSize, PVOID userContext)
{
    printDbgStruct(symInfo);
	return TRUE;
}

int main(int argc, char **argv)
{
    DWORD moduleAddr, displacement, testAddr;
    DWORD64 displacement64;
    char fileName[256];
    IMAGEHLP_LINE64 lineNumberIdent = { };
    HANDLE hProcess = (HANDLE)((rand() & ~3) + 1);

	DWORD symIndex;
	BYTE symInfoBuf[sizeof(SYMBOL_INFO)+MAXNAMELEN-1];
	PSYMBOL_INFO symInfo = (PSYMBOL_INFO)symInfoBuf;

	BYTE byNameSymbolBuf[sizeof(IMAGEHLP_SYMBOL64)+MAXNAMELEN-1];
	PIMAGEHLP_SYMBOL64 byNameSymbol = (PIMAGEHLP_SYMBOL64)byNameSymbolBuf;
	
	IMAGEHLP_STACK_FRAME stackFrame = { };

	symInfo->SizeOfStruct = sizeof(*symInfo);
	symInfo->MaxNameLen = MAXNAMELEN - 1;
	
	byNameSymbol->SizeOfStruct = sizeof(*byNameSymbol);
	byNameSymbol->MaxNameLength = MAXNAMELEN - 1;

    lineNumberIdent.SizeOfStruct = sizeof(lineNumberIdent);
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_ALLOW_ZERO_ADDRESS | SYMOPT_LOAD_ANYTHING);

    if (!SymInitialize(hProcess, NULL, FALSE))
    {
		printf("Could not initialize dbghelp\n");
		return 1;
    }

    sscanf(argv[2], "%x", &moduleAddr);
    if (!SymLoadModuleEx
		(hProcess,
		 NULL,
		 argv[1],
		 NULL,
		 moduleAddr,
		 0,
		 NULL,
		 0))
    {
		printf("Could not do SymLoadModule on %s\n", argv[1]);
		return 1;
    }

	if (argc == 3)
	{
		if (!SymGetSymFromAddr64
			(hProcess,
			 moduleAddr,
			 &displacement64,
			 byNameSymbol))
		{
			printf("Could not get root symbol at %x for %s\n", moduleAddr, argv[1]);
			return 1;
		}

		printHlpSymbol(byNameSymbol);

		if (!SymFromAddr
			(hProcess,
			 byNameSymbol->Address,
			 &displacement64,
			 symInfo))
		{
			printf("SymFromAddr failed for addr %x\n", testAddr);
			return 1;
		}

		printDbgStruct(symInfo);
		while (SymGetSymNext64(hProcess, byNameSymbol))
		{
			printHlpSymbol(byNameSymbol);

			if (!SymFromAddr
				(hProcess,
				 byNameSymbol->Address,
				 &displacement64,
				 symInfo))
			{
				printf("SymFromAddr failed for addr %x\n", testAddr);
				return 1;
			}
			
			printDbgStruct(symInfo);
		}
	}
    else if (!strcmp("-va", argv[3]))
    {
		if (argc == 4) 
		{
			printf("-va requires a global name to look up\n");
			return 1;
		}

		if (!SymGetSymFromName64(hProcess, argv[4], byNameSymbol))
		{
			printf("Couldn't find symbol named %s\n", argv[4]);
			return 1;
		}
		
		testAddr = byNameSymbol->Address;

		printHlpSymbol(byNameSymbol);

		if (!SymFromAddr
			(hProcess,
			 byNameSymbol->Address,
			 &displacement64,
			 symInfo))
		{
			printf("SymFromAddr failed for addr %x\n", testAddr);
			return 1;
		}

		printDbgStruct(symInfo);
    }
    else
    {
		sscanf(argv[3], "%x", &testAddr);
	
		if (!SymGetLineFromAddr64
			(hProcess, 
			 testAddr,
			 &displacement,
			 &lineNumberIdent))
		{
			printf("SymGetLineFromAddr64 failed for addr %x\n", testAddr);
			return 1;
		}
	
		printf("%x -> %s:%d\n", testAddr, lineNumberIdent.FileName, lineNumberIdent.LineNumber);

		if (!SymFromAddr
			(hProcess,
			 testAddr,
			 &displacement64,
			 symInfo))
		{
			printf("SymFromAddr failed for addr %x\n", testAddr);
			return 1;
		}

		printf("In function %s\n", symInfo->Name);
	}

	stackFrame.InstructionOffset = testAddr;
	
	if (SymSetContext(hProcess, &stackFrame, 0) || !GetLastError())
		SymEnumSymbols(hProcess, 0, 0, PrintSymbols, NULL);
	else
		printf("Could not set context for address %x\n", testAddr);

    return 0;
}
