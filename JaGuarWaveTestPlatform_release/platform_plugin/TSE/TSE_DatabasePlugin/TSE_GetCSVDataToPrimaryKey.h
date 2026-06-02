#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_GetCSVDataToPrimaryKey : public CTSE_TestBase
    {
    public:
        CTSE_GetCSVDataToPrimaryKey(void);
        ~CTSE_GetCSVDataToPrimaryKey(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        // 主键值
        std::wstring mstrPrimaryValueEnvironment;
        // csv 实例对应的环境变量
        std::wstring mstrCSVInstanceEnvironment;
        // 列名
        std::vector<std::string> mvstrColumnNames;
        // 主键值列名对应的值保存环境变量
        std::vector<std::wstring> mvstrSaveColumnEnvironmentValues;
    };
}
