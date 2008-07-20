#pragma once

//////////////////////////////////////////////////////////////////////////////
// IOleClientSiteFakeImpl - a fake implementation of IOleClientSite, which 
// doesn't do anything.
//
// Useful for objects that need to implement IOleClientSite just to be able to
// pass themselves to IOleObject::DoVerb
template <class T>
class ATL_NO_VTABLE IOleClientSiteFakeImpl : public IOleClientSite
{
public:
    // IOleClientSite
    STDMETHOD(SaveObject)()
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        return E_NOTIMPL;
    }

    STDMETHOD(GetMoniker)(DWORD /*dwAssign*/, DWORD /*dwWhichMoniker*/, LPMONIKER* /*ppmk*/)
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        return E_NOTIMPL;
    }

    STDMETHOD(GetContainer)(LPOLECONTAINER* ppContainer)
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        *ppContainer = NULL;
        return E_NOINTERFACE;
    }

    STDMETHOD(ShowObject)()
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        return S_OK;
    }

    STDMETHOD(OnShowWindow)(BOOL /*fShow*/)
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        return S_OK;
    }

    STDMETHOD(RequestNewObjectLayout)()
    {
        ATLTRACE(_T("%s\n"), __FUNCTION__);
        return E_NOTIMPL;
    }
};