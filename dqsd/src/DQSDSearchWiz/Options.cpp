// Options.cpp: implementation of the COptions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Options.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COptions::COptions()
	: m_bEditResults( TRUE )
	, m_strEditor( _T("notepad.exe") )
	, m_bWarnNotActive( TRUE )
	, m_bIncludeComments( TRUE )
{
	Load();
}

COptions::~COptions()
{

}

void COptions::Load()
{
	CRegKey rk;
	BOOL bEditResult = TRUE;
	if ( ERROR_SUCCESS != rk.Open( HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings") ) )
		return;

	DWORD dwValue = 0;
	if ( ERROR_SUCCESS == rk.QueryValue( dwValue, _T("EditResult") ) )
		m_bEditResults = dwValue;

	TCHAR szValue[ MAX_PATH + 1 ];
	DWORD dwSize = LENGTHOF( szValue );
	if ( ERROR_SUCCESS == rk.QueryValue( szValue, _T("ResultEditor"), &dwSize ) )
		m_strEditor = string( szValue );

	dwValue = 0;
	if ( ERROR_SUCCESS == rk.QueryValue( dwValue, _T("WarnNotActive") ) )
		m_bWarnNotActive = dwValue;

	dwValue = 0;
	if ( ERROR_SUCCESS == rk.QueryValue( dwValue, _T("IncludeComments") ) )
		m_bIncludeComments = dwValue;

}

void COptions::Save()
{
	CRegKey rk;
	BOOL bEditResult = TRUE;
	if ( ERROR_SUCCESS != rk.Open( HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings") ) )
		return;

	rk.SetValue( m_bEditResults, _T("EditResult") );
	rk.SetValue( m_strEditor.c_str(), _T("ResultEditor") );
	rk.SetValue( m_bIncludeComments, _T("IncludeComments") );
	rk.SetValue( m_bWarnNotActive, _T("WarnNotActive") );
}
