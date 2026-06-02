#pragma once
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>


namespace JGW
{
	class CTSE_TestSequenceLuaExec : public CTSE_LogListener,public CTSE_TestStatusListener
	{
	public:
		CTSE_TestSequenceLuaExec(int nIndex);
		~CTSE_TestSequenceLuaExec(void);

	protected: //! CTSE_LogListener
		virtual void OnDebugLog(const wchar_t* strDebugLog);

		virtual void OnInfoLog(const wchar_t* strInfoLog);

		virtual void OnErrorLog(const wchar_t* strErrorLog);

		virtual void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
		
	protected: //! CTSE_TestStatusListener
		virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);

		virtual void OnTestIndexChange(size_t index,int subIndex);
	private:
		int mnIndex;
	};
}
