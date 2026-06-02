#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESCheckSNStatus : public CTSE_TestBase
    {
    public:
        CTSE_MESCheckSNStatus(void);
        ~CTSE_MESCheckSNStatus(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsSMTMono;
        size_t mnSNEnvironmentValLen;
        std::string mstrOperateID;
        std::wstring mstrSNEnvironment;     
    };
}