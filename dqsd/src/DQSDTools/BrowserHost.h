#pragma once
#include "resource.h"       // main symbols

#include "CustomSecurityManager.h"
#include "DQSDTools.h"

class CBrowserHost :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CWindowImpl<CBrowserHost>,
    public IOleClientSiteFakeImpl<CBrowserHost>,
    public IServiceProviderImpl<CBrowserHost>,
    public IDocHostUIHandlerFakeImpl<CBrowserHost>,
    public IOleInPlaceSite,
    public IOleControlSite,
    public IDocHostShowUI,
    public INewWindowManager,
    public IInputObject,
    private IDispEventImpl<1, CBrowserHost, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 1>
{
private:
    enum
    {
        ALTMSGMAP_WEBBROWSER = 1,
        ALTMSGMAP_HOST
    };

public:
    CBrowserHost();

    DECLARE_NO_REGISTRY();
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    // Service map to get IInternetSecurityManager support
    BEGIN_SERVICE_MAP(CBrowserHost)
        SERVICE_ENTRY_TEAROFF(SID_SInternetSecurityManager, CCustomSecurityManager)
        SERVICE_ENTRY(SID_SNewWindowManager)
        SERVICE_ENTRY_CHAIN(m_spUnkSite)
    END_SERVICE_MAP()

    BEGIN_COM_MAP(CBrowserHost)
        COM_INTERFACE_ENTRY(IOleWindow)
        COM_INTERFACE_ENTRY(IOleInPlaceSite)
        COM_INTERFACE_ENTRY(IOleControlSite)
        COM_INTERFACE_ENTRY(IDocHostUIHandler)	// Mostly implemented by IDocHostUIHandlerFakeImpl
        COM_INTERFACE_ENTRY(IDocHostShowUI)
        COM_INTERFACE_ENTRY(INewWindowManager)
        COM_INTERFACE_ENTRY(IServiceProvider)		// Implemented by IServiceProviderImpl
        COM_INTERFACE_ENTRY(IOleClientSite)			// Implemented by IOleClientSiteFakeImpl
        COM_INTERFACE_ENTRY_IID(IID_IInputObject, IInputObject)
    END_COM_MAP()

    BEGIN_MSG_MAP(CBrowserHost)
        ALT_MSG_MAP(ALTMSGMAP_HOST)
        MESSAGE_HANDLER(WM_SIZE, OnSize)

        ALT_MSG_MAP(ALTMSGMAP_WEBBROWSER)
        MESSAGE_HANDLER(WM_KEYDOWN, OnBrowserKeyDown)
    END_MSG_MAP()

    BEGIN_SINK_MAP(CBrowserHost)
        SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigateComplete2)
        SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_WINDOWCLOSING, OnWindowClosing)
    END_SINK_MAP()

    HRESULT FinalConstruct();
    void FinalRelease();

    HRESULT Initialize(HWND hwndHost, IUnknown* pUnkSite, BSTR bstrUrl);
    HRESULT Uninitialize();
    HRESULT SetMessageTitle(LPCTSTR pszMessageTitle);

private:
    // Message handlers
    LRESULT OnSize(UINT, WPARAM, LPARAM lParam, BOOL&);
    LRESULT OnBrowserKeyDown(UINT, WPARAM, LPARAM lParam, BOOL&);

    // Event sink
    void STDMETHODCALLTYPE OnNavigateComplete2(IDispatch *pDisp, VARIANT*);
    void STDMETHODCALLTYPE OnWindowClosing(VARIANT_BOOL, VARIANT_BOOL*);


public:
    // IOleWindow
    STDMETHODIMP GetWindow(HWND *phwnd);
    STDMETHODIMP ContextSensitiveHelp(BOOL /*fEnterMode*/);

    // IOleInPlaceSite
    STDMETHODIMP CanInPlaceActivate();
    STDMETHODIMP OnInPlaceActivate();
    STDMETHODIMP OnUIActivate();
    STDMETHODIMP GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo);
    STDMETHODIMP Scroll(SIZE scrollExtent);
    STDMETHODIMP OnUIDeactivate(BOOL fUndoable);
    STDMETHODIMP OnInPlaceDeactivate();
    STDMETHODIMP DiscardUndoState();
    STDMETHODIMP DeactivateAndUndo();
    STDMETHODIMP OnPosRectChange(LPCRECT lprcPosRect); 

    // IOleControlSite
    STDMETHODIMP OnControlInfoChanged();
    STDMETHODIMP LockInPlaceActive(BOOL fLock);
    STDMETHODIMP GetExtendedControl(LPDISPATCH* ppDisp);
    STDMETHODIMP TransformCoords(POINTL* pPtlHimetric, POINTF* pPtfContainer, DWORD dwFlags);
    STDMETHODIMP TranslateAccelerator(LPMSG lpMsg, DWORD grfModifiers);
    STDMETHODIMP OnFocus(BOOL fGotFocus);
    STDMETHODIMP ShowPropertyFrame();

    // IDocHostShowUI
    STDMETHODIMP ShowMessage(HWND hwnd, LPOLESTR lpstrText, LPOLESTR lpstrCaption, DWORD dwType, LPOLESTR lpstrHelpFile, DWORD dwHelpContext, LRESULT *plResult);
    STDMETHODIMP ShowHelp(HWND hwnd, LPOLESTR pszHelpFile, UINT uCommand, DWORD dwData, POINT ptMouse, IDispatch *pDispatchObjectHit);

    // INewWindowManager
    STDMETHODIMP EvaluateNewWindow(LPCWSTR pszUrl, LPCWSTR pszName, LPCWSTR pszUrlContext, LPCWSTR pszFeatures, BOOL fReplace, DWORD dwFlags, DWORD dwUserActionTime);

    // IInputObject
    STDMETHODIMP UIActivateIO(BOOL fActivate, LPMSG lpMsg);
    STDMETHODIMP HasFocusIO();
    STDMETHODIMP TranslateAcceleratorIO(LPMSG lpMsg);

    // IDocHostUIHandler (override relevant methods)
    STDMETHODIMP GetHostInfo(DOCHOSTUIINFO *pInfo);
    STDMETHODIMP UpdateUI(void);
    STDMETHODIMP EnableModeless(BOOL fEnable);
    STDMETHODIMP TranslateAccelerator(LPMSG lpMsg, const GUID* /*pguidCmdGroup*/, DWORD /*nCmdID*/);

private:
    static HWND GetOleWindow(IUnknown* pUnk);
    static BOOL NeedsTranslation(LPMSG pMsg);

    HRESULT SetupWebBrowser(HWND hwndHost, IWebBrowser2** ppWebBrowser);
    HRESULT ResizeControl(IUnknown* pControl, LPRECT pRect = NULL);

    CComPtr<IUnknown> m_spUnkSite;
    CComPtr<IWebBrowser2> m_spWebBrowser;

    CContainedWindow m_wndHtmlDoc;
    CContainedWindow m_wndHost;

    CComBSTR m_strMessageTitle;
};
