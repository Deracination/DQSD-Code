#pragma once

class CConfigurationSettings
{
public:
	//------------------------------------------------------------------
	//Constructor that accepts 
	//		Config XML file name with the path (iSourceType=1) (default)
	//		Config XML String (iSourceType=2)
	//		Config XML to be loaded from remote HTTP Server (iSourceType=3)
	//	
	//If lpszConfigSrc parameter is ignored, tries to load <appname.exe>.config
	//For ex, if this class is used in application named Test.exe, and if the
	//no parameters are specified, it tries to load Test.exe.config file.
	//------------------------------------------------------------------
	CConfigurationSettings( LPCTSTR lpszConfigSrc = NULL, int iSourceType = 1 );
	virtual ~CConfigurationSettings(void) {}

	virtual LPTSTR GetConfigValue(LPCTSTR lpszXPathExpr);
	virtual bool SetConfigValue(LPCTSTR lpszXPathExpr, LPCTSTR lpszNewValue);
	virtual bool SetSelectionNamespaces(LPCTSTR lpszNamespaces);
	virtual bool SaveConfigFile(LPCTSTR lpszConfigFile=NULL);

	//Properties
	TCHAR m_lpszLastError[ MAX_PATH*4 ];
	HRESULT m_hLastError;
	CComPtr<MSXML2::IXMLDOMDocument2> GetXmlDoc();

protected:
	//The XML DOMDocument object 
	CComPtr<MSXML2::IXMLDOMDocument2> m_spXMLDoc;

	//If the Config Source is successfully loaded
	bool m_bConfigSourceLoaded;
};
