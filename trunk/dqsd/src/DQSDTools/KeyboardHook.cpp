// KeyboardHook.cpp : Single-thread Keyboard hook implementation
//

#include "stdafx.h"

#include "DQSDTools.h"
#include "KeyboardHook.h" 

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

	HWND hBarWnd = NULL;

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

