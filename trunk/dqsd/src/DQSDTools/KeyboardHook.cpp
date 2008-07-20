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
static HWND  hHookedWindow;


static LRESULT CALLBACK KeyboardProc(
							  int code,       // hook code
							  WPARAM wParam,  // virtual-key code
							  LPARAM lParam   // keystroke-message information
							  )
{
	if(code == HC_ACTION)
	{
		HWND hFocusWnd = GetFocus();
		if(hFocusWnd != hHookedWindow)
		{
			// This is not us - don't do anything
			ATLTRACE("Hooked message arrived with wrong focus (DQSD 0x%x, focus 0x%x)\n", hHookedWindow, hFocusWnd);
			return CallNextHookEx(hHook, code, wParam, lParam);
		}

		// Check if the IME is up (NOTE: You need to link with IMM32.LIB)
		HIMC himc = ImmGetContext(hFocusWnd);
		bool bImmOn = (ImmGetOpenStatus (himc) != 0);
		ImmReleaseContext (hFocusWnd, himc);

		if (bImmOn)
		{
			// This is not us - don't do anything
			ATLTRACE("IMM is on\n");
			return CallNextHookEx(hHook, code, wParam, lParam);
		}

		bool bKeyDown = !(lParam & 0x80000000);
		if(bKeyDown && IsWindowOnTaskbar(hFocusWnd))
		{
			if(wParam == VK_INSERT)
			{
				if(GetAsyncKeyState(VK_SHIFT) & 0x80000000)
				{
					// Shift-INS is paste - send a CTRL-V
					ATLTRACE(_T("Shift-INS: %c\n"), bKeyDown ? 'P' : 'R');
					SendMessage(hFocusWnd, WM_CHAR, 'V'-'@', 0);
					return 0; 
				}
				else if(GetAsyncKeyState(VK_CONTROL) & 0x80000000)
				{
					// Ctrl-INS is copy - send a CTRL-C
					ATLTRACE(_T("Ctrl-INS: %c\n"), bKeyDown ? 'P' : 'R');
					SendMessage(hFocusWnd, WM_CHAR, 'C'-'@', 0);
					return 0; 
				}
			}
			else if(wParam == VK_DELETE)
			{
				ATLTRACE(_T("DEL: %c\n"), bKeyDown ? 'P' : 'R');

				if(GetAsyncKeyState(VK_SHIFT) & 0x80000000)
				{ 
					// Shift-DEL is cut - send a CTRL-X
					ATLTRACE(_T("Shift-DEL: %c\n"), bKeyDown ? 'P' : 'R');
					SendMessage(hFocusWnd, WM_CHAR, 'X'-'@', 0);
					return 0; 
				}
				else
				{
					// It's a DEL. Send a CTRL-D - special handler for this in search.htm
					SendMessage(hFocusWnd, WM_CHAR, 'D'-'@', 0);
					return 0; 
				}
			}
			else if(wParam == VK_UP)
			{
				// Send a CTRL-P
				SendMessage(hFocusWnd, WM_CHAR, 'P'-'@', 0);
				return 0;
			}
			else if(wParam == VK_DOWN)
			{
				// Send a CTRL-N
				SendMessage(hFocusWnd, WM_CHAR, 'N'-'@', 0);
				return 0;
			}
			else if(g_mapKeyCodeToCharCode.find(wParam) != g_mapKeyCodeToCharCode.end())
			{
				SendMessage(hFocusWnd, WM_CHAR, g_mapKeyCodeToCharCode[wParam], 0);
				return 0;
			}
			else
			{
				ATLTRACE(_T("Hook: %d (focus 0x%x)\n"), wParam, GetFocus());
			}
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

	hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, _AtlBaseModule.GetModuleInstance(), threadId);
	hInstalledHook = hHook;
	hHookedWindow = hBarWnd;

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

void RegisterKeyCharMapping(long virtualKeyCode, long charCode)
{
    g_mapKeyCodeToCharCode[virtualKeyCode] = charCode;
}