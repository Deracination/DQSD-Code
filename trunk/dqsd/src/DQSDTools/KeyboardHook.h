
/////////////////////////////////////////////////////////////////////////////
// CKeyboardHook

#if !defined(KEYBOARDHOOK_H)
#define KEYBOARDHOOK_H


HHOOK KeyboardHookInstall();

//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT
KeyboardInstallHotkey(int vkCode);

extern std::map< long, long > g_mapKeyCodeToCharCode;

#endif // KEYBOARDHOOK_H