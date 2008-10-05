// Options.h: interface for the COptions class.
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONS_H__C61B51EE_2914_446B_AF73_EA7F41FD447D__INCLUDED_)
#define AFX_OPTIONS_H__C61B51EE_2914_446B_AF73_EA7F41FD447D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COptions  
{
public:
	COptions();

public:
	void Load();
	void Save();

	bool EditResults() { return m_bEditResults; }
	void EditResults( bool bEditResults ) { m_bEditResults = bEditResults; }
	
	string Editor() { return m_strEditor; }
	void Editor( const string& rstrEditor ) { m_strEditor = rstrEditor; }

	bool WarnNotActive() { return m_bWarnNotActive; }
	void WarnNotActive( bool bWarnNotActive ) { m_bWarnNotActive = bWarnNotActive; }

	bool IncludeComments() { return m_bIncludeComments; }
	void IncludeComments( bool bIncludeComments ) { m_bIncludeComments = bIncludeComments; }

	string FormCSS() { return m_strFormCSS; }
	void FormCSS( const string& rstrFormCSS ) { m_strFormCSS = rstrFormCSS; }

private:
	bool m_bEditResults;
	string m_strEditor;
	bool m_bWarnNotActive;
	bool m_bIncludeComments;
	string m_strFormCSS;
};

#endif // !defined(AFX_OPTIONS_H__C61B51EE_2914_446B_AF73_EA7F41FD447D__INCLUDED_)
