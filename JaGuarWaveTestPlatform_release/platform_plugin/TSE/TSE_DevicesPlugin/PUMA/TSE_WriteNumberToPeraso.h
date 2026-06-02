#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CTSE_PUMA_DEFINE.h"

namespace JGW
{
    class CTSE_WriteNumberToPeraso : public CTSE_TestBase
    {
    public:
        CTSE_WriteNumberToPeraso(void);
        ~CTSE_WriteNumberToPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbWriteOptBin;
        bool mbInitPDBInfo;
        size_t meNubmerType; //! ∫≈∂Œ¿‡–Õ
        std::wstring mstrEnvironment;
    };

}