#include "StdAfx.h"
#include "CJGW_TaskConfigSQLite.h"

namespace JGW
{
    CCJGW_TaskConfigSQLite::CCJGW_TaskConfigSQLite(void)
    {
        //!  : mnID(0),mnDebugModeMSec(0),mnUploadLogSecond(0),mnUploadTestLogSecond(300),mstrName("DEFAULT")
    }

    CCJGW_TaskConfigSQLite::~CCJGW_TaskConfigSQLite(void)
    {
    }

    bool CCJGW_TaskConfigSQLite::LoadTaskConfigSQLite(CCJGW_SQLite& sqliteDB)
    {
        return false;
    }

    std::vector<S_TASK_CONFIG>& CCJGW_TaskConfigSQLite::GetTaskConfigInfos()
    {
        return mvsTaskConfigs;
    }
}

