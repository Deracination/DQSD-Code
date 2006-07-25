#include "stdafx.h"
#include "ConfigurationSettings.h"

#define CHECK_HR_THROW(hr) { if (FAILED(hr)) { throw -1; } }

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
CConfigurationSettings::CConfigurationSettings(LPCTSTR lpszConfigSrc /* = NULL */, int iSourceType /* = 1 */ ):
	m_bConfigSourceLoaded(false), m_hLastError(S_OK)
{
	m_lpszLastError[0] = 0;

	USES_CONVERSION;

	try
	{
		//Step 1: Create MSXML DOMDocument Object
		m_hLastError = m_spXMLDoc.CoCreateInstance(MSXML2::CLSID_DOMDocument40);
		CHECK_HR_THROW(m_hLastError);

		//Load file synchronously, avoid validation, avoid external references resolution
		m_spXMLDoc->async = VARIANT_FALSE;
		m_spXMLDoc->validateOnParse = VARIANT_FALSE;
		m_spXMLDoc->resolveExternals = VARIANT_FALSE;

		//We'll be loading document synchronously, and will not use DTD
		//Hence we can safely set NewParser property to true, to get MSXML 4.0 SP1 speed benefits
		m_hLastError = m_spXMLDoc->setProperty(_bstr_t(_T("NewParser")), VARIANT_TRUE);
		CHECK_HR_THROW(m_hLastError);

		//Set the SelectionLanguage to XPath
		m_hLastError = m_spXMLDoc->setProperty(_bstr_t(_T("SelectionLanguage")), 
			CComVariant(_T("XPath")) );
		CHECK_HR_THROW(m_hLastError);


		//Step 2: Based on iSourceType, load XML Document
		TCHAR lpszLocalXMLFile[MAX_PATH*4] = {0};
		switch(iSourceType)
		{
			case 1://Local Configuration XML file
				
				if(lpszConfigSrc == NULL || (lpszConfigSrc != NULL && _tcslen(lpszConfigSrc) <= 0))
				{//Config File Name ommitted
					GetModuleFileName(NULL, lpszLocalXMLFile, MAX_PATH*2);
					_tcscat(lpszLocalXMLFile, _T(".config"));
				}
				else
				{//Config File Name specified
					_tcscpy(lpszLocalXMLFile, lpszConfigSrc);
				}

				//Try to load the local XML file
				if(m_spXMLDoc->load(CComVariant(lpszLocalXMLFile)) == VARIANT_TRUE)
				{
					m_bConfigSourceLoaded = true;
				}
				else
				{
					m_hLastError = E_FAIL;
					sprintf(m_lpszLastError, _T("Failed to load XML file '%s'. %s"), 
						lpszLocalXMLFile,
						W2A(m_spXMLDoc->parseError->reason));
				}

				break;

			case 2://Configuration XML String
				//Try to load the input XML string
				if(lpszConfigSrc == NULL || (lpszConfigSrc != NULL && _tcslen(lpszConfigSrc) <= 0))
				{
					m_hLastError = E_FAIL;
					sprintf(m_lpszLastError, _T("Missing Configuration XML string value."));
				}
				else
				{
					if(m_spXMLDoc->loadXML(_bstr_t(lpszConfigSrc)) == VARIANT_TRUE)
					{
						m_bConfigSourceLoaded = true;
					}
					else
					{
						m_hLastError = E_FAIL;
						sprintf(m_lpszLastError, _T("Failed to load Config XML string. '%s'"), 
							W2A(m_spXMLDoc->parseError->reason) );
					}
				}
				
				break;

			case 3://Remote HTTP URL
				if(lpszConfigSrc == NULL || (lpszConfigSrc != NULL && _tcslen(lpszConfigSrc) <= 0))
				{
					m_hLastError = E_FAIL;
					sprintf(m_lpszLastError, _T("Missing Configuration XML File URL."));
				}
				else
				{
					//Since its a remote HTTP file, set the ServerHTTPRequest property to true
					m_hLastError = m_spXMLDoc->setProperty(_T("ServerHTTPRequest"), VARIANT_TRUE);
					CHECK_HR_THROW(m_hLastError);

					//Try to load the remote XML file
					if(m_spXMLDoc->load(CComVariant(lpszConfigSrc)) == VARIANT_TRUE)
					{
						m_bConfigSourceLoaded = true;
					}
					else
					{
						m_hLastError = E_FAIL;
						sprintf(m_lpszLastError, _T("Failed to load remote XML file '%s'. %s"), 
							lpszConfigSrc,
							W2A(m_spXMLDoc->parseError->reason));
					}
				}

				break;

			default://Incorrect parameter
				sprintf(m_lpszLastError, _T("Incorrect parameter %d"), iSourceType);
				ATLASSERT(0);
				break;
		}

	}
	catch(...)
	{
	}
}

