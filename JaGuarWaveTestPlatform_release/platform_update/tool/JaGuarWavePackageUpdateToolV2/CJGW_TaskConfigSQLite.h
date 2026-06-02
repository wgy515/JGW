#pragma once
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <vector>
namespace JGW
{
    struct S_TASK_CONFIG
    {
        int mnID;
        //! TDO:
        int mnDebugModeMSec;
        //! TDO:
        int mnUploadLogSecond;
        //! TDO: 上传测试log时间
        int mnUploadTestLogSecond;
        //！Task Config Name
        std::string mstrName;
    };
    class CCJGW_TaskConfigSQLite
    {
    public:
        CCJGW_TaskConfigSQLite(void);
        ~CCJGW_TaskConfigSQLite(void);
    public:
        bool LoadTaskConfigSQLite(CCJGW_SQLite& sqliteDB);
        std::vector<S_TASK_CONFIG>& GetTaskConfigInfos();
    private:
        std::vector<S_TASK_CONFIG> mvsTaskConfigs;
    };
}
