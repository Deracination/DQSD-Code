// DeskBand.cpp : Implementation of CDeskBand

#include "stdafx.h"
#include "DeskBand.h"

CDeskBand::CDeskBand()
: m_pBrowserHost(NULL)
{
}

// CDeskBand
HRESULT CDeskBand::FinalConstruct()
{
    ATLTRACEMETHOD();

    CRegKey rk;
    LONG ret = rk.Open(HKEY_CLASSES_ROOT, DQSD_REG_KEY, KEY_READ);
    if (ret != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(ret);

    ULONG dwCount = sizeof(m_szInstallDir) / sizeof(TCHAR);
    ret = rk.QueryStringValue(_T("InstallDir"), m_szInstallDir, &dwCount);
    if (ret != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(ret);

    ATLTRACE(_T("CDeskband::InstallDir = %s\n"), m_szInstallDir);

    dwCount = sizeof(m_szDeskBandTitle) / sizeof(TCHAR);
    ret = rk.QueryStringValue(_T(""), m_szDeskBandTitle, &dwCount);
    if (ret != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(ret);

    ATLTRACE(_T("CDeskband::DeskBandTitle = %s\n"), m_szDeskBandTitle);

    return S_OK;
}

void CDeskBand::FinalRelease() 
{
    ATLTRACEMETHOD();
}

//////////////////////////////////////////////////////////////////////////
/// IOleWindow (parent of IDockingWindow)
STDMETHODIMP CDeskBand::GetWindow(HWND *phwnd)
{
    ATLTRACEMETHOD();

    *phwnd = m_hWnd;
    return S_OK;
}

STDMETHODIMP CDeskBand::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

//////////////////////////////////////////////////////////////////////////
/// IDockingWindow (parent of IDeskBand)
STDMETHODIMP CDeskBand::ShowDW(BOOL fShow)
{
    ATLTRACEMETHOD();

    ATLASSERT(m_pBrowserHost != NULL);

    int nCmdShow = fShow ? SW_SHOW : SW_HIDE;
    ShowWindow(nCmdShow);

    return S_OK;
}

STDMETHODIMP CDeskBand::CloseDW(DWORD)
{
    ATLTRACEMETHOD();
    return ShowDW(FALSE);
}

STDMETHODIMP CDeskBand::ResizeBorderDW(LPCRECT, IUnknown*, BOOL)
{
    ATLASSERT(FALSE && _T("This method is never called for Band Objects"));
    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////
/// IDeskBand
STDMETHODIMP CDeskBand::GetBandInfo(DWORD, DWORD, DESKBANDINFO* pdbi)
{
    ATLTRACEMETHOD();
    if (pdbi == NULL) return E_INVALIDARG;

    if (pdbi->dwMask & DBIM_MINSIZE)
    {
        // These should always be zero. See:
        // http://discussms.hosting.lsoft.com/SCRIPTS/WA-MSD.EXE?A2=ind9901a&L=atl&T=0&O=D&P=2144
        pdbi->ptMinSize.x = 0;
        pdbi->ptMinSize.y = 0;
    }

    if (pdbi->dwMask & DBIM_MAXSIZE)
    {
        pdbi->ptMaxSize.x = -1;
        pdbi->ptMaxSize.y = -1;
    }

    if (pdbi->dwMask & DBIM_INTEGRAL)
    {
        pdbi->ptIntegral.x = 1;
        pdbi->ptIntegral.y = 1;
    }

    if (pdbi->dwMask & DBIM_ACTUAL)
    {
        pdbi->ptActual.x = 50;
        pdbi->ptActual.y = 50;
    }

    if (pdbi->dwMask & DBIM_TITLE)
    {
        lstrcpyW(pdbi->wszTitle, CT2CW(m_szDeskBandTitle));
    }

    if (pdbi->dwMask & DBIM_MODEFLAGS)
    {
        pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;
    }

    if (pdbi->dwMask & DBIM_BKCOLOR)
    {
        // Use the default background color by removing this flag from dwModeFlags
        pdbi->dwModeFlags &= ~DBIMF_BKCOLOR;
    }

    return S_OK;
}

///////////////////////////////////////////////////////////////////////////
/// IObjectWithSite
STDMETHODIMP CDeskBand::SetSite(IUnknown* pUnkSite)
{
    ATLTRACEMETHOD();

    bool bNewSite = !m_spUnkSite.IsEqualObject(pUnkSite);

    // Delegate to base implementation
    HRESULT hr;
    hr = IObjectWithSiteImpl<CDeskBand>::SetSite(pUnkSite);
    if (FAILED(hr)) return hr;

    if (bNewSite)
    {
        // Kill browser host
        if (m_pBrowserHost)
        {
            m_pBrowserHost->Uninitialize();
            m_pBrowserHost->Release();
            m_pBrowserHost = NULL;
        }

        // Destroy the window
        if (m_hWnd)
        {
            DestroyWindow();
            m_hWnd = NULL;
        }
    }

    if (pUnkSite)
    {
        // Create hosting window
        HWND hwndParent = GetOleWindow(pUnkSite);
        if (hwndParent == NULL) return E_FAIL;

        HWND hwndHost = Create(hwndParent);

        // Create new browser host
        hr = CComObject< CBrowserHost >::CreateInstance(&m_pBrowserHost);
        if (FAILED(hr)) return hr;

        m_pBrowserHost->AddRef();

        // Navigate to the search page
        CComBSTR strStartUrl = m_szInstallDir;
        strStartUrl += L"\\search.htm";

        hr = m_pBrowserHost->Initialize(hwndHost, pUnkSite, strStartUrl);
        if (FAILED(hr)) return hr;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
/// Static helpers
HWND CDeskBand::GetOleWindow(IUnknown* pUnk)
{
    ATLTRACEMETHOD();

    HRESULT hr;

    HWND hwnd = NULL;

    // See if this object supports IOleWindow
    CComPtr<IOleWindow> spOleWindow;
    hr = pUnk->QueryInterface(&spOleWindow);
    ATLASSERT(SUCCEEDED(hr) && _T("Passed-in IUnknown must support IOleWindow"));

    if (SUCCEEDED(hr))
    {
        // Get the window handle
        hr = spOleWindow->GetWindow(&hwnd);
        ATLASSERT(SUCCEEDED(hr) && _T("GetWindow failed"));

        // Make sure it's a valid window
        ATLASSERT(::IsWindow(hwnd));
    }

    return hwnd;
}
