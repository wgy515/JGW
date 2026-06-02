#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
#include "JGWPackageUpdateDefine.h"
namespace JGW
{
    class CCJGW_InstallVersionSQLite
    {
    public:
        CCJGW_InstallVersionSQLite(void);
        ~CCJGW_InstallVersionSQLite(void);
    public:
        bool LoadInstallVersionSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_INTALLVERSION>& vsInstallVersion);
    };
}


