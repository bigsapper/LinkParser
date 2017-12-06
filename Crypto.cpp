// Crypto.cpp: implementation of the CCrypto class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Crypto.h"
#include "countmacros.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrypto::CCrypto()
{
	m_lpszRegistryKey = "Software\\GoochFactory\\LinkParser\\";
	m_lpszValueKey = "Password";

	// macro to log object construction in _DEBUG mode
	CTOR_COUNT_MACRO
}

CCrypto::~CCrypto()
{
	// macro to log object destruction in _DEBUG mode
	DTOR_COUNT_MACRO
}

char * CCrypto::Encrypt(char *lpszClearText)
{
	int i, lenETxt;
	char * lpszEncryptedText = NULL;
	char * lpszHexString = new CHAR[98+1];
	strcpy(lpszHexString, "");
	char * lpszRetVal = new CHAR[100+1];
	strcpy(lpszRetVal, "");

    lpszEncryptedText = Crypt(lpszClearText);
	lenETxt = strlen(lpszEncryptedText);
	
    // When writing an encrypted password to a sequential access file like the
    // INI files, you need to convert the resultant encrypted file to hex data.
    // This is because you can end up with an encrypted password that contains
    // characters which cannot be properly read using sequential access.  So,
    // before saving your encrypted password, use this routine:
    for ( i = 0; i < lenETxt; i++ )
        sprintf(lpszHexString, "%s%2X", lpszHexString, lpszEncryptedText[i]);
	    // This will create a string like "0EF31105" or some such.  Save that to
		// the INI file.

    // Store the LENGTH of the password string as 2 bytes and concatenate
	sprintf(lpszRetVal, "%2d%s", strlen(lpszHexString), lpszHexString);

	delete lpszEncryptedText;
	delete lpszHexString;

    return lpszRetVal;
}

char * CCrypto::Decrypt(char *lpszEncodedHexString)
{
    int i, HexStringLen, DecChar;
	char * lpszRetVal = NULL;
    char * lpszEncodedText = new CHAR[49+1];
	strcpy(lpszEncodedText, "");

    char * lpszHexStringLen = new CHAR[2+1];
	strncpy(lpszHexStringLen, lpszEncodedHexString, 2);
	HexStringLen = atoi(lpszHexStringLen);
	delete lpszHexStringLen;

    // Convert the password from it's hexadecimal format
    for ( i = 2; i <= HexStringLen; i += 2 ) {
		char * stopstring = NULL;
		char lpszHexChar[4+1] = {'0','x',lpszEncodedHexString[i],lpszEncodedHexString[i+1],'\0'};
		DecChar = strtol(lpszHexChar, &stopstring, 0);
        sprintf(lpszEncodedText, "%s%c", lpszEncodedText, DecChar);
    }

    // lpszEncodedText would then contain the encrypted string, which you can now
    // decrypt.
	lpszRetVal = Crypt(lpszEncodedText);

	delete lpszEncodedText;

	return lpszRetVal;
}

char * CCrypto::Crypt(char *lpszText)
{
	const char *ENCRYPTION_KEY = "SAPPER";
	int a, b, i, lenEKey, lenText;
	char * lpszNewText = NULL;
    
	lenEKey = strlen(ENCRYPTION_KEY);
	lenText = strlen(lpszText);
	lpszNewText = new CHAR[lenText+1];

    strcpy(lpszNewText, lpszText);
    
    a = 0;
	for ( i = 0; i < lenText; i++ ) {
        b = ENCRYPTION_KEY[a];
		if ( ++a >= lenEKey )
			a = 0;

        lpszNewText[i] = lpszText[i] ^ b;
	}
    
    return lpszNewText;
}

char * CCrypto::GetRegistryValue(HKEY hParentKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName)
{
    CRegKey oRegKey;

    LPTSTR lpszRetVal = new CHAR[255+1];
	memset(lpszRetVal, 0, sizeof(lpszRetVal));
	ULONG lRetValLength = sizeof(lpszRetVal);

    if ( oRegKey.Create(hParentKey, lpszKeyName) == ERROR_SUCCESS ) {
		oRegKey.QueryValue(lpszRetVal, lpszValueName, &lRetValLength);
		oRegKey.Close()     ;
    }
        
    return lpszRetVal;
}

char * CCrypto::GetRegistryValue(void)
{
    CRegKey oRegKey;

    LPTSTR lpszRetVal = new CHAR[255+1];
	memset(lpszRetVal, 0, sizeof(lpszRetVal));
	ULONG lRetValLength = sizeof(lpszRetVal);

    if ( oRegKey.Create(HKEY_LOCAL_MACHINE, this->m_lpszRegistryKey) == ERROR_SUCCESS ) {
		oRegKey.QueryValue(lpszRetVal, this->m_lpszValueKey, &lRetValLength);
		oRegKey.Close()     ;
    }
        
    return lpszRetVal;
}

void CCrypto::SetRegistryValue(HKEY hParentKey, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, LPCTSTR lpszValue)
{
	CRegKey oRegKey;

    if ( oRegKey.Create(hParentKey, lpszKeyName) == ERROR_SUCCESS ) {
		oRegKey.SetValue(lpszValue, lpszValueName); 
		oRegKey.Close();
	}
}

void CCrypto::SetRegistryValue(LPCTSTR lpszValue)
{
	CRegKey oRegKey;

    if ( oRegKey.Create(HKEY_LOCAL_MACHINE, this->m_lpszRegistryKey) == ERROR_SUCCESS ) {
		oRegKey.SetValue(lpszValue, this->m_lpszValueKey); 
		oRegKey.Close();
	}
}
