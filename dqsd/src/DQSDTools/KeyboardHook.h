#if !defined(KEYBOARDHOOK_H)
#define KEYBOARDHOOK_H

HRESULT KeyboardInstallHotkey(int vkCode, LPCTSTR pModifierNames, HWND* phwndNotification, LPDISPATCH pDispDocument);

#endif // KEYBOARDHOOK_H