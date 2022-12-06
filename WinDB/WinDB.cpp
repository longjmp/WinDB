// WinDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WinDB.h"
#include "WinDBInst.h"
#include "WinDBDatabase.h"
#include "WinDBSession.h"

//////////////////////////////////////////////////////////////////////////
//
//	class CWinDB
//
//////////////////////////////////////////////////////////////////////////
CWinDB::CWinDB()
{
	Reset(true);
}

CWinDB::~CWinDB(void)
{
	Reset(false);
}

CWinDBDatabase* CWinDB::PrepareDB(std::wstring& strInst, std::wstring& strSess, std::wstring& strDb)
//LPCTSTR lpszInst, LPCTSTR lpszSess, LPCTSTR lpszDB)
{
	auto itInst = PrepareInst(strInst.c_str());
	auto itSess = itInst->PrepareSession(strSess.c_str());

	return itSess->NewDatabase();
}

DbInst CWinDB::PrepareInst(LPCTSTR lpszInst) {
	auto it = m_insts.find(lpszInst);
	if (it == m_insts.end())
	{
		DbInsts::value_type item(
			lpszInst, 
			std::move(
				std::make_shared<CWinDBInst>(lpszInst)
			)
		);

		JET_ERR err = JetInit(*item.second);
		if (err != JET_errSuccess)
		{
			throw CWinDBErr(err);
		}

		m_insts.insert(item);
	}

	return m_insts[lpszInst];
}

bool CWinDB::Reset(bool bFirstTime /*= false*/)
{
	bool bRet = false;
	if (!bFirstTime)
	{
		//m_insts.
	}

	return bRet;
}




