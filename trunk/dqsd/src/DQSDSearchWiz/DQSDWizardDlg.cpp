// DQSDWizardDlg.cpp : Implementation of CDQSDWizardDlg

#include "stdafx.h"
#include "DQSDWizardDlg.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"

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
	m_strBaseURL = szBaseURL;

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

	// Restore some saved fields

	RestoreFields();

	// Get the forms

	CComPtr<IHTMLElementCollection> spFormCollection;
	HR(m_spDoc2->get_forms(&spFormCollection));

	long cForms = 0;
	HR(spFormCollection->get_length( &cForms ));

	int cDisplayedForms = 0;

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
			if ( FAILED( spForm->getAttribute( _bstr_t(_T("action")), 0, &varAttributeValue ) ) ||
				 !( VT_BSTR == varAttributeValue.vt && varAttributeValue.bstrVal != NULL ) )
			{
				continue; // Ignore if there's no action
			}

			cDisplayedForms += 1;

			strAction = GetAbsoluteActionPath( varAttributeValue );

			CComPtr< IHTMLElement >* pspForm = new CComPtr< IHTMLElement >( spForm ); // these are deleted in the dialog's dtor
			m_vecFormHTMLs.push_back( pspForm );

			// See if any of the fields in the form has focus
/*			
			BOOL bFormHasFocus = FALSE;

			CComQIPtr< IHTMLFormElement > spFormElement( *pspForm );
			long cFormElements = 0;
			if ( SUCCEEDED( spFormElement->get_length( &cFormElements ) ) )
			{
				for ( int iFormElem = 0; iFormElem < cFormElements; iFormElem++ )
				{
					_variant_t varItem( static_cast<long>(iFormElem), VT_I4 );
					CComPtr< IDispatch > spIDisp;
					spFormElement->item( varItem, varItem, &spIDisp );

					CComQIPtr< IHTMLInputElement > spInputElement( spIDisp );
					if ( spInputElement )
					{
						VARIANT_BOOL bFormControlSelected = FALSE;
						if ( SUCCEEDED( spInputElement->get_status( &bFormControlSelected ) ) )
						{
							bFormHasFocus = TRUE;
						}
					}
				}
			}
*/

			// Add the form information to the list
			LVITEM lvi;
			memset( &lvi, 0, sizeof lvi );

			lvi.mask = LVIF_TEXT|LVIF_PARAM;
			lvi.iItem = 256;
			lvi.pszText = const_cast<LPTSTR>(strName.c_str());
			lvi.lParam = reinterpret_cast<LPARAM>(pspForm);
			int iPos = ctlFormList2.SendMessage( LVM_INSERTITEM, 0, (LPARAM)&lvi );

//			ListView_SetCheckState( ctlFormList2.m_hWnd, iPos, bFormHasFocus );

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iPos;
			lvi.iSubItem = 1;
			lvi.pszText = const_cast<LPTSTR>(strAction.c_str());
			ctlFormList2.SendMessage( LVM_SETITEM, iPos, (LPARAM)&lvi );
		}
	}

	if ( cDisplayedForms == 1 )
		ListView_SetCheckState( ctlFormList2.m_hWnd, 0, TRUE );

	return 1;  // Let the system set the focus
}

