
#pragma once

INTERNET_SCHEME GetScheme(LPCTSTR szURL);
int URLMatchesFilename(LPCTSTR szURL, LPCTSTR szFile);

//
// Find the DQSD window (usually, but not always, on the taskbar)
//
// Return:
//		The window handle, or null if we fail
HWND
UtilitiesFindDQSDWindow();
