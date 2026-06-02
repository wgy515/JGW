#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_ReadLynxAPInfo : public CTSE_TestBase
    {
    public:
        CTSE_ReadLynxAPInfo(void);
        ~CTSE_ReadLynxAPInfo(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrSoftApConfPath;
        std::wstring mstrSSIDEnvironment;
        std::wstring mstrPasswordEnvironment;
    };
}