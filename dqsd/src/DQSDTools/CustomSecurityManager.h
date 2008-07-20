// CustomSecurityManager.h : Declaration of the CCustomSecurityManager

#pragma once
#include "resource.h"       // main symbols

class ATL_NO_VTABLE CCustomSecurityManager : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public IInternetSecurityManager
{
public:
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CCustomSecurityManager)
        COM_INTERFACE_ENTRY(IInternetSecurityManager)
    END_COM_MAP()

    HRESULT FinalConstruct()
    {
        return S_OK;
    }

    void FinalRelease() 
    {
    }

public:
    // IInternetSecurityManager methods
    STDMETHODIMP SetSecuritySite(IInternetSecurityMgrSite *pSite);
    STDMETHODIMP GetSecuritySite(IInternetSecurityMgrSite **ppSite);
    STDMETHODIMP MapUrlToZone(LPCWSTR pwszUrl, DWORD *pdwZone, DWORD dwFlags);
    STDMETHODIMP GetSecurityId(LPCWSTR pwszUrl, BYTE *pbSecurityId, DWORD *pcbSecurityId, DWORD_PTR dwReserved);
    STDMETHODIMP ProcessUrlAction(LPCWSTR pwszUrl, DWORD dwAction, BYTE *pPolicy, DWORD cbPolicy, BYTE *pContext, DWORD cbContext, DWORD dwFlags, DWORD dwReserved);
    STDMETHODIMP QueryCustomPolicy(LPCWSTR pwszUrl, REFGUID guidKey, BYTE **ppPolicy, DWORD *pcbPolicy, BYTE *pContext, DWORD cbContext, DWORD dwReserved);
    STDMETHODIMP SetZoneMapping(DWORD dwZone, LPCWSTR lpszPattern, DWORD dwFlags);
    STDMETHODIMP GetZoneMappings(DWORD dwZone, IEnumString **ppenumString, DWORD dwFlags);
};
