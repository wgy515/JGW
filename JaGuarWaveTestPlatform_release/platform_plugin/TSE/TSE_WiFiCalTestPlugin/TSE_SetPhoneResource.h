#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_SetPhoneResource : public CTSE_TestBase
    {
    public:
        CTSE_SetPhoneResource(void);
        ~CTSE_SetPhoneResource(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //RFMode meMode;
        //PhyID mePhyID;
        std::wstring mstrResourceContextEnvironment;
    };
}


