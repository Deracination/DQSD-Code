// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__FAB46B61_0076_4043_8667_338B151A9D97__INCLUDED_)
#define AFX_STDAFX_H__FAB46B61_0076_4043_8667_338B151A9D97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlwin.h>
#include <comdef.h>
#include <commctrl.h>
#include <wininet.h>
#include <msxml.h>
#include <ctype.h>
#include <time.h>
#include <atlhost.h>
#include <atlapp.h>
#include <atlctrls.h>

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Many thanks to Tim Tabor for the following (www.cheztabor.com)
#define HR(_ex) { HRESULT _hr = _ex; if (FAILED(_hr)) return _hr; }

#define LENGTHOF(a) (sizeof(a) / sizeof(a[0]))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FAB46B61_0076_4043_8667_338B151A9D97__INCLUDED)
