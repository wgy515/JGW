#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <map>
#include <vector>

namespace JGW
{
    class CTSE_RandomGlobalVariable : public CTSE_TestBase
    {
    public:
        CTSE_RandomGlobalVariable(void);
        ~CTSE_RandomGlobalVariable(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        //! 校验配置参数
        //! bool CheckParam(int nRandomIndex);
    private:
        size_t mnGlobalVariableGroupCount;
        std::vector<std::wstring> mvRandomGlobalVariable;
        std::vector<std::vector<std::wstring>> mvvGlobalVariableGroup;
        //! std::vector<std::wstring> mvGlobalVariableGroup;
        std::map<std::wstring,std::wstring> m_mParam;
    };
}