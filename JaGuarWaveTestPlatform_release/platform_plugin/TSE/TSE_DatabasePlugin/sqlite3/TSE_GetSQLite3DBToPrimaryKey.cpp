#include "StdAfx.h"
#include "TSE_GetSQLite3DBToPrimaryKey.h"
#include <JGW_WindowsFuncPlugin/CJGW_SQLite.h>

namespace JGW
{
    CTSE_GetSQLite3DBToPrimaryKey::CTSE_GetSQLite3DBToPrimaryKey(void)
    {
    }


    CTSE_GetSQLite3DBToPrimaryKey::~CTSE_GetSQLite3DBToPrimaryKey(void)
    {
    }

    const wchar_t* CTSE_GetSQLite3DBToPrimaryKey::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetSQLite3DBToPrimaryKey\",\
                \"TestName\": \"获取SQLITE DB 主键对应相关信息\",\
                \"SQLiteDBPath\": \"SQLiteDB路径，相对于当前程序目录\",\
                \"SQLiteDBTableName\": \"SQLiteDB数据库表名\",\
                \"SQLiteTableHeader\": \"SQLiteDB数据库表字段名称，以,作为分隔符\",\
                \"EnvironmentValues\": \"查找字段对应数据保存环境变量组，与SQLiteTableHeader一一对应,以,作为分隔符\",\
                \"PrimaryKey\": \"SQLiteDB数据库表字段名称,主KEY表字段\",\
                \"PrimaryValueEnvironment\": \"主KEY对应的环境变量值\"\
                }";
    }


    bool CTSE_GetSQLite3DBToPrimaryKey::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SQLiteTableHeader"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrSQLiteHeader);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentValues"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrEnvironmentValues);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryKey"))
        {
            mstrPrimaryKey = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryValueEnvironment"))
        {
            mstrPrimaryValueEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SQLiteDBPath"))
        {
            mstrSQLiteDBPath = JGW_W2A(JGW_RealativePathToAbsPath(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SQLiteDBTableName"))
        {
            mstrSQLiteDBTableName = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetSQLite3DBToPrimaryKey::TSE_Run()
    {
        if (mvstrSQLiteHeader.size() != mvstrEnvironmentValues.size())
        {
            LogE(L"SQLite表头与对应变量名称数值必须一致");
            return false;
        }

        CCJGW_SQLite sqliteDB;
        if (!sqliteDB.OpenSQLite3(mstrSQLiteDBPath)) return false;
        std::map<std::string,std::string> mapFilesRow;
        std::wstring strSQL;
        JGW_FormatWString(strSQL,L"SELECT * FROM %s WHERE %s='%s'",mstrSQLiteDBTableName.c_str(),mstrPrimaryKey.c_str(),GetGlobalEnvironment()->GetString(mstrPrimaryValueEnvironment).c_str());
        LogI_F(L"SQL: [%s]\n",strSQL.c_str());
        if (!sqliteDB.SelectSQLite3(JGW_W2A(strSQL),mapFilesRow))
        {
            return false;
        }
        //! 未找到数据
        if (mapFilesRow.empty())
        {
            LogE_F(L"本地数据库中未查找到当前%s:%s数据 fail \n",mstrPrimaryKey.c_str(),GetGlobalEnvironment()->GetString(mstrPrimaryValueEnvironment).c_str());
            return false;
        }

        for (size_t i = 0;i < mvstrSQLiteHeader.size();i ++)
        {
            //! 查找当前主键信息
            std::string strKeyValue = JGW_W2A(mvstrSQLiteHeader.at(i));
            if (mapFilesRow.end() == mapFilesRow.find(strKeyValue))
            {
                LogE_F(L"未查找到当前表头信息(%s)",mvstrSQLiteHeader.at(i));
                return false;
            }
            GetGlobalEnvironment()->PutString(mvstrEnvironmentValues[i],JGW_A2W(mapFilesRow[strKeyValue]));
        }

        return true;
    }
}