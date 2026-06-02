#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <map>

namespace JGW
{
    class CTSE_ParseConfigInfo : public CTSE_TestBase
    {
    public:
        CTSE_ParseConfigInfo(void);
        ~CTSE_ParseConfigInfo(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        bool mbIsFile;
        int m_nKeyValCount;
        std::wstring mstrParseConfigInfoEnvironment;
        std::map<std::wstring,std::wstring> m_mParam;
        std::map<std::wstring,std::wstring> mmapKeyVal;
    };
}