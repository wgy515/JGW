#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_IPQ807xPhone.h"
namespace JGW
{
    class CTSE_IPQ807xWlanSaveBDF2 : public CTSE_TestBase
    {
    public:
        CTSE_IPQ807xWlanSaveBDF2(void);
        ~CTSE_IPQ807xWlanSaveBDF2(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        BDF_OPTION meSaveOption;
        std::string mstrBDFFileName;
    };

}

