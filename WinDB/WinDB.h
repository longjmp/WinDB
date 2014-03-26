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
#endif


#define C2S(x) _T(#x)

#define ERRCODE(x) err##x
#define ERRSTR(x) _T("err")##C2S(x)

typedef enum _WinDBErrCode {
	ERRCODE(OK) = 0,
	ERRCODE(Unk),
	ERRCODE(Limit)
} WinDBErrCode;
LPCTSTR g_winDBErrs[] = {
	ERRSTR(Ok),
	ERRSTR(Unk),
	ERRSTR(Limit)
};


#define EXPCODE(x) exp##x
#define EXPSTR(x) _T("exp")##C2S(x)

typedef enum _WinDBExpCode {
	expOk = ERRCODE(Limit),
	EXPCODE(Unk),
	EXPCODE(InitWinDB),
	EXPCODE(Limit)
} WinDBExpCod;
LPCTSTR g_winDBExps[] = {
	EXPSTR(Ok),
	EXPSTR(Unk),
	EXPSTR(InitWinDB),
	EXPSTR(Limit)
};


class WINDB_API CWinDBErrExp {
public:
	CWinDBErrExp(WinDBExpCod expCod) {
		Reset();
		m_cod = expCod;
	}

	CWinDBErrExp(WinDBErrCode errCod) {
		Reset();
		m_cod = errCod;
	}
	
	CWinDBErrExp(JET_ERR jerr) {
		Reset();
		m_jerr = jerr;
	}

	operator LPCTSTR(){
		if (JET_errSuccess != m_jerr)
		{
			static LPCTSTR lpszNull = L"";
			return lpszNull;
		}
		return m_cod < ERRCODE(Limit) ? g_winDBErrs[m_cod] : 
			(m_cod < EXPCODE(Limit) ? g_winDBExps[m_cod] : NULL);
	}

	operator JET_ERR() { return m_jerr;	}

	virtual ~CWinDBErrExp() { Reset();}

private:
	void Reset() {
		m_jerr = 0;
		m_cod = 0;
	}

	JET_ERR m_jerr;
	UINT m_cod;
};

class CWinDBDatabase;
class CWinDB;
class CWinDBErrExp;
class CWinDBInst;


class WINDB_API CWinDBSession {
public:
	CWinDBSession():
		m_ses(0) {

	}
	~CWinDBSession(){ 
		if (0 != m_ses) JetEndSession(m_ses, 0); 
	}


	operator JET_SESID&() { return m_ses; }
	operator JET_SESID*() { return &m_ses; }

	CWinDBDatabase* NewDatabase(LPCTSTR lpszDBName = NULL);

private:
	JET_SESID m_ses;
};


class WINDB_API CWinDBInst {
public:
	CWinDBInst(LPCTSTR lpszName = NULL) throw (...) {
		JET_ERR err = JetCreateInstance(&m_inst, lpszName);
		if (err != JET_errSuccess){
			throw CWinDBErrExp(err);
		}		
	}

	~CWinDBInst(){
		if (0 != m_inst) JetTerm(m_inst);
	}

	operator JET_INSTANCE*() { return &m_inst; }
	operator JET_INSTANCE&() { return m_inst; }
		
	CWinDBSession* NewSession(LPCTSTR lpszUser = nullptr, LPCTSTR lpszPwd = nullptr);

private:
	JET_INSTANCE m_inst;
	LPCTSTR lpsz;
};


class CWinDBDatabase
{
public:
	CWinDBDatabase(CWinDBSession& ses)
		:m_dbId(0), 
		m_ses(ses){
	}

	~CWinDBDatabase(){

	}

protected:
private:
	CWinDBSession& m_ses;
	JET_DBID m_dbId;
};

class CWinDBTabl
{
public:
	CWinDBTabl(){}
	~CWinDBTabl(){
		//if (0 == m_dbId) JetCloseTable()
	}

	void DeleteSelf(){
		//JetCreateTable
	}
protected:
private:
	
};

class CWinDBTabl
{
public:
protected:
private:
};


class WINDB_API CWinDB {
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

	CWinDBTabl* NewTable(){
		return NULL;
	}

	void DelTable(CWinDBTabl* lpTabl){
		if (NULL != lpTabl) {
			
			delete lpTabl;
		}
	}

private:
	CWinDBInst m_inst;
	CWinDBSession* m_lpSes;	
};

extern WINDB_API int nWinDB;


class CWinDBQuickTabl{
public:
	 CWinDBTabl* Open();
	 void Close(CWinDBTabl*);
	 CWinDBTabl* New();
	 void Del(CWinDBTabl*);
};