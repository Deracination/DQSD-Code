// DQSDTools.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "DQSDTools.h"

#include "DQSDTools_i.c"
#include "KeyboardHook.h"
#include "Launcher.h"
#include "MenuBuilder.h"

#ifdef _MERGE_PROXYSTUB
extern "C" HINSTANCE hProxyDll;
#endif

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Launcher, CLauncher)
OBJECT_ENTRY(CLSID_MenuBuilder, CMenuBuilder)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    lpReserved;
#ifdef _MERGE_PROXYSTUB
    if (!PrxDllMain(hInstance, dwReason, lpReserved))
        return FALSE;
#endif
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_DQSDTOOLSLib);
        DisableThreadLibraryCalls(hInstance);
		InitCommonControls(); 

		ATLTRACE("DQSDTools loading\n");
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
        _Module.Term();
		ATLTRACE("DQSDTools unloading\n");
	}
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
	ATLTRACE("DQSD: DllCanUnloadNow called (lockcount %d)\n", _Module.GetLockCount());

#ifdef _MERGE_PROXYSTUB
    if (PrxDllCanUnloadNow() != S_OK)
	{

	}
#endif

    if(_Module.GetLockCount()==0)
	{
		ATLTRACE("DQSD: DllCanUnloadNow returns S_OK\n");
		return S_OK;
	}
	else
	{
		ATLTRACE("DQSD: DllCanUnloadNow returns S_FALSE\n");
		return S_FALSE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hRes = PrxDllRegisterServer();
    if (FAILED(hRes))
        return hRes;
#endif
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
#ifdef _MERGE_PROXYSTUB
    PrxDllUnregisterServer();
#endif
    return _Module.UnregisterServer(TRUE);
}


