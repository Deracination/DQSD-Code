#include "StdAfx.h"
#include "BandConfig.h"

namespace
{
    LPCTSTR DQSD_REG_KEY = _T("CLSID\\{EC9FE983-E520-4D8F-B1A7-ACBCA0439C70}");

    LPCTSTR DEFAULT_MESSAGETITLE = _T("Dave's Quick Search Deskbar");
    LPCTSTR DEFAULT_DESKBANDTITLE = _T("Quick Search");

    HRESULT GetDefaultInstallDirectory(LPTSTR pszBuffer, DWORD cchBuffer)
    {
        // Get module's path
        DWORD result = GetModuleFileName(_AtlBaseModule.GetModuleInstance(), pszBuffer, cchBuffer);
        if (result == 0 || result == cchBuffer)
            return HRESULT_FROM_WIN32(GetLastError());

        // Find last slash and zero out everything afterwards
        LPTSTR p = pszBuffer + result;
        while (*p != _T('\\') && p > pszBuffer)
            --p;

        *p = 0;
        
        return S_OK;
    }
}

BandConfig::~BandConfig()
{
    if (m_regkey != NULL)
        m_regkey.Close();
}

HRESULT BandConfig::GetInstallDirectory(LPTSTR pszBuffer, DWORD cchBuffer) const
{
    TCHAR szDefaultInstallDirectory[MAX_PATH] = {0};
    HRESULT hr = GetDefaultInstallDirectory(szDefaultInstallDirectory, lengthof(szDefaultInstallDirectory));
    if (FAILED(hr))
        return hr;

    return GetRegValue(_T("InstallDir"), pszBuffer, cchBuffer, szDefaultInstallDirectory);
}

HRESULT BandConfig::GetMessageTitle(LPTSTR pszBuffer, DWORD cchBuffer) const
{
    return GetRegValue(_T("HelpText"), pszBuffer, cchBuffer, DEFAULT_MESSAGETITLE);
}

HRESULT BandConfig::GetDeskbandTitle(LPTSTR pszBuffer, DWORD cchBuffer) const
{
    return GetRegValue(_T(""), pszBuffer, cchBuffer, DEFAULT_DESKBANDTITLE);
}

HRESULT BandConfig::IsAllowedURL(LPCTSTR pszURL) const
{
    CRegKey secureFilesKey(OpenConfigSubKey(_T("SecureFiles")));
    if (secureFilesKey == NULL)
        return HRESULT_FROM_WIN32(GetLastError());

    TCHAR szAllowedFilename[MAX_PATH];
    DWORD cbAllowedFilename = sizeof(szAllowedFilename);
    DWORD i = 0;
    while (ERROR_SUCCESS == RegEnumValue(secureFilesKey, i, szAllowedFilename, &cbAllowedFilename, NULL, NULL, NULL, NULL))
    {
        if (URLMatchesFilename(pszURL, szAllowedFilename))
            return S_OK;

        i++;
        cbAllowedFilename = sizeof(szAllowedFilename);
    }

    return S_FALSE;
}

// Private methods
CRegKey& BandConfig::OpenConfigKey() const
{
    if (m_regkey == NULL)
    {
        DWORD result = m_regkey.Open(HKEY_CLASSES_ROOT, DQSD_REG_KEY, KEY_READ);
        if (result != ERROR_SUCCESS)
            SetLastError(result);
    }

    return m_regkey;
}

HKEY BandConfig::OpenConfigSubKey(LPCTSTR pszKey) const
{
    CRegKey& dqsdKey = OpenConfigKey();

    CRegKey subKey;
    DWORD result = subKey.Open(dqsdKey, pszKey, KEY_READ);
    if (result != ERROR_SUCCESS)
        SetLastError(result);

    return subKey.Detach();
}

HRESULT BandConfig::GetRegValue(LPCTSTR pszValueName, LPTSTR pszBuffer, DWORD cchBuffer, LPCTSTR pszDefaultValue) const
{
    CRegKey& key = OpenConfigKey();
    if (key == NULL)
        return StringCopy(pszBuffer, cchBuffer, pszDefaultValue);

    ULONG bufferSize = cchBuffer;
    LONG result = key.QueryStringValue(pszValueName, pszBuffer, &bufferSize);
    if (result == ERROR_FILE_NOT_FOUND)
        return StringCopy(pszBuffer, cchBuffer, pszDefaultValue);

    return HRESULT_FROM_WIN32(result);
}

HRESULT BandConfig::StringCopy(LPTSTR pszDest, int cchDest, LPCTSTR pszSource) const
{
    if (lstrlen(pszSource) >= cchDest)
        return HRESULT_FROM_WIN32(ERROR_BUFFER_OVERFLOW);

    lstrcpy(pszDest, pszSource);
    return S_OK;
}
