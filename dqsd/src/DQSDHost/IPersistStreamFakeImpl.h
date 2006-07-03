#pragma once

//////////////////////////////////////////////////////////////////////////////
// IPersistStreamFakeImpl - a fake implementation of IPersistStream, which 
// doesn't do anything besides return the correct CLSID from GetClassID
//
// Useful for objects that need to implement IPersistStream, but don't have
// anything to persist (think desk bands)
template <class T>
class ATL_NO_VTABLE IPersistStreamFakeImpl : public IPersistStream
{
public:
    // IPersist
    STDMETHOD(GetClassID)(CLSID *pClassID)
    {
        if (pClassID == NULL) return E_POINTER;

        *pClassID = T::GetObjectCLSID();
        return S_OK;
    }

    // IPersistStream
    STDMETHOD(IsDirty)()
    {
        return S_FALSE;
    }

    STDMETHOD(Load)(LPSTREAM /*pStm*/)
    {
        return S_OK;
    }

    STDMETHOD(Save)(LPSTREAM /*pStm*/, BOOL /*fClearDirty*/)
    {
        return S_OK;
    }

    STDMETHOD(GetSizeMax)(ULARGE_INTEGER* /*pcbSize*/)
    {
        return E_NOTIMPL;
    }
};
