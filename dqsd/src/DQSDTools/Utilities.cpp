


#include "StdAfx.h"
#include "Utilities.h"


INTERNET_SCHEME GetScheme(LPCTSTR szURL)
{
  TCHAR             buf[32];
  URL_COMPONENTS    uc;
  ZeroMemory(&uc, sizeof uc);

  uc.dwStructSize = sizeof uc;
  uc.lpszScheme = buf;
  uc.dwSchemeLength = sizeof buf;

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
  uc.dwUrlPathLength = sizeof pathbuf;
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
	TCHAR className[201];

	UNREFERENCED_PARAMETER(lParam);

	GetClassName(hwnd, className, 200);
	if(StrCmpI(className, "OCHost") == 0)
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
//			ATLTRACE("ClassName: %s\n", className);
			thisClassName = className;
			thisClassName += ", ";
			classList.insert(0, thisClassName);
		}
		while((hWorkingWnd = GetParent(hWorkingWnd)) != NULL);

		ATLTRACE("ClassList: %s\n", classList.c_str());
		MessageBox(NULL, classList.c_str(), "DQSD Window Path", MB_OK | MB_ICONINFORMATION);
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