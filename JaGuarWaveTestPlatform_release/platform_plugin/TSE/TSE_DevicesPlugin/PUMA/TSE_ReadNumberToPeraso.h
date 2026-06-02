#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CTSE_PUMA_DEFINE.h"

namespace JGW
{
    class CTSE_ReadNumberToPeraso : public CTSE_TestBase
    {
    public:
        CTSE_ReadNumberToPeraso(void);
        ~CTSE_ReadNumberToPeraso(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        size_t mNumberLen;
        size_t meNubmerType; //! ∫≈∂Œ¿‡–Õ
        std::wstring mstrNumberPrefix; //! ∫≈∂Œ«∞◊∫
        std::wstring mstrEnvironment; 
    };
}