// CustomSecurityManager.cpp : Implementation of CCustomSecurityManager

#include "stdafx.h"
#include "CustomSecurityManager.h"

//////////////////////////////////////////////////////////////////////////////////////////
/// IInternetSecurityManager
STDMETHODIMP CCustomSecurityManager::SetSecuritySite(IInternetSecurityMgrSite* /*pSite*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::GetSecuritySite(IInternetSecurityMgrSite** /*ppSite*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::MapUrlToZone(LPCWSTR /*pwszUrl*/, DWORD* /*pdwZone*/, DWORD /*dwFlags*/)
{
    //ATLTRACE(_T("%s called for URL: %ls\n"), __FUNCTION__, pwszUrl);
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::GetSecurityId(LPCWSTR /*pwszUrl*/, BYTE* /*pbSecurityId*/, DWORD* /*pcbSecurityId*/, DWORD_PTR /*dwReserved*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::ProcessUrlAction(LPCWSTR pwszUrl, DWORD dwAction, BYTE *pPolicy, DWORD cbPolicy, BYTE* /*pContext*/, DWORD /*cbContext*/, DWORD /*dwFlags*/, DWORD /*dwReserved*/)
{
    HRESULT hr = INET_E_DEFAULT_ACTION;

    //#ifndef URLACTION_FEATURE_WINDOW_RESTRICTIONS
    //#define URLACTION_FEATURE_WINDOW_RESTRICTIONS 0x00002102
    //#endif

    if (dwAction == URLACTION_ACTIVEX_RUN ||
        dwAction == URLACTION_SCRIPT_RUN ||

        dwAction == URLACTION_ACTIVEX_OVERRIDE_SCRIPT_SAFETY ||
        dwAction == URLACTION_SCRIPT_OVERRIDE_SAFETY ||
        dwAction == URLACTION_SCRIPT_SAFE_ACTIVEX ||

        dwAction == URLACTION_HTML_SUBMIT_FORMS_FROM ||
        dwAction == URLACTION_HTML_SUBMIT_FORMS_TO || 

        dwAction == URLACTION_FEATURE_WINDOW_RESTRICTIONS ||

        dwAction == URLACTION_SCRIPT_PASTE)
    {
        ZeroMemory(pPolicy, cbPolicy);
        *pPolicy = URLPOLICY_ALLOW;
        hr = S_OK;
    }
    else
    {
        pwszUrl;
        ATLTRACE(_T("URL Action: 0x%X - Default for URL %ls\n"), dwAction, pwszUrl);
    }

    return hr;
}

STDMETHODIMP CCustomSecurityManager::QueryCustomPolicy(LPCWSTR /*pwszUrl*/, REFGUID /*guidKey*/, BYTE** /*ppPolicy*/, DWORD* /*pcbPolicy*/, BYTE* /*pContext*/, DWORD /*cbContext*/, DWORD /*dwReserved*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::SetZoneMapping(DWORD /*dwZone*/, LPCWSTR /*lpszPattern*/, DWORD /*dwFlags*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}

STDMETHODIMP CCustomSecurityManager::GetZoneMappings(DWORD /*dwZone*/, IEnumString** /*ppenumString*/, DWORD /*dwFlags*/)
{
    ATLTRACEMETHOD();
    return INET_E_DEFAULT_ACTION;
}