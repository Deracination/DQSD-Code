#include "stdafx.h"

#include "KeyboardHook.h" 

#include "DQSDTools.h"
#include "Launcher.h" 
#include "Utilities.h" 

#define HOTKEY_WINDOW_CLASS_NAME	_T("DQSDHotKeyWindowClass")
#define HOTKEY_WINDOW_NAME			_T("DQSDHotKeyWindow")

LRESULT CALLBACK NotificationWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT KeyboardInstallHotkey(int vkCode, LPCTSTR pModifierNames, HWND* phwndNotification, LPDISPATCH pDispDocument)
{
    HWND hBarWnd = UtilitiesFindDQSDWindow(pDispDocument);
    if(hBarWnd == NULL)
    {
        return CLauncher::Error(IDS_ERR_HOTKEY_NO_BAR_WINDOW, IID_ILauncher, E_FAIL);
    }

    // Create a window which we use to receive notifications
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.lpfnWndProc = NotificationWndProc;
    wc.lpszClassName = HOTKEY_WINDOW_CLASS_NAME;

    HWND hExistingWindow = FindWindow(wc.lpszClassName, HOTKEY_WINDOW_NAME);
    if(hExistingWindow != NULL)
    {
        // There's already hotkey window
        ATLTRACE("HotKey - window exists\n");
        DestroyWindow(hExistingWindow);
    }

    RegisterClass(&wc);

    *phwndNotification = CreateWindow(wc.lpszClassName, HOTKEY_WINDOW_NAME, 0, 0, 0, 0, 0, hBarWnd, NULL, NULL, NULL);
    if(*phwndNotification == NULL)
    {
        ATLTRACE("Failed to create a window for hotkeys (err %d)\n", GetLastError());
        return CLauncher::Error(IDS_ERR_HOTKEY_WINDOW_FAILED, IID_ILauncher, E_FAIL);
    }

    // Make an ID for the window and store it in the windows userdata slot
    // so that it can be used to unregister the hotkey
    ATOM hotKeyId = GlobalAddAtom(_T("DQSDHotKeyAtom"));
    SetWindowLong(*phwndNotification, GWLP_USERDATA, hotKeyId);

    // Try and work out the modifier - default to the Windows key
    UINT keyModifier = 0;
    if(StrStrI(pModifierNames, _T("WIN")) != NULL)
    {
        keyModifier |= MOD_WIN;
    }
    if(StrStrI(pModifierNames, _T("ALT")) != NULL)
    {
        keyModifier |= MOD_ALT;
    }
    if(StrStrI(pModifierNames, _T("SHIFT")) != NULL)
    {
        keyModifier |= MOD_SHIFT;
    }
    if(StrStrI(pModifierNames, _T("CONTROL"))!= NULL)
    {
        keyModifier |= MOD_CONTROL;
    }
    if(keyModifier == 0)
    {
        // Blank or invalid modifier string - use WIN
        keyModifier = MOD_WIN;
    }

    if(!RegisterHotKey(*phwndNotification, hotKeyId, keyModifier, vkCode))
    {
        DWORD error = GetLastError();
        ATLTRACE("Failed to register hotkey (err %d)\n", error);
        return CLauncher::Error(IDS_ERR_HOTKEY_REG_FAILED, IID_ILauncher, HRESULT_FROM_WIN32(error));
    }

    return S_OK;
}

LRESULT CALLBACK NotificationWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	static int nAttempts;

	if(uMsg == WM_HOTKEY || (uMsg == WM_TIMER && wParam == 0x5744))
	{
		if(uMsg == WM_HOTKEY)
		{
			nAttempts = 0;
		}
		else
		{
			if(++nAttempts > 20)
			{
				// We've failed in some way - kill the timer and return
				KillTimer(hwnd, 0x5744);
				return 0;
			}
		}

		HWND hBarWnd = g_hDQSDWindow; // UtilitiesFindDQSDWindow();
		if(hBarWnd == NULL)
		{
			return 0;
		}

		// Save the mouse position before these games...
		POINT mousePoint;
		GetCursorPos(&mousePoint);

		// Find our window
		RECT taskBarRect;
		GetWindowRect(hBarWnd, &taskBarRect);

		// We do all this larking about with the mouse because
		// SetForegroundWindow is crippled nowadays, and because the DQSD edit control
		// doesn't actually seem to get a proper caret if you SFW to it anyway.
		if(!IsWindowOnScreen(hBarWnd))
		{
			// The taskbar is auto-hidden - we need to send more than one click - one to unhide the tool bar, 
			// and one to set the focus
			ATLTRACE("Search bar is off-screen - attempt to bring it into the light...\n");
			SetTimer(hwnd, 0x5744, 50, NULL);
		}
		else
		{
			// The taskbar is not autohidden - don't send any timer messages
			KillTimer(hwnd, 0x5744);

			// Test if our window is visible (rather than overlayed by something else)
			// This only happens if you have a taskbar without always-on-top set
			POINT topLeft;
			topLeft.x = taskBarRect.left;
			topLeft.y = taskBarRect.top;
			HWND hTestWindow = WindowFromPoint(topLeft);
			ATLTRACE("HotKey: Test window 0x%x, DQSD Window 0x%x\n", hTestWindow, hBarWnd);
			if(hBarWnd != hTestWindow)
			{
				SetForegroundWindow(hBarWnd);
				// Take the focus off the bar wnd, so that we get the normal 
				// 'highlight everything' behaviour when simulating a click into the bar
				SetFocus(GetParent(hBarWnd));
			}
		}

		// Calculate the position of a simulated mouse click
		// The SendInput structure takes a position scaled 0-65536 across the primary monitor
		// Hence the muldivs
		INPUT mouseClick;
		ZeroMemory(&mouseClick, sizeof(mouseClick));
		mouseClick.type = INPUT_MOUSE;
		mouseClick.mi.dx = (taskBarRect.left + taskBarRect.right) / 2;
		mouseClick.mi.dx = MulDiv(mouseClick.mi.dx, 65536, GetSystemMetrics(SM_CXSCREEN));
		mouseClick.mi.dy = (taskBarRect.top + taskBarRect.bottom) / 2;
		mouseClick.mi.dy = MulDiv(mouseClick.mi.dy, 65536, GetSystemMetrics(SM_CYSCREEN));
		mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;
		SendInput(1, &mouseClick, sizeof(mouseClick));

		mouseClick.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &mouseClick, sizeof(mouseClick));

		// Put the mouse back where we found it, because we're nice like that
		SetCursorPos(mousePoint.x, mousePoint.y);
	}
	else if(uMsg == WM_DESTROY)
	{
		ATLTRACE("Unregistering HotKey...\n");
        int hotkeyId = static_cast<int>(GetWindowLong(hwnd, GWLP_USERDATA));
		UnregisterHotKey(hwnd, hotkeyId);
	}

	return DefWindowProc(hwnd, uMsg,wParam,lParam);
}
