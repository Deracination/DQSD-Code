// AboutDlg.h : Declaration of the CAboutDlg

#ifndef __ABOUTDLG_H_
#define __ABOUTDLG_H_

#include "resource.h"       // main symbols
#include "modulver.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
class CAboutDlg : 
	public CAxDialogImpl<CAboutDlg>
{
public:
	CAboutDlg()
	{
	}

	~CAboutDlg()
	{
	}

	enum { IDD = IDD_ABOUTDLG };

BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //__ABOUTDLG_H_
