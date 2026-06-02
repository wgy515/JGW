#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "TSE_IPQ807xDefine.h"
namespace JGW
{
    class CTSE_IPQ807xConnectDutUsingQUTS : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xConnectDutUsingQUTS(void);
        ~CTSE_IPQ807xConnectDutUsingQUTS(void);
    private:
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        const wchar_t* TSE_GetParamDescription();
        bool TSE_Run();
    private:
        HANDLE mhResourceContext;
        QLIB_TargetType_Enum meType;
        DutTypeId meWlanDut;
        std::string mstrComPort;
    };
}
