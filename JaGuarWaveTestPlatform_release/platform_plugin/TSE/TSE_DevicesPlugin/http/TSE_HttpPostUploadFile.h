#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_HttpPostUploadFile : public CTSE_TestBase
    {
    public:
        CTSE_HttpPostUploadFile(void);
        ~CTSE_HttpPostUploadFile(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        std::wstring mstrUrl;
        std::wstring mstrUploadFilePath;
        std::wstring mstrUploadFilePathEnvrionment;
        std::wstring mstrFieldName;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrSaveReceiveEnvironment;
        CTSE_CheckTestResult mCheckTestResult;
    };
}


