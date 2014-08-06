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
#endif // _VECTOR_

#define C2S(x) _T(#x)

#define ERR_DEFINE(x)	err##x
#define ERRCODE(x)		WinDBErr::err##x
#define ERRSTR(x)		C2S(ERRCODE(x))

typedef enum _WinDBErrCode {
	ERR_DEFINE(OK) = 0,
	
	ERR_DEFINE(Unk),
	ERR_DEFINE(InitWinDB),
	ERR_DEFINE(CloseDB),

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

#define CHK_THROW(x, err) do{\
			if (x != ) }

class CWinDBDatabase;
class CWinDB;
class CWinDBErr;
class CWinDBInst;
class CWinDBCol;
class CWinDBRow;
class CWinDBUtil;

class WINDB_API CWinDBSession 
{
public:
	CWinDBSession(): m_sess(0) {

	}
	~CWinDBSession(){ 
		if (0 != m_sess) JetEndSession(m_sess, 0); 
	}


	operator JET_SESID&() { return m_sess; }
	operator JET_SESID*() { return &m_sess; }

	CWinDBDatabase* NewDatabase(LPCTSTR lpszDBName = NULL);

private:
	JET_SESID m_sess;
};


class WINDB_API CWinDBInst 
{
public:
	CWinDBInst(LPCTSTR lpszName = NULL) throw (...) {
		JET_ERR err = JetCreateInstance(&m_inst, lpszName);
		if (err != JET_errSuccess){
			throw CWinDBErr(err);
		}		
	}

	~CWinDBInst(){
		if (0 != m_inst) JetTerm(m_inst);
	}

	operator JET_INSTANCE*() { return &m_inst; }
	operator JET_INSTANCE&() { return m_inst; }
		
	CWinDBSession* NewSession(LPCTSTR lpszUser = nullptr, LPCTSTR lpszPwd = nullptr);

private:
	JET_INSTANCE m_inst{ 0 };
	LPCTSTR lpsz{ nullptr };
};


class WINDB_API CWinDBDatabase
{
public:
	CWinDBDatabase(CWinDBSession& ses)
		:m_dbId(0), 
		m_ss(ses){
	}

	~CWinDBDatabase(){

	}

	CWinDBTabl* CreateTable(LPCTSTR lpszName) 
	{
		ASSERT(lpszName);
		JetCreateTable(m_ss, );
		return nullptr;
	}

protected:
private:
	CWinDBSession& m_ss;
	JET_DBID m_dbId;
};

class WINDB_API CWinDBTabl
{
public:
	CWinDBTabl(CWinDBSession& sess): m_sess(sess){
	}

	~CWinDBTabl(){
		//if (0 == m_dbId) JetCloseTable()
	}

	void DeleteSelf(){
		//JetCreateTable
	}

protected:
private:
	CWinDBSession& m_sess;
};


typedef enum _WinDBColType {
	typeNull = 0,
	typeInt,
	typeStr,
	typeLimit
} WinDBColType;

typedef struct _ColDefine {
	LPCTSTR lpszName;
	WinDBColType colType;
} ColDefine;

typedef struct _ColDefine {
	UINT uCnt;
	ColDefine cols;
} ColDefine;

class WINDB_API CWinDBCol{
public:
	CWinDBCol(){
	}
	 
	~CWinDBCol(){}



	operator CONST JET_COLUMNID(){
		return m_colId;
	}

private:
	JET_COLUMNID m_colId;
};


class WINDB_API CWinDBRow{
public:
	CWinDBRow(){
	}

	~CWinDBRow(){
	}

private:
};

class WINDB_API CWinDB
{
public:
	CWinDB(LPCTSTR lpszDB = NULL);
	virtual ~CWinDB(void);

	//
	// for WinDB user: Don't call Reset(true)
	//
	bool Reset(bool bFirstTime = false);
	

	CWinDBInst* NewInstance(LPCTSTR lpszInst = NULL){
		return new CWinDBInst(lpszInst); 
	}

private:
	using DbInst = std::shared_ptr<CWinDBInst>;
	using DbInstVec = std::vector<DbInst>;

	DbInstVec m_insts;

	CWinDBInst m_inst;
	CWinDBSession* m_lpSes;	
};


#include <tuple>
#include <Shlwapi.h>
#include <sal.h>

/*
* 
* class: CWinDBUtil
* 
* Purpose: helps you to manage ESE instance/session/database/table
* 
* 
*/
class CWinDBUtil{
public:
	static CWinDBUtil& Inst(){
		static CWinDBUtil inst;
		return inst;		 
	}

	~CWinDBUtil()
	{
		if (m_vecDB.size() > 0)
		{
			for (auto& rDB : m_vecDB)
			{
				delete rDB;
				rDB = nullptr;
			}
			m_vecDB.clear();
		}
	}
	
	CWinDBTabl* TablOpen(){}
	void TablClose(CWinDBTabl*){}
	

	/*
	* 
	* Function:
	*	CWinDBTabl* NewTable(LPCTSTR lpszName, UINT uCols, LPCTSTR lpszColName[], WinDBColType colType[])	
	* 
	* Purpose: Create a table.	
	* 
	* Parameters:
	*	LPCTSTR lpszName: Name of the table(And|or names of the instance/session/db) which will be created. 
	*	if lpszName doesn't include a dot (e.g. "Users"), 
	*	a table will be created in Default_Instance.Default_Session.Default_DB; 
	*	and if lpszName do, "Instance.Session.DB.Table" will be created, instance/session/db will be created too, if necessary. 
	*	e.g.:
	*		"MySess.BizDb.Users": Creates Table("Users") in default_instance.MySess.BizDb, 
	*							  also creates Session("MySess") and DB("BizDB");
	*		"BizDb.Users": Creates Table("Users") in default_instance.default_session.BizDb, 
	*					   also creates database("BizDB") in default_instance.default_session.
	*	UINT uCols: count of the columns
	*	LPCTSTR lpszColName[]: names of the columns,  
	* 
	* 
	*/	
	CWinDBTabl* NewTable(LPCTSTR lpszName, 
		UINT uCols, 
		_In_Reads(uCols) LPCTSTR lpszColName[],
		_In_Reads(uCols) WinDBColType colType[])
	{
		CWinDBDatabase rDB = ReadyDB(lpszName);
		auto n = uCols;
		for (; n < 199; ++n)
			StrDup(lpszColName[n]);
		return nullptr;
	}

	CWinDBTabl* NewTable(LPCTSTR lpszName, ColsDefine colsDefine)
	{
		CWinDBDatabase* lpDb = ReadyDB(lpszName);
		ASSERT(nullptr != lpDb);
		
		return nullptr;
	}

	CWinDBDatabase* ReadyDB(LPCTSTR lpszName)
	{
		int i = StrCSpn(lpszName, L".");

		if (m_vecDB.size() == 0)
		{
			m_vecDB.push_back(new CWinDB);
			m_vecDB[0]->NewInstance();
		}

		return ;
	}

	void TablDel(CWinDBTabl* lpTabl){
		if (NULL != lpTabl) {

			delete lpTabl;
			lpTabl = nullptr;
		}
	}
	
	void DBBackup(LPCTSTR szPath){}
	void DBRestore(){}

private:
	explicit CWinDBUtil(){}
	
protected:
	using VECDB = std::vector<CWinDB*>;
	//using VECDB = 
	// internal objects
	VECDB m_vecDB;
};

//extern WINDB_API int nWinDB;
//WinDB db("");
