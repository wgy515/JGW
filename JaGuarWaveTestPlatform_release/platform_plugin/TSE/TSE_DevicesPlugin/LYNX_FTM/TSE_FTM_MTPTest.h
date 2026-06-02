#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FTM_MTPTest : public CTSE_TestBase
    {
    public:
        CTSE_FTM_MTPTest(void);
        ~CTSE_FTM_MTPTest(void);
    private:
        const wchar_t* TSE_GetParamDescription();

        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);

        bool TSE_Run();
    private:
        //! MTP 制造商信息
        std::wstring mstrManufacturer;
        //! MTP 描述信息
        std::wstring mstrDescription;
    };
}