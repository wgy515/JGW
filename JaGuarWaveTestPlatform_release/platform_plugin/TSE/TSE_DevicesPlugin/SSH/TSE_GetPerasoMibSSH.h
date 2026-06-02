#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_CheckTestResult.h>
namespace JGW
{
    class CTSE_GetPerasoMibSSH : public CTSE_TestBase
    {
    public:
        CTSE_GetPerasoMibSSH(void);
        ~CTSE_GetPerasoMibSSH(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        int GetPerasoStationRow();
        bool TSE_CheckTestResult(const std::string& strRead);
    private:
        bool mbWriteOnly;
        size_t mnCheckResultTimeOut;
        size_t mnTestWaitResult;

        int mnClass;
        int mnGroup;
        int mnEntry;
        //! 
        std::wstring mstrReadResultEnvironment;
        //! 结果指令删除操作
        std::wstring mstrPrsBinPath;
        std::wstring mstrEraseStringSplit;
        std::wstring mstrMibOperationCommand;
        CTSE_CheckTestResult mCheckTestResult;
        std::vector<std::string> mvstrEraseString;
    };
}
