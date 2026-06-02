#pragma once
#include "TSE_FTM_TestBase.h"

namespace JGW
{
    class CTSE_FTM_CheckKey : public CTSE_FTM_TestBase
    {
    public:
        CTSE_FTM_CheckKey(void);
        ~CTSE_FTM_CheckKey(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run(void);
        bool TSE_Exit(void);
    private:
        size_t mKeyTimeOutSec;
        std::string mstrKeyType;
        std::string mstrEvent;
    };
}