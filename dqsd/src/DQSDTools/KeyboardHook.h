
/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook

#if !defined(KEYBOARDHOOK_H)
#define KEYBOARDHOOK_H


HRESULT KeyboardHookInstall();

//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT
KeyboardInstallHotkey(int vkCode, LPCTSTR pModifierNames);

extern std::map< long, long > g_mapKeyCodeToCharCode;

#endif // KEYBOARDHOOK_H