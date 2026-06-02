#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
namespace JGW
{
    class CTSE_UpdateSQLite3DB : public CTSE_TestBase
    {
    public:
        CTSE_UpdateSQLite3DB(void);
        ~CTSE_UpdateSQLite3DB(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        //! 构造SQL语句
        bool BuildUpdateSQLStat();
    private:
        std::wstring mstrSQLiteUpdateSQL;
        std::string mstrSQLiteDBPath; //! SQLite DB文件路径
    };
}