// SearchNameEdit.cpp: implementation of the CSearchNameEdit class.
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
