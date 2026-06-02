#pragma once
#include <JaGuarWaveHelpV2/CJaGuarWaveHelpV2Define.h>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
namespace JGW
{
    class CCJGW_HelpSqlite3
    {
    public:
        CCJGW_HelpSqlite3(void);
        ~CCJGW_HelpSqlite3(void);
    public:
        bool LoadHelpSqlite3(const std::string& strHelpSqlite3FilePath,const std::wstring& strTaskNumber,S_TASK_SQL& sTaskSQL);
    private:
        //! 
        bool GetTaskTableToTaskNumber(CCJGW_SQLite& sqliteDB,const std::wstring& strTaskNumber,S_TASK_SQL& sTaskSQL);
        //!
        bool GetInstallVersionToInstallVersionID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL);
        //!
        bool GetOTAToOTAID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL);
        //!
        bool GetConfigToConfigID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL);
        //!
        bool GetTaskConfigToTaskConfigID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL);
    };
}

