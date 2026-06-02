#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CTSE_FTMExecuteSingleTestResult : public CTSE_TestBase,public CTSE_CheckTestResult
    {
    public:
        CTSE_FTMExecuteSingleTestResult(void);
        ~CTSE_FTMExecuteSingleTestResult(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
        bool TSE_CheckTestResult();
    private:
        bool CompareStringResultFile();
        bool CompareIntResultFile();
        bool FindStringResultFile();
    private:
        size_t mnCheckResultTimeOut;
        size_t mnTestWaitResult;
        std::string mstrTestCase;
        std::wstring mstrTip;  
        std::wstring mwstrTestCase;
        std::string mstrResultFilePath;
        std::string mstrRemoteFile;
        CCJGW_ConfigIni mResultIni;
    };
}