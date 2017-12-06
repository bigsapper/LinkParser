// DictionaryDB.H : Declaration of the CDictionaryDB class

#ifndef __DICTIONARYDB_H_
#define __DICTIONARYDB_H_

#include "countmacros.h"
#include "Crypto.h"
#include "msjetoledb.h"		// MSJETOLEDB include files

class CDictionaryDBAccessor
{
public:
	LONG m_SeqNo;
	TCHAR m_StringData[256];

BEGIN_COLUMN_MAP(CDictionaryDBAccessor)
	COLUMN_ENTRY(1, m_SeqNo)
	COLUMN_ENTRY(2, m_StringData)
END_COLUMN_MAP()


	// You may wish to call this function if you are inserting a record and wish to
	// initialize all the fields, if you are not going to explicitly set all of them.
	void ClearRecord()
	{
		memset(this, 0, sizeof(*this));
	}
};

class CDictionaryDB : public CTable<CAccessor<CDictionaryDBAccessor> >
{
public:
	CDictionaryDB()
	{
		// macro to log object construction in _DEBUG mode
		CTOR_COUNT_MACRO
	}
	~CDictionaryDB()
	{
		// macro to log object destruction in _DEBUG mode
		DTOR_COUNT_MACRO
	}
	HRESULT Open(char *TableName)
	{
		HRESULT		hr;

		hr = OpenDataSource();
		if (FAILED(hr))
			return hr;

		return OpenRowset(TableName);
	}
	HRESULT OpenDataSource()
	{
		CCrypto oCryptTool;

		// get the encoded password from the registry
		char * lpszPassword;
		lpszPassword = oCryptTool.GetRegistryValue();
		if ( strlen(lpszPassword) == 0 ) {
			// prompt the user for the password
			strcpy(lpszPassword, "sailormoon");
			// set the password to the Registry
			oCryptTool.SetRegistryValue(oCryptTool.Encrypt(lpszPassword));
		} else {
			lpszPassword = oCryptTool.Decrypt(lpszPassword);
		}

		HRESULT		hr;
		CDataSource db;
		CDBPropSet	dbinit[2] = {DBPROPSET_DBINIT, DBPROPSET_JETOLEDB_DBINIT};

		dbinit[0].AddProperty(DBPROP_AUTH_CACHE_AUTHINFO, true);
		dbinit[0].AddProperty(DBPROP_AUTH_ENCRYPT_PASSWORD, false);
		dbinit[0].AddProperty(DBPROP_AUTH_MASK_PASSWORD, false);
		dbinit[0].AddProperty(DBPROP_AUTH_PASSWORD, OLESTR(""));
		dbinit[0].AddProperty(DBPROP_AUTH_PERSIST_ENCRYPTED, false);
		dbinit[0].AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
		dbinit[0].AddProperty(DBPROP_AUTH_USERID, OLESTR("Admin"));
		dbinit[0].AddProperty(DBPROP_INIT_DATASOURCE, OLESTR("Dictionary.mdb"));
		dbinit[0].AddProperty(DBPROP_INIT_MODE, (long)8);
		dbinit[0].AddProperty(DBPROP_INIT_PROMPT, (short)4);
		dbinit[0].AddProperty(DBPROP_INIT_PROVIDERSTRING, OLESTR(";COUNTRY=0;CP=1252;LANGID=0x0409"));
		dbinit[0].AddProperty(DBPROP_INIT_LCID, (long)1033);

		dbinit[1].AddProperty(DBPROP_JETOLEDB_DATABASEPASSWORD, lpszPassword);
		hr = db.Open(CLSID_JETOLEDB_3_51, dbinit, 2);

		delete lpszPassword;

		if (FAILED(hr))
			return hr;

		return m_session.Open(db);
	}
	HRESULT OpenRowset(char *TableName)
	{
		return CTable<CAccessor<CDictionaryDBAccessor> >::Open(m_session, _T(TableName));
	}
	CSession	m_session;
};

#endif // __DICTIONARYDB_H_
