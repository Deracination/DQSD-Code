
//
// DQSDTools ActiveX server for Dave's Quick Search Deskbar
//  Copyright (c) 2001 Koen Mannaerts
//  Distributed under the terms of the 
//  GNU Public License, Version 2
//  http://www.WhirlyWiryWeb.com
//
// Dave's Quick Search Deskbar
//	Copyright (c) 2001 David Bau, 
//  Distributed under the terms of the 
//  GNU Public License, Version 2
//  http://notesbydave.com/toolbar/searchdoc.htm
//

// Launcher.h : Declaration of the CLauncher

#ifndef __LAUNCHER_H_A43187EC_B518_40a5_9615_C628D10567E4__
#define __LAUNCHER_H_A43187EC_B518_40a5_9615_C628D10567E4__

#include "resource.h"       // main symbols
#include <atlctl.h>					// IObjectSafety,
#include <exdisp.h>					// IWebBrowser2,
#include <mshtml.h>
#include <shlguid.h>				// SID_SWebBrowserApp, 
#include <shellapi.h>				// ShellExecute, 

/////////////////////////////////////////////////////////////////////////////
// CLauncher
class ATL_NO_VTABLE CLauncher : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLauncher, &CLSID_Launcher>,
	public IDispatchImpl<ILauncher, &IID_ILauncher, &LIBID_DQSDTOOLSLib>,
	public IObjectWithSiteImpl<CLauncher>,
	public IObjectSafetyImpl<CLauncher, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
  public ISupportErrorInfoImpl< &IID_ILauncher >

{
public:
	CLauncher()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LAUNCHER)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CLauncher)
	COM_INTERFACE_ENTRY(ILauncher)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ILauncher
public:
	STDMETHOD(get_pathDefaultBrowser)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(OpenDocument)(/*[in]*/ BSTR strDoc);
	STDMETHOD(SubmitForm)(/*[in]*/ VARIANT idForm);
	STDMETHOD(SetSite)(IUnknown* pUnkSite);


};

#endif // __LAUNCHER_H_A43187EC_B518_40a5_9615_C628D10567E4__
