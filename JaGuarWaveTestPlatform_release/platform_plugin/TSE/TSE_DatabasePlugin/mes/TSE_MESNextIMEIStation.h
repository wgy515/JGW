#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_MESNextIMEIStation : public CTSE_TestBase
    {
    public:
        CTSE_MESNextIMEIStation(void);
        ~CTSE_MESNextIMEIStation(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mnPCBASNEnvironmentLen;
        std::string mstrOperateID;
        std::wstring mstrIMEISNEnvironment;
        std::wstring mstrPCBASNEnvironment;
        std::wstring mstrSoftWareVarTinnoEnv;
        std::wstring mstrSoftWareVarCustomerEnv;
        std::wstring mstrRemarkCommand;
        std::wstring mstrUnlockcodeCommand;
        std::wstring mstrImeiInfoCommand;
    };
}