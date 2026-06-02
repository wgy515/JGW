#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>

namespace JGW
{
    class CTSE_ReadWriteModemPort : public CTSE_TestBase
    {
    public:
        CTSE_ReadWriteModemPort(void);
        ~CTSE_ReadWriteModemPort(void);
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
        std::string mstrCommand;
        std::wstring mstrReadBuf;
        std::wstring mstrTip;  
        std::wstring mwstrTestCase;
        std::wstring mstrReadResultEnvironment;
        CTSE_CheckTestResult mCheckTestResult;
    };
}


