// AboutDlg.h : Declaration of the CAboutDlg

#ifndef __ABOUTDLG_H_
#define __ABOUTDLG_H_

#include "resource.h"       // main symbols
#include "modulver.h"
#include <atlhost.h>

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

		TCHAR szModule[ MAX_PATH + 1 ];
		if ( GetModuleFileName( _Module.GetResourceInstance(), szModule, LENGTHOF(szModule) ) )
		{
			CModuleVersion moduleVersion;
			if ( moduleVersion.GetFileVersionInfo( szModule ) )
			{
				CWindow( GetDlgItem( IDC_Version ) ).SetWindowText( ( _T("Version ") + moduleVersion.GetValue( _T("ProductVersion") ) + _T(" ") + moduleVersion.GetValue( _T("SpecialBuild") ) ).c_str() );
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
