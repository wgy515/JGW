#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <JGW_FoundationFunc/rapidcsv.hpp>

namespace JGW
{
    class CTSE_LoadCSVDatabase : public CTSE_TestBase
    {
    public:
        CTSE_LoadCSVDatabase(void);
        ~CTSE_LoadCSVDatabase(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //  主键列的索引
        int mnPrimaryColumnValueIndex;
        // 主键名称行索引
        int mnPrimaryColumnNameRowIndex;
        // CSV文件路径
        std::wstring mstrFilePath;
        // CSV文件路径COMMAND
        std::wstring mstrCSVFilePathCommand;
        // CSV实例保存环境变量
        std::wstring mstrCSVInstanceEnvironment;
        // csv doc instance
        rapidcsv::Document mscvDoc;
    };
}


