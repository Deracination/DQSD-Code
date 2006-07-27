// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__8BD68111_15F7_408F_A887_8130632A577E__INCLUDED_)
#define AFX_STDAFX_H__8BD68111_15F7_408F_A887_8130632A577E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0410
#endif

#define WINVER 0x410
#define _WIN32_IE 0x0501

#define _ATL_APARTMENT_THREADED

//#define _ATL_DEBUG_INTERFACES

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <shlwapi.h>
#include <wininet.h>

#include <atlctl.h>					// IObjectSafety,
#include <exdisp.h>					// IWebBrowser2,
#include <mshtml.h>
#include <shlguid.h>				// SID_SWebBrowserApp, 
#include <shellapi.h>				// ShellExecute, 
#include <commctrl.h>

#import <msxml4.dll> named_guids
//using namespace MSXML2;

#pragma warning(push, 3)

#pragma warning (disable : 4702)  // warning C4702: unreachable code 
#include <map>
#include <list>
#pragma warning (default : 4702) 
#include <string>
#include <memory>

#pragma warning(pop)

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8BD68111_15F7_408F_A887_8130632A577E__INCLUDED)
