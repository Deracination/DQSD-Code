


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
UtilitiesFindDQSDWindow()
{
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

			hwndDQSD = NULL;

			for ( int j = 0; j < sizeof(rgszClassNames)/sizeof(rgszClassNames[0]); j++ )
			{
				if ( NULL == ( hwndDQSD = ::FindWindowEx( hwndDQSD, NULL, rgszClassNames[j], NULL ) ) )
				{
					hwndDQSD = NULL;
				}
			}

			break;
		}
	}

	return hwndDQSD;
}