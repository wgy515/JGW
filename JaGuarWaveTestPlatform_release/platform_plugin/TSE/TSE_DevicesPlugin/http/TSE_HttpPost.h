#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_HttpPost : public CTSE_TestBase
    {
    public:
        CTSE_HttpPost(void);
        ~CTSE_HttpPost(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        bool mbWriteOnly;
        std::wstring mstrUrl;
        std::wstring mstrPostData;
        std::wstring mstrPostDataEnvrionment;
        std::wstring mstrHeader;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrSaveReceiveEnvironment;
        CTSE_CheckTestResult mCheckTestResult;
    };
}


