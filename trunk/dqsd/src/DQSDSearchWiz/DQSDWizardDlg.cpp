// DQSDWizardDlg.cpp : Implementation of CDQSDWizardDlg

#include "stdafx.h"
#include "DQSDWizardDlg.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"
#include "ModulVer.h"
#include "Options.h"

void srch_repl( string& s, const string& to_find, const string& repl_with ) 
{
	if ( s.find(to_find, 0) == string::npos )
		return;

	string result;
	string::size_type pos = 0;
	
	while ( true ) 
	{
		string::size_type next = s.find( to_find, pos );
		result.append( s, pos, next - pos );
		if( next != string::npos ) 
		{
			result.append( repl_with );
			pos = next + to_find.size();
		} 
		else 
		{   
			break;  // exit loop
		}
	} 
	s.swap( result );
}

struct CRadioButtonInfo
{
	CRadioButtonInfo( const string& rstrValue, BOOL bActiveElement = FALSE, BOOL bChecked = FALSE ) 
		: m_strValue( rstrValue )
		, m_bActiveElement( bActiveElement ) 
		, m_bChecked( bChecked )
	{ }
	string	m_strValue;
	BOOL	m_bActiveElement;
	BOOL	m_bChecked;
};

/////////////////////////////////////////////////////////////////////////////
// CDQSDWizardDlg

LRESULT CDQSDWizardDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	USES_CONVERSION;

#ifdef _DEBUG
	CWindow( GetDlgItem( IDC_SearchName ) ).SetWindowText( _T("xyzzy") );
	CWindow( GetDlgItem( IDC_SearchTitle ) ).SetWindowText( _T("xyzzy Search") );
