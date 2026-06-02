#include "StdAfx.h"
#include "CJGW_OTASQLite.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_OTASQLite::CCJGW_OTASQLite(void)
    {
    }


    CCJGW_OTASQLite::~CCJGW_OTASQLite(void)
    {
    }

    bool CCJGW_OTASQLite::LoadOTASQLite(CCJGW_SQLite& sqliteDB)
    {
        return false;
    }

    std::vector<S_OTA>& CCJGW_OTASQLite::GetOTASQLiteInfos()
    {
        return mvsOta;
    }

    bool CCJGW_OTASQLite::GetOTAInfosToInstallVersionID(CCJGW_SQLite& sqliteDB,const int installVersionID,std::vector<S_OTA>& vsOta)
    {
        std::string strSelectOTASQL;
        std::vector<std::vector<std::string>> vvFiledsNameValue;

        vsOta.clear();
        JGW_FormatString(strSelectOTASQL,"SELECT * FROM OTA WHERE InstallVersionID=%d",installVersionID);
        if (!sqliteDB.ExecSQLite3(strSelectOTASQL,vvFiledsNameValue))
        {
            return false;
        }
        if (vvFiledsNameValue.empty())
        {
            return false;
        }
        int idIndex = -1,versionIndex = -1,nameIndex = -1,desIndex = -1,md5Index = -1,ftpUrlIndex = -1,installVersionIDIndex = -1,isLatestVersionID,createDateIndex = -1;
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
                    else if (0 == JGW_StrComparenoCaseStr("InstallVersionID",it[0][index].c_str()))
                    {
                        installVersionIDIndex = (int)index;
                    }
                    else if (0 == JGW_StrComparenoCaseStr("IsLatestVersion",it[0][index].c_str()))
                    {
                        isLatestVersionID = (int)index;
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
                    || installVersionIDIndex < 0
                    || isLatestVersionID < 0
                    || createDateIndex < 0)
                {
                    return false;
                }
            }
            else
            {
                S_OTA sOTA;
                sOTA.mnID = atoi(it[0][idIndex].c_str());
                sOTA.mstrVersion = JGW_A2W(it[0][versionIndex],CP_UTF8);
                sOTA.mstrName = JGW_A2W(it[0][nameIndex],CP_UTF8);
                sOTA.mstrDes = JGW_A2W(it[0][desIndex],CP_UTF8);
                sOTA.mstrMD5 = JGW_A2W(it[0][md5Index],CP_UTF8);
                sOTA.mstrFTPUrl = JGW_A2W(it[0][ftpUrlIndex],CP_UTF8);
                sOTA.mnInstallVersionID = atoi(it[0][installVersionIDIndex].c_str());
                sOTA.mbIsLatestVersion = 1 == atoi(it[0][isLatestVersionID].c_str());
                sOTA.mstrCreateDate = JGW_A2W(it[0][createDateIndex],CP_UTF8);
                vsOta.push_back(sOTA);
            }
        }

        return true;
    }

    bool CCJGW_OTASQLite::UpdateOTADBToOTAInfo(CCJGW_SQLite& sqliteDB,const S_OTA& sOta)
    {
        //! update OTA set DES='UC600AA R3йт╡З50PCS',Version='Version20201223155488',MD5='57bf0011f1831f528c29431cdfb26b09',FTPUrl='/JGWPTP_Help/UpdateVersion/JaGuarWaveTestPlatform_V20201219/Version20201223155488.zip' where ID=2;
        std::wstring strUpdateOTASql;
        JGW_FormatWString(strUpdateOTASql,L"update OTA set DES='%s',Version='%s',MD5='%s',FTPUrl='%s',NAME='%s' where ID=%d",sOta.mstrDes.c_str(),sOta.mstrVersion.c_str(),sOta.mstrMD5.c_str(),sOta.mstrFTPUrl.c_str(),sOta.mstrName.c_str(),sOta.mnID);
        return sqliteDB.UpdateSQLite3(JGW_W2A(strUpdateOTASql,CP_UTF8));
    }

    bool CCJGW_OTASQLite::UpdateOTAIsLatestVersionToOTAInfo(CCJGW_SQLite& sqliteDB,const int installVersionID,bool isLatestVersion)
    {
        std::string strUpdateOTAIsLatestVersionSQL;
        JGW_FormatString(strUpdateOTAIsLatestVersionSQL,"update OTA set IsLatestVersion=%d where InstallVersionID=%d",isLatestVersion ? 1 : 0,installVersionID);
        return sqliteDB.UpdateSQLite3(strUpdateOTAIsLatestVersionSQL);
    }

    bool CCJGW_OTASQLite::InsertOTADBToOTAInfo(CCJGW_SQLite& sqliteDB,const S_OTA& sOta)
    {
        std::wstring strInsertOTASql;
        JGW_FormatWString(strInsertOTASql,L"INSERT INTO OTA(NAME,DES,Version,MD5,FTPUrl,IsLatestVersion,InstallVersionID) values ('%s','%s','%s','%s','%s',%d,%d)"
            ,sOta.mstrName.c_str(),sOta.mstrDes.c_str(),sOta.mstrVersion.c_str(),sOta.mstrMD5.c_str(),sOta.mstrFTPUrl.c_str(),sOta.mbIsLatestVersion ? 1 : 0,sOta.mnInstallVersionID);
        return sqliteDB.InsertSQLite3(JGW_W2A(strInsertOTASql,CP_UTF8));
    }
}
