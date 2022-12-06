#pragma once

#include "WinDBCol.h"
#include "WinDB.h"

using DB = std::unique_ptr<CWinDB>;
//using ColDefList = std::initializer_list<ColDefine>;
/*
*
* class: CWinDBUtil
*
* Purpose: helps you to manage ESE instance/session/database/table
*
*
*/
class WINDB_API CWinDBUtil {
public:
	static CWinDBUtil& Inst() {
		static CWinDBUtil inst;
		return inst;
	}

	~CWinDBUtil()
	{
		/*if (m_db.size() > 0)
		{
			for (auto& rDB : m_db)
			{
				delete rDB;
				rDB = nullptr;
			}
			m_db.clear();
		}*/
	}


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
	CWinDBTabl* NewTable(LPCTSTR lpszName, ColDefList colDef);

	void ParseTableName(LPCTSTR lpszName,
		std::wstring& strInst,
		std::wstring& strSess,
		std::wstring& strDb,
		std::wstring& strTabl);

	void TablDel(CWinDBTabl* lpTabl) {
		if (NULL != lpTabl) {

			delete lpTabl;
			lpTabl = nullptr;
		}
	}

	void DBBackup(LPCTSTR szPath) {}
	void DBRestore() {}

private:
	explicit CWinDBUtil() {}

protected:
	DB m_db;
};