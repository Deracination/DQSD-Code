// KeyboardHook.cpp : Single-thread Keyboard hook implementation
//

#include "stdafx.h"

#include "DQSDTools.h"
#include "KeyboardHook.h" 
#include "Launcher.h" 

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
					// Send a CTRL-A - special handler for this in search.htm
					SendMessage(GetFocus(), WM_CHAR, 'A'-'@', 0);
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
			_RPT1(_CRT_WARN, _T("Hook: %d\n"), wParam);
		}
	}
	return CallNextHookEx(hHook, code, wParam, lParam);
}



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

HWND hBarWnd = NULL;

LRESULT CALLBACK NotificationWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	if(uMsg == WM_HOTKEY)
	{
//		_RPT0(_CRT_WARN, "HotKey\n");
//		OutputDebugString("HotKey\n");

		// Find our window
		RECT taskBarRect;
		GetWindowRect(hBarWnd, &taskBarRect);

		// We do all this larking about with the mouse because
		// SetForegroundWindow is crippled nowadays, and because the DQSD edit control
		// doesn't actually seem to get a proper caret if you SFW to it anyway.

		// Save the mouse position before these games...
		POINT mousePoint;
		GetCursorPos(&mousePoint);

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
HHOOK KeyboardHookInstall()
{

	// Find the parent of the search window
	HWND hTrayWnd = FindWindow(_T("Shell_TrayWnd"), _T(""));
	if(hTrayWnd == NULL)
	{
		MessageBox(NULL, _T("Can't find tray window"), NULL, MB_OK);
		return FALSE;
	}

	// Look through its children to find the search window
	EnumChildWindows(hTrayWnd, EnumProc, (LPARAM)&hBarWnd);

	// Did we find the window?
	if(hBarWnd == NULL)
	{
		MessageBox(NULL, _T("Can't find search bar"), NULL, MB_OK);
		return FALSE;
	}

	// Get a handle to the thread which owns the message queue for this window
	DWORD threadId = GetWindowThreadProcessId(hTrayWnd, NULL);

	_RPT1(_CRT_WARN, "Thread ID 0x%x\n", threadId);

	hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, _Module.GetModuleInstance(), threadId);

	_RPT1(_CRT_WARN, "hHook 0x%x\n", hHook);

	return hHook;
}


//
// Install a hotkey
//
// Return:
//		HRESULT
//
HRESULT
KeyboardInstallHotkey(int vkCode)
{
	if(hBarWnd == NULL)
	{
		return CLauncher::Error(IDS_ERR_HOTKEY_NO_BAR_WINDOW, IID_ILauncher, E_FAIL);
	}

//	char buffer[100];
//	wsprintf(buffer, "HotKeyReg: %d", vkCode);
//	OutputDebugString(buffer);


	// Create a window which we use to receive notifications
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = NotificationWndProc;
	wc.lpszClassName = _T("DQSDHotKeyWindowClass");

	HWND hNotificationWindow;

	HWND hExistingWindow = FindWindow(wc.lpszClassName, _T("DQSDHotKeyWindow"));
	if(hExistingWindow != NULL)
	{
		// THere's already hotkey window
//		OutputDebugString("Hotkey - wnd exists");
		DestroyWindow(hExistingWindow);
	}
	else
	{
		RegisterClass(&wc);
	}

	hNotificationWindow = CreateWindow(wc.lpszClassName, _T("DQSDHotKeyWindow"), 0, 0, 0, 0, 0, hBarWnd, NULL, NULL, NULL);
	if(hNotificationWindow == NULL)
	{
		_RPT0(_CRT_WARN, "Failed to create a window for hotkeys\n");
		return CLauncher::Error(IDS_ERR_HOTKEY_WINDOW_FAILED, IID_ILauncher, E_FAIL);
	}

	// Make an ID for the window and store it in the windows userdata slot
	// so that it can be used to unregister the hotkey
	ATOM hotKeyId = GlobalAddAtom(_T("DQSDHotKeyAtom"));
	SetWindowLong(hNotificationWindow, GWL_USERDATA, hotKeyId);

	if(!RegisterHotKey(hNotificationWindow, hotKeyId, MOD_WIN, vkCode))
	{
		return CLauncher::Error(IDS_ERR_HOTKEY_REG_FAILED, IID_ILauncher, E_FAIL);
	}

	return S_OK;
}
