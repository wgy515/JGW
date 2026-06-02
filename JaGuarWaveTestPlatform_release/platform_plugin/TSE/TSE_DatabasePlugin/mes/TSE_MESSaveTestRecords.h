#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESSaveTestRecords : public CTSE_TestBase
    {
    public:
        CTSE_MESSaveTestRecords(void);
        ~CTSE_MESSaveTestRecords(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbUpdateStartTestTime;
        bool mbUpdateEndTestTime;
        size_t mnSNEnvironmentValLen;
        std::string mstrOperateID;
        std::wstring mstrSNIMEIEnvironment; 
        static std::string mstrStartTestTime;
        static std::string mstrEndTestTime;
    };
}