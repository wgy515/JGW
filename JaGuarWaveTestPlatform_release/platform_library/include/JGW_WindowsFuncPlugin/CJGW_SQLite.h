#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"
#include <sqlite3/sqlite3.h>
#include <map>

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_SQLite
    {
    public:
        CCJGW_SQLite(void);
        ~CCJGW_SQLite(void);
    public:
        bool OpenSQLite3(const std::string& strFileDBName);
        //! vFiledsName：字段名 vFiledsValue：字段值
        bool ExecSQLite3(const std::string& strSQL,std::vector<std::string>& vFiledsName,std::vector<std::vector<std::string>>& vFiledsValue);
        //! 第一行字段名 - 第二行字段值 Multi Data
        bool ExecSQLite3(const std::string& strSQL,std::vector<std::vector<std::string>>& vvFiledsNameValue);
        //! 更新SQLite3
        bool UpdateSQLite3(const std::string& strSQL);
        //! 增加记录
        bool InsertSQLite3(const std::string& strSQL);
        //! Single Data
        bool SelectSQLite3(const std::string& strSQL,std::string& strFiledsValue);
        //! Single Data
        bool SelectSQLite3(const std::string& strSQL,std::vector<std::string>& vFiledsName,std::vector<std::string>& vFiledsValue);
        //! Single Data
        bool SelectSQLite3(const std::string& strSQL,std::map<std::string,std::string>& vFiledsNameVal);
        //! Multi Data
        //! bool SelectSQLite3(const std::string& strSQL,std::vector<std::map<std::string,std::string>>& vmapFiledsNameVal);
        //! 关闭SQLite
        void CloseSQLite3();
    private:
        sqlite3* mpSQLiteDB;
    };
}