#include "stdafx.h"
#include "WinDBInst.h"
#include "WinDBSession.h"
//////////////////////////////////////////////////////////////////////////
//
//	CWinDBSession
//
//////////////////////////////////////////////////////////////////////////
CWinDBSession* CWinDBInst::NewSession(LPCTSTR lpszUser /*= nullptr*/, LPCTSTR lpszPwd /*= nullptr*/)
{
	CWinDBSession* lpSes = new CWinDBSession;
	JET_ERR err = JetBeginSession(m_inst, *lpSes, lpszUser, lpszPwd);
	if (JET_errSuccess != err)	throw CWinDBErr(err);
	return lpSes;
}