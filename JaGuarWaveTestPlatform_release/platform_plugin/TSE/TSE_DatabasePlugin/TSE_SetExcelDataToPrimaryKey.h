#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    class CTSE_SetExcelDataToPrimaryKey : public CTSE_TestBase
    {
    public:
        CTSE_SetExcelDataToPrimaryKey(void);
        ~CTSE_SetExcelDataToPrimaryKey(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        // 主键值
        std::wstring mstrPrimaryValueEnvironment;
        // 保存CSV文件路径
        std::wstring mstrSaveCSVFilePath;
        // csv 实例对应的环境变量
        std::wstring mstrCSVInstanceEnvironment;
        // 列名
        std::vector<std::string> mvstrColumnNames;
        // 主键值列名对应的值保存环境变量
        std::vector<std::wstring> mvstrSetColumnEnvironmentValues; 
    };
}


