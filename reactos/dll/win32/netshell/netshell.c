#include "precomp.h"

HINSTANCE netshell_hInstance;
const GUID CLSID_NetworkConnections      = {0x7007ACC7, 0x3202, 0x11D1, {0xAA, 0xD2, 0x00, 0x80, 0x5F, 0xC1, 0x27, 0x0E}};
const GUID GUID_DEVCLASS_NET             = {0x4d36e972, 0xe325, 0x11ce, {0xbf, 0xc1, 0x08, 0x00, 0x2b, 0xe1, 0x03, 0x18}};
static const WCHAR szNetConnectClass[] = L"CLSID\\{7007ACC7-3202-11D1-AAD2-00805FC1270E}";
static const WCHAR szNamespaceKey[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ControlPanel\\NameSpace\\{7007ACC7-3202-11D1-AAD2-00805FC1270E}";

static INTERFACE_TABLE InterfaceTable[] =
{
    {
        &CLSID_NetworkConnections,
        ISF_NetConnect_Constructor
    },
    {
        &CLSID_ConnectionManager,
        INetConnectionManager_Constructor
    },
    {
        NULL,
        NULL
    }
};


BOOL
WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID fImpLoad)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            netshell_hInstance = hinstDLL;
            DisableThreadLibraryCalls(netshell_hInstance);
            break;
    default:
        break;
    }

    return TRUE;
}

HRESULT
WINAPI
DllCanUnloadNow(void)
{
    return S_FALSE;
}

//#define CHECKPOINT1 ERR("\n");

STDAPI
DllRegisterServer(void)
{
    HKEY hKey, hSubKey;
    WCHAR szName[MAX_PATH] = {0};
    WCHAR szNet[20];
    UINT Length, Offset;

    //CHECKPOINT1;

    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, szNetConnectClass, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS)
        return SELFREG_E_CLASS;

    //CHECKPOINT1;

    if (LoadStringW(netshell_hInstance, IDS_NETWORKCONNECTION, szName, MAX_PATH))
    {
        szName[MAX_PATH-1] = L'\0';
        RegSetValueW(hKey, NULL, REG_SZ, szName, (wcslen(szName)+1) * sizeof(WCHAR));
    }

    //CHECKPOINT1;

    if (RegCreateKeyExW(HKEY_LOCAL_MACHINE, szNamespaceKey, 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL) == ERROR_SUCCESS)
    {
        RegSetValueW(hSubKey, NULL, REG_SZ, szName, (wcslen(szName)+1) * sizeof(WCHAR));
        RegCloseKey(hSubKey);
    }

    //CHECKPOINT1;

    Length = swprintf(szNet, L",-%u", IDS_NETWORKCONNECTION);
    Offset = GetModuleFileNameW(netshell_hInstance, &szName[1], MAX_PATH);
    if (Offset + Length + 2 < MAX_PATH)
    {
        /* set localized name */
        szName[0] = L'@';
        wcscpy(&szName[Offset+1], szNet);
        RegSetValueExW(hKey, L"LocalizedString", 0, REG_SZ, (const LPBYTE)szName, (wcslen(szName)+1) * sizeof(WCHAR));
    }


    //CHECKPOINT1;

    szName[Offset+1] = L'\0';

    //CHECKPOINT1;

    /* store default icon */
    if (RegCreateKeyExW(hKey, L"DefaultIcon", 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL) == ERROR_SUCCESS)
    {
        RegSetValueW(hSubKey, NULL, REG_SZ, &szName[1], (Offset+1) * sizeof(WCHAR));
        RegCloseKey(hSubKey);
    }
    //CHECKPOINT1;
    if (RegCreateKeyExW(hKey, L"InProcServer32", 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL) == ERROR_SUCCESS)
    {
        RegSetValueW(hSubKey, NULL, REG_SZ, &szName[1], (Offset+1) * sizeof(WCHAR));
        RegCloseKey(hSubKey);
    }


    //CHECKPOINT1;

    if (RegCreateKeyExW(hKey, L"ShellFolder", 0, NULL, 0, KEY_WRITE, NULL, &hSubKey, NULL) == ERROR_SUCCESS)
    {
        DWORD dwAttributes = SFGAO_FOLDER;
        RegSetValueExW(hSubKey, L"Attributes",0, REG_BINARY, (const LPBYTE)&dwAttributes, sizeof(DWORD));
    }

    //CHECKPOINT1;

    return S_OK;
}

STDAPI
DllUnregisterServer(void)
{
    SHDeleteKeyW(HKEY_CLASSES_ROOT, szNetConnectClass);
    SHDeleteKeyW(HKEY_LOCAL_MACHINE, szNamespaceKey);
    return S_OK;
}

STDAPI
DllGetClassObject(
  REFCLSID rclsid,
  REFIID riid,
  LPVOID* ppv 
)
{
    UINT i;
    HRESULT	hres = E_OUTOFMEMORY;
    IClassFactory * pcf = NULL;

    if (!ppv)
        return E_INVALIDARG;

    *ppv = NULL;

    for (i = 0; InterfaceTable[i].riid; i++) 
    {
        if (IsEqualIID(InterfaceTable[i].riid, rclsid)) 
        {
            pcf = IClassFactory_fnConstructor(InterfaceTable[i].lpfnCI, NULL, NULL);
            break;
        }
    }

    if (!pcf) 
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    hres = IClassFactory_QueryInterface(pcf, riid, ppv);
    IClassFactory_Release(pcf);

    return hres;
}
