// SearchWizard.cpp : Implementation of CSearchWizard
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#include "stdafx.h"
#include "SearchWizard.h"
#include "DQSDWizardDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSearchWizard

#include <initguid.h>
#include <ShlGuid.h>
#include "Options.h"

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

	HWND hwndBrowser = NULL;
	HR(spWB2->get_HWND(reinterpret_cast<long *>(&hwndBrowser)));


	// If there are frames, then get the document for the active frame
	
	long cForms = 0;
	long cFrames = 0;

	CComPtr<IHTMLElementCollection> spFormCollection;

	BOOL bCreateEmpty = FALSE;

	// If there are multiple documents, then get the active one

	CComPtr<IHTMLDocument2> spActiveFrameDoc = GetActiveFrame( spDoc2 );

	CComPtr< IUnknown > spActiveFrameDocUnk = spActiveFrameDoc;
	CComPtr< IUnknown > spMainDocUnk = spDoc2;
	if ( spMainDocUnk.IsEqualObject( spActiveFrameDocUnk ) )
	{
		// Are there any forms in the currently active document?

		HR(spDoc2->get_forms(&spFormCollection));
		HR(spFormCollection->get_length( &cForms ));

		CComQIPtr<IHTMLFramesCollection2> spFrames;
		HR(spDoc2->get_frames( &spFrames ));
		long cFrames = 0;
		HR(spFrames->get_length( &cFrames ));

		if ( cForms == 0 && cFrames > 0 )
		{
			if ( IDNO == ::MessageBox( hwndBrowser, 
									   _T("There are multiple frames in the current document, none of which are active.  "
										  "Would you like to continue and create an empty search template?  "
										  "If no, click in a frame or field to activate the frame, then try again."), 
									   _T("DQSD Search Wizard"), MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 ) )
			{
				return S_OK;
			}
			else
			{
				bCreateEmpty = TRUE;
			}
		}
	}

	// Get the forms

	spFormCollection = NULL;
	HR(spActiveFrameDoc->get_forms(&spFormCollection));

	cForms = 0;
	HR(spFormCollection->get_length( &cForms ));


	COptions options;

	if ( !bCreateEmpty )
	{
		if ( 0 == cForms )
		{
			string strMsg = string( _T("Sorry, no FORMs were found in the current ") ) + 
									  ( ( cFrames > 0 ) ? _T("frame") : _T("document") ) +
							string( _T(".  Would you like to continue and create an empty search template?") );
			if ( IDNO == ::MessageBox( hwndBrowser, strMsg.c_str(),
									   _T("DQSD Search Wizard"), MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 ) )
			{
				return S_OK;
			}
		}
		else if ( options.WarnNotActive() )
		{
			// Give the user a chance to select a FORM field
			CComPtr< IHTMLElement > spActiveElement;
			spActiveFrameDoc->get_activeElement( &spActiveElement );
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
	}
	
	CDQSDWizardDlg dlg;
	dlg.m_spDoc2 = spActiveFrameDoc;
	dlg.m_spMainDoc = spDoc2;
	dlg.DoModal( hwndBrowser, cForms );

	return S_OK;
}

// This is a recursive method that keeps drilling down through frames until
// the active frame is found.
CComPtr<IHTMLDocument2> CSearchWizard::GetActiveFrame( CComPtr<IHTMLDocument2>& spDoc )
{
	try
	{
		CComPtr< IHTMLElement > spActiveElement;
		if ( SUCCEEDED( spDoc->get_activeElement( &spActiveElement ) ) && spActiveElement )
		{
			CComPtr< IUnknown > spFrameUnk;
			CComQIPtr< IHTMLFrameElement > spFrameElement( spActiveElement );
			CComQIPtr< IHTMLIFrameElement > spIFrameElement( spActiveElement );
			
			if ( spFrameElement )
				spFrameUnk = spFrameElement;
			else
				spFrameUnk = spIFrameElement;

			if ( spFrameUnk )
			{
				CComQIPtr< IWebBrowser2 > spWebBrowser( spFrameUnk );
				if ( spWebBrowser )
				{
					CComPtr< IDispatch > spDisp;
					if ( SUCCEEDED( spWebBrowser->get_Document( &spDisp ) ) && spDisp )
					{
						CComPtr< IHTMLDocument2 > spFrame;
						if ( SUCCEEDED( spDisp.QueryInterface( &spFrame ) ) && spFrame )
						{
							return GetActiveFrame( spFrame );
						}
					}
				}
			}
		}
	}
	catch ( ... )
	{
		MessageBox( ::GetForegroundWindow(), _T("An enternal error was encountered:  CSearchWizard::GetActiveFrame"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return spDoc;
}