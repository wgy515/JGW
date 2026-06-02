#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_ReadQualcommNumber : public CTSE_TestBase
    {
    public:
        CTSE_ReadQualcommNumber(void);
        ~CTSE_ReadQualcommNumber(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run(void);
    private:
        size_t mNumberLen;
        size_t meNubmerType; //! ∫≈∂Œ¿‡–Õ
        std::wstring mstrNumberPrefix; //! ∫≈∂Œ«∞◊∫
        std::wstring mstrEnvironment;
    };
}