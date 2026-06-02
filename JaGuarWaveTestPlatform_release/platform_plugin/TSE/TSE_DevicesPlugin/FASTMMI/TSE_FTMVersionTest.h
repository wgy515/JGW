#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    struct S_ADB_COMMAND_RESULT
    {
        std::wstring mstrCommand;
        std::wstring mstrResultPos;
        std::wstring mstrResult;
    };

    class CTSE_FTMVersionTest : public CTSE_TestBase
    {
    public:
        CTSE_FTMVersionTest(void);
        ~CTSE_FTMVersionTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        bool GetAdbCommandResult(S_ADB_COMMAND_RESULT& sAdbCommandResult);
    private:
        size_t mnAdbTimeOut;
        std::wstring mstrVersion;
        std::wstring mstrModel;
        S_ADB_COMMAND_RESULT msVersion;
        S_ADB_COMMAND_RESULT msModel;
    };
}