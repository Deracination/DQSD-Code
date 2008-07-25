#pragma once

// This class encapsulates registry access for DQSD's configuration
class BandConfig
{
public:
    ~BandConfig();

    HRESULT GetInstallDirectory(LPTSTR pszBuffer, DWORD cchBuffer) const;
    HRESULT GetMessageTitle(LPTSTR pszBuffer, DWORD cchBuffer) const;
    HRESULT GetDeskbandTitle(LPTSTR pszBuffer, DWORD cchBuffer) const;
    HRESULT IsAllowedURL(LPCTSTR pszURL) const;

private:
    HRESULT GetRegValue(LPCTSTR valueName, LPTSTR pszBuffer, DWORD cchBuffer, LPCTSTR pszDefaultValue) const;
    HRESULT StringCopy(LPTSTR pszDest, int cchDest, LPCTSTR pszSource) const;

    CRegKey& OpenConfigKey() const;
    HKEY OpenConfigSubKey(LPCTSTR pszKey) const;

    mutable CRegKey m_regkey;
};