#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CompareFloat : public CTSE_TestBase
    {
    public:
        CTSE_CompareFloat(void);
        ~CTSE_CompareFloat(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 1~3 || 2~4 || 8~9
        std::wstring mstrCompareFloatRangeList;
        //£¡ 
        std::wstring mstrCompareFloatEnvironment;
    };
}