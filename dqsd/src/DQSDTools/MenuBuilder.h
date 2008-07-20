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
	: m_nMenuItem( 0 )
	, m_nHorizontalAlignment( EHorizontalAlignment::RIGHT )
	{
		m_hMain = ::CreatePopupMenu();

		ATLTRACE("CMenuBuilder - created\n");
		m_hTrackerWnd = NULL;
		m_hTooltipWnd = NULL;
	}

	virtual ~CMenuBuilder()
	{
		if(m_hTrackerWnd != NULL)
		{
			DestroyWindow(m_hTrackerWnd);
		}
		if(m_hTooltipWnd != NULL)
		{
			DestroyWindow(m_hTooltipWnd);
		}

		ATLTRACE("CMenuBuilder - destroyed\n");
		ATLTRACE("DQSDTools: Lock count %d\n", _Module.GetLockCount());
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

public:
	STDMETHOD(SetSite)(IUnknown* pUnkSite);

// IMenuBuilder
public:
	STDMETHOD(InitialiseTooltips)(long displayTimeMultiplier);
	STDMETHOD(get_HorizontalAlignment)(/*[out, retval]*/ short *pVal);
	STDMETHOD(put_HorizontalAlignment)(/*[in]*/ short newVal);
	STDMETHOD(AppendSubMenu)(/*[in]*/ BSTR bstrName, VARIANT* pvParentMenu, /*[out,retval]*/ long* phmenu);
	STDMETHOD(AppendMenuItem)(/*[in]*/ BSTR bstrItem, /*[in]*/ BSTR bstrKey, /*[in]*/ BSTR bstrToolTip, /*[in,optional]*/ VARIANT* pvhMenu);
	STDMETHOD(AppendSeparator)(/*[in,optional]*/ VARIANT* pvParentMenu);
	STDMETHOD(Display)(LPDISPATCH pDispDocument, VARIANT* pbstrSelection);
	STDMETHOD(InsertMenuItem)(/*[in]*/ BSTR bstrItem, /*[in]*/ BSTR bstrKey, /*[in]*/ BSTR bstrToolTip, /*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvhMenu );
	STDMETHOD(InsertSeparator)(/*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu);
	STDMETHOD(InsertSubMenu)(/*[in]*/ BSTR bstrName, /*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu, /*[out,retval]*/ long *phmenu);
	STDMETHOD(GetMenuItemCount)(/*[in,optional]*/ VARIANT* pvParentMenu, /*[out,retval]*/ long *pCount);
	STDMETHOD(FindSubMenu)(/*[in]*/ BSTR bstrName, /*[in,optional]*/ VARIANT* pvParentMenu, /*[out,retval]*/ long *phmenu);
	STDMETHOD(FindMenuItem)(/*[in]*/ BSTR bstrName, /*[in,optional]*/ VARIANT* pvParentMenu, /*[out,retval]*/ long *pPosition);
	STDMETHOD(GetMenuString)(/*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu, /*[out, retval]*/ BSTR* pbstrResult);
	STDMETHOD(GetMenuItemID)(/*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu, /*[out, retval]*/ long *nID);
	STDMETHOD(EnableMenuItem)(/*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu);
	STDMETHOD(DisableMenuItem)(/*[in]*/ UINT position, /*[in,optional]*/ VARIANT* pvParentMenu);

// Implementation
protected:
	HMENU			m_hMain;
	int				m_nMenuItem;
	std::map< int, std::string > m_mapKeys;
	std::map< int, std::string > m_toolTips;
	
	UINT			m_nHorizontalAlignment;

	CComPtr<IHTMLWindow2> m_spIHTMLWindow2;
	CComPtr<IHTMLDocument2> m_spIHTMLDoc2;

	UINT getHorizontalPosition();

	enum EHorizontalAlignment
	{
		CENTER,
		LEFT,
		RIGHT,
	};

protected:
	static LRESULT CALLBACK TrackerWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	);

	HWND			m_hTrackerWnd;
	static HWND		m_hTooltipWnd;

};

#endif //__MENUBUILDER_H_
