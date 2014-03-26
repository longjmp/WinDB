// WinDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WinDB.h"





//////////////////////////////////////////////////////////////////////////
//
//	class CWinDB
//
//////////////////////////////////////////////////////////////////////////
WINDB_API CWinDB::CWinDB(LPCTSTR lpszDB/* = NULL*/):
	m_inst(lpszDB)
{
	Reset(true);
	JET_ERR err = JetInit(m_inst);
	if (err != JET_errSuccess){
		throw CWinDBErrExp(err);
	}

	m_lpSes = m_inst.NewSession();

}

WINDB_API CWinDB::~CWinDB( void )
{
	Reset(false);
}

bool CWinDB::Reset( bool bFirstTime /*= false*/ )
{
	bool bRet = false;
	if (! bFirstTime)
	{
		m_inst = NULL;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	CWinDBSession
//
//////////////////////////////////////////////////////////////////////////
CWinDBSession* CWinDBInst::NewSession( LPCTSTR lpszUser /*= nullptr*/, LPCTSTR lpszPwd /*= nullptr*/ )
{
	CWinDBSession* lpSes = new CWinDBSession;
	JET_ERR err = JetBeginSession(m_inst, *lpSes, lpszUser, lpszPwd);
	if (JET_errSuccess != err)	throw CWinDBErrExp(err);
	return lpSes;
}


//////////////////////////////////////////////////////////////////////////
//
//	CWinDBDatabase
//
//////////////////////////////////////////////////////////////////////////
CWinDBDatabase* CWinDBSession::NewDatabase( LPCTSTR lpszDBName /*= NULL*/ )
{
	CWinDBDatabase* m_lpDb = new CWinDBDatabase(*this);
	//JetCreateDatabase(m_ses, lpszDBName, )
	return m_lpDb;
}
