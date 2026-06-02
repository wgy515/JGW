#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_ReadWriteSSH : public CTSE_TestBase
    {
    public:
        CTSE_ReadWriteSSH(void);
        ~CTSE_ReadWriteSSH(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();

    private:
        bool TSE_CheckTestResult(const std::string& strRead);

        bool CompareStringResultFile();
        bool CompareIntResultFile();
        bool FindStringResultFile();
    private:
        bool mbWriteOnly;
        size_t mnCheckResultTimeOut;
        size_t mnTestWaitResult;
		size_t mnReadResultRegexSearchIndex;
        std::string mstrCommand;
        std::wstring mstrReadBuf;
        std::wstring mstrTip;  
        std::wstring mwstrTestCase;
        std::wstring mstrReadResultEnvironment;
		std::wstring mstrReadResultRegexSearch;
        CTSE_CheckTestResult mCheckTestResult;
    };
}