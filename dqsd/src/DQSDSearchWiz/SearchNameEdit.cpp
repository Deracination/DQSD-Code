// SearchNameEdit.cpp: implementation of the CSearchNameEdit class.
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SearchNameEdit.h"

LRESULT CSearchNameEdit::OnChar( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( _tcschr( _T(";!@#%^&*()=+ \t~`/\\,<>:'\"[]{}|"), wParam ) )
		return 0;

	return DefWindowProc();
}
