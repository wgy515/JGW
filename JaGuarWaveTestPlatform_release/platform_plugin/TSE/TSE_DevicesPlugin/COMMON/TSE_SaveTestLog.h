#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_TestLoggingPlugin/CJGW_MoveTestLogging.h>
namespace JGW
{
    class CTSE_SaveTestLog : public CTSE_TestBase
    {
    public:
        CTSE_SaveTestLog(void);
        ~CTSE_SaveTestLog(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrLogEnvironment;
        std::wstring mstrSNEnvironment;
        CCJGW_MoveTestLogging mMoveTestLogging;
    };
}