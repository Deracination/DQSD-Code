// MenuBuilder.cpp : Implementation of CMenuBuilder
#include "stdafx.h"
#include "DQSDTools.h"
#include "MenuBuilder.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuBuilder

LPCTSTR CMenuBuilder::DQSD_REG_KEY = _T("CLSID\\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}");
LPCTSTR CMenuBuilder::DQSD_SEC_KEY = _T("CLSID\\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}\\SecureFiles");

int URLMatchesFilename(LPCTSTR szURL, LPCTSTR szFile);

/*
STDMETHODIMP CMenuBuilder::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IMenuBuilder
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
*/

STDMETHODIMP CMenuBuilder::SetSite(IUnknown* pUnkSite)
{
#if defined(DQSD_NOSECURITY) && defined(_DEBUG)
#pragma message(  __FILE__ " ** WARNING! ** Compilation without security restrictions...do not distribute the resulting binary! " )
#else
	USES_CONVERSION;

	HRESULT hr;

	m_spUnkSite = pUnkSite;

	CComPtr<IServiceProvider> spSrvProv;
	if (FAILED(hr = GetSite(IID_IServiceProvider, (void**)&spSrvProv)))
		return hr;

	CComPtr<IWebBrowser2> spWebBrowser;
	if (FAILED(hr = spSrvProv->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&spWebBrowser)))
		return hr;

	CComBSTR bstrURL;
	if (FAILED(hr = spWebBrowser->get_LocationURL(&bstrURL)))
		return hr;

	HKEY hDqsdKey;
	if (ERROR_SUCCESS != RegOpenKey(HKEY_CLASSES_ROOT, DQSD_SEC_KEY, &hDqsdKey))
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

	
	DWORD dt;
	TCHAR filebuf[MAX_PATH];
	DWORD filelen = sizeof(filebuf);
	DWORD idw = 0;
	BOOL success = FALSE;

	while (ERROR_SUCCESS == RegEnumValue(hDqsdKey, idw, filebuf, &filelen, NULL, &dt, NULL, NULL))
	{
		idw++;
		if (URLMatchesFilename(OLE2T(bstrURL), filebuf))
		{
			success = TRUE;
			break;
		}

		filelen = sizeof(filebuf);
	}

    if (success == FALSE)
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

#endif

  return S_OK;
}

STDMETHODIMP CMenuBuilder::Display( VARIANT* pvarSelection )
{
	USES_CONVERSION;

	BSTR bstrSelection = NULL;
	VariantInit( pvarSelection );
	pvarSelection->pbstrVal = NULL;
	pvarSelection->vt = VT_NULL;

	// The following window hierarchy was determined using Spy++ on Windows XP Pro Build 2600.
	// Should be the same at least on Windows 2000 and other Win XP versions.
	LPCTSTR rgszClassNames[] = { _T("Shell_TrayWnd"), 
								 _T("ReBarWindow32"), 
								 _T("OCHost"), 
								 _T("Shell Embedding"), 
								 _T("Shell DocObject View"), 
								 _T("Internet Explorer_Server") };

	//We now traverse the array of window classes. Set hwnd NULL to start with the desktop.
	HWND hwndDQSD = NULL;
	for ( int i = 0; i < sizeof(rgszClassNames)/sizeof(rgszClassNames[0]); i++ ) 
	{
		if ( NULL == ( hwndDQSD = ::FindWindowEx( hwndDQSD, NULL, rgszClassNames[i], NULL ) ) )
		{
			// If unable to traverse the first list, then assume it's a separate browser window
			LPCTSTR rgszClassNames[] = { _T("IEFrame"), 
										 _T("Shell DocObject View"), 
										 _T("Internet Explorer_Server") };

			hwndDQSD = m_hwndBrowser;

			for ( int j = 0; j < sizeof(rgszClassNames)/sizeof(rgszClassNames[0]); j++ )
			{
				if ( NULL == ( hwndDQSD = ::FindWindowEx( hwndDQSD, NULL, rgszClassNames[j], NULL ) ) )
				{
					hwndDQSD = m_hwndBrowser;
				}
			}

			break;
		}
	}

	RECT rcParentWnd;
	::GetWindowRect( hwndDQSD, &rcParentWnd );

	int iMenuItem = ::TrackPopupMenuEx( m_hMain, TPM_RETURNCMD|TPM_NONOTIFY|TPM_BOTTOMALIGN|getHorizontalAlignment(), rcParentWnd.right, rcParentWnd.top, hwndDQSD, NULL );
	if ( iMenuItem > 0 )
	{
		bstrSelection = ::SysAllocString( T2CW( m_mapKeys[ iMenuItem ].c_str() ) );

		VariantInit( pvarSelection );
		pvarSelection->bstrVal = bstrSelection;
		pvarSelection->vt = VT_BSTR;
	}

	::DestroyMenu( m_hMain );
	m_hMain = NULL;

	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendMenuItem(BSTR bstrItem, BSTR bstrKey, long hmenu)
{
	USES_CONVERSION;

	BOOL bSuccess = ::AppendMenu( (hmenu ? (HMENU)hmenu : m_hMain), MF_STRING, ++m_nMenuItem, W2T( bstrItem ) );
	if ( !bSuccess )
		return E_FAIL;

	m_mapKeys[ m_nMenuItem ] = std::string( W2T( bstrKey ) );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendSubMenu(BSTR bstrName, long *phmenu)
{
	*phmenu = (long)::CreatePopupMenu();

	USES_CONVERSION;

	::AppendMenu( (HMENU)m_hMain, MF_POPUP, (UINT_PTR)*phmenu, W2T( bstrName ) );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendSeparator(long hmenu)
{
	::AppendMenu( (hmenu ? (HMENU)hmenu : m_hMain), MF_SEPARATOR, NULL, NULL );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::get_HorizontalAlignment(short *pVal)
{
	*pVal = m_nHorizontalAlignment & 0x0000FFFF;

	return S_OK;
}

STDMETHODIMP CMenuBuilder::put_HorizontalAlignment(short newVal)
{
	switch ( newVal )
	{
	case 0:
	case 1:
	case 2:
		m_nHorizontalAlignment = newVal;
		break;
	default:
		Error(IDS_ERR_INVALIDHORIZALIGNMENT, IID_IMenuBuilder);
		return E_FAIL;
	}

	return S_OK;
}

UINT CMenuBuilder::getHorizontalAlignment()
{
	switch ( m_nHorizontalAlignment )
	{
	case 0: return TPM_CENTERALIGN;
	case 1: return TPM_LEFTALIGN;
	case 2: return TPM_RIGHTALIGN;
	}
	return TPM_RIGHTALIGN;
}