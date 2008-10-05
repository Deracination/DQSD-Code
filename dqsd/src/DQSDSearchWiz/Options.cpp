// Options.cpp: implementation of the COptions class.
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Options.h"

#include "Registry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions()
	: m_bEditResults( true )
	, m_strEditor( _T("notepad.exe") )
	, m_bWarnNotActive( true )
	, m_bIncludeComments( true )
	, m_strFormCSS( _T("background: yellow; border: 1px dotted red") )
{
	Load();
}

void COptions::Load()
{
	try
	{
		CRegKey rk;
		if (ERROR_SUCCESS != rk.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings"), KEY_READ))
			return;

		DWORD dwValue = 0;
		QueryRegValue(rk, _T("EditResult"), m_bEditResults);
        QueryRegValue(rk, _T("ResultEditor"), m_strEditor);
        QueryRegValue(rk, _T("WarnNotActive"), m_bWarnNotActive);
        QueryRegValue(rk, _T("IncludeComments"), m_bIncludeComments);
        QueryRegValue(rk, _T("FormCSS"), m_strFormCSS);
	}
	catch ( ... )
	{
		MessageBox( ::GetForegroundWindow(), _T("An internal error was encountered:  COptions::Load"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}
}

void COptions::Save()
{
	try
	{
		CRegKey rk;
		if (ERROR_SUCCESS != rk.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings"), KEY_WRITE))
			return;

		SetRegValue(rk, _T("EditResult"), m_bEditResults);
		SetRegValue(rk, _T("ResultEditor"), m_strEditor);
		SetRegValue(rk, _T("IncludeComments"), m_bIncludeComments);
		SetRegValue(rk, _T("WarnNotActive"), m_bWarnNotActive);
		SetRegValue(rk, _T("FormCSS"), m_strFormCSS);
	}
	catch ( ... )
	{
		MessageBox( ::GetForegroundWindow(), _T("An internal error was encountered:  COptions::Save"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}
}
