// SearchNameEdit.cpp: implementation of the CSearchNameEdit class.
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SearchNameEdit.h"

LRESULT CSearchNameEdit::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	try
	{
		if ( _tcschr( _T(";!@#%^&*()=+ \t~`/\\,<>:'\"[]{}|"), wParam ) )
			return 0;
	}
	catch ( ... )
	{
		MessageBox( _T("An enternal error was encountered:  CSearchNameEdit::OnChar"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return DefWindowProc();
}
