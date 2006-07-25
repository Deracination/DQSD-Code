// Launcher.cpp : Implementation of CLauncher
#include "stdafx.h"
#include "DQSDTools.h"
#include "KeyboardHook.h"
#include "Launcher.h"
#include "Utilities.h"
#include "UserOptionsDialog.h"
#include <objbase.h>
#include <shlguid.h>
#include <shlobj.h>

#pragma comment(lib, "Version.lib")

/////////////////////////////////////////////////////////////////////////////
// CLauncher

STDMETHODIMP CLauncher::SetSite(IUnknown* pUnkSite)
{
#define DQSD_NOSECURITY
#if defined(DQSD_NOSECURITY) && defined(_DEBUG)
	pUnkSite;
#pragma message(  __FILE__ " ** WARNING! ** Compilation without security restrictions...do not distribute the resulting binary! " )
#else
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
		if (URLMatchesFilename(CW2T(bstrURL), filebuf))
		{
			success = TRUE;
			break;
		}

		filelen = sizeof(filebuf);
	}

	RegCloseKey(hDqsdKey);

    if (success == FALSE)
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

#endif

  return S_OK;
}

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
	CComBSTR bstrTarget(L"_self");
	spFormElement->put_target(bstrTarget);
	CComBSTR bstrHTML;
	spHTMLElement->get_outerHTML(&bstrHTML);
	

	// Dump HTML text in temp file
	{
		DWORD cchPath = _MAX_PATH;
		DWORD dwBytesWritten;
		TCHAR szPath[_MAX_PATH];
		::GetTempPath(cchPath, szPath);

		StrNCat(szPath, _T("DQSDLaunch.html"), lengthof(szPath) - _tcslen(szPath) - 1);
		HANDLE hFile = ::CreateFile(szPath, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(::GetLastError());

		CComBSTR bstrOutput, bstrBanner;
		bstrBanner.LoadString(IDS_BANNER);
		bstrOutput.Append(L"<html><body onload=\"document.");
		bstrOutput.Append(bstrFormName);
		bstrOutput.Append(L".submit();\">\n");
		bstrOutput.Append(bstrBanner);
		bstrOutput.Append(L"\n");
		bstrOutput.Append(bstrHTML);
		bstrOutput.Append(L"\n</body></html>\n");
		::WriteFile(hFile, CW2T(bstrOutput), bstrOutput.Length(), &dwBytesWritten, NULL);
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
	// Check for optional string parameter.  Any non-existent parameter should be indicated
	// by a VARIANT of type VT_ERROR with a value of DISP_E_PARAMNOTFOUND.
	HINSTANCE hInstance = NULL;
	if (pvParameters && (VT_BSTR == pvParameters->vt))
	{
		hInstance = ::ShellExecute(NULL, NULL, CW2T(strDoc), CW2T(pvParameters->bstrVal), NULL, SW_SHOWNORMAL);
	}
	else
	{
		hInstance = ::ShellExecute(NULL, NULL, CW2T(strDoc), NULL, NULL, SW_SHOWNORMAL);
	}
	if (reinterpret_cast<INT>(hInstance) <= 32)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CLauncher::get_pathDefaultBrowser(BSTR *pVal)
{
	HRESULT hr = S_OK;

	DWORD cbDocPath = _MAX_PATH;
	TCHAR szDocPath[_MAX_PATH];
	TCHAR szExePath[_MAX_PATH];

	// First try to open a file that should always be there, search.htm
	hr = GetInstallationDirectory( szDocPath, cbDocPath );
	if ( FAILED( hr ) )
		return hr;
	_tcscat( szDocPath, _T("\\search.htm") );

	// Find associated app for .html files
	HINSTANCE hInstance = ::FindExecutable(szDocPath, _T(""), szExePath);
	if (reinterpret_cast<INT>(hInstance) <= 32)
		return E_FAIL;
	
	// Prepare string for shipping, and ship
	*pVal = T2BSTR(szExePath);

	return hr;
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
	// Get the full pathname after applying some defaults
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( CW2T( bstrFilename ), szFilename );
	if ( FAILED( hr ) )
		return hr;

	if (!IsValidFileDirectory(szFilename)) {
		return Error(_T("Filename is not in the installation or app data directory trees."), IID_ILauncher, E_FAIL);
	}

	// Make sure its extension is not one of the bad extensions
	TCHAR *szBadExtensions = _T(".exe;.dll");
	if (IsFileExtension(szFilename, szBadExtensions))
	{
		return Error(_T("Can't read that type of file."), IID_ILauncher, E_FAIL);
	}
	
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
	// Get the full pathname after applying some defaults
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( CW2T( bstrFilename ), szFilename );
	if ( FAILED( hr ) )
		return hr;

	if (!IsValidFileDirectory(szFilename)) {
		return Error(_T("Filename is not in the installation or app data directory trees."), IID_ILauncher, E_FAIL);
	}

	// Make sure its extension is not one of the bad extensions
	TCHAR *szBadExtensions = _T(".exe;.dll");
	if (IsFileExtension(szFilename, szBadExtensions))
	{
		return Error(_T("Can't write that type of file."), IID_ILauncher, E_FAIL);
	}
	
	HANDLE hFile = ::CreateFile( szFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == hFile )
		return HRESULT_FROM_WIN32(::GetLastError());

//	int cchBuffer = WideCharToMultiByte(CP_THREAD_ACP, 0, bstrValue, SysStringLen(bstrValue), NULL, -1, 0, 0);
//	if (cchBuffer == 0)
//	{
//	}

	// TODO: Avoid using conversion macros here -- it's dangerous, 
	// since there's no telling how large bstrValue is
	DWORD dwBytesWritten = 0;
	BOOL bResult = ::WriteFile( hFile, CW2T( bstrValue ), SysStringLen(bstrValue), &dwBytesWritten, NULL );
	::FlushFileBuffers( hFile );
	::CloseHandle( hFile );
	
	if ( 0 == bResult )
		return HRESULT_FROM_WIN32(::GetLastError());

	return S_OK;
}

STDMETHODIMP CLauncher::GetProtocolHandler(BSTR bstrProtocol, BSTR *pbstrHandler)
{
	TCHAR szProtocolHandlerKey[ 128 ];
	StrCpyN( szProtocolHandlerKey, CW2T( bstrProtocol ), lengthof(szProtocolHandlerKey));
	StrNCat( szProtocolHandlerKey, _T("\\shell\\open\\command"), lengthof(szProtocolHandlerKey) - SysStringLen(bstrProtocol) - 1);

	TCHAR szProtocolHandler[ _MAX_PATH ];
	DWORD dwCount = sizeof( szProtocolHandler );

	CRegKey rk;
	if ( ( ERROR_SUCCESS != rk.Open( HKEY_CLASSES_ROOT, szProtocolHandlerKey, KEY_READ ) )
		 ||
	     ( ERROR_SUCCESS != rk.QueryValue( _T(""), NULL, szProtocolHandler, &dwCount ) ) )
	{
		Error(IDS_ERR_PROTOCOLNOTFOUND, IID_ILauncher);
		return E_UNEXPECTED;
	}

	*pbstrHandler = T2BSTR(szProtocolHandler);

	return S_OK;
}

STDMETHODIMP CLauncher::GetFiles(BSTR bstrFileSpec, BSTR *pbstrFiles)
{
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( CW2T( bstrFileSpec ), szFilename, _T("*.*") );
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

		bstrFiles.Append( L"\n" );
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
	return KeyboardInstallHotkey(hotkeyVkCode, CW2T(bstrModifierName), &m_hHotkeyNotificationWindow, pDispDocument);
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

	HWND hwndShell = FindWindow(_T("Progman"), NULL);
	::PostMessage(hwndShell, WM_QUIT, 0, 0L);
	::WinExec("Explorer.exe", SW_SHOW);
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
	TCHAR szInstallDir[ _MAX_PATH ];
	HRESULT hr = GetInstallationDirectory(szInstallDir, sizeof(szInstallDir));
	if (FAILED (hr) )
	{
		return hr;
	}

	*pbstrDirectory = T2BSTR(szInstallDir);
	return S_OK;
}

STDMETHODIMP CLauncher::get_AppDataDirectory(BSTR* pbstrDirectory)
{
	CComBSTR bstrAppData;
	HRESULT hr = GetSpecialFolderLocation( CComBSTR(L"AppData"), &bstrAppData );
	if ( SUCCEEDED( hr ) )
	{
		bstrAppData.Append(L"\\DQSD");
		::CreateDirectory( CW2T( bstrAppData ), NULL );

		*pbstrDirectory = bstrAppData.Detach();
	}
	return hr;
}

typedef struct SPECIAL_FOLDER_MAP_S {
	LPCTSTR strName;
	int csidl_value;
} SPECIAL_FOLDER_MAP_T, *PSPECIAL_FOLDER_MAP_T, **PPSPECIAL_FOLDER_MAP_T;

static SPECIAL_FOLDER_MAP_T SpecialFolders[] =
{
	{_T("AppData"),             CSIDL_APPDATA},
	{_T("Desktop"),             CSIDL_DESKTOP},
	{_T("Favorites"),           CSIDL_FAVORITES},
	{_T("History"),             CSIDL_HISTORY},
	{_T("MyDocuments"),         CSIDL_PERSONAL},
	{_T("Recent"),              CSIDL_RECENT},
	{_T("StartMenu"),           CSIDL_STARTMENU}
};
#define SpecialFoldersCount  (lengthof(SpecialFolders))

STDMETHODIMP CLauncher::GetSpecialFolderLocation(BSTR bstrSpecialFolder, BSTR* pbstrLocation)
{
	// map from special folder name to id
	BOOL bFound = FALSE;
	int nFolder = 0;
	LPCTSTR ptstrSpecialFolder = CW2T(bstrSpecialFolder);
	for (int i=0; i < SpecialFoldersCount; i++) {
		PSPECIAL_FOLDER_MAP_T pMap = &SpecialFolders[i];
		if (pMap != NULL && _tcscmp(ptstrSpecialFolder, pMap->strName) == 0) {
			bFound = TRUE;
			nFolder = pMap->csidl_value;
			break;
		}
	}


	if (!bFound) {
		return E_FAIL;
	}

	LPITEMIDLIST pidl;
	HRESULT hr = ::SHGetSpecialFolderLocation(NULL, nFolder, &pidl);
	if (!SUCCEEDED(hr)) {
		return E_FAIL;
	}

	TCHAR szPath[_MAX_PATH];
	if (!::SHGetPathFromIDList(pidl, szPath)) {
		return E_FAIL;
	}

	*pbstrLocation = T2BSTR(szPath);

	return S_OK;
}

STDMETHODIMP CLauncher::GetFolders(BSTR bstrBaseFolder, BSTR* pbstrFolders)
{
	TCHAR szBaseFolder[ _MAX_PATH ];
	HRESULT hr = GetFilename( CW2T( bstrBaseFolder ), szBaseFolder, _T("*.*") );
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

		bstrFolders.Append( L"\n" );
		bstrFolders.Append( fd.cFileName );
	}

	::FindClose(handle);

	*pbstrFolders = bstrFolders.Detach();
	return S_OK;
}

STDMETHODIMP CLauncher::FileExists(BSTR bstrFilename, VARIANT_BOOL *pbExists)
{
	// Get the full pathname after applying some defaults
	TCHAR szFilename[ _MAX_PATH ];
	HRESULT hr = GetFilename( CW2T( bstrFilename ), szFilename );
	if ( FAILED( hr ) )
		return hr;

	if (!IsValidFileDirectory(szFilename)) {
		return Error(_T("Filename is not in the installation or app data directory trees."), IID_ILauncher, E_FAIL);
	}

	DWORD dwAttributes = ::GetFileAttributes(szFilename);
#pragma warning(disable: 4310) // cast truncates constant value
	*pbExists = ( (dwAttributes == -1) ? VARIANT_FALSE : VARIANT_TRUE);
#pragma warning(default: 4310) // cast truncates constant value
	
	return S_OK;
}

STDMETHODIMP CLauncher::RenameFile(BSTR bstrFromFilename, BSTR bstrToFilename)
{
	HRESULT hr;

	// Get the full from pathname after applying some defaults
	TCHAR szFromFilename[ _MAX_PATH + 1];
	hr = GetFilename( CW2T( bstrFromFilename ), szFromFilename );
	if ( FAILED( hr ) )
		return hr;

	if (!IsValidFileDirectory(szFromFilename)) {
		return Error(_T("Filename is not in the installation or app data directory trees."), IID_ILauncher, E_FAIL);
	}

	// Make sure it's extension is not one of the bad extensions
	TCHAR *szBadExtensions = _T(".386;.bat;.cmd;.com;.cpl;.dll;.drv;.eml;.exe;.hta;.js;.jse;.lnk;.mht;.mhtm;.mhtml;.msi;.ocx;.ovl;.ovr;.pif;.reg;.scr;.sct;.shb;.shs;.sys;.vbs;.vbx;.vxd;.wsc;.wsf;.wsh");
	if (IsFileExtension(szFromFilename, szBadExtensions))
	{
		return Error(_T("Can't rename that type of file."), IID_ILauncher, E_FAIL);
	}

	// add extra \0 for SHFileOperation call
	szFromFilename[lstrlen(szFromFilename)+1] = '\0';

	// Make sure that from filename already exists
	VARIANT_BOOL bExists;
	hr = FileExists( bstrFromFilename, &bExists);
	if ( FAILED(hr))
		return hr;

#pragma warning(disable: 4310) // cast truncates constant value
	if (bExists == VARIANT_FALSE)
	{
		return Error(_T("Source filename does not exist."), IID_ILauncher, E_FAIL);
	}
#pragma warning(default: 4310) // cast truncates constant value

	// Get the full to pathname after applying some defaults
	TCHAR szToFilename[ _MAX_PATH + 1 ];
	hr = GetFilename( CW2T( bstrToFilename ), szToFilename );
	if ( FAILED( hr ) )
		return hr;

	if (!IsValidFileDirectory(szToFilename)) {
		return Error(_T("Destination filename is not in the installation or app data directory trees."), IID_ILauncher, E_FAIL);
	}

	// Make sure it's extension is not one of the bad extensions
	if (IsFileExtension(szToFilename, szBadExtensions))
	{
		return Error(_T("Can't rename that type of file."), IID_ILauncher, E_FAIL);
	}

	// add extra \0 for SHFileOperation call
	szToFilename[lstrlen(szToFilename)+1] = '\0';

	// Make sure to filename doesn't already exist
	hr = FileExists( bstrToFilename, &bExists);
	if ( FAILED(hr))
		return hr;

#pragma warning(disable: 4310) // cast truncates constant value
	if (bExists == VARIANT_TRUE)
	{
		return Error(_T("Destination filename already exists."), IID_ILauncher, E_FAIL);
	}
#pragma warning(default: 4310) // cast truncates constant value

	 // Rename the file
	SHFILEOPSTRUCT sfo;
    ZeroMemory ( &sfo, sizeof ( sfo ) );

    sfo.hwnd = NULL;
    sfo.wFunc = FO_RENAME;
    sfo.pFrom = szFromFilename;
    sfo.pTo = szToFilename;
    sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_FILESONLY | FOF_NOERRORUI;

    hr = ::SHFileOperation(&sfo);
	if (FAILED( hr) )
		return hr;

	return S_OK;
}

// Private methods
HRESULT CLauncher::GetInstallationDirectory( LPTSTR szResult, DWORD dwResultSize )
{
	// Cache install directory, so we don't have to hit the registry as much
	if ( m_szInstallDir[0] == 0 )
	{
		return GetInstallationDirectory( szResult, dwResultSize );
	}

	lstrcpyn( szResult, m_szInstallDir, dwResultSize / sizeof(TCHAR) );
	
	return S_OK;
}

BOOL CLauncher::VerifyFileInDirectoryTree( LPCTSTR szFilename, LPCTSTR szDir)
{
	TCHAR szCanonFilename[_MAX_PATH];
	TCHAR szCanonDir[_MAX_PATH];

	// canonicalize the dir and filename first to remove . and ..
	if (!::PathCanonicalize(szCanonFilename, szFilename))
	{
		return FALSE;
	}

	if (!::PathCanonicalize(szCanonDir, szDir))
	{
		return FALSE;
	}

	// Make sure to filename is in the directory
	int nCommonPathLen = ::PathCommonPrefix(szCanonDir, szCanonFilename, NULL);
	return (nCommonPathLen == (int)_tcslen(szCanonDir)) ? TRUE : FALSE;
}

HRESULT CLauncher::GetFilename( LPCTSTR szName, LPTSTR szResult, LPCTSTR pszDefaultExt /*= _T(".txt")*/ )
{
	HRESULT hr;
	
	// Get the installation directory from the registry
	TCHAR szInstallDir[ _MAX_PATH ];
	hr = GetInstallationDirectory(szInstallDir, sizeof(szInstallDir));
	if(FAILED(hr)) return hr;

	// Add the install directory and an extension if not supplied
	::PathCombine( szResult, szInstallDir, szName );
	::PathAddExtension( szResult, pszDefaultExt );

	return S_OK;
}

BOOL CLauncher::IsFileExtension( LPCTSTR szFilename, LPCTSTR szExts)
{
	// szExts expects a string in the format ".ext1;.ext2" - notice the . must be included as well
	LPTSTR szFileExt = ::PathFindExtension(szFilename);
	LPTSTR szSeps = _T(";");
	LPTSTR szTempExts = _tcsdup(szExts);  // make a copy of szExts because it modifies the buffer
	LPTSTR szToken = _tcstok( szTempExts, szSeps);
	BOOL retval = FALSE;

	while (szToken != NULL)
	{
		if (_tcsicmp(szFileExt, szToken) == 0)
		{
			retval = TRUE;
			break;
		}
		szToken = _tcstok( NULL, szSeps);
	}
	free(szTempExts);
	return retval;
}

BOOL CLauncher::IsValidFileDirectory(LPCTSTR szFilename)
{
	HRESULT hr;

	// Get the installation directory from the registry to use for making sure the filenames are in the install path
	TCHAR szInstallDir[ _MAX_PATH ];
	hr = GetInstallationDirectory(szInstallDir, sizeof(szInstallDir));
	if (FAILED ( hr) )
		return FALSE;

	// Get the app data directory from the registry to use for making sure the filenames are in the appdata path
	CComBSTR bstrAppData;
	hr = get_AppDataDirectory(&bstrAppData);
	if (FAILED ( hr) )
		return FALSE;

	// Make sure from filename is in the installation or app data directory trees
	if (!VerifyFileInDirectoryTree(szFilename, szInstallDir) &&
		!VerifyFileInDirectoryTree(szFilename, CW2T((BSTR)bstrAppData)))
	{
		return FALSE;
	}
	return TRUE;
}

STDMETHODIMP CLauncher::CreateDirectory(BSTR bstrDir)
{
	if (!IsValidFileDirectory(CW2T(bstrDir))) {
		return Error(_T("Can't create directory unless in the app data or installation directory tree."), IID_ILauncher, E_FAIL);
	}

	// TODO: Re-implement later to support recursive dir creation
	// like SHCreateDirectoryEx. The latter cannot be used, since it's
	// not supported on pre-Win2k
	::CreateDirectoryEx(NULL, CW2T(bstrDir), NULL);
	int retval = ::GetLastError();
	switch (retval) {
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
		case ERROR_SUCCESS:
			return S_OK;
		case ERROR_BAD_PATHNAME:
		case ERROR_FILENAME_EXCED_RANGE:
		case ERROR_PATH_NOT_FOUND:
		default:
			return Error(_T("Failed to create directory."), IID_ILauncher, E_FAIL);
	}
}


STDMETHODIMP CLauncher::DisplayUserOptions()
{
	CUserOptionsDialog dlg;

	dlg.DoModal( NULL );

	return S_OK;
}