#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_PostHttpReqRsp : public CTSE_TestBase
    {
    public:
        CTSE_PostHttpReqRsp(void);
        ~CTSE_PostHttpReqRsp(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();

    private:
        std::string mstrURL;;
        std::string mstrParam;
        std::string mstrContentType;
        std::string mstrFindString;
        std::wstring mstrParamEnvrionment;
        std::wstring mstrSourceAddrEnvironment;
        std::wstring mstrSaveReceiveEnvironment;
    };
}