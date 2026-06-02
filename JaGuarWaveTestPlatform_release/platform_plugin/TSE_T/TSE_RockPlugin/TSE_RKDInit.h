#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "DevicesAPI.h"
namespace JGW
{
    class CTSE_RKDInit : public CTSE_TestBase
    {
    public:
        CTSE_RKDInit(void);
        ~CTSE_RKDInit(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbRKDInit;
        RKDContextHandle mContextHandle;
        std::wstring mstrContextHandleEnvironment;
    };

}

