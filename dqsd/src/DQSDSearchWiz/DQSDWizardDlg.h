// DQSDWizardDlg.h : Declaration of the CDQSDWizardDlg

#ifndef __DQSDWIZARDDLG_H_
#define __DQSDWIZARDDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

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
	}

	enum { IDD = IDD_DQSDWIZARDDLG };

BEGIN_MSG_MAP(CDQSDWizardDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

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

public:
	CComPtr<IHTMLDocument2> m_spDoc2;

};

#endif //__DQSDWIZARDDLG_H_
