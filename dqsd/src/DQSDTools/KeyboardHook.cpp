// KeyboardHook.cpp : Single-thread Keyboard hook implementation
//

#include "stdafx.h"

#include "DQSDTools.h"
#include "KeyboardHook.h" 
#include "Launcher.h" 
#include "Utilities.h" 


#define HOTKEY_WINDOW_CLASS_NAME	_T("DQSDHotKeyWindowClass")
#define HOTKEY_WINDOW_NAME			_T("DQSDHotKeyWindow")

std::map< long, long > g_mapKeyCodeToCharCode;

// The handle of our hook
static HHOOK hHook;


static LRESULT CALLBACK KeyboardProc(
							  int code,       // hook code
							  WPARAM wParam,  // virtual-key code
							  LPARAM lParam   // keystroke-message information
							  )
{
	if(code == HC_ACTION)
	{
		bool bKeyDown = !(lParam & 0x80000000);

		if(wParam == VK_INSERT)
		{
			if(GetAsyncKeyState(VK_SHIFT) & 0x80000000)
			{
				_RPT1(_CRT_WARN, _T("Shift-INS: %c\n"), bKeyDown ? 'P' : 'R');
				if(bKeyDown)
				{
					// Shift-INS is paste - send a CTRL-V
					SendMessage(GetFocus(), WM_CHAR, 'V'-'@', 0);
					return 0; 
				}
			}
			else if(GetAsyncKeyState(VK_CONTROL) & 0x80000000)
			{
				_RPT1(_CRT_WARN, _T("Ctrl-INS: %c\n"), bKeyDown ? 'P' : 'R');
				if(bKeyDown)
				{
					// Ctrl-INS is copy - send a CTRL-C
					SendMessage(GetFocus(), WM_CHAR, 'C'-'@', 0);
					return 0; 
				}
			}
		}
		else if(wParam == VK_DELETE)
		{
			_RPT1(_CRT_WARN, _T("DEL: %c\n"), bKeyDown ? 'P' : 'R');

			if(GetAsyncKeyState(VK_SHIFT) & 0x80000000)
			{ 
				_RPT1(_CRT_WARN, _T("Shift-DEL: %c\n"), bKeyDown ? 'P' : 'R');
				if(bKeyDown)
				{
					// Shift-DEL is cut - send a CTRL-X
					SendMessage(GetFocus(), WM_CHAR, 'X'-'@', 0);
					return 0; 
				}
			}
			else
			{
				// It's a DEL
				if(bKeyDown)
				{
					// Send a CTRL-D - special handler for this in search.htm
					SendMessage(GetFocus(), WM_CHAR, 'D'-'@', 0);
					return 0; 
				}
			}
		}
		else if(wParam == VK_UP)
		{
			if ( bKeyDown )
			{
				// Send a CTRL-P
				SendMessage(GetFocus(), WM_CHAR, 'P'-'@', 0);
				return 0;
			}
		}
		else if(wParam == VK_DOWN)
		{
			if ( bKeyDown )
			{
				// Send a CTRL-N
				SendMessage(GetFocus(), WM_CHAR, 'N'-'@', 0);
				return 0;
			}
		}
		else if(g_mapKeyCodeToCharCode.find( wParam ) != g_mapKeyCodeToCharCode.end() )
		{
			if ( bKeyDown )
			{
				SendMessage(GetFocus(), WM_CHAR, g_mapKeyCodeToCharCode[ wParam ], 0);
				return 0;
			}
		}
		else
		{
			ATLTRACE(_T("Hook: %d (focus 0x%x)\n"), wParam, GetFocus());
		}
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}


/*
static BOOL CALLBACK EnumProc(
							  HWND hwnd,		// handle to child window
							  LPARAM lParam		// Pointer to discovered taskbar window
							  )
{
	TCHAR className[100];
	className[0] = '\0';		// In case the call fails

	// Look for the window which corresponds to our deskbar
	GetClassName(hwnd, className, sizeof(className));
	if(StrCmpI(className, "Internet Explorer_Server") == 0)
	{
		_RPT1(_CRT_WARN, "Found bar - hWnd 0x%x\n", hwnd);
		*(HWND*)lParam = hwnd;
		return FALSE;
	}

	return TRUE;
}
*/

//HWND hBarWnd = NULL;

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
//		_RPT0(_CRT_WARN, "HotKey\n");
//		OutputDebugString("HotKey\n");

		if(uMsg == WM_HOTKEY)
		{
			nAttempts = 0;
		}
		else
		{
			nAttempts++;
			if(nAttempts > 20)
			{
				// We've failed in some way - kill the timer
				KillTimer(hwnd, 0x5744);
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
		ATLTRACE("TaskBarRect: %d,%d,%d,%d\n", taskBarRect.left, taskBarRect.top, taskBarRect.right, taskBarRect.bottom);
		if(taskBarRect.top >= GetSystemMetrics(SM_CYSCREEN) 
			|| taskBarRect.bottom < 0 
			|| taskBarRect.left >= GetSystemMetrics(SM_CXSCREEN)
			|| taskBarRect.right < 0)
		{
			// The taskbar is auto-hidden - we need to send more than one click - one to unhide the tool bar, 
			// and one to set the focus
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

		// Calculate the position of a simultated mouse click
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
//		OutputDebugString("HotKey Destroy\n");
		UnregisterHotKey(hwnd, GetWindowLong(hwnd, GWL_USERDATA));
	}

	return DefWindowProc(hwnd, uMsg,wParam,lParam);
}


//
// Install a keyboard hook on the search deskbars message handler thread
//
//
HRESULT KeyboardHookInstall(HWND hBarWnd, HHOOK& hInstalledHook)
{
	// Did we find the window?
	if(hBarWnd == NULL)
	{
		return CLauncher::Error(IDS_ERR_CANT_INSTALL_KEYBOARD_HOOK, IID_ILauncher);
	}
	
	// Get a handle to the thread which owns the message queue for this window
	DWORD threadId = GetWindowThreadProcessId(hBarWnd, NULL);

	ATLTRACE("Thread ID 0x%x\n", threadId);

	if(hHook != NULL)
	{
		ATLTRACE("DQSD: Keyboard Hook already installed\n");
		return CLauncher::Error(IDS_ERR_CANT_INSTALL_KEYBOARD_HOOK, IID_ILauncher);
	}

	hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, _Module.GetModuleInstance(), threadId);
	hInstalledHook = hHook;

	ATLTRACE("hHook 0x%x\n", hHook);

	return S_OK;
}

void
KeyboardHookRemove(HHOOK hInstalledHook)
{
	if(hInstalledHook != NULL)
	{
		UnhookWindowsHookEx(hInstalledHook);
		hHook = NULL;
	}
}


//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT
KeyboardInstallHotkey(int vkCode, LPCTSTR pModifierNames, HWND* phwndNotification, LPDISPATCH pDispDocument)
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
		// THere's already hotkey window
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
	SetWindowLong(*phwndNotification, GWL_USERDATA, hotKeyId);

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
		ATLTRACE("Failed to register hotkey (err %d)\n", GetLastError());
		return CLauncher::Error(IDS_ERR_HOTKEY_REG_FAILED, IID_ILauncher, E_FAIL);
	}

	return S_OK;
}
