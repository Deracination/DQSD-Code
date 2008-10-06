// DQSDWizardDlg.cpp : Implementation of CDQSDWizardDlg
// Copyright (c) 2002 Glenn Carr; Distributed under the terms of the GNU General Public License, Version 2

#include "stdafx.h"
#include "DQSDWizardDlg.h"
#include "AboutDlg.h"
#include "OptionsDlg.h"
#include "ModulVer.h"
#include "Options.h"

#include "HtmlElement.h"
#include "Registry.h"
#include "FormElement.h"

void srch_repl( string& s, const string& to_find, const string& repl_with ) 
{
	try
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
	catch ( ... )
	{
		MessageBox( ::GetForegroundWindow(), _T("An internal error was encountered:  srch_repl"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}
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
	try
	{
		USES_CONVERSION;

		// Create tooltip for controls

		m_tip.Create( m_hWnd );

		int rgCtrlIDs[] = { 
			IDC_SearchName,
			IDC_SearchTitle,
			IDC_Link,
			IDC_Category,
			IDC_Contributor,
			IDC_Email,
			IDC_Description,
			IDC_Switches,
			IDC_DescExamples,
			IDC_MutuallyExclusiveSwitches,
			IDC_DescSwitches,
			IDC_FormList,
			IDC_ShowHideHTML,
			IDC_FormFields,
			IDC_Options,
			IDC_About,
			IDC_ShowTips,
		};

		for ( int iCtrl = 0; iCtrl < LENGTHOF( rgCtrlIDs ); iCtrl++ )
		{
			CString csTip;
			if ( csTip.LoadString( rgCtrlIDs[ iCtrl ] ) )
			{
				m_tip.AddTool( rgCtrlIDs[ iCtrl ], csTip.LockBuffer() );
				csTip.UnlockBuffer();
			}
		}

		ATLTRACE( "CWindow( GetDlgItem( IDC_ShowTips ) ).SendMessage( BM_GETCHECK, 0, 0 ): %d\n", CWindow( GetDlgItem( IDC_ShowTips ) ).SendMessage( BM_GETCHECK, 0, 0 ) );
		ATLTRACE( "BST_CHECKED: %d\n", BST_CHECKED );
		m_tip.Activate( ( CWindow( GetDlgItem( IDC_ShowTips ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED ) );

		// Shrink dialog initially
		
		RECT rcDialog;
		GetWindowRect( &rcDialog );

		m_nTallDialogHeight = rcDialog.bottom - rcDialog.top;
		ShrinkDialog();

		// Disable controls dependent on having switches
		
		OnChangeSwitches( 0, 0, NULL, bHandled );

		// Just for debugging
	
		#ifdef _DEBUG
		CWindow( GetDlgItem( IDC_SearchName ) ).SetWindowText( _T("xyzzy") );
		CWindow( GetDlgItem( IDC_SearchTitle ) ).SetWindowText( _T("xyzzy Search") );
		#endif

		CenterWindow( GetActiveWindow() ); // ??? should probably use the actual browser window

		// Subclass search name to restrict to lowercase and no spaces
		
		m_editSearchName.SubclassWindow( GetDlgItem( IDC_SearchName ) );
		
		// Get the copyright for use in various places

		TCHAR szModule[ MAX_PATH + 1 ];
		if ( GetModuleFileName( _Module.GetResourceInstance(), szModule, LENGTHOF(szModule) ) )
		{
			CModuleVersion moduleVersion;
			if ( moduleVersion.GetFileVersionInfo( szModule ) )
			{
				m_strVersion = moduleVersion.GetValue( _T("ProductVersion") );
				if ( moduleVersion.GetValue( _T("SpecialBuild") ).size() > 0 )
					m_strVersion += _T(" ") + moduleVersion.GetValue( _T("SpecialBuild") );
				SetWindowText( ( _T("DQSD Search Wizard - Version ") + m_strVersion ).c_str() );

				m_strCopyright = moduleVersion.GetValue( _T("LegalCopyright") );
			}
		}

		// Get DQSD install dir
		CRegKey rk;
		if ( ERROR_SUCCESS == rk.Open( HKEY_CLASSES_ROOT, _T("CLSID\\{EC9FE983-E520-4D8F-B1A7-ACBCA0439C70}") ) )
		{
            QueryRegValue(rk, "InstallDir", m_strInstallDir);
		}

		// Get the base URL and put it in the 'Link' element
		CComBSTR bstrURL;
		HR(m_spMainDoc->get_URL(&bstrURL));
        SetDlgItemText(IDC_Link, CW2CT(bstrURL));

		// Save the base URL of this frame for later use
        CComBSTR bstrFrameBaseURL;
		HR(m_spActiveFrameDoc->get_URL(&bstrFrameBaseURL));
		m_strBaseURL = W2CT(bstrFrameBaseURL);

		// Set up the list for the FORM elements
		CWindow ctlFormList = GetDlgItem(IDC_FormList);
		ctlFormList.SendMessage( LVM_SETEXTENDEDLISTVIEWSTYLE, 
								  0, 
								  LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_HEADERDRAGDROP );
		LVCOLUMN lvc;
		memset( &lvc, 0, sizeof lvc );
		lvc.mask = LVCF_TEXT|LVCF_ORDER|LVCF_WIDTH;

		lvc.pszText = _T("");
		lvc.cx = 64;
		lvc.iOrder = 0;
		ctlFormList.SendMessage( LVM_INSERTCOLUMN, 0, (LPARAM)&lvc );
		
		lvc.pszText = _T("action");
		lvc.cx = 420;
		lvc.iOrder = 1;
		ctlFormList.SendMessage( LVM_INSERTCOLUMN, 1, (LPARAM)&lvc );

		// Set up the combobox
		// ??? It would be really cool to actually base these on the current
		// ??? search categories/subcategories

		LPCTSTR rgszCategories[] = {
			_T("Computers"),
			_T("Computers/Networking"),
			_T("Computers/Programming"),
			_T("Computers/Reference"),
			_T("Computers/Software"),
			_T("Entertainment"),
			_T("Finance"),
			_T("Fun"),
			_T("Functions"),
			_T("News"),
			_T("People and Places"),
			_T("Reference"),
			_T("Reference/General"),
			_T("Reference/Language"),
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
		HR(m_spActiveFrameDoc->get_forms(&spFormCollection));

		long cForms = 0;
		HR(spFormCollection->get_length( &cForms ));

		int cDisplayedForms = 0;

		// See if any of the fields in the form has focus

		CComPtr< IHTMLElement > spActiveElement;
		m_spActiveFrameDoc->get_activeElement( &spActiveElement );

		for (int iForm = 0; iForm < cForms; iForm++)
		{
			CComPtr<IDispatch> spFormDisp;
			spFormCollection->item(CComVariant(iForm), CComVariant(0), &spFormDisp);

			CComQIPtr< IHTMLElement > spForm(spFormDisp);
			if (spForm)
			{
                auto_ptr<Form> form = BuildForm(spForm);
                string action = form->GetAbsoluteActionPath(m_strBaseURL);

				// If the current form contains the active element, then we'll select
				// it by default
				VARIANT_BOOL bContainsActiveElement = VARIANT_FALSE;
				spForm->contains(spActiveElement, &bContainsActiveElement);

                AddToFormList(form->Name(), action, spForm, bContainsActiveElement == VARIANT_TRUE);
                cDisplayedForms++;
			}
		}

		// If there's only one form, select it
		if (cDisplayedForms == 1)
			ListView_SetCheckState(ctlFormList.m_hWnd, 0, TRUE);

	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::OnInitDialog"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	m_spWB2->put_StatusText( _bstr_t( ( _T("DQSD Search Wizard - Version ") + m_strVersion + _T(", ") + m_strCopyright ).c_str() ) );

	return 1;  // Let the system set the focus
}

LRESULT CDQSDWizardDlg::OnFormListItemChanged(int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled)
{
	try
	{
		USES_CONVERSION;

		NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		ATLTRACE( _T("pNMListView->uNewState = %d\n"), pNMListView->uNewState );
		ATLTRACE( _T("pNMListView->iItem = %d\n"), pNMListView->iItem );

		CWindow ctlFormList = GetDlgItem(idCtrl);
        CComPtr<IHTMLElement> spForm = GetLParamHTMLElement(ctlFormList, pNMListView->iItem);
		if ( pNMListView->uNewState == 3 )
		{
			BSTR bstr = NULL;
			if ( SUCCEEDED( spForm->get_outerHTML( &bstr ) ) )
			{
				CWindow ctlForm = GetDlgItem( IDC_FormFields );
				ctlForm.SetWindowText( W2T( bstr ) );
				::SysFreeString( bstr );
			}

			m_spSelectedStyle = NULL;
			spForm->get_style( &m_spSelectedStyle );

			CComBSTR bstrStyleText;
			m_spSelectedStyle->get_cssText( &bstrStyleText );
			m_mapUnselectedStyle[ pNMListView->iItem ] = bstrStyleText;

			m_spSelectedStyle->put_cssText( _bstr_t(m_options.FormCSS().c_str() ) );
		}
		else if ( pNMListView->uNewState == 0 )
		{
			CComPtr< IHTMLStyle > spStyle;
			spForm->get_style( &spStyle );

			spStyle->put_cssText( m_mapUnselectedStyle[ pNMListView->iItem ] );
			return 1;
		}
		else
		{
			return 1;
		}

	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::OnFormListItemChanged"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return 1;
}

LRESULT CDQSDWizardDlg::OnFormListItemDeleted( int idCtrl, LPNMHDR pNMHDR, BOOL& bHandled )
{
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

    IHTMLElement* pForm = GetLParamHTMLElement(GetDlgItem(idCtrl), pNMListView->iItem);
    if (pForm != NULL)
        pForm->Release();

    return 1;
}

LRESULT CDQSDWizardDlg::OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	try
	{
		USES_CONVERSION;

		// Make sure required fields are there

		CWindow ctlSearchName = GetDlgItem( IDC_SearchName );
		CComBSTR bstrSearchName;
		ctlSearchName.GetWindowText( &bstrSearchName );
		if ( bstrSearchName.Length() == 0 )
		{
			MessageBox( _T("Please enter a search name."), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
			ctlSearchName.SetFocus();
			return 0;
		}

		CWindow ctlSearchTitle = GetDlgItem( IDC_SearchTitle );
		CComBSTR bstrSearchTitle;
		ctlSearchTitle.GetWindowText( &bstrSearchTitle );
		if ( bstrSearchTitle.Length() == 0 )
		{
			MessageBox( _T("Please enter a search title."), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
			ctlSearchTitle.SetFocus();
			return 0;
		}

		CWindow ctlFormList = GetDlgItem( IDC_FormList );
		const int cItems = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
		BOOL bChecked = FALSE;
		for ( int i = 0; i < cItems && !bChecked; i++ )
		{
			if ( ListView_GetCheckState( GetDlgItem( IDC_FormList ), i ) )
				bChecked = TRUE;
		}
		if ( !bChecked && cItems > 0 )
		{
			MessageBox( _T("Please select one or more forms."), _T("DQSD Search Wizard"), MB_OK|MB_ICONWARNING );
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
								"\r\n    Even though this XML search will probably load and is a good start"
								"\r\n    toward a completed search, please be aware that this search will probably"
								"\r\n    not work as is and will probably require some changes."
								"\r\n"
								"\r\n  </COMMENT>");
		}

		CWindow( GetDlgItem( IDC_SearchTitle ) ).GetWindowText( &bstr );
		strSearchFile += _T("\r\n  <name>") + EscapeXML( string( W2T( bstr ) ) ) + _T("</name>");
		::SysFreeString( bstr );

		CWindow( GetDlgItem( IDC_Category ) ).GetWindowText( &bstr );
		strSearchFile += _T("\r\n  <category>") + ParseCategory( W2T( bstr ) ) + _T("</category>");
		::SysFreeString( bstr );
		
		CWindow( GetDlgItem( IDC_Contributor ) ).GetWindowText( &bstr );
		strSearchFile += _T("\r\n  <contributor>") + EscapeXML( string( W2T( bstr ) ) ) + _T("</contributor>");
		::SysFreeString( bstr );

		CWindow( GetDlgItem( IDC_Link ) ).GetWindowText( &bstr );
		string strLink = W2T( bstr );
		strSearchFile += _T("\r\n  <link>") + EscapeXML( strLink ) + _T("</link>");
		::SysFreeString( bstr );

		CWindow( GetDlgItem( IDC_Email ) ).GetWindowText( &bstr );
		strSearchFile += _T("\r\n  <email>") + string( W2T( bstr ) ) + _T("</email>");
		::SysFreeString( bstr );

		CWindow( GetDlgItem( IDC_Description ) ).GetWindowText( &bstr );
		strSearchFile += _T("\r\n  <description>");
		if ( _tcslen( W2T( bstr ) ) )
			strSearchFile += _T("\r\n  ") + EscapeXML( string( W2T( bstr ) ) );

		vector< string > vecSwitches;
		string strSwitchesCode = GetSwitches( vecSwitches );

		strSearchFile += GetExamples( vecSwitches );
		
		strSearchFile += _T("\r\n  </description>");
		::SysFreeString( bstr );

		string strFormScript;
		strSearchFile += GetForms( strSearchName, strFormScript );

		strSearchFile += _T("\r\n  <script><![CDATA["
							"\r\n    function ") + strSearchName + _T("(q)"
							"\r\n    {"
							"\r\n      if( nullArgs(\"") + strSearchName + _T("\", q) )"
							"\r\n        return;");
		
		strSearchFile += strSwitchesCode;

		strSearchFile += strFormScript;
    
		strSearchFile += _T("\r\n"
							"\r\n      submitForm(") + strSearchName + _T("f);"
							"\r\n    }"
							"\r\n  ]]></script>");

		strSearchFile += _T("\r\n  <copyright>"
							"\r\n    The following applies if this file is included and distributed with Dave's Quick Search Deskbar:"
							"\r\n    Copyright (c) 2002 David Bau; Distributed under the terms of the GNU Public License, Version 2 (http://www.gnu.org/copyleft/gpl.txt)"
							"\r\n  </copyright>"
							"\r\n  <created_by>"
							"\r\n    This search file was initially created on ") + string( szDate ) + _T(" at ") + string( szTime ) + 
						 _T("\r\n    by Dave's Quick Search Deskbar Search Wizard version ") + m_strVersion + _T(","
							"\r\n    ") + m_strCopyright +
						 _T("\r\n  </created_by>"
							"\r\n</search>");

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
				if (m_options.EditResults() )
				{
                    string commandLine;
                    commandLine += "\"" + m_options.Editor() + "\"";
                    commandLine += " ";
                    commandLine += "\"" + string(ofn.lpstrFile) + "\"";
					::WinExec(commandLine.c_str(), SW_SHOW );
				}
			}

			EndDialog(wID);
		}
	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::OnOK"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return 0;
}

string CDQSDWizardDlg::GetForms( const string& rstrSearchName, string& rstrFormScript )
{
	string strFormXML;

	try
	{
		// Grab active element so we can assign 'q' to it in the gen'd script	
		CComPtr<IHTMLElement> spActiveElement;
		m_spActiveFrameDoc->get_activeElement(&spActiveElement);

		CWindow ctlFormList = GetDlgItem( IDC_FormList );
		const int cForms = ctlFormList.SendMessage( LVM_GETITEMCOUNT, 0, 0 );
		for ( int iForm = 0; iForm < cForms; iForm++ )
		{
            int iSelectedForms = 0;

			if (ListView_GetCheckState(ctlFormList.m_hWnd, iForm))
			{
                // Parse and re-generate form
                CComPtr<IHTMLElement> spForm = GetLParamHTMLElement(ctlFormList, iForm);
                auto_ptr<Form> form = BuildForm(spForm);

                // Set a new form name based on the search name
                form->SetName(CreateFormName(iSelectedForms, rstrSearchName));

                strFormXML += "\r\n  <form name=\"" + form->Name() + "\"";
                strFormXML += "\r\n        method=\"" + form->Method() + "\"";
                strFormXML += "\r\n        action=\"" + EscapeXML(form->GetAbsoluteActionPath(m_strBaseURL)) + "\"";
                strFormXML += ">";

				// get form fields
				rstrFormScript = _T("");

				CComQIPtr< IHTMLFormElement > spFormElement(spForm);
				long cFormElements = 0;
				if ( SUCCEEDED( spFormElement->get_length( &cFormElements ) ) )
				{
					// Map a radio button to its values
					typedef vector< CRadioButtonInfo > RadioButtonValues_t;
					typedef map< string, RadioButtonValues_t > RadioButtonMap_t;
					RadioButtonMap_t mapRadioButtons;

					rstrFormScript += _T("\r\n\r\n      // FORM variables for ") + form->Name();
					
					for ( int iFormElem = 0; iFormElem < cFormElements; iFormElem++ )
					{
						_variant_t varItem( static_cast<long>(iFormElem), VT_I4 );
						CComPtr< IDispatch > spIDisp;
						spFormElement->item( varItem, varItem, &spIDisp );

						CComQIPtr< IHTMLElement > spElement( spIDisp );
						if ( !spElement )
							continue;

                        HtmlElement domElement(spElement);

                        std::string tag, inputName, inputType, inputValue;
                        bool checked = false;

                        domElement.GetTagName(tag);
                        domElement.GetAttribute(L"name", inputName);
                        domElement.GetAttribute(L"type", inputType);
                        domElement.GetAttribute(L"value", inputValue);
                        domElement.GetAttribute(L"checked", checked);

                        FormElement element(tag, "", inputName, inputValue, checked);

                        if (!element.Name().empty())
                        {
                            // Was this the active element?
                            BOOL bActiveElement = spElement.IsEqualObject(spActiveElement);

                            // Stick the value of the field in as well for two reasons... some hidden fields are required 
                            // and the user can enter a string in a visible field to see which field to use.
                            if (element.Type() == "radio")
                            {
                                CRadioButtonInfo buttonInfo(element.Value(), bActiveElement, checked);

                                RadioButtonMap_t::iterator itFound = mapRadioButtons.find(element.Name());
                                if (itFound != mapRadioButtons.end())
                                {
                                    itFound->second.push_back(buttonInfo);
                                }
                                else
                                {
                                    RadioButtonValues_t radioValues;
                                    radioValues.push_back(buttonInfo);
                                    mapRadioButtons.insert(RadioButtonMap_t::value_type(element.Name(), radioValues));
                                }
                            }
                            else
                            {
                                if (bActiveElement && m_options.IncludeComments())
                                {
                                    strFormXML += _T("\r\n\r\n    <COMMENT> The following field was active (i.e. had focus) when the search was generated. </COMMENT>\r\n");
                                }

                                strFormXML += _T("\r\n    <input type=\"hidden\" name=\"") + element.Name() + _T("\" value=\"") + element.Value() + _T("\" />");

                                // Only set the search string to the form field if it was a text field
                                if ( bActiveElement && 
                                    element.Type() != "radio" &&
                                    element.Type() != "checkbox" &&
                                    element.Tag() != "select")
                                {
                                    rstrFormScript += _T("\r\n"
                                        "\r\n      // The wizard assigned the search string to this form field value because"
                                        "\r\n      // this field was the active element when the search file was generated."
                                        "\r\n      // Change this to args.q if the search string is parsed with parseArgs."
                                        "\r\n      document.") + form->Name() + GetScriptFieldName(element.Name()) + _T(" = q;");
                                }
                                else
                                {
                                    rstrFormScript += _T("\r\n      //document.") + form->Name() + GetScriptFieldName(element.Name()) + _T(" = \"\";");
                                }

                                if (m_options.IncludeComments() && element.Type() == "checkbox")
                                {
                                    strFormXML += _T("\r\n    <COMMENT>"
                                        "\r\n      The input element above, \"") + element.Name() + _T("\", was a checkbox that was ") + 
                                        string( checked ? _T("checked.") : _T("unchecked.") );
                                    strFormXML += _T("\r\n    </COMMENT>\r\n");
                                }

                                if (m_options.IncludeComments() && element.Tag() == "select")
                                {
                                    strFormXML += _T("\r\n    <COMMENT>"
                                        "\r\n      The input element above, \"") + element.Name() + _T("\", was a SELECT element with the following options..."
                                        "\r\n      <select name=\"" + element.Name() + "\">");

                                    CComQIPtr<IHTMLSelectElement> spSelect(spElement);
                                    if (spSelect)
                                    {
                                        long cOptions = 0;
                                        spSelect->get_length(&cOptions);
                                        for (int iOption = 0; iOption < cOptions; iOption++)
                                        {
                                            CComPtr<IDispatch> spOptionDisp;
                                            _variant_t varOption( static_cast<long>(iOption), VT_I4 );
                                            spSelect->item(varOption, varOption, &spOptionDisp);

                                            CComQIPtr<IHTMLOptionElement> spOption(spOptionDisp);

                                            CComBSTR bstrOptionValue;
                                            spOption->get_value(&bstrOptionValue);

                                            CComBSTR bstrOptionText;
                                            spOption->get_text(&bstrOptionText);

                                            strFormXML += _T("\r\n        <option");
                                            if (bstrOptionValue)
                                                strFormXML += _T(" value=\"") + EscapeXML( string( CW2T( bstrOptionValue ) ) ) + _T("\"");
                                            strFormXML += _T(">");
                                            strFormXML += (bstrOptionText ? EscapeXML( string( CW2T( bstrOptionText ) ) ) : string(_T("")) ) + _T("</option>");
                                        }
                                    }

                                    strFormXML += _T("\r\n      </select>"
                                        "\r\n    </COMMENT>\r\n");
                                } // end-if SELECT
                            } // end-else is radio
                        } // end-if INPUT element had a 'name' attribute
					
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
							
							rstrFormScript += _T("\r\n      //document.") + form->Name() + GetScriptFieldName( itElements->first ) + _T(" = \"\";");

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

	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::GetForms"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return strFormXML;
}

string CDQSDWizardDlg::GetSwitches( vector< string >& rvecSwitches )
{
	USES_CONVERSION;

	string strSwitches = _T("");

	try
	{

		BSTR bstr = NULL;
		TCHAR szSwitches[ 1024 ];
		CWindow( GetDlgItem( IDC_Switches ) ).GetWindowText( &bstr );
		if ( CComBSTR( bstr ).Length() > 0 )
		{
			_tcscpy( szSwitches, W2T( bstr ) );
			const LPCTSTR szDELIMITERS =  _T("\r\n,; ");
			LPTSTR pszSwitch = _tcstok( szSwitches, szDELIMITERS );
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

			string strCase = _T("");
			BOOL bMutuallyExclusiveSwitches = CWindow( GetDlgItem( IDC_MutuallyExclusiveSwitches ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED;
			if ( bMutuallyExclusiveSwitches )
			{
				strCase += _T("\r\n      //if ( args.switches.length > 0 )"
							  "\r\n      //{"
							  "\r\n      //  switch( args.switches[0].name )"
							  "\r\n      //  {");
			}
			else
			{
				strCase += _T("\r\n      //for (var iSwitch = 0; iSwitch < args.switches.length; iSwitch++)"
							  "\r\n      //{"
							  "\r\n      //  switch( args.switches[iSwitch].name )"
							  "\r\n      //  {");
			}

			while ( pszSwitch )
			{
				rvecSwitches.push_back( pszSwitch );

				strSwitches += string( pszDelim ) + string( pszSwitch );
				strCase += _T("\r\n      //    case \"") + string( pszSwitch ) + _T("\":"
							  "\r\n      //      break;");

				_tcscpy( pszDelim, _T(", ") );
				pszSwitch = _tcstok( NULL, szDELIMITERS );
			}
			strSwitches += _T("\");");

			strCase += _T("\r\n      //    default:"
						  "\r\n      //      break;"
						  "\r\n      //  }"
						  "\r\n      //}");

			strSwitches += strCase;
		}
		::SysFreeString( bstr );
	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::GetSwitches"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return strSwitches;
}

string CDQSDWizardDlg::GetExamples( vector< string >& rvecSwitches )
{
	string strExamples = _T("");

	BOOL bIncludeDescSwitches = CWindow( GetDlgItem( IDC_DescSwitches ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED;
	BOOL bIncludeDescExamples = CWindow( GetDlgItem( IDC_DescExamples ) ).SendMessage( BM_GETCHECK, 0, 0 ) == BST_CHECKED;

	if ( bIncludeDescSwitches && rvecSwitches.size() > 0 )
	{
		strExamples =  _T("\r\n    <div class=\"helpboxDescLabels\">Switches:</div>"
						  "\r\n    <table class=\"helpboxDescTable\">");
		
		vector< string >::iterator it = rvecSwitches.begin();
		for ( ; it != rvecSwitches.end(); it++ )
		{
			strExamples += 
					   _T("\r\n      <tr><td>/") + *it + _T("</td><td> - </td><td></td></tr>");
		}
		strExamples += _T("\r\n    </table>");
	}

	if ( bIncludeDescExamples )
	{
		strExamples += _T("\r\n    <div class=\"helpboxDescLabels\">Examples:</div>"
						  "\r\n    <table class=\"helpboxDescTable\">"
						  "\r\n      <tr><td></td></tr>"
						  "\r\n      <tr><td></td></tr>"
						  "\r\n    </table>");
	}
	return strExamples;
}

string CDQSDWizardDlg::EscapeXML( string& xml )
{
	try
	{
		srch_repl( xml, _T("&"), _T("&amp;") );
		srch_repl( xml, _T("<"), _T("&lt;") );
		srch_repl( xml, _T(">"), _T("&gt;") );
	//	srch_repl( xml, _T("'"), _T("&apos;") );
	//	srch_repl( xml, _T("\""), _T("&quot;") );
	//	srch_repl( xml, _T("\r\n"), _T("\\n") );
	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::EscapeXML"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}

	return xml;
}

void CDQSDWizardDlg::SaveFields()
{
	try
	{
		CRegKey rk;
		rk.Create(HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings"));

        SetRegValue(rk, "Contributor", GetDlgItemText(IDC_Contributor));
        SetRegValue(rk, "Email", GetDlgItemText(IDC_Email));
        SetRegValue(rk, "MutuallyExclusiveSwitches", GetDlgItemCheck(IDC_MutuallyExclusiveSwitches) == BST_CHECKED);
        SetRegValue(rk, "IncludeDescriptionSwitches", GetDlgItemCheck(IDC_DescSwitches) == BST_CHECKED);
        SetRegValue(rk, "IncludeDescriptionExamples", GetDlgItemCheck(IDC_DescExamples) == BST_CHECKED);
        SetRegValue(rk, "ShowTips", GetDlgItemCheck(IDC_ShowTips) == BST_CHECKED);

		rk.Close();
	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::SaveFields"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}
}

void CDQSDWizardDlg::RestoreFields()
{
	try
	{
		CRegKey rk;
		if (ERROR_SUCCESS == rk.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Dave's Quick Search Deskbar\\DQSDSearchWizard\\Settings")))
		{
            string contributor;
            if (ERROR_SUCCESS == QueryRegValue(rk, _T("Contributor"), contributor))
                SetDlgItemText(IDC_Contributor, contributor.c_str());

            string email;
            if (ERROR_SUCCESS == QueryRegValue(rk, _T("Email"), email))
                SetDlgItemText(IDC_Email, email.c_str());

            // TODO: this is probably a bug, should check MutuallyExclusiveSwitches.
            bool includeComments = true;
            QueryRegValue(rk, "IncludeComments", includeComments);
            SetDlgItemCheck(IDC_MutuallyExclusiveSwitches, includeComments);

            bool includeDescriptiveSwitches = true;
            QueryRegValue(rk, "IncludeDescriptionSwitches", includeDescriptiveSwitches);
            SetDlgItemCheck(IDC_DescSwitches, includeDescriptiveSwitches);

            bool includeDescriptionExamples = true;
            QueryRegValue(rk, "IncludeDescriptionExamples", includeDescriptionExamples);
            SetDlgItemCheck(IDC_DescExamples, includeDescriptionExamples);

            bool showTips = false;
            QueryRegValue(rk, "ShowTips", showTips);
            SetDlgItemCheck(IDC_ShowTips, showTips);

            m_tip.Activate(showTips);
		}
		else
		{
			rk.Open(HKEY_CURRENT_USER, _T("Software\\Microsoft\\MS Setup (ACME)\\User Info"), KEY_READ);

            string contributor;
            if (ERROR_SUCCESS == QueryRegValue(rk, _T("DefName"), contributor))
                SetDlgItemText(IDC_Contributor, contributor.c_str());
		}
	}
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::RestoreFields"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
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
	try
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
	catch ( ... )
	{
		MessageBox( _T("An internal error was encountered:  CDQSDWizardDlg::GetScriptFieldName"), _T("DQSD Search Wizard Exception"), MB_OK|MB_ICONERROR );
	}
	return _T("");
}

LRESULT CDQSDWizardDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ( m_spSelectedStyle )
	{
		m_spSelectedStyle->put_cssText( m_bstrUnselectedStyle );
		m_spSelectedStyle = NULL;
	}

	return 0;
}

LRESULT CDQSDWizardDlg::OnClickedShowHideHTML(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	RECT rcDialog;
	GetWindowRect( &rcDialog );

	if ( m_nTallDialogHeight == ( rcDialog.bottom - rcDialog.top ) )
	{
		ShrinkDialog();
	}
	else
	{
		GrowDialog();
	}
	return 0;
}

void CDQSDWizardDlg::ShrinkDialog()
{
	RECT rcDialog;
	GetWindowRect( &rcDialog );

	RECT rcHTML;
	CWindow( GetDlgItem( IDC_FormFields ) ).GetWindowRect( &rcHTML );
	CWindow( GetDlgItem( IDC_ShowHideHTML ) ).SetWindowText( _T("&HTML >>") );

	rcDialog.bottom -= ( rcHTML.bottom - rcHTML.top + 7 );

	SetWindowPos( HWND_NOTOPMOST, &rcDialog, 0 );
}

void CDQSDWizardDlg::GrowDialog()
{
	RECT rcDialog;
	GetWindowRect( &rcDialog );

	CWindow( GetDlgItem( IDC_ShowHideHTML ) ).SetWindowText( _T("<< &HTML") );

	rcDialog.bottom = ( rcDialog.top + m_nTallDialogHeight );

	SetWindowPos( HWND_NOTOPMOST, &rcDialog, 0 );
}

LRESULT CDQSDWizardDlg::OnChangeSwitches(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	TCHAR szSwitches[ 8 ];
	::GetWindowText( GetDlgItem( IDC_Switches ), szSwitches, LENGTHOF( szSwitches ) - 1 );

	CWindow( GetDlgItem( IDC_MutuallyExclusiveSwitches ) ).EnableWindow( _tcslen( szSwitches ) > 0 );
	CWindow( GetDlgItem( IDC_DescSwitches ) ).EnableWindow( _tcslen( szSwitches ) > 0 );

	return 0;
}

string CDQSDWizardDlg::ParseCategory( LPCTSTR sz )
{
	string XMLCategory( _T("") );
	LPTSTR szRawCategories = (LPTSTR)_alloca( _tcslen(sz) * sizeof(TCHAR) + 2 );
	_tcscpy( szRawCategories, sz );
	LPTSTR pszCategory = _tcstok( szRawCategories, _T("/") );
	int cCategories = 0;
	while ( pszCategory )
	{
		XMLCategory.append( EscapeXML( string( pszCategory ) ) );
		pszCategory = _tcstok( NULL, _T("/") );
		cCategories += 1;
		if ( pszCategory )
			XMLCategory.append( _T("<category>") );
	}
	for ( ; cCategories > 1; cCategories-- )
		XMLCategory.append( _T("</category>") );

	return XMLCategory;
}

IHTMLElement* CDQSDWizardDlg::GetLParamHTMLElement(CWindow &formListControl, int index)
{
    LVITEM lvi = {0};
    lvi.mask = LVIF_PARAM;
    lvi.iItem = index;
    formListControl.SendMessage(LVM_GETITEM, index, (LPARAM)&lvi);
    IHTMLElement* pForm = reinterpret_cast<IHTMLElement*>(lvi.lParam);

    return pForm;
}

string CDQSDWizardDlg::CreateFormName(int formIndex, const string& searchName)
{
    char szFormNum[32] = {0};
    if (formIndex > 0)
    {
        sprintf(szFormNum, "%d", formIndex + 1);
    }

    return (searchName + "f" + string(szFormNum));
}

auto_ptr<Form> CDQSDWizardDlg::BuildForm(IHTMLElement* pElement)
{
    HtmlElement element(pElement);

    string name, method, action;
    element.GetAttribute(L"name", name);
    element.GetAttribute(L"method", method);
    element.GetAttribute(L"action", action);

    return auto_ptr<Form>(new Form(name, method, action));
}

LRESULT CDQSDWizardDlg::GetDlgItemCheck(UINT idCtrl)
{
    return GetDlgItem(idCtrl).SendMessage(BM_GETCHECK, 0, 0);
}

std::string CDQSDWizardDlg::GetDlgItemText(UINT idCtrl)
{
    CComBSTR text;
    GetDlgItem(idCtrl).GetWindowText(&text);

    return std::string(CW2A(text));
}

void CDQSDWizardDlg::SetDlgItemCheck(UINT idCtrl, WORD checkState)
{
    GetDlgItem(idCtrl).SendMessage(BM_SETCHECK, checkState);
}

void CDQSDWizardDlg::SetDlgItemCheck(UINT idCtrl, bool checkState)
{
    GetDlgItem(idCtrl).SendMessage(BM_SETCHECK, checkState ? BST_CHECKED : BST_UNCHECKED);
}

void CDQSDWizardDlg::AddToFormList(const string& name, const string& action, IHTMLElement* pForm, bool checkState)
{
    CWindow formListControl = GetDlgItem(IDC_FormList);

    // Add the form information to the list
    LVITEM lvi = {0};
    lvi.mask = LVIF_TEXT|LVIF_PARAM;
    lvi.iItem = 256;
    lvi.pszText = const_cast<LPTSTR>(name.c_str());
    lvi.lParam = reinterpret_cast<LPARAM>(pForm);
    int iPos = formListControl.SendMessage( LVM_INSERTITEM, 0, (LPARAM)&lvi );

    // AddRef the HTMLElement pointer to keep it alive as long as the
    // LPARAM is connected to the list item. OnFormListItemDeleted
    // does the corresponding Release
    pForm->AddRef();

    lvi.mask = LVIF_TEXT;
    lvi.iItem = iPos;
    lvi.iSubItem = 1;
    lvi.pszText = const_cast<LPTSTR>(action.c_str());
    formListControl.SendMessage( LVM_SETITEM, iPos, (LPARAM)&lvi );

    ListView_SetCheckState(formListControl.m_hWnd, iPos, checkState);
}