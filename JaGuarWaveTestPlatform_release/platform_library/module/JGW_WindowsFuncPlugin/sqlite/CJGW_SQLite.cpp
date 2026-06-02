#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    typedef struct  
    {
        void* mpFiledName;
        void* mpFileVal;
    }S_SQLITE_KEY_VALUE,*PS_SQLITE_KEY_VALUE;

    CCJGW_SQLite::CCJGW_SQLite(void) : mpSQLiteDB(NULL)
    {
    }


    CCJGW_SQLite::~CCJGW_SQLite(void)
    {
        CloseSQLite3();
    }

    bool CCJGW_SQLite::OpenSQLite3(const std::string& strFileDBName)
    {
        if (SQLITE_OK != sqlite3_open(strFileDBName.c_str(), &mpSQLiteDB))
        {
            PELOG4WW_F(L"Can't open database(%s): %s",JGW_A2W(strFileDBName).c_str(), JGW_A2W_A(sqlite3_errmsg(mpSQLiteDB)).c_str());
            return false;
        }
        return true;
    }

    void CCJGW_SQLite::CloseSQLite3()
    {
        if (!mpSQLiteDB) return;
        sqlite3_close(mpSQLiteDB);
        mpSQLiteDB = NULL;
    }

    static int callbackEmpty(void* pParam, int argc, char **argv, char **azColName){
        return 0;
    }

    static int callbackSignalValue(void* pParam, int argc, char **argv, char **azColName){
        std::string* pstrFiledsValue = (std::string*)pParam;
        pstrFiledsValue[0] = argv[0] ? argv[0] : "";
        return 0;
    }

    static int callbackSignalRow(void* pParam, int argc, char **argv, char **azColName){
        PS_SQLITE_KEY_VALUE psSQLiteKeyVal = (PS_SQLITE_KEY_VALUE) pParam;
        ((std::vector<std::string>*)psSQLiteKeyVal->mpFiledName)->clear();
        ((std::vector<std::string>*)psSQLiteKeyVal->mpFileVal)->clear();
        for (int i = 0;i < argc;i ++)
        {
            ((std::vector<std::string>*)psSQLiteKeyVal->mpFiledName)->push_back(azColName[i]);
            ((std::vector<std::string>*)psSQLiteKeyVal->mpFileVal)->push_back(argv[i]?argv[i]:"");
        }
        return 0;
    }

    static int callbackMultiRow(void* pParam, int argc, char **argv, char **azColName){
        PS_SQLITE_KEY_VALUE psSQLiteKeyVal = (PS_SQLITE_KEY_VALUE) pParam;
        if(((std::vector<std::string>*)psSQLiteKeyVal->mpFiledName)->empty())
        {
            for (int i = 0;i < argc;i ++)
            {
                ((std::vector<std::string>*)psSQLiteKeyVal->mpFiledName)->push_back(azColName[i]);
            }
        }
        std::vector<std::string> vVals;
        for (int i = 0;i < argc;i ++)
        {
            vVals.push_back(argv[i]?argv[i]:"");
        }
        ((std::vector<std::vector<std::string>>*)psSQLiteKeyVal->mpFileVal)->push_back(vVals);
        return 0;
    }

    static int callbackMultiNameValRow(void* pParam, int argc, char **argv, char **azColName){
        PS_SQLITE_KEY_VALUE psSQLiteKeyVal = (PS_SQLITE_KEY_VALUE) pParam;
        //! ADD FiledsName
        if(((std::vector<std::vector<std::string>>*)psSQLiteKeyVal->mpFileVal)->empty())
        {
            std::vector<std::string> vVals;
            for (int i = 0;i < argc;i ++)
            {
                vVals.push_back(azColName[i]);
            }
            ((std::vector<std::vector<std::string>>*)psSQLiteKeyVal->mpFileVal)->push_back(vVals);
        }
        {
            std::vector<std::string> vVals;
            for (int i = 0;i < argc;i ++)
            {
                vVals.push_back(argv[i]?argv[i]:"");
            }
            ((std::vector<std::vector<std::string>>*)psSQLiteKeyVal->mpFileVal)->push_back(vVals);
        }  
        return 0;
    }

    bool CCJGW_SQLite::SelectSQLite3(const std::string& strSQL,std::string& strFiledsValue)
    {
        char *zErrMsg = NULL;
        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackSignalValue, &strFiledsValue, &zErrMsg))
        {
            PELOG4WW_F(L"Select SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    bool CCJGW_SQLite::InsertSQLite3(const std::string& strSQL)
    {
        char *zErrMsg = NULL;
        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackEmpty,NULL, &zErrMsg))
        {
            PELOG4WW_F(L"Insert SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    bool CCJGW_SQLite::UpdateSQLite3(const std::string& strSQL)
    {
        char *zErrMsg = NULL;
        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackEmpty,NULL, &zErrMsg))
        {
            PELOG4WW_F(L"Update SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    bool CCJGW_SQLite::SelectSQLite3(const std::string& strSQL,std::vector<std::string>& vFiledsName,std::vector<std::string>& vFiledsValue)
    {
        char *zErrMsg = NULL;
        S_SQLITE_KEY_VALUE sSQLiteKeyValue = {&vFiledsName,&vFiledsValue};
        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackSignalRow,&sSQLiteKeyValue, &zErrMsg))
        {
            PELOG4WW_F(L"Insert SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    bool CCJGW_SQLite::ExecSQLite3(const std::string& strSQL,std::vector<std::string>& vFiledsName,std::vector<std::vector<std::string>>& vvFiledsValue)
    {
        vFiledsName.clear();
        char *zErrMsg = NULL;
        S_SQLITE_KEY_VALUE sSQLiteKeyValue = {&vFiledsName,&vvFiledsValue};

        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackMultiRow,&sSQLiteKeyValue, &zErrMsg))
        {
            PELOG4WW_F(L"Exec SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    bool CCJGW_SQLite::ExecSQLite3(const std::string& strSQL,std::vector<std::vector<std::string>>& vvFiledsNameValue)
    {
        char *zErrMsg = NULL;
        vvFiledsNameValue.clear();
        S_SQLITE_KEY_VALUE sSQLiteKeyValue = {NULL,&vvFiledsNameValue};

        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackMultiNameValRow,&sSQLiteKeyValue, &zErrMsg))
        {
            PELOG4WW_F(L"Exec SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }

    static int callbackMultiMapNameValRow(void* pParam, int argc, char **argv, char **azColName){
        PS_SQLITE_KEY_VALUE psSQLiteKeyVal = (PS_SQLITE_KEY_VALUE) pParam;
        std::map<std::string, std::string>* psFiledInfo = (std::map<std::string, std::string>*)psSQLiteKeyVal->mpFileVal;
        psFiledInfo->clear();

        for (int i = 0;i < argc;i ++)
        {
           psFiledInfo->insert(std::map<std::string, std::string>::value_type(azColName[i],argv[i]?argv[i]:""));
        }
        return 0;
    }

    bool CCJGW_SQLite::SelectSQLite3(const std::string& strSQL,std::map<std::string, std::string>& vFiledsNameVal)
    {
        char *zErrMsg = NULL;
        vFiledsNameVal.clear();
        S_SQLITE_KEY_VALUE sSQLiteKeyValue = {NULL,&vFiledsNameVal};

        if (SQLITE_OK != sqlite3_exec(mpSQLiteDB, strSQL.c_str(), callbackMultiMapNameValRow,&sSQLiteKeyValue, &zErrMsg))
        {
            PELOG4WW_F(L"Select SQLite3 Fail: %s", JGW_A2W_A(zErrMsg).c_str());
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }
}