#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_CompareStringList : public CTSE_TestBase
    {
    public:
        CTSE_CompareStringList(void);
        ~CTSE_CompareStringList(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIgnoreCase;
        //! 需要查找的字符串
        std::wstring mstrCompareStrEnvironment;
        //! 字符串集合
        std::wstring mstrStringListCommand;
    };
}


