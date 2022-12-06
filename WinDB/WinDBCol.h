#pragma once

#include <initializer_list>
#include "WinDB.h"



class WINDB_API CWinDBCol {
public:
	CWinDBCol() {
	}

	~CWinDBCol() {}



	operator CONST JET_COLUMNID() {
		return m_colId;
	}

private:
	JET_COLUMNID m_colId;
};