#endif

	CenterWindow( GetActiveWindow() ); // ??? should probably use the actual browser window

	m_editSearchName.SubclassWindow( GetDlgItem( IDC_SearchName ) );
	
	TCHAR szModule[ MAX_PATH + 1 ];
	if ( GetModuleFileName( _Module.GetResourceInstance(), szModule, LENGTHOF(szModule) ) )
	{
		CModuleVersion moduleVersion;
		if ( moduleVersion.GetFileVersionInfo( szModule ) )
		{
			m_strVersion = moduleVersion.GetValue( _T("ProductVersion") ) + _T(" ") + moduleVersion.GetValue( _T("SpecialBuild") );
			SetWindowText( ( _T("DQSD Search Wizard - Version ") + m_strVersion ).c_str() );
		}
	}

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
		if ( uc.dwHostNameLength )
		{
			DWORD dwLength = uc.dwSchemeLength + 3 + uc.dwHostNameLength + 1;
			_tcsncpy( szBaseURL, uc.lpszScheme, dwLength );
			szBaseURL[ dwLength ] = _T('\0');

			memset( szScheme, 0, sizeof szScheme );
			_tcsncpy( szScheme, uc.lpszScheme, uc.dwSchemeLength + 3 );
		}
		else
		{
			_tcscpy( szBaseURL, W2T( bstrURL ) );
		}

		CWindow ctl = GetDlgItem( IDC_Link );
		ctl.SetWindowText( szBaseURL );
	}

	// Save the base URL for later use

	szBaseURL[ _tcslen( szBaseURL ) ] = _T('\0');
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

	// See if any of the fields in the form has focus

	CComPtr< IHTMLElement > spActiveElement;
	m_spDoc2->get_activeElement( &spActiveElement );

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

			// If the current form contains the active element, then will select
			// it by default

			VARIANT_BOOL bContainsActiveElement = VARIANT_FALSE;
			spForm->contains( spActiveElement, &bContainsActiveElement );

			// Add the form information to the list
			
			LVITEM lvi;
			memset( &lvi, 0, sizeof lvi );

			lvi.mask = LVIF_TEXT|LVIF_PARAM;
			lvi.iItem = 256;
			lvi.pszText = const_cast<LPTSTR>(strName.c_str());
			lvi.lParam = reinterpret_cast<LPARAM>(pspForm);
			int iPos = ctlFormList2.SendMessage( LVM_INSERTITEM, 0, (LPARAM)&lvi );

			ListView_SetCheckState( ctlFormList2.m_hWnd, iPos, bContainsActiveElement );

			lvi.mask = LVIF_TEXT;
			lvi.iItem = iPos;
			lvi.iSubItem = 1;
			lvi.pszText = const_cast<LPTSTR>(strAction.c_str());
			ctlFormList2.SendMessage( LVM_SETITEM, iPos, (LPARAM)&lvi );
		}
	}

	// If there's only one form, select it

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
	if ( !bChecked && cItems > 0 )
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

	TCHAR szDate[ 128 ];
	_tstrdate( szDate );
	TCHAR szTime[ 128 ];
	_tstrtime( szTime );

	if ( m_options.IncludeComments() )
	{
		strSearchFile += _T("\r\n  <COMMENT>"
							"\r\n"
							"\r\n    This search file was initially created by Dave's Quick Search Deskbar"
							"\r\n    Search Wizard version ") + m_strVersion + _T(" on ") + string( szDate ) + _T(" at ") + string( szTime );
		strSearchFile += _T("\r\n"
							"\r\n    Even though this XML search will probably load and is a healthy start"
							"\r\n    toward a completed search, please be aware that this search will probably"
							"\r\n    not work as is and will require some human inspection and modification."
							"\r\n"
							"\r\n  </COMMENT>");
	}

	CWindow( GetDlgItem( IDC_SearchTitle ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <name>") + EscapeXML( string( W2T( bstr ) ) ) + _T("</name>");
	::SysFreeString( bstr );

	CWindow( GetDlgItem( IDC_Category ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <category>") + EscapeXML( string( W2T( bstr ) ) ) + _T("</category>");
	::SysFreeString( bstr );
	
	CWindow( GetDlgItem( IDC_Contributor ) ).GetWindowText( &bstr );
	strSearchFile += _T("\r\n  <contributor>") + EscapeXML( string( W2T( bstr ) ) ) + _T("</contributor>");
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
		strSearchFile += _T("\r\n  ") + EscapeXML( string( W2T( bstr ) ) );
	strSearchFile += _T("\r\n  </description>");
	::SysFreeString( bstr );

	string strFormScript;
	strSearchFile += GetForms( strSearchName, strFormScript );

	strSearchFile += _T("\r\n  <script><![CDATA["
                        "\r\n    function ") + strSearchName + _T("(q)"
                        "\r\n    {"
                        "\r\n      if( nullArgs(\"") + strSearchName + _T("\", q) )"
                        "\r\n        return;"
						"\r\n"
						"\r\n      if( q == \"\" )"
						"\r\n      {"
						"\r\n        openSearchWindow(\"") + strLink + _T("\");"
						"\r\n        return;"
						"\r\n      }");
	
	strSearchFile += GetSwitches();

	strSearchFile += strFormScript;
    
	strSearchFile += _T("\r\n"
						"\r\n      submitForm(") + strSearchName + _T("f);"
						"\r\n    }"
						"\r\n  ]]></script>");

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
			// Save the file

			DWORD dwBytesWritten = 0;
			::WriteFile( hFile, strSearchFile.c_str(), strSearchFile.length(), &dwBytesWritten, NULL );
			::FlushFileBuffers( hFile );
			::CloseHandle( hFile );

			// Throw the file up in an editor if they so choose

			COptions options;
			options.Load();
			if ( options.EditResults() )
			{
				TCHAR szCommandLine[ 1024 ];
				_tcscpy( szCommandLine, ( _T("\"") + options.Editor() + _T("\" \"") + string( ofn.lpstrFile ) + _T("\"") ).c_str() );
				::WinExec( szCommandLine, SW_SHOW );
			}
		}

		EndDialog(wID);
	}

	return 0;
}

string CDQSDWizardDlg::GetForms( const string& rstrSearchName, string& rstrFormScript )
{
	USES_CONVERSION;

	string strFormXML = _T("");

	// Grab active element so we can assign 'q' to it in the gen'd script	
	CComPtr< IHTMLElement > spActiveElement;
	m_spDoc2->get_activeElement( &spActiveElement );
	CComPtr< IUnknown > spActiveElementUnk = spActiveElement;

	CWindow ctlFormList = GetDlgItem( IDC_FormList2 );
	int iSelectedForms = 0;
	const int cForms = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
	for ( int iForm = 0; iForm < cForms; iForm++ )
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
				// Map a radio button to its values
				typedef vector< CRadioButtonInfo > RadioButtonValues_t;
				typedef map< string, RadioButtonValues_t > RadioButtonMap_t;
				RadioButtonMap_t mapRadioButtons;

				rstrFormScript += _T("\r\n\r\n      // FORM variables for ") + strFormName;
				
				for ( int iFormElem = 0; iFormElem < cFormElements; iFormElem++ )
				{
					_variant_t varItem( static_cast<long>(iFormElem), VT_I4 );
					CComPtr< IDispatch > spIDisp;
					spFormElement->item( varItem, varItem, &spIDisp );

					CComQIPtr< IHTMLElement > spElement( spIDisp );
					if ( !spElement )
						continue;

					BSTR bstrTagName = NULL;
					spElement->get_tagName( &bstrTagName );

					// Skip if the element doesn't have a name

					_variant_t varInputName;
					string strInputName;
					if ( SUCCEEDED( spElement->getAttribute( _bstr_t( _T("name") ), 0, &varInputName ) ) && varInputName.bstrVal )
					{
						strInputName = W2T( varInputName.bstrVal );

						// Ignore INPUT type=submit

						string strInputType = _T("");
						_variant_t varInputType;
						if ( SUCCEEDED( spElement->getAttribute( _bstr_t( _T("type") ), 0, &varInputType ) ) )
						{
							strInputType = W2T( varInputType.bstrVal );
						}
						if ( !_tcsicmp( _T("submit"), strInputType.c_str() ) )
							continue;

						// Was this the active element?

						CComPtr< IUnknown > spElementUnk = spElement;
						BOOL bActiveElement = spElementUnk.IsEqualObject( spActiveElementUnk );


						// Get the value

						_variant_t varInputValue;
						string strInputValue = _T("");
						if ( SUCCEEDED( spElement->getAttribute( _bstr_t( _T("value") ), 0, &varInputValue ) ) && varInputValue.bstrVal )
						{
							strInputValue = EscapeXML( string( W2T( varInputValue.bstrVal ) ) );
						}

						BOOL bChecked = FALSE;
						_variant_t varChecked;
						if ( SUCCEEDED( spElement->getAttribute( _bstr_t( _T("checked") ), 0, &varChecked ) ) )
						{
							bChecked = varChecked.boolVal;
						}

						// Stick the value of the field in as well for two reasons... some hidden fields are required 
						// and the user can enter a string in a visible field to see which field to use.
						
						if ( !_tcsicmp( _T("radio"), strInputType.c_str() ) )
						{
							RadioButtonMap_t::iterator itFound = mapRadioButtons.find( strInputName );
							RadioButtonValues_t radioValues;
							if ( itFound != mapRadioButtons.end() )
							{
								itFound->second.push_back( CRadioButtonInfo( strInputValue, bActiveElement, bChecked ) );
								ATLTRACE( _T("%s = %d\n"), strInputName.c_str(), itFound->second.size() );
							}
							else
							{
								radioValues.push_back( CRadioButtonInfo( strInputValue, bActiveElement, bChecked ) );
								mapRadioButtons.insert( RadioButtonMap_t::value_type( strInputName, radioValues ) );
							}
							continue;
						}

						if ( bActiveElement && m_options.IncludeComments() )
						{
							strFormXML += _T("\r\n\r\n    <COMMENT> The following field was active (i.e. had focus) when the search was generated. </COMMENT>\r\n");
						}

						strFormXML += _T("\r\n    <input type=\"hidden\" name=\"") + strInputName + _T("\" value=\"") + strInputValue + _T("\" />");

						// Only set the search string to the form field if it was a text field

						if ( bActiveElement && 
							 _tcsicmp( _T("radio"), strInputType.c_str() ) && 
							 _tcsicmp( _T("checkbox"), strInputType.c_str() ) &&
							 _tcsicmp( W2T( bstrTagName ), _T("SELECT") ) )
						{
							rstrFormScript += _T("\r\n"
												 "\r\n      // The wizard assigned the search string to this form field value because"
												 "\r\n      // this field was the active element when the search file was generated."
												 "\r\n      // Change this to args.q if the search string is parsed with parseArgs."
												 "\r\n      document.") + strFormName + GetScriptFieldName( strInputName ) + _T(" = q;");
						}
						else
						{
							rstrFormScript += _T("\r\n      //document.") + strFormName + GetScriptFieldName( strInputName ) + _T(" = \"\";");
						}

						if ( m_options.IncludeComments() && !_tcsicmp( _T("checkbox"), strInputType.c_str() ) )
						{
							strFormXML += _T("\r\n    <COMMENT>"
											 "\r\n      The input element above, \"") + strInputName + _T("\", was a checkbox that was ") + 
											 string( bChecked ? _T("checked.") : _T("unchecked.") );
							strFormXML += _T("\r\n    </COMMENT>\r\n");
						}

						if ( m_options.IncludeComments() && !_tcsicmp( W2T( bstrTagName ), _T("SELECT") ) )
						{
							strFormXML += _T("\r\n    <COMMENT>"
											 "\r\n      The input element above, \"") + strInputName + _T("\", was a SELECT element with the following options..."
											 "\r\n      <select name=\"" + strInputName + "\">");
							
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
									
									strFormXML += _T("\r\n        <option");
									if ( bstrOptionValue )
										strFormXML += _T(" value=\"") + EscapeXML( string( W2T( bstrOptionValue ) ) ) + _T("\"");
									strFormXML += _T(">");
									strFormXML += EscapeXML( string( W2T( bstrOptionText ) ) ) + _T("</option>");

									::SysFreeString( bstrOptionValue );
								}
							}

							strFormXML += _T("\r\n      </select>"
											 "\r\n    </COMMENT>\r\n");
						
						} // end-if SELECT
					
					} // end-if INPUT element had a 'name' attribute

					::SysFreeString( bstrTagName );
				
				} // end-for each form element


				// Append radio INPUT elements

				if ( mapRadioButtons.size() > 0 )
				{
					RadioButtonMap_t::const_iterator itElements = mapRadioButtons.begin();
					for ( ; itElements != mapRadioButtons.end(); itElements++ )
					{
						string strRadioElement = _T("\r\n    <input type=\"hidden\" name=\"") + itElements->first + _T("\" value=\"");
						string strRadioValues =  _T("\r\n    <COMMENT>"
													"\r\n      The input element above, \"") + itElements->first + ("\" was a set of radio buttons with the following options...");

						RadioButtonValues_t::const_iterator itValues = itElements->second.begin();
						string strDelim = _T("\r\n      ");
						for ( ; itValues != itElements->second.end(); itValues++ )
						{
							strRadioValues += strDelim + _T("\"") + itValues->m_strValue + _T("\"");
							strDelim = _T(", ");
							if ( itValues->m_bChecked )
							{
								strRadioElement += itValues->m_strValue;
							}
						}
						
						rstrFormScript += _T("\r\n      //document.") + strFormName + GetScriptFieldName( itElements->first ) + _T(" = \"\";");

						strRadioElement += _T("\" />");
						strRadioValues += _T("\r\n    </COMMENT>\r\n");

						strFormXML += strRadioElement;
						
						if ( m_options.IncludeComments() )
						{
							strFormXML += strRadioValues;
						}
					}
				}

			
			} // end-if able to get form element count

			
			strFormXML += _T("\r\n  </form>");

			++iSelectedForms;
		
		} // end-if this form was selected by user
	
	} // end-for each form

	
	// If there were no forms, then go ahead and create a template for one

	if ( 0 == cForms )
	{
		strFormXML += _T("\r\n  <form name=\"") + rstrSearchName + _T("f\""
						 "\r\n        method=\"get\""
						 "\r\n        action=\"\">"
						 "\r\n    <input type=\"hidden\" name=\"field1\" value=\"\"/>"
						 "\r\n    <input type=\"hidden\" name=\"field2\" value=\"\"/>"
						 "\r\n  </form>");
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
	else if ( _tcslen( W2CT(varAction.bstrVal) ) == 0 )
	{
		strAction = m_strBaseURL;
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
		strSwitches = _T("\r\n"
						 "\r\n      // Parse switches with parseArgs:");
		
		if ( m_options.IncludeComments() )
		{
			strSwitches += _T("\r\n"
							 "\r\n      // parseArgs usage:"
							 "\r\n      // Arguments:"
							 "\r\n      //    q                - string from the search function"
							 "\r\n      //    expectedSwitches - list or array of the expected switch values"
							 "\r\n      //    expandSwitches   - optional parameter [default = true] used to determine "
							 "\r\n      //                       if the switch shortcuts should be expanded (i.e. /f becomes /foo)"
							 "\r\n      // Returns an object with these properties:"
							 "\r\n      //    q        - the input string with the switches removed"
							 "\r\n      //    switches - array of objects with these two properties:"
							 "\r\n      //                  name:   expanded name of the matched switch (i.e. foo as in /foo:bar)"
							 "\r\n      //                  value:  value of switch (i.e. bar as in /foo:bar)"
							 "\r\n      //    switch_val - associative array with the switch name as the key with the switch value "
							 "\r\n      //                 as the value. (i.e. switch_val[\"foo\"] = \"bar\" as in /foo:bar)"
							 "\r\n");
		}
		
		strSwitches += _T("\r\n      //var args = parseArgs(q, \"");
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
			strCase += _T("\r\n      //    case \"") + string( pszSwitch ) + _T("\":"
						  "\r\n      //         break;");

			_tcscpy( pszDelim, _T(", ") );
			pszSwitch = _tcstok( NULL, _T("\r\n") );
		}
		strSwitches += _T("\");");

		strCase += _T("\r\n      //    default:"
					  "\r\n      //         break;"
					  "\r\n      //  } //end-switch"
					  "\r\n      //}");

		strSwitches += strCase;
	}
	::SysFreeString( bstr );

	return strSwitches;
}

