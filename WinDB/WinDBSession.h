#pragma once

#include "WinDB.h"

class WINDB_API CWinDBSession
{
public:
	CWinDBSession() : m_sess(0) {

	}
	virtual ~CWinDBSession() {
		if (0 != m_sess) JetEndSession(m_sess, 0);
	}


	operator JET_SESID& () { return m_sess; }
	operator JET_SESID* () { return &m_sess; }

	CWinDBDatabase* NewDatabase(LPCTSTR lpszDBName = NULL);


private:
	JET_SESID m_sess;
};
