#pragma once

#include "WinDBCol.h"

class CWinDBSession;
class CWinDBTabl;

class CWinDBDatabase
{
public:
	CWinDBDatabase(CWinDBSession* lpSs = nullptr)
		:m_dbId(0)
		,m_lpSess(lpSs)
	{
	}

	~CWinDBDatabase() {

	}

	CWinDBTabl* CreateTable(LPCTSTR lpszName, ColDefList colDef);

	operator JET_DBID() {
		return m_dbId;
	}

protected:
private:
	CWinDBSession* m_lpSess;
	JET_DBID m_dbId;
};

