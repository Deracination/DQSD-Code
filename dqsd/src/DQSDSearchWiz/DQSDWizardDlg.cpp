// DQSDWizardDlg.cpp : Implementation of CDQSDWizardDlg

#include "stdafx.h"
#include "DQSDWizardDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDQSDWizardDlg

LRESULT CDQSDWizardDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	CenterWindow( GetActiveWindow() );

	// Get the base URL

	CComBSTR bstrURL;
	HR( m_spDoc2->get_URL( &bstrURL ) );

	URL_COMPONENTS uc;
	memset( &uc, 0, sizeof uc );
	uc.dwStructSize = sizeof(uc);
	uc.dwSchemeLength = uc.dwHostNameLength = uc.dwUrlPathLength = 1;
	TCHAR szBaseURL[ MAX_PATH ];
	TCHAR szScheme[ 128 ];
	if ( InternetCrackUrl( W2T( bstrURL ), 0, 0, &uc ) )
	{
		DWORD dwLength = uc.dwSchemeLength + 3 + uc.dwHostNameLength + 1;
		_tcsncpy( szBaseURL, uc.lpszScheme, dwLength );
		szBaseURL[ dwLength ] = _T('\0');
		
		memset( szScheme, 0, sizeof szScheme );
		_tcsncpy( szScheme, uc.lpszScheme, uc.dwSchemeLength + 3 );

		CWindow ctl = GetDlgItem( IDC_Link );
		ctl.SetWindowText( szBaseURL );
	}
	szBaseURL[ _tcslen( szBaseURL ) - 1 ] = _T('\0');

	// Set up the list for the FORM elements

	CWindow ctlFormList2 = GetDlgItem( IDC_FormList2 );
	ctlFormList2.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 
							  0, 
							  LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP );
	LVCOLUMN lvc;
	memset( &lvc, 0, sizeof lvc );
	lvc.mask = LVCF_TEXT|LVCF_ORDER|LVCF_WIDTH;

	lvc.pszText = _T("");
	lvc.cx = 25;
	lvc.iOrder = 0;
	ctlFormList2.SendMessage( LVM_INSERTCOLUMN, 0, (LPARAM)&lvc );
	
	lvc.pszText = _T("action");
	lvc.cx = 250;
	lvc.iOrder = 1;
	ctlFormList2.SendMessage( LVM_INSERTCOLUMN, 1, (LPARAM)&lvc );


	// Get the forms

	CComPtr<IHTMLElementCollection> spFormCollection;
	HR(m_spDoc2->get_forms(&spFormCollection));

	long cForms = 0;
	HR(spFormCollection->get_length( &cForms ));

	for ( int iForm = 0; iForm < cForms; iForm++ )
	{
		CComPtr< IDispatch > spFormDisp;
		spFormCollection->item( CComVariant( iForm ), CComVariant( 0 ), &spFormDisp );

		string strName = _T("");
		string strAction = _T("");

		CComQIPtr< IHTMLElement > spForm( spFormDisp );
		if ( spForm )
		{
			_variant_t varAttributeValue;
			if ( SUCCEEDED( spForm->getAttribute( _bstr_t(_T("action")), 0, &varAttributeValue ) ) )
			{
				if ( VT_BSTR == varAttributeValue.vt && varAttributeValue.bstrVal != NULL )
				{
					if ( !_tcsnicmp( _T("http"), W2CT(varAttributeValue.bstrVal), 4 ) )
					{
						strAction = W2A( varAttributeValue.bstrVal );
					}
					else
					{
						strAction = string( szBaseURL ) + ( *varAttributeValue.bstrVal != L'/' ? _T("/") : _T("") ) + W2A( varAttributeValue.bstrVal );
					}
				}
			}

			LVITEM lvi;
			memset( &lvi, 0, sizeof lvi );
			lvi.mask = LVIF_TEXT;
			lvi.iItem = 256;
			lvi.pszText = const_cast<LPTSTR>(strName.c_str());
			int iPos = ctlFormList2.SendMessage( LVM_INSERTITEM, 0, (LPARAM)&lvi );

			lvi.pszText = const_cast<LPTSTR>(strAction.c_str());
			lvi.mask = LVIF_TEXT;
			lvi.iItem = iPos;
			lvi.iSubItem = 1;
			lvi.pszText = const_cast<LPTSTR>(strAction.c_str());
			ctlFormList2.SendMessage( LVM_SETITEMTEXT, iPos, (LPARAM)&lvi );

			CComQIPtr< IHTMLFormElement > spFormElement( spFormDisp );
			long cFormElements = 0;
			HR( spFormElement->get_length( &cFormElements ) );
		
		}

	}

	return 1;  // Let the system set the focus
}
