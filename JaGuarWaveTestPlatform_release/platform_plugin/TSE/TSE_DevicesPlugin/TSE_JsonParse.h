#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_JsonParse : public CTSE_TestBase
    {
    public:
        CTSE_JsonParse(void);
        ~CTSE_JsonParse(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrJsonEnvironment;
        //std::wstring mstrJsonTreeList;
        std::wstring mstrSaveJsonValueEnvironment;
        std::vector<std::string> mvJsonTreeList;
    };

}

