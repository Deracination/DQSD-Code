// Launcher.cpp : Implementation of CLauncher
#include "stdafx.h"
#include "DQSDTools.h"
#include "KeyboardHook.h"
#include "Launcher.h"
#include "Utilities.h"
#include <objbase.h>
#include <shlguid.h>
#include <shlobj.h>

#pragma comment(lib, "Version.lib")

/////////////////////////////////////////////////////////////////////////////
// CLauncher

LPCTSTR CLauncher::DQSD_REG_KEY = _T("CLSID\\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}");
LPCTSTR CLauncher::DQSD_SEC_KEY = _T("CLSID\\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}\\SecureFiles");


STDMETHODIMP CLauncher::SetSite(IUnknown* pUnkSite)
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

		StrNCat(szPath, _T("DQSDLaunch.html"), sizeof(szPath)/sizeof(TCHAR)-_tcslen(szPath)-1);
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
		HINSTANCE hInstance = ::ShellExecute(NULL, NULL, szPath, NULL, NULL, SW_SHOWNORMAL);
		if (reinterpret_cast<INT>(hInstance) <= 32)
			return E_FAIL;
	}		

	return S_OK;
}


STDMETHODIMP CLauncher::OpenDocument(BSTR strDoc, VARIANT* pvParameters)
{
	USES_CONVERSION;

	// Check for optional string parameter.  Any non-existent parameter should be indicated
	// by a VARIANT of type VT_ERROR with a value of DISP_E_PARAMNOTFOUND.
	HINSTANCE hInstance = NULL;
	if (pvParameters && (VT_BSTR == pvParameters->vt))
	{
		hInstance = ::ShellExecute(NULL, NULL, OLE2T(strDoc), OLE2T(pvParameters->bstrVal), NULL, SW_SHOWNORMAL);
	}
	else
	{
		hInstance = ::ShellExecute(NULL, NULL, OLE2T(strDoc), NULL, NULL, SW_SHOWNORMAL);
	}
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
	StrNCat(szDocPath, _T("DQSDLaunch.html"), sizeof(szDocPath)/sizeof(TCHAR)-_tcslen(szDocPath)-1);
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

STDMETHODIMP CLauncher::get_Debug(VARIANT_BOOL* pbDebug)
{
	if (NULL == pbDebug)
		return E_INVALIDARG;

#pragma warning(disable: 4310) // cast truncates constant value
	*pbDebug = (m_bDebug ? VARIANT_TRUE : VARIANT_FALSE);
#pragma warning(default: 4310) // cast truncates constant value

	return S_OK;
}

STDMETHODIMP CLauncher::put_Debug(VARIANT_BOOL bDebug)
{
	m_bDebug = bDebug != VARIANT_FALSE;

	return S_OK;
}


STDMETHODIMP CLauncher::ReadFile(BSTR bstrFilename, BSTR *pbstrResult)
{
	USES_CONVERSION;

	// Get the full pathname after applying some defaults
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( W2CT( bstrFilename ), szFilename );
	if ( FAILED( hr ) )
		return hr;
	
	// Try to open the file
	HANDLE hFile = ::CreateFile( szFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if ( INVALID_HANDLE_VALUE == hFile )
		return HRESULT_FROM_WIN32(::GetLastError());


	// Read in the string data
	TCHAR szData[ 1024 ];
	DWORD dwBytesRead = 0;
	CComBSTR bstrResult;
	do
	{
		memset( szData, 0, sizeof szData );
		::ReadFile( hFile, &szData, sizeof szData - 1, &dwBytesRead, NULL );
		bstrResult.Append( szData );
	}
	while ( dwBytesRead > 0 );
	::CloseHandle( hFile );

	// Return the file data in a big string
	*pbstrResult = bstrResult.Detach();

	return S_OK;
}

STDMETHODIMP CLauncher::WriteFile(BSTR bstrFilename, BSTR bstrValue)
{
	USES_CONVERSION;

	// Get the full pathname after applying some defaults
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( W2CT( bstrFilename ), szFilename );
	if ( FAILED( hr ) )
		return hr;
	
	HANDLE hFile = ::CreateFile( szFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == hFile )
		HRESULT_FROM_WIN32(::GetLastError());

	DWORD dwBytesWritten = 0;
	BOOL bResult = ::WriteFile( hFile, W2CT( bstrValue ), lstrlenW( bstrValue ), &dwBytesWritten, NULL );
	::FlushFileBuffers( hFile );
	::CloseHandle( hFile );
	
	if ( 0 == bResult )
		HRESULT_FROM_WIN32(::GetLastError());

	return S_OK;
}

// Private methods

HRESULT CLauncher::GetFilename( LPCTSTR szName, LPTSTR szResult, LPCTSTR pszDefaultExt /*= _T(".txt")*/ )
{
	USES_CONVERSION;

	// Get the installation directory from the registry
	CRegKey rk;
	if ( ERROR_SUCCESS != rk.Open( HKEY_CLASSES_ROOT, DQSD_REG_KEY, KEY_READ ) )
	{
		Error(IDS_ERR_REGKEYNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}

	TCHAR szInstallDir[ _MAX_PATH ];
	DWORD dwCount = sizeof( szInstallDir );
	if ( ERROR_SUCCESS != rk.QueryValue( szInstallDir, _T("InstallDir"), &dwCount ) )
	{
		Error(IDS_ERR_REGKEYNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}

	// Add the install directory and an extension if not supplied
	::PathCombine( szResult, szInstallDir, szName );
	::PathAddExtension( szResult, pszDefaultExt );

	return S_OK;
}

STDMETHODIMP CLauncher::GetProtocolHandler(BSTR bstrProtocol, BSTR *pbstrHandler)
{
	USES_CONVERSION;

	TCHAR szProtocolHandlerKey[ 128 ];
	StrCpyN( szProtocolHandlerKey, W2T( bstrProtocol ), sizeof(szProtocolHandlerKey)/sizeof(TCHAR));
	StrNCat( szProtocolHandlerKey, _T("\\shell\\open\\command"), sizeof(szProtocolHandlerKey)/sizeof(TCHAR)-lstrlenW(bstrProtocol)-1);

	TCHAR szProtocolHandler[ _MAX_PATH ];
	DWORD dwCount = sizeof( szProtocolHandler );

	CRegKey rk;
	if ( ( ERROR_SUCCESS != rk.Open( HKEY_CLASSES_ROOT, szProtocolHandlerKey, KEY_READ ) )
		 ||
	     ( ERROR_SUCCESS != rk.QueryValue( szProtocolHandler, _T(""), &dwCount ) ) )
	{
		Error(IDS_ERR_PROTOCOLNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}

	*pbstrHandler = ::SysAllocString( T2W( szProtocolHandler ) );

	return S_OK;
}

STDMETHODIMP CLauncher::GetFiles(BSTR bstrFileSpec, BSTR *pbstrFiles)
{
	USES_CONVERSION;

	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( W2CT( bstrFileSpec ), szFilename, _T("*.*") );
	if ( FAILED( hr ) )
	{
		Error(IDS_ERR_FILENOTFOUND, IID_ILauncher, hr);
		return hr;
	}

	WIN32_FIND_DATA fd;
	memset( &fd, 0, sizeof(fd) );
	HANDLE handle = ::FindFirstFile( szFilename, &fd );
	if (INVALID_HANDLE_VALUE == handle)
	{
		Error(IDS_ERR_FILENOTFOUND, IID_ILauncher, HRESULT_FROM_WIN32(::GetLastError()));
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	CComBSTR bstrFiles( fd.cFileName );
	while ( ::FindNextFile( handle, &fd ) )
	{
		if ( !StrCmp( fd.cFileName, _T(".") ) || !StrCmp( fd.cFileName, _T("..") ) )
			continue;

		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		bstrFiles.Append( _T("\n") );
		bstrFiles.Append( fd.cFileName );
	}

	::FindClose(handle);

	*pbstrFiles = bstrFiles.Detach();

	return S_OK;
}

//
// This is nothing to do with the launcher - I've just piggybacked it on to save
// creating another class/interface
//
STDMETHODIMP CLauncher::InstallKeyboardHook(LPDISPATCH pDispDocument)
{
	return KeyboardHookInstall(UtilitiesFindDQSDWindow(pDispDocument), m_hKeyboardHook);
}

STDMETHODIMP CLauncher::RegisterHotKey(long hotkeyVkCode, BSTR bstrModifierName, LPDISPATCH pDispDocument)
{
	USES_CONVERSION;

	return KeyboardInstallHotkey(hotkeyVkCode, W2T(bstrModifierName), &m_hHotkeyNotificationWindow, pDispDocument);
}

//
// This allows mapping of a virtual keycode to a character code
// sent with WM_CHAR in KeyboardProc.  It prevents having to modify
// KeyboardProc for every mapping
STDMETHODIMP CLauncher::MapKeyCode(long lVKCode, long lCharCode)
{
	g_mapKeyCodeToCharCode[ lVKCode ] = lCharCode;
	return S_OK;
}


STDMETHODIMP CLauncher::get_VersionIsCorrect(int v1, int v2, int v3, int v4, VARIANT_BOOL *pVal)
{
	// Check if our version resource is same or later version than 
	// the one specified by the script in v1.v2.v3.v4

	try
	{
//		LPVOID pVersion;
//		HRSRC hVerRes;
//		HGLOBAL hVerResMem;
		VS_FIXEDFILEINFO* pFixInfo;

		ATLTRACE("CLauncher: Starting get_VersionIsCorrect\n");

		TCHAR moduleName[MAX_PATH+1];
		GetModuleFileName(_Module.GetModuleInstance(), moduleName, MAX_PATH);
		DWORD dummyZero;
		DWORD versionSize = GetFileVersionInfoSize(moduleName, &dummyZero);
		if(versionSize == 0)
		{
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
		BYTE* pData = new BYTE[versionSize];
		if(pData == NULL)
		{
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
		if(!GetFileVersionInfo(moduleName, NULL, versionSize, pData))
		{
			delete [] pData;
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
/*	
		hVerRes = FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		if(hVerRes == NULL)
		{
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
		hVerResMem = LoadResource (_Module.GetResourceInstance(), hVerRes) ;
		if(hVerResMem == NULL)
		{
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
		pVersion = LockResource (hVerResMem) ;
		if(pVersion == NULL)
		{
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}
*/
		ATLTRACE("CLauncher: About to query value\n");

		UINT length;
		pFixInfo = NULL;
		if(!VerQueryValue(pData, _T("\\"), (LPVOID*)&pFixInfo, &length))
		{
			delete [] pData;
			return Error(IDS_ERR_VERSION_RESOURCE, IID_ILauncher, E_FAIL);
		}

		ATLTRACE("CLauncher: value queried\n");

		int dllV1, dllV2, dllV3, dllV4;
		dllV1 = pFixInfo->dwProductVersionMS >> 16;
		dllV2 = pFixInfo->dwProductVersionMS & 0xffff;
		dllV3 = pFixInfo->dwProductVersionLS >> 16;
		dllV4 = pFixInfo->dwProductVersionLS & 0xffff;

		ATLTRACE("CLauncher: DLL Version %d.%d.%d.%d\n", dllV1, dllV2, dllV3, dllV4);

		// Assume we're going to pass
#pragma warning(disable: 4310) // cast truncates constant value
		*pVal = VARIANT_TRUE;
#pragma warning(default: 4310) // cast truncates constant value

		// Test the version in order from MS to LS
		// This could have been done as one 64 bit comparison, but...
		if(dllV1 < v1)
		{
			*pVal = VARIANT_FALSE;
		}
		else if(dllV2 < v2)
		{
			*pVal = VARIANT_FALSE;
		}
		else if(dllV3 < v3)
		{
			*pVal = VARIANT_FALSE;
		}
		else if(dllV4 < v4)
		{
			*pVal = VARIANT_FALSE;
		}

		delete [] pData;

		return S_OK; 
	}
	catch(...)
	{
		return Error(IDS_EXCEPTION_IN_VERSION_CHECK, IID_ILauncher, E_FAIL);
	}
}


__declspec(dllexport) void CALLBACK RestartExplorer(HWND hParent, HINSTANCE hInst, LPTSTR lpCmdLine, int nShow)
{
	UNREFERENCED_PARAMETER(hParent);
	UNREFERENCED_PARAMETER(hInst);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShow);

	HWND hwndShell = FindWindow("Progman", NULL);
	::PostMessage(hwndShell, WM_QUIT, 0, 0L);
	::WinExec("Explorer.exe",SW_SHOW);
}

DWORD WINAPI ShutdownThread(void* pParam)
{
	HWND hDQSDWnd = (HWND)pParam;

	// Wait for DQSD to be gone
	LONG startTime = GetTickCount();
	while(((LONG)GetTickCount() - startTime) < 10000)
	{
		if(!IsWindow(hDQSDWnd))
		{
			// We're gone

			// Restart Explorer
			RestartExplorer(NULL,NULL,NULL,0);
			return 0;
		}
		Sleep(100);
	}
	return 0;
}


STDMETHODIMP CLauncher::ShutdownBar(LPDISPATCH pDispDocument)
{
//	return Error(_T("Shutdown bar not implemented yet..."), IID_ILauncher, E_NOTIMPL);


	HWND hDQSDWnd = UtilitiesFindDQSDWindow(pDispDocument);
	if(hDQSDWnd == NULL)
	{
		return Error(_T("DQSD was not found on the taskbar"), IID_ILauncher, E_FAIL);
	}

	// The window hierachy goes 
//		 _T("Shell_TrayWnd"), 
//		_T("ReBarWindow32"), 
//		_T("OCHost"), 
//		_T("Shell Embedding"), 
//		_T("Shell DocObject View"), 
//		_T("Internet Explorer_Server") <== This is us


	HWND hRebarBand = GetParent(GetParent(GetParent(hDQSDWnd)));
	HWND hRebar = GetParent(hRebarBand);

	ATLTRACE("Rebar: 0x%x, Pane 0x%x\n", hRebar, hRebarBand);

	int nBands = ::SendMessage(hRebar, RB_GETBANDCOUNT, 0, 0);
	for(int nBand = 0; nBand < nBands; nBand++)
	{
		REBARBANDINFO info;
		ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);
		info.fMask = RBBIM_CHILD;

		if(::SendMessage(hRebar, RB_GETBANDINFO, nBand, (LPARAM)&info))
		{
			ATLTRACE("Band %d - hWnd = 0x%x\n", nBand, info.hwndChild);

			if(info.hwndChild == hRebarBand)
			{
				// We've found our band - shut it down
				::SendMessage(hRebar, RB_SHOWBAND, nBand, FALSE);
				::SendMessage(hRebar, RB_DELETEBAND, nBand, 0);

				ATLTRACE("Destroying band window: 0x%x\n", hRebarBand);
				DestroyWindow(hRebarBand);

				DWORD threadId;
				::CreateThread(NULL, 0, ShutdownThread, hDQSDWnd,0, &threadId);

				return S_OK;
			}
		}
	}

	return Error(_T("Failed to shut DQSD bar"), IID_ILauncher, E_FAIL);
}

STDMETHODIMP CLauncher::RefreshTrayIcons()
{
	static DWORD result;
	UINT msg = ::RegisterWindowMessage(_T("TaskbarCreated"));

	ATLTRACE("DQSDTools: Refreshing tray icons (msg 0x%x)\n", msg);

	::SendMessageTimeout(HWND_BROADCAST, msg, 0, 0, SMTO_ABORTIFHUNG, 1000, &result);

	return S_OK;
}

STDMETHODIMP CLauncher::get_InstallationDirectory(BSTR* pbstrDirectory)
{
	USES_CONVERSION;

	// Get the installation directory from the registry
	CRegKey rk;
	if ( ERROR_SUCCESS != rk.Open( HKEY_CLASSES_ROOT, DQSD_REG_KEY, KEY_READ ) )
	{
		Error(IDS_ERR_REGKEYNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}

	TCHAR szInstallDir[ _MAX_PATH ];
	DWORD dwCount = sizeof( szInstallDir );
	if ( ERROR_SUCCESS != rk.QueryValue( szInstallDir, _T("InstallDir"), &dwCount ) )
	{
		Error(IDS_ERR_REGKEYNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}
	CComBSTR bstrInstallDir;
	bstrInstallDir.Append(szInstallDir);
	*pbstrDirectory = bstrInstallDir.Detach();
	return S_OK;
}

typedef struct SPECIAL_FOLDER_MAP_S {
	LPCTSTR strName;
	int csidl_value;
} SPECIAL_FOLDER_MAP_T, *PSPECIAL_FOLDER_MAP_T, **PPSPECIAL_FOLDER_MAP_T;

static SPECIAL_FOLDER_MAP_T SpecialFolders[] =
{
	{"AppData",             CSIDL_APPDATA},
	{"Desktop",             CSIDL_DESKTOP},
	{"Favorites",           CSIDL_FAVORITES},
	{"History",             CSIDL_HISTORY},
	{"MyDocuments",         CSIDL_PERSONAL},
	{"Recent",              CSIDL_RECENT},
	{"StartMenu",           CSIDL_STARTMENU}
};
#define SpecialFoldersCount  (sizeof(SpecialFolders) / sizeof(SPECIAL_FOLDER_MAP_T))

STDMETHODIMP CLauncher::GetSpecialFolderLocation(BSTR bstrSpecialFolder, BSTR* pbstrLocation)
{
	USES_CONVERSION;
	
	// map from special folder name to id
	BOOL bFound = FALSE;
	int nFolder = 0;
	LPCTSTR ptstrSpecialFolder = W2CT(bstrSpecialFolder);
	for (int i=0; i < SpecialFoldersCount; i++) {
		PSPECIAL_FOLDER_MAP_T pMap = &SpecialFolders[i];
		if (pMap != NULL && _tcscmp(ptstrSpecialFolder, pMap->strName) == 0) {
			bFound = TRUE;
			nFolder = pMap->csidl_value;
		}
	}


	if (!bFound) {
		return E_FAIL;
	}

	LPITEMIDLIST pidl;
	HRESULT hr = SHGetSpecialFolderLocation(NULL, nFolder, &pidl);
	if (!SUCCEEDED(hr)) {
		return E_FAIL;
	}

	TCHAR szPath[_MAX_PATH];
	if (!::SHGetPathFromIDList(pidl, szPath)) {
		return E_FAIL;
	}

	CComBSTR bstrLocation;
	bstrLocation.Append(szPath);
	*pbstrLocation = bstrLocation.Detach();
	return S_OK;
}

STDMETHODIMP CLauncher::GetFolders(BSTR bstrBaseFolder, BSTR* pbstrFolders)
{
	USES_CONVERSION;

	TCHAR szBaseFolder[ _MAX_PATH ];
	HRESULT hr = GetFilename( W2CT( bstrBaseFolder ), szBaseFolder, _T("*.*") );
	if ( FAILED( hr ) )
	{
		Error(IDS_ERR_FILENOTFOUND, IID_ILauncher, hr);
		return hr;
	}

	WIN32_FIND_DATA fd;
	memset( &fd, 0, sizeof(fd) );
	HANDLE handle = ::FindFirstFile( szBaseFolder, &fd );
	if (INVALID_HANDLE_VALUE == handle)
	{
		Error(IDS_ERR_FILENOTFOUND, IID_ILauncher, HRESULT_FROM_WIN32(::GetLastError()));
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	CComBSTR bstrFolders( fd.cFileName );
	while ( ::FindNextFile( handle, &fd ) )
	{
		if (_tcscmp(fd.cFileName, _T(".")) == 0 || _tcscmp(fd.cFileName, _T("..")) == 0)
			continue;

		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			continue;

		bstrFolders.Append( _T("\n") );
		bstrFolders.Append( fd.cFileName );
	}

	::FindClose(handle);

	*pbstrFolders = bstrFolders.Detach();
	return S_OK;
}
