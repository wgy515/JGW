#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SplitString : public CTSE_TestBase
    {
    public:
        CTSE_SplitString(void);
        ~CTSE_SplitString(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 分隔符
        std::wstring mstrSeparator;
        //! 需要分割的字符串
        std::wstring mstrSrcStringEnvironment;
        //! 保存到对应环境集合中
        std::wstring mstrSaveStringEnvironmentList;
        //!
        std::map<std::wstring,std::wstring> mmapstrParamValuesRegexRule;
    };
}



