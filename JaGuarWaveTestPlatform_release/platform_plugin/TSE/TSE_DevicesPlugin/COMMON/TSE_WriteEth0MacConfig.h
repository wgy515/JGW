#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_WriteEth0MacConfig : public CTSE_TestBase
    {
    public:
        CTSE_WriteEth0MacConfig(void);
        ~CTSE_WriteEth0MacConfig(void);

    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        size_t mnEnvironmentSize;
        std::string mstrConfigFileBuf;
        std::string mstrDestEth0MacConfigPath;
        std::string mstrSrcEth0MacConfigPath;
        std::wstring mstrEnvironment;
    };
}