// Crypto.h: interface for the CCrypto class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYPTO_H__5AD5FA78_9426_11D2_B921_00002441B3CB__INCLUDED_)
#define AFX_CRYPTO_H__5AD5FA78_9426_11D2_B921_00002441B3CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winnt.h>
#include <winreg.h>

class CCrypto  
{
public:
	void SetRegistryValue(HKEY hParentKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, LPCTSTR lpszValue);
	void SetRegistryValue(LPCTSTR lpszValue);
	char * GetRegistryValue(HKEY hParentKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName);
	char * GetRegistryValue(void);
	char * Decrypt(char *lpszEncodedText);
	char * Encrypt(char * lpszClearText);
	CCrypto();
	virtual ~CCrypto();
private:
	LPCTSTR m_lpszValueKey;
	LPCTSTR m_lpszRegistryKey;
	char * Crypt(char *lpszText);
};

#endif // !defined(AFX_CRYPTO_H__5AD5FA78_9426_11D2_B921_00002441B3CB__INCLUDED_)
