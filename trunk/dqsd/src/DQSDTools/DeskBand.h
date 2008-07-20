////////////////////////////////////////////////////////////////////////////////////
//
// CDeskBand registers as a shell deskband, and implements the
// relevant interfaces:
//
// - IObjectWithSite for initialization
// - IPersistStream, which signals to the shell that its position can be persisted
// - IDeskBand, which provides band-info and docking window capabilities
//
////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "resource.h"       // main symbols

#include "DQSDTools.h"
#include "BrowserHost.h"

typedef CWinTraits<WS_CHILD, 0> CDeskBandTraits;

class ATL_NO_VTABLE CDeskBand : 
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CDeskBand, &CLSID_DeskBand>,
    public IObjectWithSiteImpl<CDeskBand>,
    public IPersistStreamFakeImpl<CDeskBand>,
    public IDeskBand,
    public CWindowImpl< CDeskBand, CWindow, CDeskBandTraits >
{
public:
    CDeskBand();

    DECLARE_REGISTRY_RESOURCEID(IDR_DESKBAND)
    DECLARE_PROTECT_FINAL_CONSTRUCT()

    // Category map registers object as deskband
    BEGIN_CATEGORY_MAP(CDeskBand)
        IMPLEMENTED_CATEGORY(CATID_DeskBand)
    END_CATEGORY_MAP()

    BEGIN_COM_MAP(CDeskBand)
        COM_INTERFACE_ENTRY(IOleWindow)
        COM_INTERFACE_ENTRY(IDockingWindow)
        COM_INTERFACE_ENTRY(IDeskBand)
        COM_INTERFACE_ENTRY(IObjectWithSite)  // Mostly implemented by IObjectWithSiteImpl
        COM_INTERFACE_ENTRY(IPersist)         // Implemented by IPersistStreamFakeImpl
        COM_INTERFACE_ENTRY(IPersistStream)   // Implemented by IPersistStreamFakeImpl
    END_COM_MAP()

    BEGIN_MSG_MAP(CDeskBand)
        // Empty message map - bad idea?
    END_MSG_MAP()

    HRESULT FinalConstruct();
    void FinalRelease();

public:
    // IOleWindow
    STDMETHODIMP GetWindow(HWND *phwnd);
    STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);

    // IDockingWindow
    STDMETHODIMP ShowDW(BOOL fShow);
    STDMETHODIMP CloseDW(DWORD dwReserved);
    STDMETHODIMP ResizeBorderDW(LPCRECT prcBorder, IUnknown* punkToolbarSite, BOOL fReserved);

    // IDeskBand
    STDMETHODIMP GetBandInfo(DWORD dwBandID, DWORD dwViewMode, DESKBANDINFO* pdbi);

    // IObjectWithSite (partial override)
    STDMETHODIMP SetSite(IUnknown* pUnkSite);

private:
    static HWND GetOleWindow(IUnknown* pUnk);

    CComObject< CBrowserHost >* m_pBrowserHost;

    TCHAR m_szInstallDir[4096];
    TCHAR m_szDeskBandTitle[256];
    TCHAR m_szMessageTitle[256];
};

OBJECT_ENTRY_AUTO(__uuidof(DeskBand), CDeskBand)
