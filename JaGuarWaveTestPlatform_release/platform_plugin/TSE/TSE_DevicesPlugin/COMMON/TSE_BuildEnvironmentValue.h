#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    struct s_left_right
    {
        int left;
        int right; //! 默认取所有
    };
    class CTSE_BuildEnvironmentValue : public CTSE_TestBase
    {
    public:
        CTSE_BuildEnvironmentValue(void);
        ~CTSE_BuildEnvironmentValue(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        void GetEnvironmentCommand(std::wstring& strCommand);
        void ParseStringFormat();
    private:
        std::vector<s_left_right> mvFormat;
        std::wstring mtrSrcEnvironmentValue; //! 目标环境变量值
        std::wstring mstrSrcFormat; //! a,c  a：( 0 : 左边 -1 右边) b:(>0 : 取第几位字符串 < 0 取所有)
        std::wstring mstrBuildEnvironmentName; //! 构造的字符串保存环境变量名
    };
}