#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_ClearWiFiTestRecord : public CTSE_TestBase
    {
    public:
        CTSE_ClearWiFiTestRecord(void);
        ~CTSE_ClearWiFiTestRecord(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TES_Run();
    private:
        std::wstring mstrWiFiTestResultRecordEnvironment;
    };

}

