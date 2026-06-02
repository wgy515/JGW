#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESNextSNStation : public CTSE_TestBase
    {
    public:
        CTSE_MESNextSNStation(void);
        ~CTSE_MESNextSNStation(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnSNEnvironmentValLen;
        std::string mstrOperateID;
        std::wstring mstrSNEnvironment;   
    };
}