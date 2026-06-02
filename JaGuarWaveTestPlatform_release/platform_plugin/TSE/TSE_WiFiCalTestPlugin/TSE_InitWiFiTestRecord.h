#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_WiFiTestResultRecord.h"
namespace JGW
{
    class CTSE_InitWiFiTestRecord : public CTSE_TestBase
    {
    public:
        CTSE_InitWiFiTestRecord(void);
        ~CTSE_InitWiFiTestRecord(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSaveWiFiTestResultRecordEnvironment;
        CCJGW_WiFiTestResultRecord mcWiFiTestResultRecord;
    };
}


