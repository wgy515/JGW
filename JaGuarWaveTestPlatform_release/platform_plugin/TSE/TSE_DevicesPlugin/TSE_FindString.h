#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_FindString : public CTSE_TestBase
    {
    public:
        CTSE_FindString(void);
        ~CTSE_FindString(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        bool mbIgnoreCase;
        //! 
        std::wstring mstrSrcStringEnvironment;
        //£¡ 
        std::wstring mstrFindString;
    };

}
