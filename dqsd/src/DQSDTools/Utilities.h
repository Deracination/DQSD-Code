#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#pragma once

//
// Returns the number of elements of a statically allocated array
// 
#define lengthof(arr) (sizeof(arr) / sizeof(*arr))

//
// ATLTRACE out the current function/method name
//
#define ATLTRACEMETHOD() ATLTRACE(_T("%s\n"), __FUNCTION__)

//
// URL helper functions
//
INTERNET_SCHEME GetScheme(LPCTSTR szURL);
int URLMatchesFilename(LPCTSTR szURL, LPCTSTR szFile);

//
// Find the DQSD window (usually, but not always, on the taskbar)
//
// Return: The window handle, or null if we fail
//
HWND UtilitiesFindDQSDWindow(LPDISPATCH pDispDocument);

extern HWND g_hDQSDWindow;
extern LPCTSTR DQSD_REG_KEY;
extern LPCTSTR DQSD_SEC_KEY;

// 
// Attempts to determine whether the window is placed on the taskbar or not.
//
BOOL IsWindowOnTaskbar(HWND hwnd);

// 
// Attempts to determine whether the window is visible on screen
//
BOOL IsWindowOnScreen(HWND hwnd);

HRESULT GetInstallationDirectory( LPTSTR szResult, DWORD dwResultSize );

#endif //__UTILITIES_H__
