// AboutDlg.h : Declaration of the CAboutDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#ifndef __ABOUTDLG_H_
#define __ABOUTDLG_H_

#include "resource.h"       // main symbols
#include "modulver.h"

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg
class CAboutDlg : 
	public CDialogImpl<CAboutDlg>
{
public:
	CAboutDlg();
	~CAboutDlg();

	enum { IDD = IDD_ABOUTDLG };

BEGIN_MSG_MAP(CAboutDlg)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

private:
	CHyperLink m_hlAboutURL;
	CHyperLink m_hlAboutMailTo;
};

#endif //__ABOUTDLG_H_
