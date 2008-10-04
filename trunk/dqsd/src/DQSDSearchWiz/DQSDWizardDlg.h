// DQSDWizardDlg.h : Declaration of the CDQSDWizardDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#ifndef __DQSDWIZARDDLG_H_
#define __DQSDWIZARDDLG_H_

#include "SearchNameEdit.h"
#include "Options.h"
#include "resource.h"       // main symbols
#include "DialogToolTipCtrl.h"
#include "Form.h"

/////////////////////////////////////////////////////////////////////////////
// CDQSDWizardDlg
class CDQSDWizardDlg : 
	public CDialogImpl<CDQSDWizardDlg>
{
public:
	CDQSDWizardDlg()
		: m_nTallDialogHeight( 0 )
	{
	}

	enum { IDD = IDD_DQSDWIZARDDLG };

BEGIN_MSG_MAP(CDQSDWizardDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(WM_HELP, OnHelp)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_FormList2, LVN_ITEMCHANGED, OnFormListItemChanged)
    NOTIFY_HANDLER(IDC_FormList2, LVN_DELETEITEM, OnFormListItemDeleted)
	COMMAND_HANDLER(IDC_About, BN_CLICKED, OnClickedAbout)
	COMMAND_HANDLER(IDC_Options, BN_CLICKED, OnClickedOptions)
	COMMAND_HANDLER(IDC_ShowHideHTML, BN_CLICKED, OnClickedShowHideHTML)
	COMMAND_HANDLER(IDC_Switches, EN_CHANGE, OnChangeSwitches)
	COMMAND_HANDLER(IDC_ShowTips, BN_CLICKED, OnClickedShowtips)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnHelp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LPHELPINFO lphi = (LPHELPINFO)lParam;

		return 0;
	}
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
	LRESULT OnFormListItemChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
    LRESULT OnFormListItemDeleted(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
	LRESULT OnClickedAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedShowHideHTML(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnChangeSwitches(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedShowtips(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		m_tip.Activate( ( CWindow( GetDlgItem( IDC_ShowTips ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED ) );
		return 0;
	}

public:
	CComPtr<IHTMLDocument2> m_spActiveFrameDoc;
	CComPtr<IHTMLDocument2> m_spMainDoc;
	CComPtr<IWebBrowser2> m_spWB2;

private:
	string m_strInstallDir;
	string m_strBaseURL;
	string m_strVersion;
	string m_strCopyright;
	CSearchNameEdit m_editSearchName;
	COptions m_options;
	_bstr_t m_bstrUnselectedStyle;
	map< int, CComBSTR > m_mapUnselectedStyle;
	CComPtr< IHTMLStyle > m_spSelectedStyle;
	int m_nTallDialogHeight;
	CDialogToolTipCtrl m_tip;

private:
	string GetForms(const string& rstrSearchName, string& rstrFormScript);

    string CreateFormName(int formIndex, const string& searchName);
    IHTMLElement* GetLParamHTMLElement(CWindow &formListControl, int index);
    auto_ptr<Form> BuildForm(IHTMLElement* pElement);

	string GetSwitches( vector< string >& rvecSwitches );
	string GetExamples( vector< string >& rvecSwitches );
	string EscapeXML( string& xml );
	void SaveFields();
	void RestoreFields();
	string GetScriptFieldName( const string& rstrFieldName );
	void ShrinkDialog();
	void GrowDialog();
	string ParseCategory( LPCTSTR sz );
};

#endif //__DQSDWIZARDDLG_H_
