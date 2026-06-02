// JGW_SQLiteDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <sqlite3/sqlite3.h>
#include <JGW_FoundationFunc/CUtf8String.hpp>
#include <string>
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? W(argv[i]) : "NULL");
    }
    printf("\n");
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
//     JGW::CCJGW_SQLite csql;
//     std::vector<std::string> vFiledsName;
//     //std::vector<std::string> vFiledsValue;
//     std::vector<std::vector<std::string>> vFiledsValue;
//     std::map<std::string,std::string> mFiledsInfo;
//     csql.OpenSQLite3("test.db");
//     csql.SelectSQLite3("select SN,455 from LYNX_IMEI where IMEI='863408025040941'",mFiledsInfo);
    std::wstring mstrSQLiteUpdateSQL = L"UPDATE LYNX_IMEI SET SN='%TSE_READ_SN_NUMBER%' WHERE IMEI='%TSE_IMEI_NUMBER%%1222%";
    std::wstring strSQLUpdate(mstrSQLiteUpdateSQL),strEnvironment;
    int start = mstrSQLiteUpdateSQL.find('%'),end = 0;
    std::vector<std::wstring> vstrEnvironment;
    while (std::wstring::npos != start)
    {
        end = mstrSQLiteUpdateSQL.find('%',start + 1);
        vstrEnvironment.push_back(mstrSQLiteUpdateSQL.substr(start + 1,end - start - 1));
        start = mstrSQLiteUpdateSQL.find('%',end + 1);
    }

    for (size_t i = 0;i < vstrEnvironment.size();i ++)
    {
        //JGW_FormatWString(strEnvironment,L"%s%",vstrEnvironment[i].c_str());
        //JGW_ReplaceStringW(mstrSQLiteUpdateSQL,strEnvironment,GetGlobalEnvironment()->GetString(vstrEnvironment[i]));
    }

#if 0
    sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        //exit(0);
    }else{
        fprintf(stdout, "Opened database successfully\n");
    }
    //!

    std::string strsql = "UPDATE LYNX_IMEI set SN = '中国' where IMEI='8634025040941'";
        
        //"select * from LYNX_IMEI where IMEI='863408025040941'";
        //！U("UPDATE LYNX_IMEI set SN = '中国' where IMEI='863408025040941'");

        //"select * from LYNX_IMEI where IMEI='863408025040941'";

        //SELECT * from LYNX_IMEI";
    /* Create SQL statement */
//     sql = "CREATE TABLE COMPANY("  \
//         "ID INT PRIMARY KEY     NOT NULL," \
//         "NAME           TEXT    NOT NULL," \
//         "AGE            INT     NOT NULL," \
//         "ADDRESS        CHAR(50)," \
//         "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, strsql.c_str(), callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }else{
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
#endif
    return 0;
}