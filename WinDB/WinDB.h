#pragma once

#ifdef WINDB_EXPORTS
#define WINDB_API __declspec(dllexport)
#else
#define WINDB_API __declspec(dllimport)
#endif

#if (defined(UNICODE) || defined(_UNICODE))
#define JET_UNICODE
#endif

#include <esent.h>
#pragma comment (lib, "esent.lib")

#ifndef _EXCEPTION_
	#include <exception>
#endif

#ifndef LPCTSTR
	#include <windows.h>
	#include <tchar.h>	
#endif // LPCTSTR

#ifndef ASSERT
	#include <assert.h>
	#define ASSERT assert
#endif // ASSERT

#ifndef _VECTOR_
	#include <vector>
	#include <algorithm>
	#include <memory>
	#include <map>
	#include <string>
#endif // _VECTOR_


#include <tuple>
#include <cstring>
#include <shellapi.h>
#include <sal.h>


#include "WinDBErr.h"



class CWinDBDatabase;
class CWinDB;
class CWinDBErr;
class CWinDBInst;
class CWinDBCol;
class CWinDBRow;
class CWinDBUtil;
class CWinDBTabl;



typedef enum _WinDBColType {
	typeNull = 0,
	typeInt,
	typeStr,
	typeLimit
} WinDBColType;

typedef struct _ColDefine {
	LPCTSTR lpszName;		// names of the column
	WinDBColType colType;	// data type of the column
} ColDefine;

using ColDefList = std::initializer_list<ColDefine>;


class WINDB_API CWinDBTabl
{
public:
	CWinDBTabl(JET_TABLEID id) {
	}

	~CWinDBTabl() {
		//if (0 == m_dbId) JetCloseTable()
	}

	void Add() {}

	void Remove() {
		//JetCreateTable
	}

	constexpr operator auto () const {
		return m_tablId;
	}

	operator auto& () {
		return m_tablId;
	}

protected:
private:
	JET_TABLEID m_tablId;
};



class WINDB_API CWinDBRow {
public:
	CWinDBRow() {
	}

	virtual ~CWinDBRow() override{
	}

private:
};




using DbInst = std::shared_ptr<CWinDBInst>;
using DbInsts = std::map<LPCTSTR, DbInst>;

class CWinDB
{
public:	
	CWinDB();
	virtual ~CWinDB(void);

	
	CWinDBDatabase* PrepareDB(std::wstring& strInst,
		std::wstring& strSess,
		std::wstring& strDb);	

private:
	DbInst PrepareInst(LPCTSTR lpszInst);

	//
	// for WinDB user: Don't call Reset(true)
	//
	bool Reset(bool bFirstTime = false);

	DbInsts m_insts;
};



#include "WinDBUtil.h"
//extern WINDB_API int nWinDB;
//WinDB db("");
