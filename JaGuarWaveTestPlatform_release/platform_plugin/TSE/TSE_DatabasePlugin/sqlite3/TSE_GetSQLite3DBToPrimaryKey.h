#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_GetSQLite3DBToPrimaryKey : public CTSE_TestBase
    {
    public:
        CTSE_GetSQLite3DBToPrimaryKey(void);
        ~CTSE_GetSQLite3DBToPrimaryKey(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        std::wstring mstrPrimaryKey; //! 主键名称
        std::wstring mstrPrimaryValueEnvironment;//! 主键对应的环境变量名称
        std::string mstrSQLiteDBPath; //! SQLite DB文件路径
        std::wstring mstrSQLiteDBTableName; //! SQLite DB 表名
        std::vector<std::wstring> mvstrSQLiteHeader; //! SQLite 表头
        std::vector<std::wstring> mvstrEnvironmentValues;
    };
}