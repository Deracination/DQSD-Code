
#ifndef __UTILITIES_B16F5720_3657_43ed_8D07_49F9347F85BA__
#define __UTILITIES_B16F5720_3657_43ed_8D07_49F9347F85BA__

#include <wininet.h>



INTERNET_SCHEME GetScheme(LPCTSTR szURL)
{
  TCHAR						buf[32];
  URL_COMPONENTS	uc;
  ZeroMemory(&uc, sizeof uc);

  uc.dwStructSize = sizeof uc;
  uc.lpszScheme = buf;
  uc.dwSchemeLength = sizeof buf;

  if (InternetCrackUrl(szURL, _tcsclen(szURL), ICU_DECODE, &uc))
     return uc.nScheme;
  else
     return INTERNET_SCHEME_UNKNOWN;
}


#endif // __UTILITIES_B16F5720_3657_43ed_8D07_49F9347F85BA__