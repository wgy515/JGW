#include "StdAfx.h"
#include "TSE_LoadExcelDatabase.h"

namespace JGW
{
    CTSE_LoadExcelDatabase::CTSE_LoadExcelDatabase(void) : mstrEnvironmentExcelFilePath(L"TSE_NUMBER_EXCEL_FILE_PATH"),mstrEnvironmentExcelContent(L"TSE_NUMBER_EXCEL_CONTENT")
    {
    }

    CTSE_LoadExcelDatabase::~CTSE_LoadExcelDatabase(void)
    {
    }

    const wchar_t* CTSE_LoadExcelDatabase::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_LoadExcelDatabase\",\
                \"TestName\": \"加载EXCEL文件到内存中\",\
                \"EnvironmentExcelFilePath\":\"excel文件路径基于当前程序目录，保存在环境变量中\",\
                \"EnvironmentExcelContent\":\"解析EXCEL后将数据保存在对应环境变量中\"\
                }";
    }

    bool CTSE_LoadExcelDatabase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentExcelFilePath"))
        {
            mstrEnvironmentExcelFilePath = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentExcelContent"))
        {
            mstrEnvironmentExcelContent = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_LoadExcelDatabase::TSE_Run()
    {
        std::wstring strTemp = JGW_RealativePathToAbsPath(GetGlobalEnvironment()->GetString(mstrEnvironmentExcelFilePath).c_str());

        if (!strTemp.empty() && NULL == strTemp.compare(msExcelContent.mstrExcelFilePath))
        {
            LogI(L"当前EXCEL已经加载成功!!!\n");
            return true;
        }
        GetGlobalEnvironment()->PutInt(mstrEnvironmentExcelContent,NULL);
        if (!mcLoadExcelDataBase.LoadExcelDatabBase(strTemp,msExcelContent))
        {
            return false;
        }
        msExcelContent.mstrExcelFilePath = strTemp;
        GetGlobalEnvironment()->PutInt(mstrEnvironmentExcelContent,(int)(&msExcelContent));

        return true;
    }
}