#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    class CTSE_BuildMESLog : public CTSE_TestBase
    {
    public:
        CTSE_BuildMESLog(void);
        ~CTSE_BuildMESLog(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        jgw_timer mTimer;
        std::wstring mstrSrcTempleteFilePath;
        std::wstring mstrSaveMESLogEnvionment;
        std::wstring mstrRecordingTimerEnvionment;
		std::wstring mstrComputeName;
    };
}
