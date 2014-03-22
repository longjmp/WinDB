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
	ERRSTR(Limit)
};


#define EXPCODE(x) err##x
#define EXPSTR(x) _T("exp")##C2S(x)

typedef enum _WinDBExpCode {
	expOk = ERRCODE(Limit),
	expLimit
} WinDBExpCod;
LPCTSTR g_winDBExps[] = {
	EXPSTR(Ok),
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
			
		}
		return m_cod < ERRCODE(Limit) ? g_winDBErrs[m_cod] : 
			(m_cod < EXPCODE(Limit) ? g_winDBExps[m_cod] : NULL);
	}

	virtual ~CWinDBErrExp() {
		;
	}

private:
	void Reset() {
		m_jerr = 0;
		m_cod = 0;
	}
	JET_ERR m_jerr;
	UINT m_cod;
};



class WINDB_API CWinDBInst {
public:
	CWinDBInst(LPCTSTR lpszName = NULL) throw (...) {
		JET_ERR err = JetCreateInstance(&m_inst, lpszName);
		if (err != JET_errSuccess){
			//throw
		}
	}

	~CWinDBInst(){}

	operator JET_INSTANCE*() { return &m_inst; }
	operator JET_INSTANCE&() { return m_inst; }

private:
	JET_INSTANCE m_inst;
	LPCTSTR lpsz;
};



class WINDB_API CWinDB {
public:
	CWinDB(void);
	virtual ~CWinDB(void);

	//
	// for WinDB user: Don't call Reset(true)
	//
	bool Reset(bool bFirstTime = false);
	
//	bool Conn(){};
//	void Disconn(){};
private:
	CWinDBInst m_inst;
};

extern WINDB_API int nWinDB;
