// DQSDWizardDlg.h : Declaration of the CDQSDWizardDlg

#ifndef __DQSDWIZARDDLG_H_
#define __DQSDWIZARDDLG_H_

#include "SearchNameEdit.h"

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDQSDWizardDlg
class CDQSDWizardDlg : 
	public CAxDialogImpl<CDQSDWizardDlg>
{
public:
	CDQSDWizardDlg()
	{
	}

	~CDQSDWizardDlg()
	{
		vector< CComPtr< IHTMLElement >* >::iterator it = m_vecFormHTMLs.begin();
		for ( ; it != m_vecFormHTMLs.end(); it++ )
			delete *it;
	}

	enum { IDD = IDD_DQSDWIZARDDLG };

BEGIN_MSG_MAP(CDQSDWizardDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	NOTIFY_HANDLER(IDC_FormList2, LVN_ITEMCHANGED, OnFormListItemChanged)
	COMMAND_HANDLER(IDC_About, BN_CLICKED, OnClickedAbout)
	COMMAND_HANDLER(IDC_Options, BN_CLICKED, OnClickedOptions)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnFormListItemChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled);
	LRESULT OnClickedAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnClickedOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

public:
	CComPtr<IHTMLDocument2> m_spDoc2;

private:
	vector< CComPtr< IHTMLElement >* > m_vecFormHTMLs;
	string m_strInstallDir;
	string m_strBaseURL;
	string m_strVersion;

private:
	string GetAbsoluteActionPath( _variant_t& varAction );
	string GetForms( string& rstrSearchName, string& rstrFormScript );
	string GetSwitches();
	string EscapeXML( string& xml );
	void SaveFields();
	void RestoreFields();

	CSearchNameEdit m_editSearchName;
};

#endif //__DQSDWIZARDDLG_H_
