#include "StdAfx.h"
#include "Utilities.h"
#include "BandConfig.h"

#define COMPILE_MULTIMON_STUBS
#include <multimon.h>

HWND g_hDQSDWindow;

INTERNET_SCHEME GetScheme(LPCTSTR szURL)
{
  TCHAR             buf[32];
  URL_COMPONENTS    uc;
  ZeroMemory(&uc, sizeof uc);

  uc.dwStructSize = sizeof uc;
  uc.lpszScheme = buf;
  uc.dwSchemeLength = lengthof(buf);

  if (InternetCrackUrl(szURL, lstrlen(szURL), ICU_DECODE, &uc))
     return uc.nScheme;
  else
     return INTERNET_SCHEME_UNKNOWN;
}

int URLMatchesFilename(LPCTSTR szURL, LPCTSTR szFile)
{
  TCHAR             pathbuf[_MAX_PATH];
  URL_COMPONENTS    uc;
  ZeroMemory(&uc, sizeof uc);

  uc.dwStructSize = sizeof uc;
  uc.lpszUrlPath = pathbuf;
  uc.dwUrlPathLength = lengthof(pathbuf);
  if (!InternetCrackUrl(szURL, lstrlen(szURL), ICU_DECODE, &uc))
     return FALSE;

  if (uc.nScheme != INTERNET_SCHEME_FILE)
    return FALSE;

  if (StrCmpI(uc.lpszUrlPath, szFile))
    return FALSE;

  return TRUE;
}

//
// Find the DQSD window (usually, but not always, on the taskbar)
//
// Return:
//		The window handle, or null if we fail
HWND
UtilitiesFindDQSDWindow(LPDISPATCH pDispDocument)
{
	IOleWindowPtr pOleWindow;

	try
	{
		HRESULT hResult = pDispDocument->QueryInterface(IID_IOleWindow, (LPVOID*)&pOleWindow);
		if(SUCCEEDED(hResult))
		{
			HWND hWnd;

			hResult = pOleWindow->GetWindow(&hWnd);
			ATLTRACE("UtilitiesFindDQSDWindow: hResult 0x%x, hWnd 0x%x\n", hResult, hWnd);
			if(SUCCEEDED(hResult))
			{
				g_hDQSDWindow = hWnd;
				return hWnd;
			}
		}
		else
		{
			ATLTRACE("UtilitiesFindDQSDWindow: GetSiteFailed hResult 0x%x\n", hResult);
		}

	}
	catch(_com_error& e)
	{
		e;
		ATLTRACE("UtilitiesFindDQSDWindow: COM exception: Desc %s, Message %d\n", 
			(LPCTSTR)e.Description(), 
			(LPCTSTR)e.ErrorMessage());
	}
	catch(...)
	{
		ATLTRACE("UtilitiesFindDQSDWindow: Unknown Exception Caught\n");
	}
	return NULL;
}

// 
// Attempts to determine whether the window is visible on screen
//
BOOL IsWindowOnScreen(HWND hwnd)
{
	// From multimon.h - should degrade gracefully on Win9x
	return (MonitorFromWindow(hwnd, MONITOR_DEFAULTTONULL) != NULL);
}

// 
// Attempts to determine whether the window is docked on the taskbar or not.
//
BOOL IsWindowOnTaskbar(HWND hwnd)
{
	// Get window dimensions
	RECT wndRect;
	GetWindowRect(hwnd, &wndRect);

	// Find the taskbar window
	HWND hwndTaskbar = FindWindow(_T("Shell_traywnd"), NULL);
	RECT taskbarRect;
	GetWindowRect(hwndTaskbar, &taskbarRect);

	// If wndRect is contained in taskbarRect, it's docked
	return (wndRect.top >= taskbarRect.top && 
			wndRect.left >= taskbarRect.left && 
			wndRect.bottom <= taskbarRect.bottom && 
			wndRect.right <= taskbarRect.right);
}

HRESULT IsAllowedURL(IObjectWithSite* pSite)
{
    HRESULT hr;

    CComPtr<IServiceProvider> spSrvProv;
    if (FAILED(hr = pSite->GetSite(IID_IServiceProvider, (void**)&spSrvProv)))
        return hr;

    CComPtr<IWebBrowser2> spWebBrowser;
    if (FAILED(hr = spSrvProv->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&spWebBrowser)))
        return hr;

    CComBSTR bstrURL;
    if (FAILED(hr = spWebBrowser->get_LocationURL(&bstrURL)))
        return hr;

    BandConfig config;
    return config.IsAllowedURL(CW2T(bstrURL));
}
