#include "stdafx.h"
#include "WinDBSession.h"
#include "WinDBDatabase.h"

CWinDBDatabase* CWinDBSession::NewDatabase(LPCTSTR lpszDBName)
{
	CWinDBDatabase* m_lpDb = new CWinDBDatabase(this);
	//JetCreateDatabase(m_ses, lpszDBName, )
	return m_lpDb;
}
