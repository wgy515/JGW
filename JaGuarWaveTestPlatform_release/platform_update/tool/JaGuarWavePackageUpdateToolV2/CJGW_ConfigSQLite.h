#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    class CCJGW_ConfigSQLite
    {
    public:
        CCJGW_ConfigSQLite(void);
        ~CCJGW_ConfigSQLite(void);
    public:
        bool LoadConfigSQLite(CCJGW_SQLite& sqliteDB);
        bool GetConfigDBToConfigID(CCJGW_SQLite& sqliteDB,int configID,S_CONFIG& sConfig);
        bool UpdateConfigDBToConfigID(CCJGW_SQLite& sqliteDB,int configID,const S_CONFIG& sConfig);

        bool GetConfigDBToProjectID(CCJGW_SQLite& sqliteDB,int projectID,std::vector<S_CONFIG>& vsConfig);

        bool AddConfigDBToConfigInfo(CCJGW_SQLite& sqliteDB,const S_CONFIG& sConfig);

        std::vector<S_CONFIG>& GetConfigInfos();
    private:
        std::vector<S_CONFIG> mvsConfig;

    };
}