LRESULT CDQSDWizardDlg::OnFormListItemChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{
	USES_CONVERSION;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if ( pNMListView->uNewState != 3 )
		return 1;

	// Get the current FORM selection and display it's innerHTML
	// just to help the user know which form to use

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

	CWindow ctlSearchName = GetDlgItem( IDC_SearchName );
	CComBSTR bstrSearchName;
	ctlSearchName.GetWindowText( &bstrSearchName );
	if ( bstrSearchName.Length() == 0 )
	{
		MessageBox( _T("Enter a search name"), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
		ctlSearchName.SetFocus();
		return 0;
	}

	CWindow ctlSearchTitle = GetDlgItem( IDC_SearchTitle );
	CComBSTR bstrSearchTitle;
	ctlSearchTitle.GetWindowText( &bstrSearchTitle );
	if ( bstrSearchTitle.Length() == 0 )
	{
		MessageBox( _T("Enter a search title"), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
		ctlSearchTitle.SetFocus();
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

	// Save values for next time

	SaveFields();

	// Build the search file (??? probably should be done with the XMLDOM, but this is so much easier)

	BSTR bstr = NULL;
	string strSearchFile = _T("");
	string strSearchName = W2T( bstrSearchName.m_str );
	strSearchFile += _T("<search function=\"") + strSearchName + _T("\">");

	CWindow( GetDlgItem( IDC_SearchTitle ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <name>") + string( W2T( bstr ) ) + _T("</name>");
	::SysFreeString( bstr );

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
	if ( _tcslen( W2T( bstr ) ) )
		strSearchFile += _T("\r\n  ") + string( W2T( bstr ) );
	strSearchFile += _T("\r\n  </description>");
	::SysFreeString( bstr );

	string strFormScript;
	strSearchFile += GetForms( strSearchName, strFormScript );

	strSearchFile += _T("\r\n  <script><![CDATA[");
    strSearchFile += _T("\r\n    function ") + strSearchName + _T("(q)");
    strSearchFile += _T("\r\n    {");
    strSearchFile += _T("\r\n      if( nullArgs(\"") + strSearchName + _T("\", q) )");
    strSearchFile += _T("\r\n        return false;");
    strSearchFile += _T("\r\n");
    strSearchFile += _T("\r\n      if( q == \"\" )");
    strSearchFile += _T("\r\n      {");
    strSearchFile += _T("\r\n        openSearchWindow(\"") + strLink + _T("\");");
    strSearchFile += _T("\r\n        return true;");
    strSearchFile += _T("\r\n      }");
	
	strSearchFile += GetSwitches();

	strSearchFile += strFormScript;
    strSearchFile += _T("\r\n      submitForm(") + strSearchName + _T("f);");
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

string CDQSDWizardDlg::GetForms( string& rstrSearchName, string& rstrFormScript )
{
	USES_CONVERSION;

	string strFormXML = _T("");

	CWindow ctlFormList = GetDlgItem( IDC_FormList2 );
	int iSelectedForms = 0;
	const int cItems = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
	for ( int iForm = 0; iForm < cItems; iForm++ )
	{
		if ( ListView_GetCheckState( ctlFormList.m_hWnd, iForm ) )
		{
			TCHAR szFormNum[ 32 ];
			_stprintf( szFormNum, (iSelectedForms > 0 ? _T("%d") : _T("")), iSelectedForms + 1 );
			string strFormName = rstrSearchName + _T("f") + string(szFormNum);

			strFormXML += _T("\r\n  <form name=\"") + strFormName + _T("\"");

			LVITEM lvi;
			memset( &lvi, 0, sizeof lvi );
			lvi.mask = LVIF_PARAM;
			lvi.iItem = iForm;
			ctlFormList.SendMessage( LVM_GETITEM, iForm, (LPARAM)&lvi );
			CComPtr< IHTMLElement >* pspForm = reinterpret_cast<CComPtr< IHTMLElement >*>(lvi.lParam);

			_variant_t varAttributeValue;

			// method attribute
			
			string strMethod = _T("");
			if ( SUCCEEDED( (*pspForm)->getAttribute( _bstr_t(_T("method")), 0, &varAttributeValue ) ) )
			{
				strMethod = W2A( varAttributeValue.bstrVal );
			}
			strFormXML += _T("\r\n        method=\"") + strMethod + _T("\"");;

			// action attribute

			string strAction = _T("");
			if ( SUCCEEDED( (*pspForm)->getAttribute( _bstr_t(_T("action")), 0, &varAttributeValue ) ) )
			{
				strAction = GetAbsoluteActionPath( varAttributeValue );
			}
			strFormXML += _T("\r\n        action=\"") + strAction + _T("\"");;


			strFormXML += _T(">");


			// get form fields

			rstrFormScript = _T("");

			CComQIPtr< IHTMLFormElement > spFormElement( *pspForm );
			long cFormElements = 0;
			if ( SUCCEEDED( spFormElement->get_length( &cFormElements ) ) )
			{
				rstrFormScript += _T("\r\n\r\n      // FORM variables for ") + strFormName;
				
				for ( int iFormElem = 0; iFormElem < cFormElements; iFormElem++ )
				{
					_variant_t varItem( static_cast<long>(iFormElem), VT_I4 );
					CComPtr< IDispatch > spIDisp;
					spFormElement->item( varItem, varItem, &spIDisp );

					CComQIPtr< IHTMLElement > spElement( spIDisp );
					if ( !spElement )
						continue;

					BSTR bstrName = NULL;
					spElement->get_tagName( &bstrName );

					// Skip if the element doesn't have a name

					_variant_t varInputName;
					if ( SUCCEEDED( spElement->getAttribute( _bstr_t( _T("name") ), 0, &varInputName ) ) && varInputName.bstrVal )
					{
						strFormXML += _T("\r\n    <input type=\"hidden\" value=\"\"");
						strFormXML += _T(" name=\"") + string( W2T( varInputName.bstrVal ) ) + _T("\"");
						strFormXML += _T("/>");
						rstrFormScript += _T("\r\n      //document.") + strFormName + _T(".") + string( W2T( varInputName.bstrVal ) ) + _T(".value = \"\";");

						if ( !_tcsicmp( W2T( bstrName ), _T("SELECT") ) )
						{
							strFormXML += _T("\r\n      <COMMENT>  The input element above was a SELECT element with the following options...");
							strFormXML += _T("\r\n        <select name=\"" + string( W2T( varInputName.bstrVal ) ) + "\">");
							
							CComQIPtr< IHTMLSelectElement > spSelect( spElement );
							if ( spSelect )
							{
								long cOptions = 0;
								spSelect->get_length( &cOptions );
								for ( int iOption = 0; iOption < cOptions; iOption++ )
								{
									CComPtr< IDispatch > spOptionDisp;
									_variant_t varOption( static_cast<long>(iOption), VT_I4 );
									spSelect->item( varOption, varOption, &spOptionDisp );

									CComQIPtr< IHTMLOptionElement > spOption( spOptionDisp );
									
									BSTR bstrOptionValue = NULL;
									spOption->get_value( &bstrOptionValue );
									
									BSTR bstrOptionText = NULL;
									spOption->get_text( &bstrOptionText );
									
									strFormXML += _T("\r\n          <option");
									if ( bstrOptionValue )
										strFormXML += _T(" value=\"") + string( W2T( bstrOptionValue ) ) + _T("\"");
									strFormXML += _T(">");
									strFormXML += EscapeXML( string( W2T( bstrOptionText ) ) ) + _T("</option>");

									::SysFreeString( bstrOptionValue );
								}
							}

							strFormXML += _T("\r\n        </select>");
							strFormXML += _T("\r\n      </COMMENT>\r\n");
						}
					}

					::SysFreeString( bstrName );
				}
			}

			
			strFormXML += _T("\r\n  </form>");

			++iSelectedForms;
		}
	}

	return strFormXML;
}

string CDQSDWizardDlg::GetAbsoluteActionPath( _variant_t& varAction )
{
	USES_CONVERSION;

	string strAction = _T("");

	if ( !_tcsnicmp( _T("http"), W2CT(varAction.bstrVal), 4 ) )
	{
		strAction = W2A( varAction.bstrVal );
	}
	else
	{
		strAction = m_strBaseURL + ( *varAction.bstrVal != L'/' ? _T("/") : _T("") ) + W2A( varAction.bstrVal );
	}

	return strAction;
}

string CDQSDWizardDlg::GetSwitches()
{
	USES_CONVERSION;

	string strSwitches = _T("");

	BSTR bstr = NULL;
	TCHAR szSwitches[ 1024 ];
	CWindow( GetDlgItem( IDC_Switches ) ).GetWindowText( &bstr );
	if ( CComBSTR( bstr ).Length() > 0 )
	{
		_tcscpy( szSwitches, W2T( bstr ) );
		LPTSTR pszSwitch = _tcstok( szSwitches, _T("\r\n") );
		strSwitches =  _T("\r\n\r\n      // Parse switches\r\n      //var args = parseArgs(q, \"");
		TCHAR pszDelim[ 16 ];
		_tcscpy( pszDelim, _T("") );

		string 
		strCase  = _T("\r\n      //if ( args.switches.length > 0 )");
		strCase += _T("\r\n      //{");
		strCase += _T("\r\n      //  switch( args.switches[0].name )");
		strCase += _T("\r\n      //  {");

		while ( pszSwitch )
		{
			strSwitches += string( pszDelim ) + string( pszSwitch );
			strCase += _T("\r\n      //    case \"") + string( pszSwitch ) + _T("\":");
			strCase += _T("\r\n      //         break;");

			_tcscpy( pszDelim, _T(", ") );
			pszSwitch = _tcstok( NULL, _T("\r\n") );
		}
		strSwitches += _T("\");");

		strCase += _T("\r\n      //    default:");
		strCase += _T("\r\n      //         break;");
		strCase += _T("\r\n      //  } //end-switch");
		strCase += _T("\r\n      //}");

		strSwitches += strCase;
	}
	::SysFreeString( bstr );

	return strSwitches;
}

string CDQSDWizardDlg::EscapeXML( string& xml )
{
	string strEscapedXML = xml;

	// ??? Very quick hack to get rid of ampersands until we figure out a better way
	replace( strEscapedXML.begin(), strEscapedXML.end(), _T('&'), _T('~') );

	return strEscapedXML;
}

void CDQSDWizardDlg::SaveFields()
{
	USES_CONVERSION;

	CRegKey rk;
	rk.Create( HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings") );

	BSTR bstr = NULL;
	CWindow( GetDlgItem( IDC_Contributor ) ).GetWindowText( &bstr );
	rk.SetValue( W2T( bstr ), _T("Contributor") );
	::SysFreeString( bstr );

	CWindow( GetDlgItem( IDC_Email ) ).GetWindowText( &bstr );
	rk.SetValue( W2T( bstr ), _T("Email") );
	::SysFreeString( bstr );

	rk.Close();
}

void CDQSDWizardDlg::RestoreFields()
{
	USES_CONVERSION;

	DWORD dwSize = 0;
	TCHAR szValue[ 1024 ];

	CRegKey rk;
	if ( ERROR_SUCCESS == rk.Open( HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings") ) )
	{
		dwSize = LENGTHOF( szValue );
		if ( ERROR_SUCCESS == rk.QueryValue( szValue, _T("Contributor"), &dwSize ) )
		{
			CWindow( GetDlgItem( IDC_Contributor ) ).SetWindowText( szValue );
		}

		dwSize = LENGTHOF( szValue );
		if ( ERROR_SUCCESS == rk.QueryValue( szValue, _T("Email"), &dwSize ) )
		{
			CWindow( GetDlgItem( IDC_Email ) ).SetWindowText( szValue );
		}
	}
	else
	{
		rk.Open( HKEY_CURRENT_USER, _T("Software\\Microsoft\\MS Setup (ACME)\\User Info"), KEY_READ );

		dwSize = LENGTHOF( szValue );
		if ( ERROR_SUCCESS == rk.QueryValue( szValue, _T("DefName"), &dwSize ) )
		{
			CWindow( GetDlgItem( IDC_Contributor ) ).SetWindowText( szValue );
		}
	}
	
}

LRESULT CDQSDWizardDlg::OnClickedAbout(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CAboutDlg().DoModal();
	return 0;
}

LRESULT CDQSDWizardDlg::OnClickedOptions(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	USES_CONVERSION;

	DWORD dwSize = 0;
	TCHAR szValue[ 1024 ];
	DWORD dwValue = 0;

	COptionsDlg dlgOptions;

	CRegKey rk;
	if ( ERROR_SUCCESS == rk.Open( HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings") ) )
	{
		if ( ERROR_SUCCESS == rk.QueryValue( dwValue, _T("EditResult") ) )
		{
			dlgOptions.m_bEditResult = dwValue;
		}

		dwSize = LENGTHOF( szValue );
		if ( ERROR_SUCCESS == rk.QueryValue( szValue, _T("ResultEditor"), &dwSize ) )
		{
			dlgOptions.m_strEditor = string( szValue );
		}
	}
	
	if ( dlgOptions.DoModal() == IDOK )
	{
		rk.SetValue( dlgOptions.m_bEditResult, _T("EditResult") );
		rk.SetValue( dlgOptions.m_strEditor.c_str(), _T("ResultEditor") );
	}

	rk.Close();
	return 0;
}
