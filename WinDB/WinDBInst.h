#pragma once

#include "WinDB.h"

class CWinDBSession;

using DBSess = std::shared_ptr<CWinDBSession>;
using DBSesses = std::map<LPCTSTR, DBSess>;

class WINDB_API CWinDBInst
{
public:
	CWinDBInst(LPCTSTR lpszName = NULL) throw (...)
	{
		JET_ERR err = JetCreateInstance(&m_inst, lpszName);
		if (err != JET_errSuccess) {
			throw CWinDBErr(err);
		}
	}

	~CWinDBInst() {
		if (0 != m_inst) JetTerm(m_inst);
	}

	operator JET_INSTANCE* () { return &m_inst; }
	operator JET_INSTANCE& () { return m_inst; }

	CWinDBSession* NewSession(LPCTSTR lpszUser = nullptr, LPCTSTR lpszPwd = nullptr);

	DBSess PrepareSession(LPCTSTR lpszSess)
	{
		auto it = m_sses.find(lpszSess);
		if (it == m_sses.end())
			m_sses[lpszSess] = std::move(std::make_shared<CWinDBSession>());
		return m_sses[lpszSess];
	}

private:
	DBSesses m_sses;
	JET_INSTANCE m_inst{ 0 };
};
