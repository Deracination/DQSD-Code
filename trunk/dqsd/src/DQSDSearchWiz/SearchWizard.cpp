// SearchWizard.cpp : Implementation of CSearchWizard
#include "stdafx.h"
#include "DQSDSearchWiz.h"
#include "SearchWizard.h"
#include "DQSDWizardDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSearchWizard

#include <initguid.h>
#include <ShlGuid.h>

STDMETHODIMP CSearchWizard::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD prgCmds[], OLECMDTEXT* pCmdText)
{
	if (cCmds == 0) return E_INVALIDARG;
	if (prgCmds == 0) return E_POINTER;

	if (!::InlineIsEqualGUID(*pguidCmdGroup, CLSID_ToolbarExtButtons)) return OLECMDERR_E_UNKNOWNGROUP;

	prgCmds[0].cmdf = OLECMDF_ENABLED;
	return S_OK;
}

STDMETHODIMP CSearchWizard::Exec(const GUID*, DWORD nCmdID, DWORD, VARIANTARG*, VARIANTARG*)
{
	if (m_spUnkSite == 0) return S_OK;

	CComPtr<IServiceProvider> spSP;
	HR(m_spUnkSite->QueryInterface(&spSP));

	CComPtr<IWebBrowser2> spWB2;
	HR(spSP->QueryService(IID_IWebBrowserApp, &spWB2));


	CComPtr<IDispatch> spDispDoc;
	HR(spWB2->get_Document(&spDispDoc));


	CComPtr<IHTMLDocument2> spDoc2;
	HR(spDispDoc->QueryInterface(&spDoc2));

	// Get the forms

	CComPtr<IHTMLElementCollection> spFormCollection;
	HR(spDoc2->get_forms(&spFormCollection));

	long cForms = 0;
	HR(spFormCollection->get_length( &cForms ));


	HWND hwndBrowser = NULL;
	HR(spWB2->get_HWND(reinterpret_cast<long *>(&hwndBrowser)));
	
	if ( 0 == cForms )
	{
		if ( IDNO == ::MessageBox( hwndBrowser, 
								   _T("Sorry, no FORMs were found in the current document.  "
									  "Would you like to continue and create an empty search template?"), 
								   _T("DQSD Search Wizard"), MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 ) )
		{
			return S_OK;
		}
	}
	else
	{
		// Give the user a chance to select a FORM field
		CComPtr< IHTMLElement > spActiveElement;
		spDoc2->get_activeElement( &spActiveElement );
		CComPtr< IHTMLInputElement > spActiveInputElement;
		if ( !spActiveElement || FAILED( spActiveElement.QueryInterface( &spActiveInputElement ) ) )
		{
			if ( IDNO == ::MessageBox( hwndBrowser, 
							   _T("No fields have been selected.  Selecting or clicking in the field which contains the search "
								  "string will add some helpful information to the search and make "
								  "it more complete.\r\n\r\n"
								  "Would you like to continue anyway?"), 
							   _T("DQSD Search Wizard"), MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 ) )
			{
				return S_OK;
			}
		}
	}
	
	CDQSDWizardDlg dlg;
	dlg.m_spDoc2 = spDoc2;
	dlg.DoModal( hwndBrowser, cForms );

	return S_OK;
}
