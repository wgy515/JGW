#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
#include "JGWPackageUpdateDefine.h"
namespace JGW
{
    class CCJGW_TaskSQLite
    {
    public:
        CCJGW_TaskSQLite(void);
        ~CCJGW_TaskSQLite(void);
    public:
        bool LoadTaskSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_TASK>& vsTask);

        bool UpdateTaskSQLite(CCJGW_SQLite& sqliteDB,const S_TASK &sTask);

        bool AddTaskSQLite(CCJGW_SQLite& sqliteDB,const S_TASK &sTask);
    };
}
