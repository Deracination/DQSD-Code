// DQSDWizardDlg.cpp : Implementation of CDQSDWizardDlg

#include "stdafx.h"
#include "DQSDWizardDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CDQSDWizardDlg

LRESULT CDQSDWizardDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

	CenterWindow( GetActiveWindow() ); // ??? should probably use the actual browser window

	// Get DQSD install dir

	CRegKey rk;
	TCHAR szInstallDir[ MAX_PATH + 1 ];
	memset( szInstallDir, 0, sizeof szInstallDir );
	if ( ERROR_SUCCESS == rk.Open( HKEY_CLASSES_ROOT, _T("CLSID\\{226b64e8-dc75-4eea-a6c8-abcb4d1d37ff}") ) )
	{
		DWORD dwLength = LENGTHOF( szInstallDir );
		rk.QueryValue( szInstallDir, _T("InstallDir"), &dwLength );
		szInstallDir[ dwLength ] = _T('\0');
	}
	m_strInstallDir = szInstallDir;

	// Get the base URL and put it in the 'Link' element

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

	// Save the base URL for later use

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

	// Set up the combobox

	LPCTSTR rgszCategories[] = {
		_T("Computers"),
		_T("Entertainment"),
		_T("Finance"),
		_T("Fun"),
		_T("Functions"),
		_T("News"),
		_T("People and Places"),
		_T("Reference"),
		_T("Search the Web"),
		_T("Shipping"),
		_T("Shopping"),
	};

	for ( int i = 0; i < LENGTHOF( rgszCategories ); i++ )
	{
		CWindow ctlCategories = GetDlgItem( IDC_Category );
		ctlCategories.SendMessage( CB_ADDSTRING, 0, reinterpret_cast<LPARAM>( rgszCategories[ i ] ) );
	}


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

			CComPtr< IHTMLElement >* pspForm = new CComPtr< IHTMLElement >( spForm ); // these are deleted in the dialog's dtor
			m_vecFormHTMLs.push_back( pspForm );

			// Add the form information to the list
			LVITEM lvi;
			memset( &lvi, 0, sizeof lvi );

			lvi.mask = LVIF_TEXT|LVIF_PARAM;
			lvi.iItem = 256;
			lvi.pszText = const_cast<LPTSTR>(strName.c_str());
			lvi.lParam = reinterpret_cast<LPARAM>(pspForm);
			int iPos = ctlFormList2.SendMessage( LVM_INSERTITEM, 0, (LPARAM)&lvi );

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iPos;
			lvi.iSubItem = 1;
			lvi.pszText = const_cast<LPTSTR>(strAction.c_str());
			ctlFormList2.SendMessage( LVM_SETITEM, iPos, (LPARAM)&lvi );

//			CComQIPtr< IHTMLFormElement > spFormElement( spFormDisp );
//			long cFormElements = 0;
//			HR( spFormElement->get_length( &cFormElements ) );
		}
	}

	SetOKSensitivity();

	return 1;  // Let the system set the focus
}

LRESULT CDQSDWizardDlg::OnFormListItemChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{
	USES_CONVERSION;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ( pNMListView->uNewState != 3 )
		return 1;

	// Get the current FORM selection and display it's innerHTML
	CWindow ctlFormList = GetDlgItem( IDC_FormList2 );
	const int cSelected = ctlFormList.SendMessage( LVM_GETSELECTEDCOUNT, 0, 0 );
	if ( cSelected > 0 )
	{
		const int cItems = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
		for ( int i = 0; i < cItems; i++ )
		{
			if ( ctlFormList.SendMessage( LVM_GETITEMSTATE, i, LVIS_SELECTED ) )
			{
				LVITEM lvi;
				memset( &lvi, 0, sizeof lvi );
				lvi.mask = LVIF_PARAM;
				lvi.iItem = i;
				ctlFormList.SendMessage( LVM_GETITEM, i, (LPARAM)&lvi );
				CComPtr< IHTMLElement >* pspForm = reinterpret_cast<CComPtr< IHTMLElement >*>(lvi.lParam);

				BSTR bstr = NULL;
				if ( SUCCEEDED( (*pspForm)->get_innerHTML( &bstr ) ) )
				{
					CWindow ctlForm = GetDlgItem( IDC_FormFields );
					ctlForm.SetWindowText( W2T( bstr ) );
					::SysFreeString( bstr );
				}
				return 1;
			}
		}
	}

	return 1;
}

