
/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook

#if !defined(KEYBOARDHOOK_H)
#define KEYBOARDHOOK_H

HRESULT KeyboardHookInstall(HWND hWnd);

//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT
KeyboardInstallHotkey(int vkCode, LPCTSTR pModifierNames, HWND* phwndNotification, LPDISPATCH pDispDocument);

extern std::map< long, long > g_mapKeyCodeToCharCode;

#endif // KEYBOARDHOOK_H