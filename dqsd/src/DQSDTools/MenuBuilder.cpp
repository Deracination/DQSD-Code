// MenuBuilder.cpp : Implementation of CMenuBuilder
#include "stdafx.h"

#include "DQSDTools.h"
#include "MenuBuilder.h"
#include "Utilities.h"


/////////////////////////////////////////////////////////////////////////////
// CMenuBuilder

#define TOOLBAR_TRACKER_WINDOW_CLASS_NAME		_T("DQSDMenuTrackerClass")
#define TOOLBAR_TRACKER_WINDOW_NAME				_T("DQSDMenuTracker")

HWND		CMenuBuilder::m_hTooltipWnd;

STDMETHODIMP CMenuBuilder::SetSite(IUnknown* pUnkSite)
{
#if defined(DQSD_NOSECURITY) && defined(_DEBUG)
#pragma message(  __FILE__ " ** WARNING! ** Compilation without security restrictions...do not distribute the resulting binary! " )
#else
	USES_CONVERSION;

	HRESULT hr;

	m_spUnkSite = pUnkSite;

	CComPtr<IServiceProvider> spSrvProv;
	if (FAILED(hr = GetSite(IID_IServiceProvider, (void**)&spSrvProv)))
		return hr;

	CComPtr<IWebBrowser2> spWebBrowser;
	if (FAILED(hr = spSrvProv->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (void**)&spWebBrowser)))
		return hr;

	CComBSTR bstrURL;
	if (FAILED(hr = spWebBrowser->get_LocationURL(&bstrURL)))
		return hr;

	HKEY hDqsdKey;
	if (ERROR_SUCCESS != RegOpenKey(HKEY_CLASSES_ROOT, DQSD_SEC_KEY, &hDqsdKey))
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

	
	DWORD dt;
	TCHAR filebuf[MAX_PATH];
	DWORD filelen = sizeof(filebuf);
	DWORD idw = 0;
	BOOL success = FALSE;

	while (ERROR_SUCCESS == RegEnumValue(hDqsdKey, idw, filebuf, &filelen, NULL, &dt, NULL, NULL))
	{
		idw++;
		if (URLMatchesFilename(OLE2T(bstrURL), filebuf))
		{
			success = TRUE;
			break;
		}

		filelen = sizeof(filebuf);
	}

	RegCloseKey(hDqsdKey);

    if (success == FALSE)
	{
		Error(IDS_ERR_UNAUTHCALLER, IID_ILauncher);
		return E_FAIL;
	}

#endif

  return S_OK;
}

STDMETHODIMP CMenuBuilder::Display(LPDISPATCH pDispDocument, VARIANT* pvarSelection)
{
	USES_CONVERSION;

	BSTR bstrSelection = NULL;
	VariantInit( pvarSelection );
	pvarSelection->pbstrVal = NULL;
	pvarSelection->vt = VT_NULL;

	HWND hwndDQSD = UtilitiesFindDQSDWindow(pDispDocument);

	RECT rcParentWnd;
	::GetWindowRect( hwndDQSD, &rcParentWnd );

	HWND hPopupLinkedWindow;
	UINT menuOptions = TPM_RETURNCMD|TPM_BOTTOMALIGN|getHorizontalPosition();
	// Are we going to try tooltip tracking?
	if(m_hTrackerWnd != NULL)
	{
		hPopupLinkedWindow = m_hTrackerWnd;
	}
	else
	{
		hPopupLinkedWindow = hwndDQSD;
		menuOptions |= TPM_NONOTIFY;
	}

	int nMenuX = rcParentWnd.right;
	switch ( m_nHorizontalAlignment )
	{
	case CMenuBuilder::CENTER: 
		nMenuX = rcParentWnd.left + ( ( rcParentWnd.right - rcParentWnd.left ) / 2 );
		break;
	case CMenuBuilder::LEFT:
		nMenuX = rcParentWnd.left;
		break;
	case CMenuBuilder::RIGHT:
		nMenuX = rcParentWnd.right;
		break;
	}

	int iMenuItem = ::TrackPopupMenuEx( m_hMain, menuOptions, nMenuX, rcParentWnd.top, hPopupLinkedWindow, NULL );
	if ( iMenuItem > 0 )
	{
		bstrSelection = A2BSTR( m_mapKeys[ iMenuItem ].c_str() );

		VariantInit( pvarSelection );
		pvarSelection->bstrVal = bstrSelection;
		pvarSelection->vt = VT_BSTR;
	}

	::DestroyMenu( m_hMain );
	m_hMain = NULL;

	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendMenuItem(BSTR bstrItem, BSTR bstrKey, BSTR bstrToolTip, VARIANT* pvhMenu )
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvhMenu && ((VT_I4 == pvhMenu->vt) || (VT_I2 == pvhMenu->vt)) )
	{
		hmenu = pvhMenu->intVal ? (HMENU)pvhMenu->intVal : m_hMain;
	}

	BOOL bSuccess = ::AppendMenu( hmenu, MF_STRING, m_nMenuItem+1, W2T( bstrItem ) );
	if ( !bSuccess )
		return E_FAIL;

	m_nMenuItem++;
	m_mapKeys[ m_nMenuItem ] = std::string( OLE2A( bstrKey ) );
	m_toolTips[m_nMenuItem] = std::string( OLE2A( bstrToolTip ) );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendSubMenu(BSTR bstrName, VARIANT* pvParentMenu, long *phmenu)
{
	*phmenu = (long)::CreatePopupMenu();

	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	::AppendMenu( hmenu, MF_POPUP, (UINT_PTR)*phmenu, W2T( bstrName ) );
	
	return S_OK;
}

