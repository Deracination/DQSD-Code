#pragma once

#include <atlbase.h>

#include <shlguid.h>
#include <UrlMon.h>

static HRESULT ResolveWellKnownServiceGUID(REFGUID guid, LPTSTR pszBuffer, DWORD cchBuffer)
{
    struct GUID2Name
    {
        GUID guid;
        TCHAR name[256];
    };

#define GUID_NAME_ENTRY(g) \
    { g, _T("" #g "") }

    static const GUID2Name wellKnownServices[] = 
    {
        //GUID_NAME_ENTRY(SID_BindHost),
        //GUID_NAME_ENTRY(SID_CtxQueryAssociations),
        GUID_NAME_ENTRY(SID_DefView),
            //GUID_NAME_ENTRY(SID_IBindHost),
            GUID_NAME_ENTRY(SID_LinkSite),
            GUID_NAME_ENTRY(SID_MenuShellFolder),
            GUID_NAME_ENTRY(SID_SCommDlgBrowser),
            GUID_NAME_ENTRY(SID_PublishingWizard),
            //GUID_NAME_ENTRY(SID_SBandSite),
            GUID_NAME_ENTRY(SID_SBindHost),
            GUID_NAME_ENTRY(SID_SContainerDispatch),
            //GUID_NAME_ENTRY(SID_SContextMenuSite),
            //GUID_NAME_ENTRY(SID_SEditCommandTarget),
            GUID_NAME_ENTRY(SID_SElementBehaviorFactory),
            GUID_NAME_ENTRY(SID_SFolderView),
            //GUID_NAME_ENTRY(SID_SGetViewFromViewDual),
            GUID_NAME_ENTRY(SID_ShellFolderViewCB),
            //GUID_NAME_ENTRY(SID_SHTMLEditHost),
            GUID_NAME_ENTRY(SID_SHTMLEditServices),
            GUID_NAME_ENTRY(SID_SHTMLWindow),
            GUID_NAME_ENTRY(SID_SInternetExplorer),
            GUID_NAME_ENTRY(SID_SInternetHostSecurityManager),
            GUID_NAME_ENTRY(SID_SInternetSecurityManager)
    };

    for (int i=0; i < sizeof(wellKnownServices) / sizeof(*wellKnownServices); ++i)
    {
        if (InlineIsEqualGUID(wellKnownServices[i].guid, guid))
        {
            lstrcpyn(pszBuffer, wellKnownServices[i].name, cchBuffer);
            return S_OK;
        }
    }

    return S_FALSE;
}

// Similar to the lookup in AtlDumpIID - first tries HKCR\Interface, then HKCR\CLSID
static HRESULT ResolveNameOfGUID(REFGUID guid, LPTSTR pszBuffer, DWORD cchBuffer)
{
    HRESULT hr;

    TCHAR szName[128] = {0};
    DWORD dwType = 0;
    DWORD dw = sizeof(szName);

    OLECHAR szGuid[50] = {0};
    hr = StringFromGUID2(guid, szGuid, sizeof(szGuid) / sizeof(*szGuid));
    if (FAILED(hr)) return hr;

    bool bFoundIt = false;

    // Attempt to find it in the interfaces section
    ATL::CRegKey key;
    if (key.Open(HKEY_CLASSES_ROOT, _T("Interface"), KEY_READ) == ERROR_SUCCESS)
    {
        if (key.Open(key, ATL::CW2T(szGuid), KEY_READ) == ERROR_SUCCESS)
        {
            if (RegQueryValueEx(key.m_hKey, (LPTSTR)NULL, NULL, &dwType, (LPBYTE)szName, &dw) == ERROR_SUCCESS)
            {
                if (dw / sizeof(TCHAR) + 5 > cchBuffer)
                {
                    return E_INVALIDARG;
                }

                wsprintf(pszBuffer, "IID_%s", szName);
                bFoundIt = true;
            }
        }
    }

    if (!bFoundIt)
    {
        // Attempt to find it in the clsid section
        if (key.Open(HKEY_CLASSES_ROOT, _T("CLSID"), KEY_READ) == ERROR_SUCCESS)
        {
            if (key.Open(key, ATL::CW2T(szGuid), KEY_READ) == ERROR_SUCCESS)
            {
                if (RegQueryValueEx(key.m_hKey, (LPTSTR)NULL, NULL, &dwType, (LPBYTE)szName, &dw) == ERROR_SUCCESS)
                {
                    if (dw / sizeof(TCHAR) + 7 > cchBuffer)
                    {
                        return E_INVALIDARG;
                    }

                    wsprintf(pszBuffer, "CLSID_%s", szName);
                    bFoundIt = true;
                }
            }
        }
    }

    if (!bFoundIt)
    {
        // Just return the GUID as a string
        if (cchBuffer < 39)
        {
            return E_INVALIDARG;
        }

        lstrcpy(pszBuffer, ATL::CW2T(szGuid));
    }

    return S_OK;
}

// Dumps <pszClassName>::QueryService(<guidService>, <iid>), and attempts
// to translate <guidService> and <iid> into human-readable names
static HRESULT DumpServiceIDs(LPCTSTR pszClassName, REFGUID guidService, REFIID iid)
{
    TCHAR szIIDName[128] = {0};
    ResolveNameOfGUID(iid, szIIDName, sizeof(szIIDName) / sizeof(*szIIDName));

    TCHAR szServiceName[128] = {0};
    if (ResolveWellKnownServiceGUID(guidService, szServiceName, sizeof(szServiceName) / sizeof(*szServiceName)) != S_OK)
    {
        ResolveNameOfGUID(guidService, szServiceName, sizeof(szServiceName) / sizeof(*szServiceName));
    }

    OutputDebugString(pszClassName);
    OutputDebugString(_T("::QueryService("));
    OutputDebugString(szServiceName);
    OutputDebugString(_T(", "));
    OutputDebugString(szIIDName);
    OutputDebugString(_T(")"));
    OutputDebugString(_T("\n"));

    return S_OK;
}

// SERVICE_ENTRY_TRACE is used to trace all IServiceProvider::QueryService calls
// with as high fidelity as possible
#define SERVICE_ENTRY_TRACE(_className) \
    DumpServiceIDs(_className, guidService, riid); \

// SERVICE_ENTRY_TEAROFF for services implemented on a distinct COM identity
#define SERVICE_ENTRY_TEAROFF(x, _ServiceClass) \
    if (InlineIsEqualGUID(guidService, x))        \
{                                             \
    CComObject< _ServiceClass >* pSvcImpl = NULL;       \
    if (SUCCEEDED(pSvcImpl->CreateInstance(&pSvcImpl))) \
{                                                   \
    return pSvcImpl->QueryInterface(riid, ppvObject); \
}                                                   \
}                                                     \


