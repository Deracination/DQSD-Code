// OptionsDlg.cpp : Implementation of COptionsDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#include "stdafx.h"
#include "OptionsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg

LRESULT COptionsDlg::OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof ofn );
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("Executable Files (*.exe)\0*.exe\0\0");
	TCHAR szFile[ MAX_PATH + 1 ];
	memset( szFile, 0, sizeof szFile );
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = LENGTHOF( szFile );
	ofn.lpstrTitle = _T("Open Search Editor Application");
	ofn.Flags = OFN_FILEMUSTEXIST;
	if ( GetOpenFileName( &ofn ) )
	{
		CWindow( GetDlgItem( IDC_Editor ) ).SetWindowText( ofn.lpstrFile );
	}

	return 0;
}
