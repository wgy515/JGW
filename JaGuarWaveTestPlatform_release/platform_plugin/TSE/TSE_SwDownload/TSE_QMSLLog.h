#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_QMSLLog : public CTSE_TestBase
    {
    public:
        CTSE_QMSLLog(void);
        ~CTSE_QMSLLog(void);
    private:     
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbDutLog;
        bool mbLogOn;
        std::string mstrFileName;
        std::wstring mstrResourceContextEnv;
    };
}


