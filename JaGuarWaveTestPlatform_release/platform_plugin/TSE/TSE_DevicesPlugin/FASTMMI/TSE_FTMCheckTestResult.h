#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CTSE_FTMCheckTestResult : public CTSE_TestBase,public CTSE_CheckTestResult
    {
    public:
        CTSE_FTMCheckTestResult(void);
        ~CTSE_FTMCheckTestResult(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Init();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
        bool TSE_Exit();
    private:
        bool CompareStringResultFile();
        bool CompareIntResultFile();
        bool FindStringResultFile();
    private:
        std::wstring mstrTestCase;
        std::string mstrResultFilePath;
        std::string mstrRemoteFile;
        CCJGW_ConfigIni mResultIni;
    };
}