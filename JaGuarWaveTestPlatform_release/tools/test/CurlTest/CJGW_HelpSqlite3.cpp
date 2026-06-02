#include "StdAfx.h"
#include "CJGW_HelpSqlite3.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <map>
namespace JGW
{
    CCJGW_HelpSqlite3::CCJGW_HelpSqlite3(void)
    {
    }


    CCJGW_HelpSqlite3::~CCJGW_HelpSqlite3(void)
    {
    }

    bool CCJGW_HelpSqlite3::LoadHelpSqlite3(const std::string& strHelpSqlite3FilePath,const std::wstring& strTaskNumber,S_TASK_SQL& sTaskSQL)
    {
        CCJGW_SQLite sqliteDB;
        sTaskSQL.mstrTaskNumber = strTaskNumber;
        if (!sqliteDB.OpenSQLite3(strHelpSqlite3FilePath))
        {
            return false;
        }

        if (!GetTaskTableToTaskNumber(sqliteDB,sTaskSQL.mstrTaskNumber,sTaskSQL))
        {
            return false;
        }

        if (!GetInstallVersionToInstallVersionID(sqliteDB,sTaskSQL))
        {
            return false;
        }

        if (!GetOTAToOTAID(sqliteDB,sTaskSQL))
        {
            return false;
        }

        if (!GetConfigToConfigID(sqliteDB,sTaskSQL))
        {
            return false;
        }

        if (!GetTaskConfigToTaskConfigID(sqliteDB,sTaskSQL))
        {
            return false;
        }

        return true;
    }

    bool CCJGW_HelpSqlite3::GetTaskTableToTaskNumber(CCJGW_SQLite& sqliteDB,const std::wstring& strTaskNumber,S_TASK_SQL& sTaskSQL)
    {
        std::string strSelectTaskSQL;
        std::map<std::string,std::string> mapFiledsNameVal;

        JGW_FormatString(strSelectTaskSQL,"SELECT * FROM TASK WHERE NAME='%s'",JGW_W2A(strTaskNumber).c_str());
        //! strSelectTaskSQL = "SELECT * FROM TASK";
        if (!sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }

        sTaskSQL.mstrDescription = JGW_A2W(mapFiledsNameVal["DES"],CP_UTF8);
        sTaskSQL.mnInstallVersionID = atoi(mapFiledsNameVal["InstallVersionID"].c_str());
        sTaskSQL.mnOTAID = atoi(mapFiledsNameVal["OTAID"].c_str());
        sTaskSQL.mnConfigID = atoi(mapFiledsNameVal["ConfigID"].c_str());
        sTaskSQL.mnTaskConfigID = atoi(mapFiledsNameVal["TaskConfigID"].c_str());

        return true;
    }

