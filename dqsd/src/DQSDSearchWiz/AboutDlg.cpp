// AboutDlg.cpp : Implementation of CAboutDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#include "stdafx.h"
#include "AboutDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
CAboutDlg::CAboutDlg()
{
}

CAboutDlg::~CAboutDlg()
{
}

LRESULT CAboutDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow( GetActiveWindow() );

	// Get file version
	TCHAR szModule[ MAX_PATH + 1 ];
	if ( GetModuleFileName( _Module.GetResourceInstance(), szModule, LENGTHOF(szModule) ) )
	{
		CModuleVersion moduleVersion;
		if ( moduleVersion.GetFileVersionInfo( szModule ) )
		{
			CWindow( GetDlgItem( IDC_Version ) ).SetWindowText( ( _T("Version ") + moduleVersion.GetValue( _T("ProductVersion") ) + _T(" ") + moduleVersion.GetValue( _T("SpecialBuild") ) ).c_str() );
		}
	}

	// Get change log history
	HRSRC hrsc = FindResource( _Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_CHANGELOG), _T("TEXT") );
	if ( hrsc )
	{
		HGLOBAL hdata = LoadResource( _Module.GetResourceInstance(), hrsc );
		if ( hdata )
		{
			const DWORD dwResLen = SizeofResource( _Module.GetResourceInstance(), hrsc );
			LPTSTR pszChangeLog = (LPTSTR)LockResource( hdata );
			if ( pszChangeLog )
			{
				pszChangeLog[ dwResLen * sizeof TCHAR ] = _T('\0');
				CWindow ctlHistory( GetDlgItem( IDC_ChangeHistory ) );
				ctlHistory.SetWindowText( pszChangeLog );
				UnlockResource( hdata );
			}
			FreeResource( hdata );
		}
	}

	// Make the caption a hyperlink to the DQSD web site
	CString csURL;
	csURL.LoadString( IDS_AboutURL );
	m_hlAboutURL.SetHyperLink( csURL );
	m_hlAboutURL.SubclassWindow( GetDlgItem( IDC_Caption ) );

	// mailto: hyperlink
	csURL.LoadString( IDS_MailToGlennCarr );
	m_hlAboutMailTo.SetHyperLink( csURL );
	m_hlAboutMailTo.SubclassWindow( GetDlgItem( IDC_GlennCarr ) );

	return 1;  // Let the system set the focus
}

LRESULT CAboutDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAboutDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	EndDialog(wID);
	return 0;
}
