#pragma once

//////////////////////////////////////////////////////////////////////////////
// IDocHostUIHandlerFakeImpl - a fake implementation of IDocHostUIHandler, 
// which doesn't do anything.
//
// Useful for objects that need to implement IDocHostUIHandler just to be 
// able to override GetExternal, for example, as a way of not having
// to list all methods and return E_NOTIMPL.
template <class T>
class ATL_NO_VTABLE IDocHostUIHandlerFakeImpl : public IDocHostUIHandler
{
public:
    // IDocHostUIHandler
    STDMETHOD(ShowContextMenu)(DWORD /*dwID*/, POINT* /*ppt*/, IUnknown* /*pcmdtReserved*/, IDispatch* /*pdispReserved*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(GetHostInfo)(DOCHOSTUIINFO* /*pInfo*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(ShowUI)(DWORD /*dwID*/, IOleInPlaceActiveObject* /*pActiveObject*/, IOleCommandTarget* /*pCommandTarget*/, IOleInPlaceFrame* /*pFrame*/, IOleInPlaceUIWindow* /*pDoc*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(HideUI)(void)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(UpdateUI)(void)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(EnableModeless)(BOOL /*fEnable*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(OnDocWindowActivate)(BOOL /*fActivate*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(OnFrameWindowActivate)(BOOL /*fActivate*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(ResizeBorder)(LPCRECT /*prcBorder*/, IOleInPlaceUIWindow* /*pUIWindow*/, BOOL /*fRameWindow*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(TranslateAccelerator)(LPMSG /*lpMsg*/, const GUID* /*pguidCmdGroup*/, DWORD /*nCmdID*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(GetOptionKeyPath)(LPOLESTR* /*pchKey*/, DWORD /*dw*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(GetDropTarget)(IDropTarget* /*pDropTarget*/, IDropTarget** /*ppDropTarget*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(GetExternal)(IDispatch** ppDispatch)
    {
        ATLTRACEMETHOD();
        *ppDispatch = NULL;
        return E_NOTIMPL;
    }

    STDMETHOD(TranslateUrl)(DWORD /*dwTranslate*/, OLECHAR* /*pchURLIn*/, OLECHAR** /*ppchURLOut*/)
    {
        ATLTRACEMETHOD();
        return E_NOTIMPL;
    }

    STDMETHOD(FilterDataObject)(IDataObject* /*pDO*/, IDataObject** ppDORet)
    {
        ATLTRACEMETHOD();
        *ppDORet = 0;
        return S_FALSE;
    }
};
