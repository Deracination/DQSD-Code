////////////////////////////////////////////////////////////////
// 1999 Microsoft Systems Journal
//
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef __MODULEVER_H
#define __MODULEVER_H

#ifndef DLLVER_PLATFORM_WINDOWS
// NOTE: DllGetVersion defs are in shlwapi.h, which is in newer versions
// of the SDK, and also in VC6. The definitions are also in the PixieLib
// file vc6defs.h.
#include <shlwapi.h>
#endif

// tell linker to link with version.lib for VerQueryValue, etc.
#pragma comment(linker, "/defaultlib:version.lib")

//////////////////
// This class loads a library. Destructor frees for automatic cleanup.
//
class CLoadLibrary {
private:
	HINSTANCE m_hinst;
public:
	CLoadLibrary(LPCTSTR lpszName) : m_hinst(LoadLibrary(lpszName)) { }
	~CLoadLibrary()		 { FreeLibrary(m_hinst); }
	operator HINSTANCE () { return m_hinst; } 	// cast operator
};

//////////////////
// CModuleVersion version info about a module.
// To use:
//
// CModuleVersion ver
// if (ver.GetFileVersionInfo("_T("mymodule))) {
//		// info is in ver, you can call GetValue to get variable info like
//		CString s = ver.GetValue(_T("CompanyName"));
// }
//
// You can also call the static fn DllGetVersion to get DLLVERSIONINFO.
//
class CModuleVersion : public VS_FIXEDFILEINFO {
protected:
	BYTE* m_pVersionInfo;	// all version info

	struct TRANSLATION {
		WORD langID;			// language ID
		WORD charset;			// character set (code page)
	} m_translation;

public:
	CModuleVersion();
	virtual ~CModuleVersion();

	BOOL		GetFileVersionInfo(LPCTSTR modulename);
	string GetValue(LPCTSTR lpKeyName);
	static BOOL DllGetVersion(LPCTSTR modulename, DLLVERSIONINFO& dvi);
};

#endif
