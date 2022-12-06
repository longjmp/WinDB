#include "stdafx.h"
#include "WinDBUtil.h"
#include "WinDBDatabase.h"

/*
*
* Function:
*	CWinDBTabl* NewTable(LPCTSTR lpszName, ColDefList colDef)
*
* Purpose: Create a table.
*
* Parameters:
*	LPCTSTR lpszName: Name of the table(And|or names of the instance/session/db) which will be created.
*	if lpszName doesn't include a dot (e.g. "Users"),
*	a table will be created in Default_Instance.Default_Session.Default_DB;
*	and if lpszName do, "Instance.Session.DB.Table" will be created, instance/session/db will be created too, if necessary.
*	e.g.:
*		"MySess.BizDb.Users": Creates Table("Users") in default_instance.MySess.BizDb,
*							  also creates Session("MySess") and DB("BizDB");
*		"BizDb.Users": Creates Table("Users") in default_instance.default_session.BizDb,
*					   also creates database("BizDB") in default_instance.default_session.
*	ColDefList colDef: table columns define
*
*
*/

CWinDBTabl* CWinDBUtil::NewTable(LPCTSTR lpszName, ColDefList colDef)
{
	std::wstring strInst,
		strSess,
		strDb,
		strTabl;

	ParseTableName(lpszName, strInst, strSess, strDb, strTabl);

	if (m_db == nullptr)
	{
		m_db = std::make_unique<CWinDB>();
	}

	CWinDBDatabase* lpDB = m_db->PrepareDB(strInst, strSess, strDb);
	ASSERT(lpDB);


	if (colDef.size() > 1024 * 8)
	{
		THROW(ColCntLimit);
	}

	return lpDB->CreateTable(lpszName, colDef);
}

void CWinDBUtil::ParseTableName(LPCTSTR lpszName, std::wstring& strInst, std::wstring& strSess, std::wstring& strDb, std::wstring& strTabl)
{
	if (nullptr == lpszName || nullptr == std::wcschr(lpszName, L'.'))
	{
		strTabl = lpszName;
		return;
	}
	else
	{
		// handle forms like "xx.xx.xx.xx", not yet implemented
		LPCWSTR lpszDot = std::wcsrchr(lpszName, L'.');
		strTabl = lpszDot;

		// to do£º parse 
		//strDb.assign();
	}

}
