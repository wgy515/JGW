#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_LongReadWriteSerialComPort : public CTSE_TestBase
    {
    public:
        CTSE_LongReadWriteSerialComPort(void);
        ~CTSE_LongReadWriteSerialComPort(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        bool WriteSerialBuffer();
        bool CheckReadSerialResult();
        static void ReadSerialThreadCallback(void *lpParam);
    private:
        static bool mbStartReadThread;
        bool mbIsHexReadWrite;
        bool mbIsClearReadBuffer;
        bool mbReadOnly;
        size_t mnCheckResultTimeOutSec;
        size_t mnTestWaitResultMS;
        size_t mnWriteTimeOutSec;
        size_t mnReadResultRegexSearchIndex;
		std::wstring mstrStartRspCommand;
		std::wstring mstrEndRspCommand;
        std::wstring mstrErrorHandler;
        std::wstring mstrWrite;
        std::wstring mstrReadResultEnvironment;
        std::wstring mstrReadResultRegexSearch;
        CTSE_CheckTestResult mCheckTestResult;
    };
}
