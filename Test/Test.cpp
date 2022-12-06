// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "../WinDB/WinDB.h"
#pragma comment(lib, "../Debug/WinDB.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	try{

		auto lpTable = CWinDBUtil::Inst().NewTable(
			L"Users",
			{
				{ L"ID",	WinDBColType::typeInt },
				{ L"Name",	WinDBColType::typeStr },
				{ L"Sex",	WinDBColType::typeInt },
				{ L"Age",	WinDBColType::typeInt }
			}
		);

	} catch(CWinDBErr& e){
		_tprintf(e);
	}

	return 0;
}

