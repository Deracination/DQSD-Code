// SearchNameEdit.h: interface for the CSearchNameEdit class.
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHNAMEEDIT_H__46BB21E8_4BA4_4713_9DAD_09F6A09C168D__INCLUDED_)
#define AFX_SEARCHNAMEEDIT_H__46BB21E8_4BA4_4713_9DAD_09F6A09C168D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSearchNameEdit : public CWindowImpl<CSearchNameEdit>
{
public:
	BEGIN_MSG_MAP(CSearchNameEdit)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
	END_MSG_MAP()

	CSearchNameEdit() {}
	virtual ~CSearchNameEdit() {}

	LRESULT OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif // !defined(AFX_SEARCHNAMEEDIT_H__46BB21E8_4BA4_4713_9DAD_09F6A09C168D__INCLUDED_)
