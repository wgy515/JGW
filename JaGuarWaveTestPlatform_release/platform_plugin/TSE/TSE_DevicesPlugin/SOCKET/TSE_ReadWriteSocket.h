#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_ReadWriteSocket : public CTSE_TestBase
    {
    public:
        CTSE_ReadWriteSocket(void);
        ~CTSE_ReadWriteSocket(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
		bool mbReadOnly;
        bool mbIsHexReadWrite;
        size_t mnCheckResultTimeOutSec;
        size_t mnTestWaitResultMS;
        size_t mnWriteTimeOutSec;
        std::wstring mstrWrite;
        std::wstring mstrReadResultEnvironment;
        std::wstring mstrConnectSocketInstanceEnvironment;
        CTSE_CheckTestResult mCheckTestResult;
    };
}
