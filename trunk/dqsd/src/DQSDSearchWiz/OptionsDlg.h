// OptionsDlg.h : Declaration of the COptionsDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#ifndef __OPTIONSDLG_H_
#define __OPTIONSDLG_H_

#include "resource.h"       // main symbols
#include "options.h"

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg
class COptionsDlg : 
	public CAxDialogImpl<COptionsDlg>
{
public:
	COptionsDlg()
	{
		m_options.Load();
	}

	~COptionsDlg()
	{
	}

	enum { IDD = IDD_OPTIONSDLG };

BEGIN_MSG_MAP(COptionsDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_HANDLER(IDC_EditXML, BN_CLICKED, OnClickedEditXML)
	COMMAND_HANDLER(IDC_Browse, BN_CLICKED, OnClickedBrowse)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow( GetActiveWindow() );

		CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_SETCHECK, m_options.EditResults() ? BST_CHECKED : BST_UNCHECKED );
		CWindow( GetDlgItem( IDC_Editor ) ).SetWindowText( m_options.Editor().c_str() );
		CWindow( GetDlgItem( IDC_CSSbackgroundColor ) ).SetWindowText( m_options.FormBackgroundColor().c_str() );
		CWindow( GetDlgItem( IDC_CSSborder ) ).SetWindowText( m_options.FormBorder().c_str() );

		OnClickedEditXML(0, 0, 0, bHandled );

		CWindow( GetDlgItem( IDC_WarnNotActive ) ).SendMessage( BM_SETCHECK, m_options.WarnNotActive() ? BST_CHECKED : BST_UNCHECKED );
		CWindow( GetDlgItem( IDC_IncludeComments ) ).SendMessage( BM_SETCHECK, m_options.IncludeComments() ? BST_CHECKED : BST_UNCHECKED );

		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		USES_CONVERSION;

		m_options.EditResults( CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		
		BSTR bstrValue = NULL;
		CWindow( GetDlgItem( IDC_Editor ) ).GetWindowText( &bstrValue );
		m_options.Editor( W2T( bstrValue ) );
		::SysFreeString( bstrValue );

		CWindow( GetDlgItem( IDC_CSSbackgroundColor ) ).GetWindowText( &bstrValue );
		m_options.FormBackgroundColor( W2T( bstrValue ) );
		::SysFreeString( bstrValue );
		
		CWindow( GetDlgItem( IDC_CSSborder ) ).GetWindowText( &bstrValue );
		m_options.FormBorder( W2T( bstrValue ) );
		::SysFreeString( bstrValue );

		m_options.IncludeComments( CWindow( GetDlgItem( IDC_IncludeComments ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		m_options.WarnNotActive( CWindow( GetDlgItem( IDC_WarnNotActive ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		m_options.Save();

		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClickedEditXML(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		CWindow( GetDlgItem( IDC_Editor ) ).EnableWindow( CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		CWindow( GetDlgItem( IDC_Browse ) ).EnableWindow( CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		CWindow( GetDlgItem( IDC_EditLabel ) ).EnableWindow( CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED );
		return 0;
	}

	LRESULT OnClickedBrowse(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	COptions m_options;
};

#endif //__OPTIONSDLG_H_