CComPtr<MSXML2::IXMLDOMDocument2> CConfigurationSettings::GetXmlDoc()
{
	ATLASSERT(m_spXMLDoc.p != NULL);
	ATLASSERT(m_bConfigSourceLoaded == true);

	return m_spXMLDoc;
}


LPTSTR CConfigurationSettings::GetConfigValue(LPCTSTR lpszXPathExpr)
{
	CComPtr<MSXML2::IXMLDOMNode> spMatchingNode;
	USES_CONVERSION;

	ATLASSERT(m_spXMLDoc.p != NULL);
	ATLASSERT(m_bConfigSourceLoaded == true);
	ATLASSERT(lpszXPathExpr != NULL);

	//Evaluate XPath expression, and get the matching node
	if ( m_spXMLDoc && m_bConfigSourceLoaded && lpszXPathExpr )
		spMatchingNode = m_spXMLDoc->selectSingleNode(_bstr_t(lpszXPathExpr));

	//if matching node found
	if(spMatchingNode)
	{
		TCHAR* lpszReturnValue = new TCHAR[MAX_PATH*2];
		lpszReturnValue[0] = 0;
		
		CComVariant cvNodeTypedValue = spMatchingNode->nodeTypedValue;;

		if(cvNodeTypedValue.vt == VT_BSTR)
			_tcscpy(lpszReturnValue, W2A(cvNodeTypedValue.bstrVal));

		return lpszReturnValue;
	}
	else
		return NULL;
}

bool CConfigurationSettings::SetConfigValue(LPCTSTR lpszXPathExpr, LPCTSTR lpszNewValue)
{
	bool bRetVal = false;
	CComPtr<MSXML2::IXMLDOMNode> spMatchingNode;
	USES_CONVERSION;

	ATLASSERT(m_spXMLDoc.p != NULL);
	ATLASSERT(m_bConfigSourceLoaded == true);
	ATLASSERT(lpszXPathExpr != NULL);

	if(m_spXMLDoc && m_bConfigSourceLoaded  && lpszXPathExpr)
		spMatchingNode =  m_spXMLDoc->selectSingleNode(_bstr_t(lpszXPathExpr));

	if(spMatchingNode)
	{
		spMatchingNode->text = _bstr_t(lpszNewValue);
		return true;
	}

	return bRetVal;
}

bool CConfigurationSettings::SaveConfigFile(LPCTSTR lpszConfigFile)
{
	ATLASSERT(m_spXMLDoc.p != NULL);
	ATLASSERT(m_bConfigSourceLoaded == true);

	bool bRetVal = false;
	//If lpszConfigFile param ommitted, save to the same location from 
	//where the file was loaded - works for local files only (iSourceType=1)
	if(lpszConfigFile == NULL && m_spXMLDoc && m_bConfigSourceLoaded)
	{
		m_hLastError = m_spXMLDoc->save(m_spXMLDoc->url);
	}
	else if(lpszConfigFile && m_spXMLDoc && m_bConfigSourceLoaded)
	{
		m_hLastError = m_spXMLDoc->save(CComVariant(lpszConfigFile));
	}
	CHECK_HR_THROW(m_hLastError);

	return bRetVal;
}

bool CConfigurationSettings::SetSelectionNamespaces(LPCTSTR lpszNamespaces)
{
	ATLASSERT(m_spXMLDoc.p != NULL);
	ATLASSERT(m_bConfigSourceLoaded == true);
	ATLASSERT(lpszNamespaces != NULL);

	bool bRetVal = false;
	if(lpszNamespaces && m_spXMLDoc && m_bConfigSourceLoaded)
	{
		m_hLastError = m_spXMLDoc->setProperty(_bstr_t(_T("SelectionNamespaces")), 
			CComVariant(lpszNamespaces));
		CHECK_HR_THROW(m_hLastError);

		bRetVal = true;
	}

	return bRetVal;
}
