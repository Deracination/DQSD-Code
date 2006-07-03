#include "StdAfx.h"
#include "BrowserHost.h"

CBrowserHost::CBrowserHost()
    : m_wndHtmlDoc(this, ALTMSGMAP_WEBBROWSER)
    , m_wndHost(this, ALTMSGMAP_HOST)
{
}

// CBrowserHost
HRESULT CBrowserHost::FinalConstruct()
{
    ATLTRACEMETHOD();

    CRegKey rk;
    LONG ret = rk.Open(HKEY_CLASSES_ROOT, DQSD_REG_KEY, KEY_READ);
    if (ret != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(ret);

    ULONG dwCount = sizeof(m_szMessageBoxTitle) / sizeof(TCHAR);
    ret = rk.QueryStringValue(_T("HelpText"), m_szMessageBoxTitle, &dwCount);
    if (ret != ERROR_SUCCESS)
        return HRESULT_FROM_WIN32(ret);

    ATLTRACE(_T("CBrowserHost::MessageBoxTitle = %s\n"), m_szMessageBoxTitle);

    return S_OK;
}

void CBrowserHost::FinalRelease() 
{
    ATLTRACEMETHOD();
}

HRESULT CBrowserHost::Initialize(HWND hwndHost, IUnknown* pUnkSite, BSTR bstrUrl)
{
    ATLTRACEMETHOD();

    HRESULT hr;

    if (pUnkSite == NULL) return E_INVALIDARG;

    m_spUnkSite = pUnkSite;

    // Enforce invariants
    ATLASSERT(!IsWindow());
    ATLASSERT(m_spWebBrowser == NULL);

    m_wndHost.SubclassWindow(hwndHost);

    // Create web browser control, and in-place activate it
    hr = SetupWebBrowser(hwndHost, &m_spWebBrowser);
    if (FAILED(hr)) return hr;

    // Sink events
    hr = DispEventAdvise(m_spWebBrowser);
    if (FAILED(hr)) return hr;

    CComVariant vaEmpty;
    hr = m_spWebBrowser->Navigate(bstrUrl, &vaEmpty, &vaEmpty, &vaEmpty, &vaEmpty);
    if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT CBrowserHost::Uninitialize()
{
    ATLTRACEMETHOD();

    HRESULT hr;

    if (m_wndHtmlDoc.IsWindow())
    {
        m_wndHtmlDoc.UnsubclassWindow();
    }

    if (m_wndHost.IsWindow())
    {
        m_wndHost.UnsubclassWindow();
    }

    if (m_spWebBrowser)
    {
        // Unadvise event subscription
        hr = DispEventUnadvise(m_spWebBrowser);
        ATLASSERT(SUCCEEDED(hr));

        // Reset the browser's client site - necessary to decrease my reference count
        CComQIPtr<IOleObject> spOleObject = m_spWebBrowser;
        m_spWebBrowser.Release();

        if (spOleObject)
        {
            hr = spOleObject->SetClientSite(NULL);
            ATLASSERT(SUCCEEDED(hr));
        }
        else
        {
            ATLASSERT(FALSE && _T("Web browser must implement IOleObject!"));
        }
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////
/// Message handlers
LRESULT CBrowserHost::OnSize(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
    ATLTRACEMETHOD();

    // Resize browser control to match new window size
    if (FAILED(ResizeControl(m_spWebBrowser, NULL)))
    {
        bHandled = FALSE;
    }

    return 0;
}

LRESULT CBrowserHost::OnBrowserKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ATLTRACEMETHOD();

    //HRESULT hr;

    // // Get an IOleInPlaceActiveObject pointer, and let it translate 
    // // the accelerator for us
    // CComPtr<IOleInPlaceActiveObject> spInPlaceActiveObject;
    // hr = m_spWebBrowser->QueryInterface(&spInPlaceActiveObject);
    // if (SUCCEEDED(hr))
    // {
    //	// Clone the message
    //	MSG msg = {0};
    //	msg.hwnd = GetFocus();
    //	msg.message = uMsg;
    //	msg.wParam = wParam;
    //	msg.lParam = lParam;

    //	hr = spInPlaceActiveObject->TranslateAccelerator(&msg);
    //	bHandled = (hr == S_OK);
    //}

    // Clone the message
    MSG msg = {0};
    msg.hwnd = GetFocus();
    msg.message = uMsg;
    msg.wParam = wParam;
    msg.lParam = lParam;

    // Forward to TranslateAcceleratorIO
    bHandled = (TranslateAcceleratorIO(&msg) == S_OK);

    return bHandled ? 0 : 1;
}

//////////////////////////////////////////////////////////////////////////
/// IOleWindow
STDMETHODIMP CBrowserHost::GetWindow(HWND *phwnd)
{
    ATLTRACEMETHOD();
    *phwnd = m_wndHost.m_hWnd;
    return S_OK;
}

STDMETHODIMP CBrowserHost::ContextSensitiveHelp(BOOL /*fEnterMode*/)
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

//////////////////////////////////////////////////////////////////////////
/// IOleInPlaceSite
STDMETHODIMP CBrowserHost::CanInPlaceActivate()
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::OnInPlaceActivate()
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::OnUIActivate()
{
    ATLTRACEMETHOD();
    return E_NOTIMPL;
}

STDMETHODIMP CBrowserHost::GetWindowContext(IOleInPlaceFrame** /*ppFrame*/, IOleInPlaceUIWindow** /*ppIIPUIWin*/, 
                                            LPRECT lprcPosRect, LPRECT lprcClipRect,
                                            LPOLEINPLACEFRAMEINFO /*lpFrameInfo*/)
{
    ATLTRACEMETHOD();

    m_wndHost.GetClientRect(lprcPosRect);
    m_wndHost.GetClientRect(lprcClipRect);

    return S_OK;
}

STDMETHODIMP CBrowserHost::Scroll(SIZE /*scrollExtent*/)
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

STDMETHODIMP CBrowserHost::OnUIDeactivate(BOOL /*fUndoable*/)
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::OnInPlaceDeactivate()
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::DiscardUndoState()
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

STDMETHODIMP CBrowserHost::DeactivateAndUndo()
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

STDMETHODIMP CBrowserHost::OnPosRectChange(LPCRECT /*lprcPosRect*/) 
{
    ATLTRACENOTIMPL(__FUNCTION__);
}

///////////////////////////////////////////////////////////////////////////
/// IOleControlSite
STDMETHODIMP CBrowserHost::OnControlInfoChanged()
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::LockInPlaceActive(BOOL /*fLock*/)
{
    ATLTRACEMETHOD();
    return S_OK;
}

STDMETHODIMP CBrowserHost::GetExtendedControl(LPDISPATCH* ppDisp)
{
    ATLTRACEMETHOD();

    if (ppDisp == NULL) return E_POINTER;

    return m_spWebBrowser.QueryInterface(ppDisp);
}

STDMETHODIMP CBrowserHost::TransformCoords(POINTL* /*pPtlHimetric*/, POINTF* /*pPtfContainer*/, DWORD /*dwFlags*/)
{
    ATLTRACEMETHOD();
    return E_NOTIMPL;
}

STDMETHODIMP CBrowserHost::TranslateAccelerator(LPMSG /*lpMsg*/, DWORD /*grfModifiers*/)
{
    ATLTRACEMETHOD();
    return S_FALSE;
}

STDMETHODIMP CBrowserHost::OnFocus(BOOL fGotFocus)
{
    //ATLTRACEMETHOD();

    HRESULT hr;

    if (fGotFocus)
    {
        ATLTRACE(_T("%s - got focus\n"), __FUNCTION__);
    }
    else
    {
        ATLTRACE(_T("%s - lost focus\n"), __FUNCTION__);
    }

    ATLASSERT(m_spUnkSite != NULL);

    CComPtr<IInputObjectSite> spInputObjectSite;
    hr = m_spUnkSite->QueryInterface(IID_IInputObjectSite, (void**)&spInputObjectSite);
    if (SUCCEEDED(hr))
    {
        hr = spInputObjectSite->OnFocusChangeIS(GetUnknown(), fGotFocus);
        ATLTRACE(_T("%s - OnFocusChangeIS returned 0x%X\n"), __FUNCTION__, hr);
    }

    return S_OK;
}

STDMETHODIMP CBrowserHost::ShowPropertyFrame()
{
    ATLTRACEMETHOD();
    return E_NOTIMPL;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// IDocHostUIHandler implementation
STDMETHODIMP CBrowserHost::GetHostInfo(DOCHOSTUIINFO *pInfo)
{
    ATLTRACEMETHOD();

    pInfo->cbSize = sizeof(DOCHOSTUIINFO);
    pInfo->dwDoubleClick = DOCHOSTUIDBLCLK_DEFAULT;

    DWORD dwFlags = pInfo->dwFlags;

    // Mask off script disable flag
    dwFlags &= ~DOCHOSTUIFLAG_DISABLE_SCRIPT_INACTIVE;
    pInfo->dwFlags = dwFlags;

    return S_OK;
}

STDMETHODIMP CBrowserHost::UpdateUI()
{
    return S_OK;
}

STDMETHODIMP CBrowserHost::EnableModeless(BOOL /*fEnable*/)
{
    ATLTRACEMETHOD();
    return S_OK;
}

//STDMETHODIMP CBrowserHost::GetExternal(IDispatch **ppDispatch)
//{
//    ATLTRACEMETHOD();
//
//    HRESULT hr;
//
//    CComObject<CWindowLauncher>* pLauncher = 0;
//    hr = CComObject<CWindowLauncher>::CreateInstance(&pLauncher);
//    if (FAILED(hr)) return hr;
//
//    pLauncher->AddRef();
//
//    hr = pLauncher->QueryInterface(ppDispatch);
//
//    pLauncher->Release();
//
//    return hr;
//}

STDMETHODIMP CBrowserHost::TranslateAccelerator(LPMSG /*pMsg*/, const GUID* /*pguidCmdGroup*/, DWORD /*nCmdID*/)
{
    ATLTRACE(_T("CBrowserHost::IDocHostUIHandler::TranslateAccelerator\n"));
    return S_FALSE;
}


//////////////////////////////////////////////////////////////////////////
/// IDocHostShowUI
STDMETHODIMP CBrowserHost::ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR /*lpstrCaption*/, DWORD dwType, LPOLESTR /*lpstrHelpFile*/, DWORD /*dwHelpContext*/, LRESULT *plResult)
{
    ATLTRACEMETHOD();

    // Custom error reporting!
    int mbRet = ::MessageBox(hwnd, COLE2T(lpstrText), m_szMessageBoxTitle, dwType);

    // If ShowMessage is called when you have specified no execution of ActiveX 
    // controls and there is an ActiveX control on the page, plResult is NULL
    if (plResult)
    {
        *plResult = mbRet;
    }

    return S_OK;
}

STDMETHODIMP CBrowserHost::ShowHelp(HWND /*hwnd*/, LPOLESTR /*pszHelpFile*/, UINT /*uCommand*/, DWORD /*dwData*/, POINT /*ptMouse*/, IDispatch* /*pDispatchObjectHit*/)
{
    ATLTRACEMETHOD();
    // S_FALSE allows onhelp invocations to bubble through
    return S_FALSE;
}

//////////////////////////////////////////////////////////////////////////
/// INewWindowManager
STDMETHODIMP CBrowserHost::EvaluateNewWindow(LPCWSTR /*pszUrl*/, LPCWSTR /*pszName*/, LPCWSTR /*pszUrlContext*/, LPCWSTR /*pszFeatures*/, BOOL /*fReplace*/, DWORD /*dwFlags*/, DWORD /*dwUserActionTime*/)
{
    ATLTRACEMETHOD();
    // Always allow popups
    return S_OK;
}

///////////////////////////////////////////////////////////////////////////
/// IInputObject
STDMETHODIMP CBrowserHost::UIActivateIO(BOOL fActivate, LPMSG /*pMsg*/)
{
    ATLTRACEMETHOD();

    if (fActivate)
    {
        // TODO: Shouldn't we UI-activate?

        // Set focus and translate any keyboard messages
        // SetFocus();
        // if (pMsg)
        // {
        //   TranslateAcceleratorIO(pMsg);
        // }
    }
    else
    {
        // UI-deactivate
        CComQIPtr<IOleInPlaceObject> spInPlaceObj = m_spWebBrowser;
        if (spInPlaceObj)
        {
            spInPlaceObj->UIDeactivate();
        }
    }

    return S_OK;
}

// If the browser window or one of its descendants has the focus, return S_OK or S_FALSE otherwise
STDMETHODIMP CBrowserHost::HasFocusIO()
{
    ATLTRACEMETHOD();

    HWND hwnd = GetFocus();
    HWND hwndTmp = GetOleWindow(m_spWebBrowser);

    // See if the focus has been set to any of the children
    while (hwnd && hwndTmp)
    {
        if (hwnd == hwndTmp) return S_OK;
        hwndTmp = ::GetWindow(hwndTmp, GW_CHILD);
    }

    return S_FALSE;
}

// If the accelerator is translated, return S_OK or S_FALSE otherwise.
STDMETHODIMP CBrowserHost::TranslateAcceleratorIO(LPMSG pMsg)
{
    ATLTRACEMETHOD();

    HRESULT hr = S_FALSE;

    if (NeedsTranslation(pMsg))
    {
        CComPtr<IDispatch> spApp;
        hr = m_spWebBrowser->get_Application(&spApp);
        if (SUCCEEDED(hr))
        {
            CComPtr<IOleInPlaceActiveObject> spOleInPlaceActiveObject;
            hr = spApp->QueryInterface(&spOleInPlaceActiveObject);
            if (SUCCEEDED(hr))
            {
                hr = spOleInPlaceActiveObject->TranslateAccelerator(pMsg);
            }
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////
/// Event sink
void STDMETHODCALLTYPE CBrowserHost::OnNavigateComplete2(IDispatch *pDisp, VARIANT*)
{
    ATLTRACEMETHOD();

    HRESULT hr;

    if (m_spWebBrowser.IsEqualObject(pDisp))
    {
        // Get the current document
        CComPtr<IDispatch> spDocument;
        hr = m_spWebBrowser->get_Document(&spDocument);
        if (SUCCEEDED(hr))
        {
            // Get document window and subclass it to be able to handle accelerators
            HWND hwndDoc = GetOleWindow(spDocument);
            if (m_wndHtmlDoc != hwndDoc)
            {
                if (m_wndHtmlDoc.IsWindow())
                {
                    m_wndHtmlDoc.UnsubclassWindow();
                }

                if (hwndDoc != NULL)
                {
                    m_wndHtmlDoc.SubclassWindow(hwndDoc);
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
/// Private methods
HRESULT CBrowserHost::SetupWebBrowser(HWND hwndHost, IWebBrowser2** ppWebBrowser)
{
    ATLTRACEMETHOD();

    HRESULT hr;

    // Create and initialize the web browser control that we are hosting.
    CComPtr<IWebBrowser2> spBrowser;
    hr = spBrowser.CoCreateInstance(CLSID_WebBrowser);
    if (FAILED(hr)) return hr;

    // Don't register as top-level browser
    hr = spBrowser->put_RegisterAsBrowser(VARIANT_FALSE);
    if (FAILED(hr)) return hr;

    // Set the browser's client site
    CComPtr<IOleObject> spOleObject;
    hr = spBrowser->QueryInterface(&spOleObject);
    if (FAILED(hr)) return hr;

    hr = spOleObject->SetClientSite(this);
    if (FAILED(hr)) return hr;

    // In-place activate the web browser control
    RECT rcClient = { CW_USEDEFAULT, 0, 0, 0 };
    ::GetClientRect(hwndHost, &rcClient);

    MSG msg = {0};
    hr = spOleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, &msg, this, 0, hwndHost, &rcClient);
    if (FAILED(hr)) return hr;

    // Set the browser control to the same size as the containing window
    hr = ResizeControl(spBrowser, &rcClient);
    if (FAILED(hr)) return hr;

    // Return browser object
    *ppWebBrowser = spBrowser.Detach();

    return S_OK;
}

HRESULT CBrowserHost::ResizeControl(IUnknown* pControl, LPRECT pRect /* = NULL */)
{
    ATLTRACEMETHOD();

    HRESULT hr = S_OK;

    if (pControl)
    {
        if (pRect == NULL)
        {
            // If no RECT is provided, use the current client rect
            RECT rcClient = {0};
            m_wndHost.GetClientRect(&rcClient);

            // Recurse in one step to get into other branch
            hr = ResizeControl(pControl, &rcClient);
        }
        else
        {
            // Resize in-place object
            CComPtr<IOleInPlaceObject> spInPlaceObject;
            hr = pControl->QueryInterface(&spInPlaceObject);
            if (FAILED(hr)) return hr;

            hr = spInPlaceObject->SetObjectRects(pRect, pRect);
        }
    }

    return hr;
}

//////////////////////////////////////////////////////////////////////////
/// Static helpers
HWND CBrowserHost::GetOleWindow(IUnknown* pUnk)
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

BOOL CBrowserHost::NeedsTranslation(LPMSG pMsg)
{
    // Note: Need this method to avoid repeating Enter keypress, for example.

    ATLTRACEMETHOD();

    // Only keydown messages need processing
    if (pMsg->message != WM_KEYDOWN)
    {
        return FALSE;
    }

    // Get state of modifier keys
    BOOL bCtrlState = ((GetKeyState(VK_CONTROL) & 0x80000000) > 0);
    BOOL bShiftState = ((GetKeyState(VK_SHIFT) & 0x80000000) > 0);

    // Pick the specific keypresses we want to pass through
    // TranslateAcceleratorIO
    if (pMsg->wParam == VK_DELETE)
    {
        // Shift+Delete, Delete or Ctrl+Delete
        return TRUE;
    }
    else if (pMsg->wParam == VK_INSERT)
    {
        // Shift+Insert or Ctrl+Insert
        return (bCtrlState || bShiftState);
    }
    else if (pMsg->wParam == 'V' || pMsg->wParam == 'X' || pMsg->wParam == 'C')
    {
        // Ctrl+V, Ctrl+X, Ctrl+C
        return bCtrlState;
    }
    else if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
    {
        // Arrow keys
        return TRUE;
    }

    return FALSE;
}
