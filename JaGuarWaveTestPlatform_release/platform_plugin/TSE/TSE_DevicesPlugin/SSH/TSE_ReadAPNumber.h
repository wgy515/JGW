#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_ReadAPNumber : public CTSE_TestBase
    {
    public:
        CTSE_ReadAPNumber(void);
        ~CTSE_ReadAPNumber(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrAPNumberResultEnvironment;
        std::wstring mstrPCBASNEnvironment; //! 组装SN
        std::wstring mstrSerialNumberEnvironment; //! 出货SN
    };
}