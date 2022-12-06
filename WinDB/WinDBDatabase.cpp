#include "stdafx.h"
#include "WinDBDatabase.h"
#include "WinDB.h"
#include "WinDBSession.h"

CWinDBTabl* CWinDBDatabase::CreateTable(LPCTSTR lpszName, ColDefList colDef)
{
	ASSERT(lpszName);

	JET_TABLEID tablId = 0;
	JetCreateTable(*m_lpSess, this[0], lpszName, 0, 0, &tablId);
	return new CWinDBTabl(tablId);
}
