// OptionsDlg.h : Declaration of the COptionsDlg

#ifndef __OPTIONSDLG_H_
#define __OPTIONSDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// COptionsDlg
class COptionsDlg : 
	public CAxDialogImpl<COptionsDlg>
{
public:
	COptionsDlg()
		: m_bEditResult( TRUE )
		, m_strEditor( _T("notepad.exe") )
	{
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
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		CenterWindow( GetActiveWindow() );
		
		CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_SETCHECK, m_bEditResult ? BST_CHECKED : BST_UNCHECKED );

		CWindow( GetDlgItem( IDC_Editor ) ).SetWindowText( m_strEditor.c_str() );

		OnClickedEditXML(0, 0, 0, bHandled );

		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		USES_CONVERSION;

		m_bEditResult = CWindow( GetDlgItem( IDC_EditXML ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED ? TRUE : FALSE;

		BSTR bstrEditor = NULL;
		CWindow( GetDlgItem( IDC_Editor ) ).GetWindowText( &bstrEditor );
		m_strEditor = W2T( bstrEditor );
		::SysFreeString( bstrEditor );
		
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
		return 0;
	}

public:
	BOOL m_bEditResult;
	string m_strEditor;
};

#endif //__OPTIONSDLG_H_
