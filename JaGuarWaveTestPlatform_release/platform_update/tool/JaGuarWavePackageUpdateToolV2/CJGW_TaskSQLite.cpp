#include "StdAfx.h"
#include "CJGW_TaskSQLite.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_TaskSQLite::CCJGW_TaskSQLite(void)
    {
    }


    CCJGW_TaskSQLite::~CCJGW_TaskSQLite(void)
    {
    }

    bool CCJGW_TaskSQLite::LoadTaskSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_TASK>& vsTask)
    {
        std::string strSelectProjectSQL ("SELECT * FROM TASK");
        std::vector<std::vector<std::string>> vvFiledsNameValue;
        vsTask.clear();
        //! 
        if (!sqliteDB.ExecSQLite3(strSelectProjectSQL,vvFiledsNameValue))
        {
            return false;
        }

        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,installVersionIndex = -1,otaIDIndex = -1,configIDIndex = -1,taskConfigIDIndex = -1,nameIndex = -1,desIndex = -1,createDateIndex = -1;
        for (std::vector<std::vector<std::string>>::iterator it = vvFiledsNameValue.begin();
            it != vvFiledsNameValue.end();
            ++ it)
        {
            if (it == vvFiledsNameValue.begin())
            {
                for (size_t index = 0;index < it[0].size();index++)
                {
                    if (0 == JGW_StrComparenoCaseStr("ID",it[0][index].c_str()))
                    {
                        idIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("InstallVersionID",it[0][index].c_str()))
                    {
                        installVersionIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("OTAID",it[0][index].c_str()))
                    {
                        otaIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("ConfigID",it[0][index].c_str()))
                    {
                        configIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("TaskConfigID",it[0][index].c_str()))
                    {
                        taskConfigIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("NAME",it[0][index].c_str()))
                    {
                        nameIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("DES",it[0][index].c_str()))
                    {
                        desIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("CreateDate",it[0][index].c_str()))
                    {
                        createDateIndex = (int)index;
                    }
                }
                if (idIndex < 0 
                    || installVersionIndex < 0 
                    || otaIDIndex < 0
                    || configIDIndex < 0
                    || taskConfigIDIndex < 0
                    || nameIndex < 0 
                    || desIndex < 0 
                    || createDateIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                S_TASK sTask;
                sTask.mnID = atoi(it[0][idIndex].c_str());
                sTask.mnInstallVersionID = atoi(it[0][installVersionIndex].c_str());
                sTask.mnOTAID = atoi(it[0][otaIDIndex].c_str());
                sTask.mnConfigID = atoi(it[0][configIDIndex].c_str());
                sTask.mnTaskConfigID = atoi(it[0][configIDIndex].c_str());
                sTask.mstrName = it[0][nameIndex].c_str();
                sTask.mstrDes = it[0][desIndex].c_str();
                sTask.mstrCreateDate = it[0][createDateIndex].c_str();
                vsTask.push_back(sTask);
            }
        }
        return true;
    }

    bool CCJGW_TaskSQLite::UpdateTaskSQLite(CCJGW_SQLite& sqliteDB,const S_TASK &sTask)
    {
        std::string strUpdateTaskSQL;
        //! UPDATE TASK SET InstallVersionID=2,ConfigID=3,OTAID=2 where NAME='DHW206996A';
        JGW_FormatString(strUpdateTaskSQL,"UPDATE TASK SET InstallVersionID=%d,ConfigID=%d,DES='%s' where ID=%d"
            ,sTask.mnInstallVersionID,sTask.mnConfigID,sTask.mstrDes.c_str(),sTask.mnID);
        if (sTask.mnID <= 0 || !sqliteDB.UpdateSQLite3(strUpdateTaskSQL))
        {
            return false;
        }
        return true;
    }

    bool CCJGW_TaskSQLite::AddTaskSQLite(CCJGW_SQLite& sqliteDB,const S_TASK &sTask)
    {
        std::string strAddTaskSQL;
        //! INSERT INTO TASK(NAME,DES,InstallVersionID,OTAID,ConfigID,TaskConfigID) values ('DHW222310','PRM2141-04 2022-05-26 ',5,-1,14,1);
        JGW_FormatString(strAddTaskSQL,"INSERT INTO TASK(NAME,DES,InstallVersionID,OTAID,ConfigID,TaskConfigID) values ('%s','%s ',%d,%d,%d,%d)"
            ,sTask.mstrName.c_str(),sTask.mstrDes.c_str(),sTask.mnInstallVersionID,sTask.mnOTAID,sTask.mnConfigID,sTask.mnTaskConfigID);
        if (!sqliteDB.InsertSQLite3(strAddTaskSQL))
        {
            return false;
        }
        return true;
    }
}
