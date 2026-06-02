#include "StdAfx.h"
#include "TSE_LoadCSVDatabase.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_LoadCSVDatabase::CTSE_LoadCSVDatabase(void) : mstrCSVInstanceEnvironment(L"TSE_CSV_INSTANCE"),mscvDoc("",rapidcsv::LabelParams(0, 0)),mnPrimaryColumnValueIndex(0),mnPrimaryColumnNameRowIndex(0)
    {
    }


    CTSE_LoadCSVDatabase::~CTSE_LoadCSVDatabase(void)
    {
    }

    const wchar_t* CTSE_LoadCSVDatabase::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_LoadCSVDatabase\",\
                \"TestName\": \"加载CSV数据资源\",\
                \"CSVFilePathCommand\":\"CSV文件路径COMMAND\",\
                \"CSVInstanceEnvironment\":\"保存解析的CSV DOC环境变量\",\
                \"PrimaryColumnValueIndex\":\"配置主键列值的索引\",\
                \"PrimaryColumnNameRowIndex\":\"配置主键列名称的行索引\"\
                }";
    }

    bool CTSE_LoadCSVDatabase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"CSVFilePathCommand"))
        {
            mstrCSVFilePathCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CSVInstanceEnvironment"))
        {
            mstrCSVInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryColumnValueIndex"))
        {
            mnPrimaryColumnValueIndex = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryColumnNameRowIndex"))
        {
            mnPrimaryColumnNameRowIndex = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_LoadCSVDatabase::TSE_Run()
    {
        std::wstring strCSVFilePath (mstrCSVFilePathCommand);
        GetEnvironmentCommandString (strCSVFilePath);
        strCSVFilePath = JGW_RealativePathToAbsPath(strCSVFilePath.c_str());
        //! 判断加载的文件路径是否有变化以及当前CSV实例保存的环境变量是否被清空
        if (0 == JGW_WStrComparenoCaseWStr(strCSVFilePath.c_str(),mstrFilePath.c_str()) && NULL != GetGlobalEnvironment()->GetInt(mstrCSVInstanceEnvironment))
        {
            return true;
        }
        //! 检查CSV文件是否存在 
        if (!JGW_FileExistsToFilePath(strCSVFilePath.c_str()))
        {
            LogE_F("The file(%s) is not exist ",strCSVFilePath.c_str());
            mscvDoc.Clear();
            GetGlobalEnvironment()->PutInt(mstrCSVInstanceEnvironment,0);
            return false;
        }

        std::string strCSVFilePathA = JGW_W2A(strCSVFilePath,CP_UTF8);
        mscvDoc.Load(strCSVFilePathA,rapidcsv::LabelParams(mnPrimaryColumnNameRowIndex, mnPrimaryColumnValueIndex));
        GetGlobalEnvironment()->PutInt(mstrCSVInstanceEnvironment,(int)(&mscvDoc));
        return true;
    }
}
