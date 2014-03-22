// WinDB.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WinDB.h"





//////////////////////////////////////////////////////////////////////////
//
//	class CWinDB
//
//////////////////////////////////////////////////////////////////////////
WINDB_API CWinDB::CWinDB(void)
{
	Reset(true);
	JetInit(m_inst);
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
