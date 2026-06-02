#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_DutLog : public CTSE_TestBase
    {
    public:
        CTSE_DutLog(void);
        ~CTSE_DutLog(void);
    private:     
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbDutLog;
        bool mbLogOn;
        std::string mstrFileName;
    };
}


