// DialogToolTipCtrl.cpp: implementation of the CDialogToolTipCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DialogToolTipCtrl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDialogToolTipCtrl::CDialogToolTipCtrl()
	: m_hWndParent( NULL )
{
}

CDialogToolTipCtrl::~CDialogToolTipCtrl()
{
}

HWND CDialogToolTipCtrl::Create(HWND hWndParent, _U_RECT rect, LPCTSTR szWindowName, DWORD dwStyle, DWORD dwExStyle, _U_MENUorID MenuOrID, LPVOID lpCreateParam)
{
	m_hWndParent = hWndParent;
	HWND hwnd = CToolTipCtrl::Create( hWndParent, rect, szWindowName, dwStyle, dwExStyle, MenuOrID, lpCreateParam );

	// ??? Should these be options?  Probably.
	SetMaxTipWidth( 300 );
	SetDelayTime( TTDT_INITIAL, 0 );
	SetDelayTime( TTDT_AUTOPOP, 30 * 1000 );

	return hwnd;
}

BOOL CDialogToolTipCtrl::AddTool( int nCtrlID, LPTSTR pszTip )
{
	ATLASSERT( ::IsWindow( m_hWnd ) ); // Need to call Create first

	HWND hwndCtrl = ::GetDlgItem( m_hWndParent, nCtrlID );
	ATLASSERT( hwndCtrl );

	CWindow( hwndCtrl ).ModifyStyleEx( WS_EX_NOPARENTNOTIFY, 0 ); // This is off by default for dialog controls

	CToolInfo cti( TTF_IDISHWND| TTF_SUBCLASS, hwndCtrl, (WPARAM)hwndCtrl, 0, pszTip );
	
	return CToolTipCtrl::AddTool( &cti );
}
