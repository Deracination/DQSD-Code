#include "StdAfx.h"
#include <comdef.h>

#include "Utilities.h"

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


BOOL CALLBACK EnumChildProc(
  HWND hwnd,      // handle to child window
  LPARAM lParam   // application-defined value
)
{
	USES_CONVERSION;

	TCHAR className[201];

	UNREFERENCED_PARAMETER(lParam);

	GetClassName(hwnd, className, 200);
	if(StrCmpI(className, _T("OCHost")) == 0)
	{
		// We've found a likely looking window 
		std::string classList, thisClassName;
		HWND hWorkingWnd = hwnd;
		HWND hNextWindow;

		// Go to the bottom of the hierachy
		while((hNextWindow = GetWindow(hWorkingWnd, GW_CHILD)) != NULL)
		{
			hWorkingWnd = hNextWindow;
		}

		// Work back up
		do
		{
			GetClassName(hWorkingWnd, className, 200);
			// Using conversion macros in a loop is irresponsible, but it should be OK here
			// since the window hierarchy shouldn't be that deep.
			// Besides, T2A is a NOOP in ANSI builds.
			thisClassName = T2A(className); 
			thisClassName += ", ";
			classList.insert(0, thisClassName);
		}
		while((hWorkingWnd = GetParent(hWorkingWnd)) != NULL);

		ATLTRACE("ClassList: %s\n", classList.c_str());
		MessageBox(NULL, A2CT(classList.c_str()), _T("DQSD Window Path"), MB_OK | MB_ICONINFORMATION);
	}
	return TRUE;
}


BOOL CALLBACK TopLevelEnumCallback(
  HWND hwnd,      
  LPARAM lParam   
)
{
	UNREFERENCED_PARAMETER(lParam);

	EnumChildWindows(hwnd, EnumChildProc, NULL);
	return TRUE;
}


static void
WindowWalker()
{
	EnumWindows(TopLevelEnumCallback, NULL);
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
// Helper to return the screen coordinates as a RECT
//
BOOL GetScreenRect(LPRECT lpRect)
{
	// We need these without defining WINVER >= 0x0500
	// They are only used on a valid platform (> Win98)
	#define DQSD_SM_XVIRTUALSCREEN       76
	#define DQSD_SM_YVIRTUALSCREEN       77
	#define DQSD_SM_CXVIRTUALSCREEN      78
	#define DQSD_SM_CYVIRTUALSCREEN      79

	// Get screen rect depending on OS version
	OSVERSIONINFO vi = {0};
	vi.dwOSVersionInfoSize = sizeof(vi);
	if(!GetVersionEx(&vi))
		return FALSE;

	if((vi.dwMajorVersion > 4) || (vi.dwMajorVersion == 4 && vi.dwMinorVersion >= 10))
	{
		// Win98, ME, 2000, XP - Support for multiple monitors
		lpRect->top = GetSystemMetrics(DQSD_SM_YVIRTUALSCREEN);
		lpRect->left = GetSystemMetrics(DQSD_SM_XVIRTUALSCREEN);
		lpRect->bottom = lpRect->top + GetSystemMetrics(DQSD_SM_CYVIRTUALSCREEN);
		lpRect->right = lpRect->left + GetSystemMetrics(DQSD_SM_CXVIRTUALSCREEN);
	}
	else
	{
		// Win95 or other - No support for multiple monitors
		lpRect->top = 0;
		lpRect->left = 0;
		lpRect->bottom = GetSystemMetrics(SM_CYSCREEN);
		lpRect->right = GetSystemMetrics(SM_CXSCREEN);
	}

	return TRUE;
}

// 
// Attempts to determine whether the window is visible on screen
//
BOOL IsWindowOnScreen(HWND hwnd)
{
	// Get window dimensions
	RECT wndRect;
	GetWindowRect(hwnd, &wndRect);

	// Get screen dimensions
	RECT screenRect;
	GetScreenRect(&screenRect);

	// If wndRect is contained in screenRect, it's visible
	return (wndRect.top >= screenRect.top && wndRect.left >= screenRect.left && wndRect.bottom <= screenRect.bottom && wndRect.right <= screenRect.right);
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
	return (wndRect.top >= taskbarRect.top && wndRect.left >= taskbarRect.left && wndRect.bottom <= taskbarRect.bottom && wndRect.right <= taskbarRect.right);
}

/*

// This code looks untested, risky, not quite correct, and it's unused. (kimgrasman, 2003-06-26)

//
// Find the DQSD window (usually, but not always, on the taskbar)
//
// Return:
//		The window handle, or null if we fail
HWND
UtilitiesFindDQSDWindow(bool bCheckForNonTaskbar)
{
	// The following window hierarchy was determined using Spy++ on Windows XP Pro Build 2600.
	// Should be the same at least on Windows 2000 and other Win XP versions.
	static LPCTSTR rgszClassNames[][10] = 
	{
		{ 
			// This is a normal taskbar window
			_T("Shell_TrayWnd"), 
			_T("ReBarWindow32"), 
			_T("OCHost"), 
			_T("Shell Embedding"), 
			_T("Shell DocObject View"), 
			_T("Internet Explorer_Server"),
			NULL
		},
		// This is if you dock DQSD on it's own to a desktop edge
		{ 
			_T("BaseBar"), 
			_T("ReBarWindow32"), 
			_T("OCHost"), 
			_T("Shell Embedding"), 
			_T("Shell DocObject View"), 
			_T("Internet Explorer_Server"),
			NULL
		},
		{ 
			// I think this finds more than just the DQSD window, so where that matters, it's disabled by the bCheckForNonTaskbar parameter
			_T("IEFrame"), 
			_T("Shell DocObject View"), 
			_T("Internet Explorer_Server"),
			NULL,
		},

	};

	int maxTrees = bCheckForNonTaskbar ? 3 : 2;

	//We now traverse the array of window classes. 
	// The first window class is OCHost
	HWND hwndDQSD = NULL;

	// Make up to three passes with a delay here
	for(int nAttempt = 0; hwndDQSD == NULL && nAttempt < 3; nAttempt++)
	{
		for(int nTree = 0; hwndDQSD == NULL && nTree < maxTrees; nTree++)
		{
			for ( int i = 0; i < sizeof(rgszClassNames[0])/sizeof(rgszClassNames[0][0]); i++ ) 
			{
				LPCTSTR pClassName = rgszClassNames[nTree][i];
 
				if(pClassName == NULL)
				{
					// We've reached the end of a list - we must have found the window
					break;
				}

				hwndDQSD = ::FindWindowEx( hwndDQSD, NULL, pClassName, NULL );

				if ( NULL == hwndDQSD )
				{
					// We've failed to find the window using this attempt - break out of the 
					// inner traversal loop and attempt another class name list
					break;
				}
			}
		} 

		if(hwndDQSD == NULL)
		{
			Sleep(300);
		}
	}

	if(hwndDQSD == NULL)
	{
		WindowWalker();
	}

	ATLTRACE("UtilitiesFindDQSDWindow: Returning 0x%x\n", hwndDQSD);

	return hwndDQSD;
}
*/