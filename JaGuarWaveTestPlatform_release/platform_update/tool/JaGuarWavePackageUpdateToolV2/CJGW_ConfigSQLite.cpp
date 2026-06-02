#include "StdAfx.h"
#include "CJGW_ConfigSQLite.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_ConfigSQLite::CCJGW_ConfigSQLite(void)
    {
    }


    CCJGW_ConfigSQLite::~CCJGW_ConfigSQLite(void)
    {
    }
    /*
    CREATE TABLE [CONFIG] (
    [ID] integer  PRIMARY KEY AUTOINCREMENT NULL,
    [Version] VARCHAR(64)  UNIQUE NOT NULL,
    [NAME] TEXT  UNIQUE NOT NULL,
    [DES] TEXT  NULL,
    [MD5] varchar(32)  NULL,
    [FTPUrl] varchar(512)  NULL,
    [ProjectID] INT  NULL,
    [CreateDate] datetime DEFAULT '''''''datetime(''''''''now'''''''', ''''''''localtime'''''''')''''''' NULL
    )
    */
    bool CCJGW_ConfigSQLite::LoadConfigSQLite(CCJGW_SQLite& sqliteDB)
    {
        std::string strSelectProjectSQL ("SELECT * FROM CONFIG");
        std::vector<std::vector<std::string>> vvFiledsNameValue;

        //! 
        if (!sqliteDB.ExecSQLite3(strSelectProjectSQL,vvFiledsNameValue))
        {
            return false;
        }

        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,versionIndex = -1,nameIndex = -1,desIndex = -1,md5Index = -1,ftpUrlIndex = -1,projectIDIndex = -1,createDateIndex = -1;
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
                    else if (0 == JGW_StrComparenoCaseStr("Version",it[0][index].c_str()))
                    {
                        versionIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("NAME",it[0][index].c_str()))
                    {
                        nameIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("DES",it[0][index].c_str()))
                    {
                        desIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("MD5",it[0][index].c_str()))
                    {
                        md5Index = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("FTPUrl",it[0][index].c_str()))
                    {
                        ftpUrlIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("ProjectID",it[0][index].c_str()))
                    {
                        projectIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("CreateDate",it[0][index].c_str()))
                    {
                        createDateIndex = (int)index;
                    }
                }
                if (idIndex < 0 || versionIndex < 0 
                    || nameIndex < 0 
                    || desIndex < 0 
                    || md5Index < 0 
                    || ftpUrlIndex < 0
                    || projectIDIndex < 0
                    || createDateIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                //S_CONFIG sConfig;
                //sConfig.mnID = atoi(it[0][idIndex].c_str());
                //sConfig.mstrVersion =  it[0][versionIndex].c_str();
                //sConfig.mstrName = it[0][nameIndex].c_str();
                //sConfig.mstrDes = it[0][desIndex].c_str();
                //sConfig.mstrMD5 = it[0][md5Index].c_str();
                //sConfig.mstrFTPUrl = it[0][ftpUrlIndex].c_str();
                //sConfig.mnProjectID = atoi(it[0][projectIDIndex].c_str());
                //sConfig.mstrCreateDate = it[0][createDateIndex].c_str();
                //mvsConfig.push_back(sConfig);
            }
        }
        return true;
    }

    bool CCJGW_ConfigSQLite::GetConfigDBToConfigID(CCJGW_SQLite& sqliteDB,int configID,S_CONFIG& sConfig)
    {
        std::string strSelectConfigSQL;
        std::map<std::string,std::string> mapFiledsNameVal;

        JGW_FormatString(strSelectConfigSQL,"SELECT * FROM CONFIG WHERE ID=%d",configID);
        //! strSelectTaskSQL = "SELECT * FROM TASK";
        if (configID <= 0 || !sqliteDB.SelectSQLite3(strSelectConfigSQL,mapFiledsNameVal))
        {
            return false;
        }

        if (mapFiledsNameVal.empty())
        {
            return false;
        }
        sConfig.mnID = configID;
        sConfig.mnProjectID = atoi(mapFiledsNameVal["ProjectID"].c_str());
        sConfig.mstrVersion = JGW_A2W(mapFiledsNameVal["Version"],CP_UTF8);
        sConfig.mstrName = JGW_A2W(mapFiledsNameVal["NAME"],CP_UTF8);
        sConfig.mstrDes = JGW_A2W(mapFiledsNameVal["DES"],CP_UTF8);
        sConfig.mstrMD5 = JGW_A2W(mapFiledsNameVal["MD5"],CP_UTF8);
        sConfig.mstrFTPUrl = JGW_A2W(mapFiledsNameVal["FTPUrl"],CP_UTF8);
        sConfig.mstrCreateDate = JGW_A2W(mapFiledsNameVal["CreateDate"],CP_UTF8);

        return true;
    }

    bool CCJGW_ConfigSQLite::UpdateConfigDBToConfigID(CCJGW_SQLite& sqliteDB,int configID,const S_CONFIG& sConfig)
    {
        //! UPDATE CONFIG SET MD5='8a04742ade0cdbea3e55caec102136a8',Version='Config20220613202812',FTPUrl='/JGWPTP_Help/Project/PRM2141-04/Config20220613202812.zip' where ID=14;
        std::string strUpdateConfigSQL;

        JGW_FormatString(strUpdateConfigSQL,"UPDATE CONFIG SET NAME='%s',DES='%s',MD5='%s',Version='%s',FTPUrl='%s' where ID=%d",JGW_W2A(sConfig.mstrName,CP_UTF8).c_str(),JGW_W2A(sConfig.mstrDes,CP_UTF8).c_str(),JGW_W2A(sConfig.mstrMD5,CP_UTF8).c_str(),JGW_W2A(sConfig.mstrVersion,CP_UTF8).c_str(),JGW_W2A(sConfig.mstrFTPUrl,CP_UTF8).c_str(),configID);
        if (configID <= 0 || !sqliteDB.UpdateSQLite3(strUpdateConfigSQL))
        {
            return false;
        }
        return true;
    }

    bool CCJGW_ConfigSQLite::AddConfigDBToConfigInfo(CCJGW_SQLite& sqliteDB,const S_CONFIG& sConfig)
    {
        std::wstring strAddConfigSQL;
        //! INSERT INTO CONFIG(NAME,DES,MD5,Version,FTPUrl,ProjectID) values ('PTP6151-DHW217152','PTP6151 2021-09-24 DHW217152','79e6f2bd8a629a8c4d7d9165ba5d4a9c','Config20210924153866','/JGWPTP_Help/Project/PTP6151/Config20210924153866.zip',6);
        JGW_FormatWString(strAddConfigSQL,L"INSERT INTO CONFIG(NAME,DES,MD5,Version,FTPUrl,ProjectID) values ('%s','%s','%s','%s','%s','%d')"
            ,sConfig.mstrName.c_str()
            ,sConfig.mstrDes.c_str()
            ,sConfig.mstrMD5.c_str()
            ,sConfig.mstrVersion.c_str()
            ,sConfig.mstrFTPUrl.c_str()
            ,sConfig.mnProjectID);

        if (!sqliteDB.InsertSQLite3(JGW_W2A(strAddConfigSQL,CP_UTF8)))
        {
            return false;
        }
        return true;
    }

    std::vector<S_CONFIG>& CCJGW_ConfigSQLite::GetConfigInfos()
    {
        return mvsConfig;
    }

    bool CCJGW_ConfigSQLite::GetConfigDBToProjectID(CCJGW_SQLite& sqliteDB,int projectID,std::vector<S_CONFIG>& vsConfig)
    {
        std::string strSelectConfigSQL;
        std::vector<std::vector<std::string>> vvFiledsNameValue;

        vsConfig.clear();
        JGW_FormatString(strSelectConfigSQL,"SELECT * FROM CONFIG WHERE ProjectID=%d",projectID);
        if (!sqliteDB.ExecSQLite3(strSelectConfigSQL,vvFiledsNameValue))
        {
            return false;
        }
        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,versionIndex = -1,nameIndex = -1,desIndex = -1,md5Index = -1,ftpUrlIndex = -1,projectIDIndex = -1,createDateIndex = -1;
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
                    else if (0 == JGW_StrComparenoCaseStr("Version",it[0][index].c_str()))
                    {
                        versionIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("NAME",it[0][index].c_str()))
                    {
                        nameIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("DES",it[0][index].c_str()))
                    {
                        desIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("MD5",it[0][index].c_str()))
                    {
                        md5Index = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("FTPUrl",it[0][index].c_str()))
                    {
                        ftpUrlIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("ProjectID",it[0][index].c_str()))
                    {
                        projectIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("CreateDate",it[0][index].c_str()))
                    {
                        createDateIndex = (int)index;
                    }
                }
                if (idIndex < 0 || versionIndex < 0 
                    || nameIndex < 0 
                    || desIndex < 0 
                    || md5Index < 0 
                    || ftpUrlIndex < 0
                    || projectIDIndex < 0
                    || createDateIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                S_CONFIG sConfig;
                sConfig.mnID = atoi(it[0][idIndex].c_str());
                sConfig.mstrVersion = JGW_A2W(it[0][versionIndex],CP_UTF8);
                sConfig.mstrName = JGW_A2W(it[0][nameIndex],CP_UTF8);
                sConfig.mstrDes = JGW_A2W(it[0][desIndex],CP_UTF8);
                sConfig.mstrMD5 = JGW_A2W(it[0][md5Index],CP_UTF8);
                sConfig.mstrFTPUrl = JGW_A2W(it[0][ftpUrlIndex],CP_UTF8);
                sConfig.mnProjectID = atoi(it[0][projectIDIndex].c_str());
                sConfig.mstrCreateDate = JGW_A2W(it[0][createDateIndex],CP_UTF8);
                vsConfig.push_back(sConfig);
            }
        }

        return true;
    }
}
