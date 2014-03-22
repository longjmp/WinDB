// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../WinDB/WinDB.h"
#pragma comment(lib, "../Debug/WinDB.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	try{
		CWinDB db;
		db.Reset();
		
	} catch(CWinDBErrExp& e){
		_tprintf(e);
	}

	return 0;
}

