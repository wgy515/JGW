#include "StdAfx.h"
#include "TSE_TestSequenceLuaExec.h"
#if _DEBUG
#include <iostream>
#endif
namespace JGW
{
	CTSE_TestSequenceLuaExec::CTSE_TestSequenceLuaExec(int nIndex) : mnIndex(nIndex)
	{
	}


	CTSE_TestSequenceLuaExec::~CTSE_TestSequenceLuaExec(void)
	{
	}

	void CTSE_TestSequenceLuaExec::OnDebugLog(const wchar_t* strDebugLog)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << strDebugLog << std::endl;
#endif
	}

	void CTSE_TestSequenceLuaExec::OnErrorLog(const wchar_t* strErrorLog)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << strErrorLog << std::endl;
#endif
	}

	void CTSE_TestSequenceLuaExec::OnInfoLog(const wchar_t* strInfoLog)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << strInfoLog << std::endl;
#endif
	}

	void CTSE_TestSequenceLuaExec::OnUserDefinedLog(int id,const wchar_t* strCustomLog)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << L"," << id << L"," << strCustomLog << std::endl;
#endif
	}
	/*
	E_INIT_TEST_STATUS = 0,
	E_RUN_TEST_STATUS,//! 运行
	E_RUNING_TEST_STATUS, //! 正在运行
	E_FAIL_TEST_STATUS,
	E_PASS_TEST_STATUS
	*/
	const wchar_t gszTestStatus[][48] = {L"INIT_TEST",L"RUN_TEST",L"RUNING_TEST",L"FAIL_TEST",L"PASS_TEST"};


	void CTSE_TestSequenceLuaExec::OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << strTestName << L" ==> " << gszTestStatus[eTestStatus] << std::endl;
#endif
	}

	void CTSE_TestSequenceLuaExec::OnTestIndexChange(size_t index,int subIndex)
	{
#if _DEBUG
		std::wcout << L"[" << mnIndex << L"] " << L"," << index << L"," << subIndex << std::endl;
#endif
	}
}