STDMETHODIMP CMenuBuilder::AppendSeparator(VARIANT* pvParentMenu)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	::AppendMenu( hmenu, MF_SEPARATOR, NULL, NULL );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::get_HorizontalAlignment(short *pVal)
{
	*pVal = (SHORT)(m_nHorizontalAlignment & 0x0000FFFF);

	return S_OK;
}

STDMETHODIMP CMenuBuilder::put_HorizontalAlignment(short newVal)
{
	switch ( newVal )
	{
	case CMenuBuilder::CENTER:
	case CMenuBuilder::LEFT:
	case CMenuBuilder::RIGHT:
		m_nHorizontalAlignment = newVal;
		break;
	default:
		Error(IDS_ERR_INVALIDHORIZALIGNMENT, IID_IMenuBuilder);
		return E_FAIL;
	}

	return S_OK;
}

UINT CMenuBuilder::getHorizontalPosition()
{
	switch ( m_nHorizontalAlignment )
	{
	case CMenuBuilder::CENTER: return TPM_CENTERALIGN;
	case CMenuBuilder::LEFT: return TPM_LEFTALIGN;
	case CMenuBuilder::RIGHT: return TPM_RIGHTALIGN;
	}
	return TPM_RIGHTALIGN;
}

LRESULT CALLBACK CMenuBuilder::TrackerWndProc(
	HWND hwnd,      // handle to window
	UINT uMsg,      // message identifier
	WPARAM wParam,  // first message parameter
	LPARAM lParam   // second message parameter
	)
{
	static bool bAdded;

	if(uMsg == WM_INITMENUPOPUP)
	{
		// It's a new popup - delete any tips
		LRESULT nTools = ::SendMessage(m_hTooltipWnd, TTM_GETTOOLCOUNT, 0, 0);
		ATLTRACE("Tooltips: %d existing tools\n", nTools);
		for(LRESULT nTool = nTools-1; nTool >= 0; nTool--)
		{
			TOOLINFO ti;
			ZeroMemory(&ti, sizeof(ti));
			ti.cbSize = sizeof(TOOLINFO);

			if(::SendMessage(m_hTooltipWnd, TTM_ENUMTOOLS, nTool, (LPARAM)&ti))
			{
				ATLTRACE("Tool %d - hWnd 0x%x, id %d\n", nTool, ti.hwnd, ti.uId);

				::SendMessage(m_hTooltipWnd, TTM_DELTOOL, 0, (LPARAM)&ti);

			}
		}
		bAdded = false;
	}
	if(uMsg == WM_MENUSELECT)
	{
		if(HIWORD(wParam) & MF_POPUP)
		{
			ATLTRACE("Popup - ignoring\n");
		}
		else
		{
			UINT itemId = LOWORD(wParam);
			HMENU hMenu = (HMENU)lParam;
			CMenuBuilder* pThis;

			pThis = (CMenuBuilder*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

			// Is there a tooltip for this item?
			if(pThis != NULL && pThis->m_toolTips.count(itemId) > 0)
			{
				// It's worse than I thought.
				// Because very long menus can scroll, nItemPosition needs to be relative
				// to the topmost DISPLAYED item, rather than the top of the whole menu.
				// I've resorted to hit-testing each item rectangle, to find the one that we want
				// Because I'm too lazy to change the code, there's a redudant second look-up
				// of the item rectangle after this loop completes.
				POINT cursorPos;
				DWORD messagePos = GetMessagePos();
				cursorPos.x = GET_X_LPARAM(messagePos);
				cursorPos.y = GET_Y_LPARAM(messagePos);

				int nItems = ::GetMenuItemCount(hMenu);
				int nItemPosition;
				for(nItemPosition = 0; nItemPosition < nItems; nItemPosition++)
				{
					RECT itemRect;
					GetMenuItemRect(NULL, hMenu, nItemPosition, &itemRect);
					if(PtInRect(&itemRect, cursorPos))
					{
						// We've found our item
						break;
					}
				}

//				ATLTRACE("ItemId: %d\n", itemId);

				// If we don't find it, nItemPosition will be == nItems, so the GetMenuItemRect call will fail
				// and we'll cope with that
				RECT itemRect;
				if(GetMenuItemRect(NULL, hMenu, nItemPosition, &itemRect))
				{

	//				ATLTRACE("ItemRect: %d,%d,%d,%d\n", itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);

					TOOLINFO ti;
					ZeroMemory(&ti, sizeof(ti));
					ti.cbSize = sizeof(ti);

					if(bAdded)
					{
						::SendMessage(m_hTooltipWnd, TTM_GETTOOLINFO, 0, (LPARAM)&ti);
					}

//					Rectangle(GetDC(NULL), itemRect.left, itemRect.top, itemRect.right, itemRect.bottom);

					POINT topLeft;
					topLeft.x = itemRect.left;
					topLeft.y = itemRect.top;

					// Find the menu's window
					ti.hwnd = WindowFromPoint(topLeft);

					ti.uFlags = TTF_TRANSPARENT;
					ti.uId = 1;		

					// This looks horrible, because I'm casting away the const, but it's safe
					// because I don't ask the ToolTip window to overwrite any data
					ti.lpszText = (LPTSTR)(pThis->m_toolTips[itemId].c_str());

					// Map the rectangle relative to the menu window
					MapWindowPoints(NULL, ti.hwnd, (LPPOINT)&itemRect, 2);
					ti.rect = itemRect;
					
//					ATLTRACE("ToolRect: (wnd %x) %d,%d,%d,%d\n", ti.hwnd, ti.rect.left, ti.rect.top, ti.rect.right, ti.rect.bottom);
			
					// We only have one tool tip - if it's already been created,
					// we move it to the new position and change its text


					if(bAdded)
					{
//						ATLTRACE("Moving\n");
						// When we just move an existing tooltip, we deactivate it
						// then reactivate it in the new position.  This has the effect of starting
						// the display delay again, which is an improvement (and makes the tt behaviour
						// closer to that of the XP start menu)
						::SendMessage(m_hTooltipWnd, TTM_ACTIVATE, FALSE, 0);

						::SendMessage(m_hTooltipWnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);
						::SendMessage(m_hTooltipWnd, TTM_SETTOOLINFO, 0, (LPARAM)&ti);
						::SendMessage(m_hTooltipWnd, TTM_NEWTOOLRECT, 0, (LPARAM)&ti);

						::SendMessage(m_hTooltipWnd, TTM_ACTIVATE, TRUE, 0);
					}
					else
					{ 
//						ATLTRACE("Adding\n");
						if(!::SendMessage(m_hTooltipWnd, TTM_ADDTOOL, 0, (LPARAM)&ti))
						{
							ATLTRACE("AddTool failed\n");
						}
						else
						{
							bAdded = true;
						}
					}

					// Send a simulated mouse-move to the tooltip window to let it know that it should display the tip
					static MSG msgHere = { NULL,WM_MOUSEMOVE,0,0,0,0 };
					msgHere.hwnd = ti.hwnd;
					msgHere.lParam = MAKELPARAM(ti.rect.left, ti.rect.top);
					msgHere.pt.x = ti.rect.left + 5;
					msgHere.pt.y = ti.rect.top + 5;

//					ATLTRACE("Sending mouse move %d,%d\n", ti.rect.left, ti.rect.top);

					::SendMessage(m_hTooltipWnd, TTM_RELAYEVENT, 0, (LPARAM)&msgHere);
				}
			}
		}
	}

	return DefWindowProc(hwnd, uMsg,wParam,lParam);
}


// Set up the tracking window which can follow the menu position and display tooltips
STDMETHODIMP CMenuBuilder::InitialiseTooltips(long displayTimeMultiplier)
{
	// Create a window which we use to receive menu tracking info
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = TrackerWndProc;
	wc.lpszClassName = TOOLBAR_TRACKER_WINDOW_CLASS_NAME;

	HWND hExistingWindow = FindWindow(wc.lpszClassName, TOOLBAR_TRACKER_WINDOW_NAME);
	if(hExistingWindow != NULL)
	{
		// THere's already hotkey window
		ATLTRACE("MenuTracker - window exists\n");
		DestroyWindow(hExistingWindow);
	}

	RegisterClass(&wc);

	// Create the menu tracking window
	m_hTrackerWnd = CreateWindow(wc.lpszClassName, TOOLBAR_TRACKER_WINDOW_NAME, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	if(m_hTrackerWnd == NULL)
	{
		ATLTRACE("Failed to create a window for MenuTracker (err %d)\n", GetLastError());
		return Error(IDS_ERR_MENU_TRACKER_WINDOW_FAILED, IID_ILauncher, E_FAIL);
	}

	// Store a 'this' pointer on the tracker window
	::SetWindowLongPtr(m_hTrackerWnd, GWLP_USERDATA, (LONG_PTR)this);

	// Create a tooltip window
	if(m_hTooltipWnd != NULL)
	{
		DestroyWindow(m_hTooltipWnd);
	}

	m_hTooltipWnd = CreateWindow(TOOLTIPS_CLASS,
		NULL,
		TTS_NOPREFIX | TTS_ALWAYSTIP,		 
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		_AtlBaseModule.GetModuleInstance(),
		NULL
		);
	if(m_hTooltipWnd == NULL)
	{
		return Error(_T("Failed to create tooltip window"), IID_ILauncher, E_FAIL);
	}

	::SetWindowPos(m_hTooltipWnd,
		HWND_TOPMOST,
		0,
		0,
		0,
		0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREDRAW);

	RECT tipMargin = { 5,5,5,5 };
	::SendMessage(m_hTooltipWnd, TTM_SETMARGIN, 0, (LPARAM)&tipMargin);

	// Set a maximum width - this enables multi-line tooltips
	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&workArea, 0);
	::SendMessage(m_hTooltipWnd, TTM_SETMAXTIPWIDTH, 0, (workArea.right-workArea.left)/3);

	// Set the tool tip time multiplier
	// (GetDoubleClickTime()*10 is the system default)
	ATLTRACE("InitToolTips: DisplayTimeMult %d\n", displayTimeMultiplier);
	displayTimeMultiplier = max(1, displayTimeMultiplier);
	::SendMessage(m_hTooltipWnd, TTM_SETDELAYTIME, TTDT_AUTOPOP, GetDoubleClickTime()*10*displayTimeMultiplier);

// Subclass the tooltip window - just for debugging
//	lpfnOldWndProc = (WNDPROC)SetWindowLong(m_hTooltipWnd, GWL_WNDPROC, (DWORD)ToolTipSubClassFunc);

	return S_OK;
}

STDMETHODIMP CMenuBuilder::InsertMenuItem(BSTR bstrItem, BSTR bstrKey, BSTR bstrToolTip, UINT position, VARIANT* pvhMenu )
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvhMenu && ((VT_I4 == pvhMenu->vt) || (VT_I2 == pvhMenu->vt)) )
	{
		hmenu = pvhMenu->intVal ? (HMENU)pvhMenu->intVal : m_hMain;
	}

	MENUITEMINFO menuItem;
	memset(&menuItem, 0, sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
	menuItem.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID; 
	menuItem.fType = MFT_STRING; 
	menuItem.fState = MFS_ENABLED; 
	menuItem.wID = (m_nMenuItem+1);
	menuItem.dwTypeData = W2T( bstrItem );
	menuItem.cch = lstrlen( W2T( bstrItem) );

	BOOL bSuccess = ::InsertMenuItem( hmenu, position, TRUE, &menuItem);
	if ( !bSuccess)
		return E_FAIL;

	m_nMenuItem++;
	m_mapKeys[ m_nMenuItem ] = std::string( OLE2A( bstrKey ) );
	m_toolTips[m_nMenuItem] = std::string( OLE2A( bstrToolTip ) );

	return S_OK;
}

