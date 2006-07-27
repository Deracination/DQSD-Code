// OptionsDialog.cpp : Implementation of COptionsDialog

#include "stdafx.h"
#include "OptionsDialog.h"
#include "ConfigurationSettings.h"
#include "Utilities.h"


// COptionsDialog
COptionsDialog::COptionsDialog()
{
}

COptionsDialog::~COptionsDialog()
{
}


LRESULT COptionsDialog::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CAxDialogImpl<COptionsDialog>::OnInitDialog(uMsg, wParam, lParam, bHandled);

	CenterWindow();

	m_optionList.SubclassWindow(GetDlgItem(IDC_OptionList));
	m_optionList.SetExtendedListStyle(PLS_EX_XPLOOK);
	m_optionList.SetExtendedListStyle(PLS_EX_CATEGORIZED);
	m_optionList.SetExtendedListStyle(PLS_EX_SHOWSELALWAYS);


	TCHAR szOptionsFile[ _MAX_PATH ];
	GetInstallationDirectory( szOptionsFile, sizeof(szOptionsFile) );
	StrNCat( szOptionsFile, _T("\\preferences.xml"), lengthof(szOptionsFile) - _tcslen(szOptionsFile) - 1);

	CConfigurationSettings configSettings( szOptionsFile );

	CComPtr<MSXML2::IXMLDOMNodeList> spOptionNodes = configSettings.GetXmlDoc()->selectNodes( _bstr_t( _T("/configuration/option") ) );

	m_optionList.AddItem( PropCreateCategory(_T("General Options") ) );
	TCHAR szValue[ 256 ];
	for ( int iOption = 0; iOption < spOptionNodes->length; iOption++ )
	{
		CComPtr<MSXML2::IXMLDOMNode> spOptionNode = spOptionNodes->Getitem( iOption );
		CComPtr<MSXML2::IXMLDOMNamedNodeMap> spAttributes = spOptionNode->Getattributes();
		
		CComPtr<MSXML2::IXMLDOMNode> spNameAttr = spAttributes->getNamedItem( _bstr_t( _T("name") ) );
		CComPtr<MSXML2::IXMLDOMNode> spValueAttr = spAttributes->getNamedItem( _bstr_t( _T("value") ) );
		CComPtr<MSXML2::IXMLDOMNode> spTypeAttr = spAttributes->getNamedItem( _bstr_t( _T("type") ) );
		CComPtr<MSXML2::IXMLDOMNodeList> spValidValues = spOptionNode->selectNodes( _bstr_t( _T("validValues/value") ) );
		std::string sType = spTypeAttr->Gettext();
		if ( sType == std::string( _T("bool") ) )
		{
			_tcscpy( szValue, static_cast<TCHAR*>(spValueAttr->Gettext()) );
			m_optionList.AddItem( PropCreateSimple( static_cast<TCHAR*>(spNameAttr->Gettext()), _tcsicmp( szValue, _T("true") ) == 0 ) );
		}
		else
		{
			if ( spValidValues->length > 0 )
			{
				LPCTSTR* list = new LPCTSTR[ spValidValues->length ];
				try
				{
					for ( int iValue = 0; iValue < spValidValues->length; iValue++ )
					{
						CComPtr<MSXML2::IXMLDOMNode> spValidValueNode = spValidValues->Getitem( iValue );
						CComPtr<MSXML2::IXMLDOMNode> spDisplayAttr = spValidValueNode->Getattributes()->getNamedItem( _bstr_t( _T("display") ) );
						LPTSTR pszValue = new TCHAR[ 256 ];
						_tcscpy( pszValue, static_cast<TCHAR*>( spValidValueNode->Gettext() ) );
						_tcscat( pszValue, _T(" - ") );
						_tcscat( pszValue, static_cast<TCHAR*>( spDisplayAttr->Gettext() ) ); 
						list[ iValue ] = pszValue;
					}
					list[ iValue ] = NULL;
					m_optionList.AddItem( PropCreateList( static_cast<TCHAR*>( spNameAttr->Gettext() ), list, 0 ) );
				}
				catch ( ... )
				{
					// gc, how I miss thee...
					for ( int iValue = 0; list[ iValue ] != NULL; iValue++ )
						delete list[ iValue ];
					delete [] list;
				}
			}
			else
			{
				m_optionList.AddItem( PropCreateSimple( static_cast<TCHAR*>(spNameAttr->Gettext()), static_cast<TCHAR*>(spValueAttr->Gettext()) ) );
			}
		}
	}

	return 1;  // Let the system set the focus
}

LRESULT COptionsDialog::OnClickedOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT COptionsDialog::OnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
