// DQSDHost.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "DQSDHost.h"

class CDQSDHostModule : public CAtlDllModuleT< CDQSDHostModule >
{
public :
    DECLARE_LIBID(LIBID_DQSDHostLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DQSDHOST, "{3A9F5B7B-92EF-474F-8373-B3125E6C0F5D}")
};

CDQSDHostModule _AtlModule;


// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    hInstance;

#ifdef _DEBUG
    // Note - enable this if necessary
    ATL::atlTraceCOM.SetStatus(ATLTRACESTATUS_DISABLED);
    ATL::atlTraceRegistrar.SetStatus(ATLTRACESTATUS_DISABLED);
#endif

    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer(TRUE);
    return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
    HRESULT hr = _AtlModule.DllUnregisterServer(TRUE);
    return hr;
}
