// MenuBuilder.h : Declaration of the CMenuBuilder

#ifndef __MENUBUILDER_H_
#define __MENUBUILDER_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMenuBuilder
class ATL_NO_VTABLE CMenuBuilder : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMenuBuilder, &CLSID_MenuBuilder>,
	public IObjectWithSiteImpl<CMenuBuilder>,
	public ISupportErrorInfoImpl<&IID_IMenuBuilder>,
	public IObjectSafetyImpl<CMenuBuilder, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public IDispatchImpl<IMenuBuilder, &IID_IMenuBuilder, &LIBID_DQSDTOOLSLib>
{
public:
	CMenuBuilder() 
	: m_hwndBrowser( NULL )
	, m_nMenuItem( 0 )
	, m_nHorizontalAlignment( 2 )
	{
		m_hMain = ::CreatePopupMenu();
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MENUBUILDER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMenuBuilder)
	COM_INTERFACE_ENTRY(IMenuBuilder)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
//public:
//	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

public:
	STDMETHOD(SetSite)(IUnknown* pUnkSite);

// IMenuBuilder
public:
	STDMETHOD(get_HorizontalAlignment)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_HorizontalAlignment)(/*[in]*/ short newVal);
	STDMETHOD(AppendSubMenu)(/*[in]*/ BSTR bstrName, VARIANT* pvParentMenu, /*[out,retval]*/ long* phmenu);
	STDMETHOD(AppendMenuItem)(/*[in]*/ BSTR bstrItem, /*[in]*/ BSTR bstrKey, /*[in,optional]*/ VARIANT* pvhMenu);
	STDMETHOD(AppendSeparator)(/*[in]*/ long hmenu);
	STDMETHOD(Display)(VARIANT* pbstrSelection);

// Implementation
protected:
	static LPCTSTR	DQSD_REG_KEY;
	static LPCTSTR	DQSD_SEC_KEY;
	HWND			m_hwndBrowser;
	HMENU			m_hMain;
	int				m_nMenuItem;
	std::map< int, std::string > m_mapKeys;
	UINT			m_nHorizontalAlignment;

	CComPtr<IHTMLWindow2> m_spIHTMLWindow2;
	CComPtr<IHTMLDocument2> m_spIHTMLDoc2;

	UINT getHorizontalAlignment();
};

#endif //__MENUBUILDER_H_
