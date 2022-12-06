#pragma once

#include "WinDB.h"

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

	operator LPCTSTR() {
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

	operator JET_ERR() { return m_jerr; }

	virtual ~CWinDBErr() { Reset(); }

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