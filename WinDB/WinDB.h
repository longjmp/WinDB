#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WINDB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WINDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
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




#define C2S(x) _T(#x)

#define ERR_DEFINE(x)	err##x
#define ERRCODE(x)		WinDBErr::err##x
#define ERRSTR(x)		C2S(ERRCODE(x))

typedef enum _WinDBErrCode {
	ERR_DEFINE(OK) = 0,
	
	ERR_DEFINE(Unk),
	ERR_DEFINE(InitWinDB),
	ERR_DEFINE(CloseDB),
	ERR_DEFINE(ColCntLimit),

	ERR_DEFINE(Limit)
} WinDBErr;

const UINT32 ERR_ITEM_COUNT = (static_cast<UINT32>(WinDBErr::errLimit)) + 1;

class WINDB_API CWinDBErr {
public:
	CWinDBErr(WinDBErr errCod) {
		Reset();
		m_cod = errCod;
	}
	
	CWinDBErr(JET_ERR jerr) {
		Reset();
		m_jerr = jerr;
	}

	constexpr bool operator ==(const UINT uVal) const
	{
		return (static_cast<decltype(uVal)>(m_cod) == uVal);
	}

	operator LPCTSTR(){
		static LPCTSTR lpszErrTabl[] = 
		{
			ERRSTR(Ok),

			ERRSTR(Unk),
			ERRSTR(InitWinDB),
			ERRSTR(CloseDB),
			ERRSTR(ColCntLimit),

			ERRSTR(Limit)
		};

		static_assert(_countof(lpszErrTabl) == ERR_ITEM_COUNT, "Check error code-string table..\n");
		
		UINT32 u32Idx = static_cast<UINT32>(m_cod);
	
		if (JET_errSuccess != m_jerr)
		{
			ASSERT(u32Idx > ERRCODE(OK));
			ASSERT(u32Idx < ERRCODE(Limit));
		}
		else
		{
			ASSERT(u32Idx == ERRCODE(OK));
		}

		return lpszErrTabl[u32Idx < ERRCODE(Limit) ? u32Idx : ERRCODE(Limit)];
	}

	operator JET_ERR() { return m_jerr;	}

	virtual ~CWinDBErr() { Reset();}

private:
	void Reset() {
		m_jerr = 0;
		m_cod = ERRCODE(OK);
	}

	JET_ERR m_jerr;
	WinDBErr m_cod;
};

#define CHK_THROW(x, err) \
		do{ if (x != ERR_DEFINE(OK)) \
				throw CWinDBErr(ERR_DEFINE(err));\
		}while (false)

#define THROW(err) do{ throw CWinDBErr(ERR_DEFINE(err));} while(false)


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

	void DeleteSelf() {
		//JetCreateTable
	}

	operator auto () {
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

	~CWinDBRow() {
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
