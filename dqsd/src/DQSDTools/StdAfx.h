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
#define _WIN32_WINNT 0x0502 // Windows XP SP2
#endif

#define WINVER 0x410
#define _WIN32_IE 0x0603 // IE6 with XP SP2

#define _CRT_SECURE_NO_WARNINGS
#define _ATL_APARTMENT_THREADED
//#define _ATL_DEBUG_INTERFACES

#include <atlbase.h>
#include <atlcom.h>

#include <shlwapi.h>
#include <wininet.h>

#include <atlctl.h>					// IObjectSafety
#include <exdisp.h>					// IWebBrowser2
#include <exdispid.h>			    // DISPID_NAVIGATECOMPLETE, etc
#include <shlguid.h>				// SID_SWebBrowserApp 
#include <shellapi.h>				// ShellExecute 
#include <shlobj.h>
#include <shobjidl.h>
#include <shlguid.h>

#pragma warning (disable : 4702)  // warning C4702: unreachable code 
#include <map>
#pragma warning (default : 4702) 

#import <msxml4.dll> named_guids

#include "Utilities.h"
#include "ServiceEntryTearOff.h"
#include "IDocHostUIHandlerFakeImpl.h"
#include "IPersistStreamFakeImpl.h"
#include "IOleClientSiteFakeImpl.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8BD68111_15F7_408F_A887_8130632A577E__INCLUDED)
