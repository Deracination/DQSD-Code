// DialogToolTipCtrl.h: interface for the CDialogToolTipCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DialogToolTIPCTRL_H__BD9E58D7_B31B_463E_BF99_314ED69C9F1B__INCLUDED_)
#define AFX_DialogToolTIPCTRL_H__BD9E58D7_B31B_463E_BF99_314ED69C9F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogToolTipCtrl : public CToolTipCtrl  
{
public:
	CDialogToolTipCtrl();
	virtual ~CDialogToolTipCtrl();

	HWND Create( HWND hWndParent, _U_RECT rect = NULL, LPCTSTR szWindowName = NULL, DWORD dwStyle = 0, DWORD dwExStyle = 0, _U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL );
	BOOL AddTool( int nCtrlID, LPTSTR pszTip );

private:
	HWND m_hWndParent;
};

#endif // !defined(AFX_DialogToolTIPCTRL_H__BD9E58D7_B31B_463E_BF99_314ED69C9F1B__INCLUDED_)