LRESULT CDQSDWizardDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	// Make sure required fields are there

	CWindow ctlSearchTitle = GetDlgItem( IDC_SearchTitle );
	CComBSTR bstrSearchTitle;
	ctlSearchTitle.GetWindowText( &bstrSearchTitle );
	if ( bstrSearchTitle.Length() == 0 )
	{
		MessageBox( _T("Enter a search title"), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
		ctlSearchTitle.SetFocus();
		return 0;
	}

	CWindow ctlSearchName = GetDlgItem( IDC_SearchName );
	CComBSTR bstrSearchName;
	ctlSearchName.GetWindowText( &bstrSearchName );
	if ( bstrSearchName.Length() == 0 )
	{
		MessageBox( _T("Enter a search name"), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
		ctlSearchName.SetFocus();
		return 0;
	}

	CWindow ctlFormList = GetDlgItem( IDC_FormList2 );
	const int cItems = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
	BOOL bChecked = FALSE;
	for ( int i = 0; i < cItems && !bChecked; i++ )
	{
		if ( ListView_GetCheckState( GetDlgItem( IDC_FormList2 ), i ) )
			bChecked = TRUE;
	}
	if ( !bChecked )
	{
		MessageBox( _T("Select one or more of the FORMs"), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
		ctlFormList.SetFocus();
		return 0;
	}

	// Build the search file (??? probably should be done with the XMLDOM, but this is so much easier)
	BSTR bstr = NULL;
	string strSearchFile = _T("");
	string strSearchName = W2T( bstrSearchName.m_str );
	strSearchFile += _T("<search function=\"") + strSearchName + _T("\">");
	strSearchFile += _T("\r\n  <name>") + strSearchName + _T("</name>");

	CWindow( GetDlgItem( IDC_Category ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <category>") + string( W2T( bstr ) ) + _T("</category>");
	::SysFreeString( bstr );
	
	CWindow( GetDlgItem( IDC_Contributor ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <contributor>") + string( W2T( bstr ) ) + _T("</contributor>");
	::SysFreeString( bstr );

	CWindow( GetDlgItem( IDC_Link ) ).GetWindowText( &bstr );
	string strLink = W2T( bstr );
	strSearchFile += _T("\r\n  <link>") + strLink + _T("</link>");
	::SysFreeString( bstr );

	CWindow( GetDlgItem( IDC_Email ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <email>") + string( W2T( bstr ) ) + _T("</email>");
	::SysFreeString( bstr );

	CWindow( GetDlgItem( IDC_Description ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <description>");
	strSearchFile += _T("\r\n  ") + string( W2T( bstr ) );
	strSearchFile += _T("\r\n  </description>");
	::SysFreeString( bstr );

	strSearchFile += _T("\r\n  <script><![CDATA[");
    strSearchFile += _T("\r\n    function ") + strSearchName + _T("(q)");
    strSearchFile += _T("\r\n    {");
    strSearchFile += _T("\r\n      if( nullArgs(\"") + strSearchName + _T("\", q) )");
    strSearchFile += _T("\r\n        return false;");
    strSearchFile += _T("\r\n");
    strSearchFile += _T("\r\n      if( q == \"\" )");
    strSearchFile += _T("\r\n      {");
    strSearchFile += _T("\r\n        openSearchWindow(\"") + strLink + _T("\");");
    strSearchFile += _T("\r\n      }");
    strSearchFile += _T("\r\n      else");
    strSearchFile += _T("\r\n      {");
    strSearchFile += _T("\r\n        submitForm(") + strSearchName + _T("f);");
    strSearchFile += _T("\r\n      }");
    strSearchFile += _T("\r\n    }");
	strSearchFile += _T("\r\n  ]]></script>");

	strSearchFile += _T("\r\n</search>");

	TCHAR szFile[ MAX_PATH + 1 ];
	_tcscpy( szFile, strSearchName.c_str() );
	_tcscat( szFile, _T(".xml") );
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof ofn );
	ofn.lStructSize = sizeof ofn;
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("Search Files (*.xml)\0*.xml\0\0");
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = LENGTHOF( szFile );
	TCHAR szSearchesDir[ MAX_PATH + 1 ];
	_tcscpy( szSearchesDir, m_strInstallDir.c_str() );
	_tcscat( szSearchesDir, _T("\\searches") );
	ofn.lpstrInitialDir = szSearchesDir;
	ofn.lpstrTitle = _T("Search DQSD Search As");
	ofn.Flags = OFN_OVERWRITEPROMPT;
	if ( GetSaveFileName( &ofn ) )
	{
		HANDLE hFile = ::CreateFile(ofn.lpstrFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if ( hFile != INVALID_HANDLE_VALUE )
		{
			DWORD dwBytesWritten = 0;
			::WriteFile( hFile, strSearchFile.c_str(), strSearchFile.length(), &dwBytesWritten, NULL );
			::FlushFileBuffers( hFile );
			::CloseHandle( hFile );
		}

		EndDialog(wID);
	}

	return 0;
}

void CDQSDWizardDlg::SetOKSensitivity()
{
//	BOOL bEnabled = TRUE;
//
//	CWindow ctlSearchTitle = GetDlgItem( IDC_SearchTitle );
//	CComBSTR bstrSearchName;
//	ctlSearchTitle.GetWindowText( &bstrSearchName );
//	if ( bstrSearchName.Length() == 0 )
//		bEnabled = FALSE;
//
//	if ( bEnabled )
//	{
//		CWindow ctlFormList = GetDlgItem( IDC_FormList2 );
//		const int cItems = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
//
//		for ( int i = 0; i < cItems; i++ )
//		{
//			if ( ListView_GetCheckState( GetDlgItem( IDC_FormList2 ), i ) )
//			{
//				;
//			}
//		}
//	}
//
//	CWindow ctlOK = GetDlgItem( IDOK );
//	ctlOK.EnableWindow( bEnabled );
}

LRESULT CDQSDWizardDlg::OnChangeSearchName(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	SetOKSensitivity();
	return 0;
}
