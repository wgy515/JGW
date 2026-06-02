#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESGetSNInfo : public CTSE_TestBase
    {
    public:
        CTSE_MESGetSNInfo(void);
        ~CTSE_MESGetSNInfo(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIsSMTMono;
        size_t mnSNEnvironmentValLen;
        std::string mstrOperateID;
        std::wstring mstrPCBASNEnvironment;
        std::vector<std::wstring> mvstrParamNames;
        std::vector<std::wstring> mvstrEnvironmentValues;
        std::map<std::wstring,std::wstring> mmapstrParamValuesRegexRule;
    };
}