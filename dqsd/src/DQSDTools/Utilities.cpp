


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

  if (InternetCrackUrl(szURL, _tcsclen(szURL), ICU_DECODE, &uc))
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
  if (!InternetCrackUrl(szURL, _tcsclen(szURL), ICU_DECODE, &uc))
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

	int maxAttempts = bCheckForNonTaskbar ? 3 : 2;

	//We now traverse the array of window classes. 
	// The first window class is OCHost
	HWND hwndDQSD = NULL;
	for(int nAttempt = 0; hwndDQSD == NULL && nAttempt < maxAttempts; nAttempt++)
	{
		for ( int i = 0; i < sizeof(rgszClassNames[0])/sizeof(rgszClassNames[0][0]); i++ ) 
		{
			LPCTSTR pClassName = rgszClassNames[nAttempt][i];

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

	ATLTRACE("UtilitiesFindDQSDWindow: Returning 0x%x\n", hwndDQSD);

	return hwndDQSD;
}