// SearchWizard.h : Declaration of the CSearchWizard

#ifndef __SEARCHWIZARD_H_
#define __SEARCHWIZARD_H_

#include "resource.h"       // main symbols
#include "DQSDSearchWiz.h"

/////////////////////////////////////////////////////////////////////////////
// CSearchWizard
class ATL_NO_VTABLE CSearchWizard : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSearchWizard, &CLSID_SearchWizard>,
	public IObjectWithSiteImpl<CSearchWizard>,
	public IOleCommandTarget
{
public:
	CSearchWizard()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SEARCHWIZARD)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CSearchWizard)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IOleCommandTarget)
END_COM_MAP()

public:
	STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText);
	STDMETHOD(Exec)(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG* pvaOut);
};

#endif //__SEARCHWIZARD_H_
