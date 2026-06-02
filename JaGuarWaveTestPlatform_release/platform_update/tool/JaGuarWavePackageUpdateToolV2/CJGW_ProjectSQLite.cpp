#include "StdAfx.h"
#include "CJGW_ProjectSQLite.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_ProjectSQLite::CCJGW_ProjectSQLite(void)
    {
    }


    CCJGW_ProjectSQLite::~CCJGW_ProjectSQLite(void)
    {
    }
    /*
    CREATE TABLE [PROJECT] (
    [ID] integer  PRIMARY KEY AUTOINCREMENT NULL,
        [NAME] TEXT  UNIQUE NOT NULL,
        [DES] TEXT  NULL,
        [CreateDate] datetime default (datetime('now', 'localtime'))    -- 时间
        )
    */
    bool CCJGW_ProjectSQLite::LoadProjectSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_PROJECT>& vsProject)
    {
        std::string strSelectProjectSQL ("SELECT * FROM PROJECT");
        std::vector<std::vector<std::string>> vvFiledsNameValue;
        vsProject.clear();
        //! 
        if (!sqliteDB.ExecSQLite3(strSelectProjectSQL,vvFiledsNameValue))
        {
            return false;
        }

        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,nameIndex = -1,desIndex = -1,createDateIndex = -1;
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
                if (idIndex < 0 || nameIndex < 0 || desIndex < 0 || createDateIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                S_PROJECT sProject;
                sProject.mnID = atoi(it[0][idIndex].c_str());
                sProject.mstrName = JGW_A2W(it[0][nameIndex],CP_UTF8);
                sProject.mstrDes = JGW_A2W(it[0][desIndex],CP_UTF8);
                sProject.mstrCreateData = JGW_A2W(it[0][createDateIndex],CP_UTF8);
                vsProject.push_back(sProject);
            }
        }
        return true;
    }

    bool CCJGW_ProjectSQLite::AddProjectInfo(CCJGW_SQLite& sqliteDB,const S_PROJECT& sProject)
    {
        std::wstring strInsertProjectSQL;

        JGW_FormatWString(strInsertProjectSQL,L"INSERT INTO PROJECT(NAME,DES) values ('%s','%s')",sProject.mstrName.c_str(),sProject.mstrDes.c_str());
        return sqliteDB.InsertSQLite3(JGW_W2A(strInsertProjectSQL,CP_UTF8));
    }

    bool CCJGW_ProjectSQLite::UpdateProjectInfo(CCJGW_SQLite& sqliteDB,const S_PROJECT& sProject)
    {
        std::wstring strUpdateProjectSQL;
        JGW_FormatWString(strUpdateProjectSQL,L"UPDATE PROJECT SET NAME='%s',DES='%s' WHERE ID=%d",sProject.mstrName.c_str(),sProject.mstrDes.c_str(),sProject.mnID);
        return sqliteDB.UpdateSQLite3(JGW_W2A(strUpdateProjectSQL,CP_UTF8));
    }

    bool CCJGW_ProjectSQLite::GetProjectDBToProjectID(CCJGW_SQLite& sqliteDB,int projectID,S_PROJECT& sProject)
    {
        std::string strSelectProjectSQL;
        std::map<std::string,std::string> mapFiledsNameVal;

        JGW_FormatString(strSelectProjectSQL,"SELECT * FROM PROJECT WHERE ID=%d",projectID);
        //! strSelectTaskSQL = "SELECT * FROM TASK";
        if (projectID <= 0 || !sqliteDB.SelectSQLite3(strSelectProjectSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }
        sProject.mnID = projectID;
        sProject.mstrName = JGW_A2W(mapFiledsNameVal["NAME"],CP_UTF8);
        sProject.mstrDes = JGW_A2W(mapFiledsNameVal["DES"],CP_UTF8);;
        sProject.mstrCreateData = JGW_A2W(mapFiledsNameVal["CreateDate"],CP_UTF8);

        return true;
    }
}
