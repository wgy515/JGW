#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <TSE_FoundationLib/TSE_BaseConfigXML.h>
#include <map>
namespace JGW
{
    class CTSE_BaseConfig : public CTSE_TestBase
    {
    public:
        CTSE_BaseConfig(void);
        ~CTSE_BaseConfig(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        size_t mnConfigCount;
        std::vector<S_TSE_BASE_ITEM> mvsTSEBaseItems;
        std::map<std::wstring,std::wstring> m_mParam;
    };
}