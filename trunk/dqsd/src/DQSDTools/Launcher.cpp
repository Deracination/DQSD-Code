// Launcher.cpp : Implementation of CLauncher
#include "stdafx.h"
#include "DQSDTools.h"
#include "Launcher.h"


#include "utilities.h"


/////////////////////////////////////////////////////////////////////////////
// CLauncher



STDMETHODIMP CLauncher::SetSite(IUnknown* pUnkSite)
{
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

	if (GetScheme(OLE2T(bstrURL)) != INTERNET_SCHEME_FILE)
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

  return S_OK;
};



STDMETHODIMP CLauncher::SubmitForm(VARIANT idForm)
{
	HRESULT hr;


	// Check for correct VARIANT type
	if (idForm.vt != VT_DISPATCH)
	{
		Error(IDS_ERR_NEEDFORMOBJECT, IID_ILauncher );
		return E_INVALIDARG;
	}


	// Switch to genuine IHTMLElement Interface
	CComPtr<IHTMLElement> spHTMLElement;
	if (FAILED(hr = idForm.pdispVal->QueryInterface(IID_IHTMLElement, (void**)&spHTMLElement)))
	{
		Error(IDS_ERR_NEEDFORMOBJECT, IID_ILauncher, hr);
		return hr;
	}


	// Also keep IHTMLFormElement Interface ready
	CComPtr<IHTMLFormElement> spFormElement;
	CComBSTR bstrFormName;
	if (FAILED(hr = idForm.pdispVal->QueryInterface(IID_IHTMLFormElement, (void**)&spFormElement)))
	{
		Error(IDS_ERR_NEEDFORMOBJECT, IID_ILauncher, hr);
		return E_INVALIDARG;
	}
	if (FAILED(hr = spFormElement->get_name(&bstrFormName)))
	{
		Error(IDS_ERR_NEEDFORMNAME, IID_ILauncher, hr);
		return E_INVALIDARG;
	}


	// Retrieve FORM from DOM, manipulate target window, and get HTML text
	CComBSTR bstrTarget(_T("_self"));
	spFormElement->put_target(bstrTarget);
	CComBSTR bstrHTML;
	spHTMLElement->get_outerHTML(&bstrHTML);
	

	// Dump HTML text in temp file
	{
		DWORD cbPath = _MAX_PATH;
		DWORD dwBytesWritten;
		TCHAR szPath[_MAX_PATH];
		::GetTempPath(cbPath, szPath);

		_tcscat(szPath, _T("DQSDLaunch.html"));
		HANDLE hFile = ::CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());

		USES_CONVERSION;

		CComBSTR bstrOutput, bstrBanner;
		bstrBanner.LoadString(IDS_BANNER);
		bstrOutput.Append(_T("<html><body onload=\"document."));
		bstrOutput.Append(bstrFormName);
		bstrOutput.Append(_T(".submit();\">\n"));
		bstrOutput.Append(bstrBanner);
		bstrOutput.Append(_T("\n"));
		bstrOutput.Append(bstrHTML);
		bstrOutput.Append(_T("\n</body></html>\n"));
		::WriteFile(hFile, OLE2T(bstrOutput), bstrOutput.Length(), &dwBytesWritten, NULL);
		::FlushFileBuffers(hFile);

		::CloseHandle(hFile);


		// Open html associated application, passing DQSDLaunch.html
		HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), szPath, NULL, NULL, SW_SHOWNORMAL);
		if (reinterpret_cast<INT>(hInstance) <= 32)
			return E_FAIL;
	}		

	return S_OK;
}


STDMETHODIMP CLauncher::OpenDocument(BSTR strDoc)
{
	USES_CONVERSION;


	HINSTANCE hInstance = ::ShellExecute(NULL, _T("open"), OLE2T(strDoc), NULL, NULL, SW_SHOWNORMAL);
	if (reinterpret_cast<INT>(hInstance) <= 32)
		return E_FAIL;

	return S_OK;
}



STDMETHODIMP CLauncher::get_pathDefaultBrowser(BSTR *pVal)
{
	USES_CONVERSION;

	DWORD cbDocPath = _MAX_PATH;
	TCHAR szDocPath[_MAX_PATH];
	TCHAR szExePath[_MAX_PATH];

	// Create temp file with desired .html extension
	::GetTempPath(cbDocPath, szDocPath);
	_tcscat(szDocPath, _T("DQSDLaunch.html"));
	HANDLE hFile = ::CreateFile(szDocPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return HRESULT_FROM_WIN32(::GetLastError());
	::CloseHandle(hFile);
	
	// Find associated app for .html files
	HINSTANCE hInstance = ::FindExecutable(szDocPath, _T(""), szExePath);
	if (reinterpret_cast<INT>(hInstance) <= 32)
		return E_FAIL;
	
	// Prepare string for shipping, and ship
	*pVal = ::SysAllocString(T2OLE(szExePath));

	return S_OK;
}
