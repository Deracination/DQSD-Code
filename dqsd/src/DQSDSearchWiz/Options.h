// Options.h: interface for the COptions class.
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
	virtual ~COptions();

public:
	void Load();
	void Save();

	BOOL EditResults() { return m_bEditResults; }
	void EditResults( BOOL bEditResults ) { m_bEditResults = bEditResults; }
	
	string Editor() { return m_strEditor; }
	void Editor( const string& rstrEditor ) { m_strEditor = rstrEditor; }

	BOOL WarnNotActive() { return m_bWarnNotActive; }
	void WarnNotActive( BOOL bWarnNotActive ) { m_bWarnNotActive = bWarnNotActive; }

	BOOL IncludeComments() { return m_bIncludeComments; }
	void IncludeComments( BOOL bIncludeComments ) { m_bIncludeComments = bIncludeComments; }

private:
	BOOL m_bEditResults;
	string m_strEditor;
	BOOL m_bWarnNotActive;
	BOOL m_bIncludeComments;
};

#endif // !defined(AFX_OPTIONS_H__C61B51EE_2914_446B_AF73_EA7F41FD447D__INCLUDED_)