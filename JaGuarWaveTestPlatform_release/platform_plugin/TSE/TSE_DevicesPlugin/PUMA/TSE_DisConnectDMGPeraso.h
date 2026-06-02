#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_DisConnectDMGPeraso : public CTSE_TestBase
    {
    public:
        CTSE_DisConnectDMGPeraso(void);
        ~CTSE_DisConnectDMGPeraso(void);
    private:
        //! 
        const wchar_t* TSE_GetParamDescription();
        //! 
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 
        bool TSE_Run();
    private:
        std::wstring mstrGuSerialComPortEnv;
        std::wstring mstrDutSerialComPortEnv;
    };
}


