#include "StdAfx.h"
#include "TSE_UpdateSQLite3DB.h"
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>

namespace JGW
{
    CTSE_UpdateSQLite3DB::CTSE_UpdateSQLite3DB(void)
    {
    }


    CTSE_UpdateSQLite3DB::~CTSE_UpdateSQLite3DB(void)
    {
    }

    const wchar_t* CTSE_UpdateSQLite3DB::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_UpdateSQLite3DB\",\
                \"TestName\": \"更新SQLite3DB数据信息\",\
                \"SQLiteDBPath\": \"SQLiteDB路径，相对于当前程序目录\",\
                \"SQLiteUpdateSQL\": \"SQLiteDB数据库更新SQL语句，$xxxx$格式表示当前字段引用了环境字段，$xxxx$会解析成xxxx对应的环境变量值\"\
                }";
    }

    bool CTSE_UpdateSQLite3DB::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SQLiteDBPath"))
        {
            mstrSQLiteDBPath = JGW_W2A(JGW_RealativePathToAbsPath(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SQLiteUpdateSQL"))
        {
            mstrSQLiteUpdateSQL = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_UpdateSQLite3DB::BuildUpdateSQLStat()
    {
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
            JGW_FormatWString(strEnvironment,L"%%%s%%",vstrEnvironment[i].c_str());
            JGW_ReplaceStringW(mstrSQLiteUpdateSQL,strEnvironment,GetGlobalEnvironment()->GetString(vstrEnvironment[i]));
        }

        return true;
    }

    bool CTSE_UpdateSQLite3DB::TSE_Run()
    {
        BuildUpdateSQLStat();
        CCJGW_SQLite sqliteDB;
        if (!sqliteDB.OpenSQLite3(mstrSQLiteDBPath)) return false;

        LogI_F(L"Update SQL: [%s]\n",mstrSQLiteUpdateSQL.c_str());
        if (!sqliteDB.UpdateSQLite3(JGW_W2A(mstrSQLiteUpdateSQL)))
        {
            return false;
        }

        return true;
    }
}