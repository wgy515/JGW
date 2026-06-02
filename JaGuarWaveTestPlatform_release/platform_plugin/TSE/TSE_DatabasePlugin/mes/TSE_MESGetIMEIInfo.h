#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESGetIMEIInfo : public CTSE_TestBase
    {
    public:
        CTSE_MESGetIMEIInfo(void);
        ~CTSE_MESGetIMEIInfo(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnSNEnvironmentValLen;
        std::string mstrOperateID;
        std::wstring mstrSNEnvironment;
        std::wstring mstrIMEIEnvironment;
        std::vector<std::wstring> mvstrParamNames;
        std::vector<std::wstring> mvstrEnvironmentValues;
        std::map<std::wstring,std::wstring> mmapstrParamValuesRegexRule;
    };
}