string CDQSDWizardDlg::EscapeXML( string& xml )
{
	srch_repl( xml, _T("&"), _T("&amp;") );
	srch_repl( xml, _T("<"), _T("&lt;") );
	srch_repl( xml, _T(">"), _T("&gt;") );
//	srch_repl( xml, _T("'"), _T("&apos;") );
//	srch_repl( xml, _T("\""), _T("&quot;") );
//	srch_repl( xml, _T("\r\n"), _T("\\n") );

	return xml;
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
	COptionsDlg dlgOptions;
	if ( IDOK == dlgOptions.DoModal() )
		m_options.Load();
	return 0;
}

// Convert the fieldname to a valid script form of the name
// If there are any non-alpha characters in the INPUT field name, 
// then use different notation in the script

string CDQSDWizardDlg::GetScriptFieldName( const string& rstrFieldName )
{
	string strScriptInputName = _T(".") + rstrFieldName + _T(".value");
	if ( ( rstrFieldName.find_first_not_of( _T("_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890"), 0 ) != string::npos ) ||
		 ( !_tcsicmp( rstrFieldName.c_str(), _T("target") ) ) ||
		 ( !_tcsicmp( rstrFieldName.c_str(), _T("submit") ) ) )
	{
		strScriptInputName = _T("[\"") + rstrFieldName + _T("\"].value");
	}
	return strScriptInputName;
}
