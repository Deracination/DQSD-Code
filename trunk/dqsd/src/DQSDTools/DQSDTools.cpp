// DQSDTools.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "DQSDTools.h"

class CDQSDToolsModule : public CAtlDllModuleT< CDQSDToolsModule >
{
public :
    DECLARE_LIBID(LIBID_DQSDTOOLSLib)
    DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DQSDTOOLS, "{5A8D10BB-E86F-4d79-8E43-6D6594C9BFCD}")
};

CDQSDToolsModule _AtlModule;

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID lpReserved)
{
#ifdef _DEBUG
    // Note - enable this if necessary
    ATL::atlTraceCOM.SetStatus(ATLTRACESTATUS_DISABLED);
    ATL::atlTraceRegistrar.SetStatus(ATLTRACESTATUS_DISABLED);
#endif

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        InitCommonControls(); 
    }

    return _AtlModule.DllMain(dwReason, lpReserved);
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    return _AtlModule.DllRegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _AtlModule.DllUnregisterServer(TRUE);
}