    bool CCJGW_HelpSqlite3::GetInstallVersionToInstallVersionID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL)
    {
        std::string strSelectTaskSQL;
        std::map<std::string,std::string> mapFiledsNameVal;

        JGW_FormatString(strSelectTaskSQL,"SELECT * FROM INSTALLVERSION WHERE ID=%d",sTaskSQL.mnInstallVersionID);
        //! strSelectTaskSQL = "SELECT * FROM TASK";
        if (sTaskSQL.mnInstallVersionID <= 0 || !sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }
        sTaskSQL.msInstallVersion.mstrName = JGW_A2W(mapFiledsNameVal["NAME"],CP_UTF8);
        sTaskSQL.msInstallVersion.mstrDescription = JGW_A2W(mapFiledsNameVal["DES"],CP_UTF8);
        sTaskSQL.msInstallVersion.mstrVersion = JGW_A2W(mapFiledsNameVal["Version"],CP_UTF8);
        sTaskSQL.msInstallVersion.mstrMD5 = JGW_A2W(mapFiledsNameVal["MD5"],CP_UTF8);
        sTaskSQL.msInstallVersion.mstrFTPUrl = JGW_A2W(mapFiledsNameVal["FTPUrl"],CP_UTF8);

        return true;
    }

    bool CCJGW_HelpSqlite3::GetOTAToOTAID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL)
    {
        std::string strSelectTaskSQL;
        std::map<std::string,std::string> mapFiledsNameVal;
        //! ID 小于等于0 默认使用最新的版本配置
        if (sTaskSQL.mnOTAID <= 0)
        {
            //! 最新版本标识查找版本
            JGW_FormatString(strSelectTaskSQL,"SELECT * FROM OTA WHERE InstallVersionID=%d AND IsLatestVersion=1",sTaskSQL.mnInstallVersionID);
            if (sTaskSQL.mnInstallVersionID <= 0 || !sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
            {
                return false;
            }
            //!如果未找到最新版本标识 查找当前版本最进更新的相关版本
            if (mapFiledsNameVal.empty())
            {
                //! select * from TASK ORDER BY ID DESC limit 1;
                JGW_FormatString(strSelectTaskSQL,"SELECT * FROM OTA WHERE InstallVersionID=%d ORDER BY ID DESC LIMIT 1",sTaskSQL.mnInstallVersionID);
                if (!sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
                {
                    return false;
                }
            }
        }
        else
        {
            JGW_FormatString(strSelectTaskSQL,"SELECT * FROM OTA WHERE ID=%d AND InstallVersionID=%d",sTaskSQL.mnOTAID,sTaskSQL.mnInstallVersionID);
            //! strSelectTaskSQL = "SELECT * FROM TASK";
            if (!sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
            {
                return false;
            }
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }

        sTaskSQL.msOTA.mstrName = JGW_A2W(mapFiledsNameVal["NAME"],CP_UTF8);
        sTaskSQL.msOTA.mstrDescription = JGW_A2W(mapFiledsNameVal["DES"],CP_UTF8);
        sTaskSQL.msOTA.mstrVersion = JGW_A2W(mapFiledsNameVal["Version"],CP_UTF8);
        sTaskSQL.msOTA.mstrMD5 = JGW_A2W(mapFiledsNameVal["MD5"],CP_UTF8);
        sTaskSQL.msOTA.mstrFTPUrl = JGW_A2W(mapFiledsNameVal["FTPUrl"],CP_UTF8);

        return true;
    }

    bool CCJGW_HelpSqlite3::GetConfigToConfigID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL)
    {
        std::string strSelectTaskSQL;
        std::map<std::string,std::string> mapFiledsNameVal;
        //! SELECT * FROM CONFIG,PROJECT WHERE PROJECT.ID=CONFIG.ID AND CONFIG.ID=1;
        //! SELECT PROJECT.NAME AS PROJECT_NAME,PROJECT.DES AS PROJECT_DES, * FROM CONFIG,PROJECT WHERE PROJECT.ID=CONFIG.ID AND CONFIG.ID=1;
        //! SELECT PROJECT.NAME AS PROJECT_NAME,PROJECT.DES AS PROJECT_DES,CONFIG.NAME AS CONFIG_NAME,CONFIG.DES AS CONFIG_DES,CONFIG.MD5 AS CONFIG_MD5,CONFIG.FTPURL AS CONFIG_FTPURL FROM CONFIG,PROJECT WHERE PROJECT.ID=CONFIG.ID AND CONFIG.ID=1;
        JGW_FormatString(strSelectTaskSQL,"SELECT PROJECT.NAME AS PROJECT_NAME,PROJECT.DES AS PROJECT_DES,CONFIG.NAME AS CONFIG_NAME,CONFIG.DES AS CONFIG_DES,CONFIG.MD5 AS CONFIG_MD5,CONFIG.FTPURL AS CONFIG_FTPURL FROM CONFIG,PROJECT WHERE PROJECT.ID=CONFIG.ID AND CONFIG.ID=%d",sTaskSQL.mnConfigID);
        if (sTaskSQL.mnConfigID <= 0 || !sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }

        sTaskSQL.msConfig.mstrName = JGW_A2W(mapFiledsNameVal["CONFIG_NAME"],CP_UTF8);
        sTaskSQL.msConfig.mstrDescription = JGW_A2W(mapFiledsNameVal["CONFIG_DES"],CP_UTF8);
        sTaskSQL.msConfig.mstrMD5 = JGW_A2W(mapFiledsNameVal["CONFIG_MD5"],CP_UTF8);
        sTaskSQL.msConfig.mstrFTPUrl = JGW_A2W(mapFiledsNameVal["CONFIG_FTPURL"],CP_UTF8);
        sTaskSQL.msConfig.msProjectSQL.mstrName = JGW_A2W(mapFiledsNameVal["PROJECT_NAME"],CP_UTF8);
        sTaskSQL.msConfig.msProjectSQL.mstrDescription = JGW_A2W(mapFiledsNameVal["PROJECT_DES"],CP_UTF8);

        return true;
    }

    bool CCJGW_HelpSqlite3::GetTaskConfigToTaskConfigID(CCJGW_SQLite& sqliteDB,S_TASK_SQL& sTaskSQL)
    {
        std::string strSelectTaskSQL;
        std::map<std::string,std::string> mapFiledsNameVal;

        JGW_FormatString(strSelectTaskSQL,"SELECT * FROM TASKCONFIG WHERE ID=%d",sTaskSQL.mnTaskConfigID);
        if (sTaskSQL.mnTaskConfigID <= 0 || !sqliteDB.SelectSQLite3(strSelectTaskSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }

        sTaskSQL.msTaskConfig.mnDebugModeMs = atoi(mapFiledsNameVal["DebugModeMSec"].c_str());
        sTaskSQL.msTaskConfig.mnUploadLogSec = atoi(mapFiledsNameVal["UploadLogSecond"].c_str());
        sTaskSQL.msTaskConfig.mnUploadTestLogSec = atoi(mapFiledsNameVal["UploadTestLogSecond"].c_str());

        return true;
    }
}
