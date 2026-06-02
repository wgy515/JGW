#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SaveMESLog : public CTSE_TestBase
    {
    public:
        CTSE_SaveMESLog(void);
        ~CTSE_SaveMESLog(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSaveMESLogEnvionment;
        std::wstring mstrSrcSaveMESLogEnvionment;
        std::wstring mstrRecordingTimerEnvionment;
        std::wstring mstrSerialNumberEnvionment;
    };
}
