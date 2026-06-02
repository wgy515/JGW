#include "StdAfx.h"
#include "CJGW_InstallVersionSQLite.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_InstallVersionSQLite::CCJGW_InstallVersionSQLite(void)
    {
    }


    CCJGW_InstallVersionSQLite::~CCJGW_InstallVersionSQLite(void)
    {
    }

    bool CCJGW_InstallVersionSQLite::LoadInstallVersionSQLite(CCJGW_SQLite& sqliteDB,std::vector<S_INTALLVERSION>& vsInstallVersion)
    {
        std::string strSelectInstallVersionSQL ("SELECT * FROM INSTALLVERSION");
        std::vector<std::vector<std::string>> vvFiledsNameValue;
        vsInstallVersion.clear();
        //! 
        if (!sqliteDB.ExecSQLite3(strSelectInstallVersionSQL,vvFiledsNameValue))
        {
            return false;
        }

        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,nameIndex = -1,desIndex = -1,createDateIndex = -1,versionIndex = -1,md5Index = -1,ftpUrlIndex = -1;
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
                    else if (0 == JGW_StrComparenoCaseStr("Version",it[0][index].c_str()))
                    {
                        versionIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("MD5",it[0][index].c_str()))
                    {
                        md5Index = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("FTPUrl",it[0][index].c_str()))
                    {
                        ftpUrlIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("CreateDate",it[0][index].c_str()))
                    {
                        createDateIndex = (int)index;
                    }
                }
                if (idIndex < 0 || nameIndex < 0 || desIndex < 0 || createDateIndex < 0 
                    || versionIndex < 0 || md5Index < 0 || ftpUrlIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                S_INTALLVERSION sIntallVersion;
                sIntallVersion.mnID = atoi(it[0][idIndex].c_str());
                sIntallVersion.mstrName = JGW_A2W(it[0][nameIndex],CP_UTF8);
                sIntallVersion.mstrDes = JGW_A2W(it[0][desIndex],CP_UTF8);
                sIntallVersion.mstrCreateDate = JGW_A2W(it[0][createDateIndex],CP_UTF8);
                sIntallVersion.mstrVersion = JGW_A2W(it[0][versionIndex],CP_UTF8);
                sIntallVersion.mstrMD5 = JGW_A2W(it[0][md5Index],CP_UTF8);
                sIntallVersion.mstrFTPUrl = JGW_A2W(it[0][ftpUrlIndex],CP_UTF8);
                vsInstallVersion.push_back(sIntallVersion);
            }
        }
        return true;
    }
}
