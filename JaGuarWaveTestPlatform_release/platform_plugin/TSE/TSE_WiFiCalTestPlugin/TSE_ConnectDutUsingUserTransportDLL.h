#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <QMSL_inc/QLib.h>
#include <QMSL_inc/QLib_Defines.h>

namespace JGW
{
    class CTSE_ConnectDutUsingUserTransportDLL : public CTSE_TestBase
    {
    public:
        CTSE_ConnectDutUsingUserTransportDLL(void);
        ~CTSE_ConnectDutUsingUserTransportDLL(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
		bool mbReloadUserDefineDll;
        QLIB_TargetType_Enum meType;
        std::string mstrWlanID;
        std::string mstrComPort;
        std::string mstrUserTransPortDll;
    };
}