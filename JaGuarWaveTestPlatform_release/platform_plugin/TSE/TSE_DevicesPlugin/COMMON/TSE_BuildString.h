#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_BuildString : public CTSE_TestBase
    {
    public:
        CTSE_BuildString(void);
        ~CTSE_BuildString(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        //! ,ºÅ×ö·Ö¸ô·û \r,\n
        std::wstring mstrEraseStringSplit;
        std::wstring mstrSrcStringCommand;
        std::wstring mstrSaveEnvironment;
		std::wstring mstrRegexRule;
        std::vector<std::wstring> mvstrEraseString;
    };
}


