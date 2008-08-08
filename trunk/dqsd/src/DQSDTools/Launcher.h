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
	STDMETHOD(OpenDocument)(/*[in]*/ BSTR strDoc, VARIANT* pvParameters);
	STDMETHOD(SubmitForm)(/*[in]*/ VARIANT idForm);
	STDMETHOD(get_Debug)(/*[out,retval]*/ VARIANT_BOOL* pbDebug );
	STDMETHOD(put_Debug)(/*[in]*/ VARIANT_BOOL bDebug );
	STDMETHOD(ReadFile)(/*[in]*/ BSTR bstrFilename, /*[out,retval]*/ BSTR* pbstrResult);
	STDMETHOD(WriteFile)(/*[in]*/ BSTR bstrFilename, /*[in]*/ BSTR bstrValue);
	STDMETHOD(GetProtocolHandler)(/*[in]*/ BSTR bstrProtocol, /*[out,retval]*/ BSTR* pbstrHandler);
	STDMETHOD(GetFiles)(/*[in]*/ BSTR bstrFileSpec, /*[out,retval]*/ BSTR* pbstrFiles);
	STDMETHOD(get_InstallationDirectory)(/*[out,retval]*/ BSTR* pbstrDirectory);
	STDMETHOD(GetSpecialFolderLocation)(/*[in]*/ BSTR bstrSpecialFolder, /*[out,retval]*/ BSTR* pbstrLocation);
	STDMETHOD(GetFolders)(/*[in]*/ BSTR bstrBaseFolder, /*[out,retval]*/ BSTR* pbstrFolders);
	STDMETHOD(FileExists)(/*[in]*/ BSTR bstrFilename, /*[out,retval]*/ VARIANT_BOOL* pbExists);
	STDMETHOD(RenameFile)(/*[in]*/ BSTR bstrFromFilename, /*[in]*/ BSTR bstrToFilename);
	STDMETHOD(RegisterHotKey)(long hotkeyVkCode, BSTR bstrModifierName, LPDISPATCH pDispDocument);
	STDMETHOD(RefreshTrayIcons)();
	STDMETHOD(ShutdownBar)(LPDISPATCH pDispDocument );
	STDMETHOD(get_VersionIsCorrect)(int v1, int v2, int v3, int v4, /*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(SetSite)(IUnknown* pUnkSite);
	STDMETHOD(get_AppDataDirectory)(/*[out,retval]*/ BSTR* pbstrDirectory);
	STDMETHOD(CreateDirectory)(/*[in]*/ BSTR bstrDirectory);
	STDMETHOD(DisplayUserOptions)();

    HRESULT FinalConstruct();
    void FinalRelease();

// Implementation
private:
    HRESULT GetFilename( LPCTSTR szName, LPTSTR szResult, LPCTSTR pszDefaultExt = _T(".txt") );
    HRESULT GetInstallationDirectory( LPTSTR szResult, DWORD dwResultSize);
    BOOL VerifyFileInDirectoryTree( LPCTSTR szFilename, LPCTSTR szDir);
    BOOL IsValidFileDirectory(LPCTSTR szFilename);
    BOOL IsFileExtension( LPCTSTR szFilename, LPCTSTR szExts);

	HHOOK	m_hKeyboardHook;
	HWND	m_hHotkeyNotificationWindow;
	TCHAR	m_szInstallDir[MAX_PATH];
};

OBJECT_ENTRY_AUTO(__uuidof(Launcher), CLauncher);

#endif // __LAUNCHER_H_A43187EC_B518_40a5_9615_C628D10567E4__
