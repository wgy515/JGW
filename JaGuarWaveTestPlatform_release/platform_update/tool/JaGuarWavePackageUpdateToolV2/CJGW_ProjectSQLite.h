#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
#include "JGWPackageUpdateDefine.h"

namespace JGW
{
    class CCJGW_ProjectSQLite
    {
    public:
        CCJGW_ProjectSQLite(void);
        ~CCJGW_ProjectSQLite(void);
    public:
        bool LoadProjectSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_PROJECT>& vsProject);

        bool GetProjectDBToProjectID(CCJGW_SQLite& sqliteDB,int projectID,S_PROJECT& sProject);
        bool AddProjectInfo(CCJGW_SQLite& sqliteDB,const S_PROJECT& sProject);
        bool UpdateProjectInfo(CCJGW_SQLite& sqliteDB,const S_PROJECT& sProject);
    };
}