STDMETHODIMP CMenuBuilder::InsertSeparator(UINT position, VARIANT* pvParentMenu)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	MENUITEMINFO menuItem;
	memset(&menuItem, 0, sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
	menuItem.fMask = MIIM_TYPE | MIIM_STATE; 
	menuItem.fType = MFT_SEPARATOR; 
	menuItem.fState = MFS_ENABLED; 

	BOOL bSuccess = ::InsertMenuItem( (HMENU)hmenu, position, TRUE, &menuItem);
	if ( !bSuccess)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CMenuBuilder::InsertSubMenu(BSTR bstrName, UINT position, VARIANT* pvParentMenu, long *phmenu)
{
	*phmenu = (long)::CreatePopupMenu();

	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	MENUITEMINFO menuItem;
	memset(&menuItem, 0, sizeof(MENUITEMINFO));
	menuItem.cbSize = sizeof(MENUITEMINFO);
	menuItem.fMask = MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU; 
	menuItem.fType = MFT_STRING; 
	menuItem.fState = MFS_ENABLED; 
	menuItem.hSubMenu = (HMENU)*phmenu;
	menuItem.dwTypeData = W2T( bstrName );
	menuItem.cch = lstrlen( W2T( bstrName) );

	BOOL bSuccess = ::InsertMenuItem( (HMENU)hmenu, position, TRUE, &menuItem);
	if ( !bSuccess)
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CMenuBuilder::GetMenuItemCount(VARIANT* pvParentMenu, long *pCount)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	int retval = ::GetMenuItemCount(hmenu);
	if (retval == -1)
	{
		return Error(_T("Invalid menu handle"), IID_IMenuBuilder, E_FAIL);
	}
	*pCount = (long)retval;
	return S_OK;
}

STDMETHODIMP CMenuBuilder::FindSubMenu(BSTR bstrName, VARIANT* pvParentMenu, long *phmenu)
{
	USES_CONVERSION;

	*phmenu = -1;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	int nItems = ::GetMenuItemCount(hmenu);
	int nItemPosition;
	for(nItemPosition = 0; nItemPosition < nItems; nItemPosition++)
	{
		TCHAR menuName[101];
		int len = ::GetMenuString(hmenu, nItemPosition, menuName, 100, MF_BYPOSITION);
		if (len != 0)
		{
			if (lstrcmp(W2T(bstrName), menuName)==0)
			{
				*phmenu = (long)::GetSubMenu(hmenu, nItemPosition);
				if (*phmenu == 0)
				{
					*phmenu = -1;
				}
				break;
			}
		}
	}

	return S_OK;
}

STDMETHODIMP CMenuBuilder::FindMenuItem(BSTR bstrName, VARIANT* pvParentMenu, long *pPosition)
{
	USES_CONVERSION;

	*pPosition = -1;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	int nItems = ::GetMenuItemCount(hmenu);
	int nItemPosition;
	for(nItemPosition = 0; nItemPosition < nItems; nItemPosition++)
	{
		TCHAR menuName[101];
		int len = ::GetMenuString(hmenu, nItemPosition, menuName, 100, MF_BYPOSITION);
		if (len != 0)
		{
			if (lstrcmp(W2T(bstrName), menuName)==0)
			{
				*pPosition = (long)nItemPosition;
				break;
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CMenuBuilder::GetMenuString(UINT position, VARIANT* pvParentMenu, BSTR* pbstrResult)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	CComBSTR bstrMenuName;
	TCHAR menuName[101];
	int nResult = ::GetMenuString(hmenu, position, menuName, 100, MF_BYPOSITION);
	if (nResult > 0) {
		bstrMenuName.Append(menuName);
	}
	*pbstrResult = bstrMenuName.Detach();
	return S_OK;
}

STDMETHODIMP CMenuBuilder::GetMenuItemID(UINT position, VARIANT* pvParentMenu, long *nID)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	if (nID != NULL) {
		*nID = (long)::GetMenuItemID(hmenu, position);
		return S_OK;
	} else {
		return E_FAIL;
	}
}

STDMETHODIMP CMenuBuilder::EnableMenuItem(UINT position, VARIANT* pvParentMenu)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}

	::EnableMenuItem(hmenu, position, MF_BYPOSITION | MF_ENABLED);
	return S_OK;
}


STDMETHODIMP CMenuBuilder::DisableMenuItem(UINT position, VARIANT* pvParentMenu)
{
	USES_CONVERSION;

	HMENU hmenu = m_hMain;
	if (pvParentMenu && ((VT_I4 == pvParentMenu->vt) || (VT_I2 == pvParentMenu->vt)) )
	{
		hmenu = pvParentMenu->intVal ? (HMENU)pvParentMenu->intVal : m_hMain;
	}
	::EnableMenuItem(hmenu, position, MF_BYPOSITION | MF_GRAYED);
	return S_OK;
}
