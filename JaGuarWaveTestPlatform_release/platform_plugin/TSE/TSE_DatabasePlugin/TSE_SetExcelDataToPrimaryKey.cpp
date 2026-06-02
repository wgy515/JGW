#include "StdAfx.h"
#include "TSE_SetExcelDataToPrimaryKey.h"
#include <JGW_FoundationFunc/rapidcsv.hpp>

namespace JGW
{
    CTSE_SetExcelDataToPrimaryKey::CTSE_SetExcelDataToPrimaryKey(void) : mstrCSVInstanceEnvironment (L"TSE_CSV_INSTANCE")
    {
    }


    CTSE_SetExcelDataToPrimaryKey::~CTSE_SetExcelDataToPrimaryKey(void)
    {
    }

    const wchar_t* CTSE_SetExcelDataToPrimaryKey::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SetExcelDataToPrimaryKey\",\
                \"TestName\": \"设置主键EXCEL对应的内容\",\
                \"PrimaryValueEnvironment\":\"主键对应的环境变量\",\
                \"CSVInstanceEnvironment\":\"保存解析的CSV DOC环境变量\",\
                \"ColumnNames\":\"列名数组\",\
                \"SetColumnEnvironmentValues\":\"将环境变量值设置到对应列名的值中\",\
                \"SaveCSVFilePath\":\"保存CSV文件路径\"\
                }";
    }

    bool CTSE_SetExcelDataToPrimaryKey::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PrimaryValueEnvironment"))
        {
            mstrPrimaryValueEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CSVInstanceEnvironment"))
        {
            mstrCSVInstanceEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ColumnNames"))
        {
            std::string paramValue = JGW_W2A(strParamValue);
            mvstrColumnNames.clear();
            JGW_ParserStrA(paramValue.c_str(),",",mvstrColumnNames);

        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SetColumnEnvironmentValues"))
        {
            mvstrSetColumnEnvironmentValues.clear();
            JGW_ParserStrW(strParamValue,L",",mvstrSetColumnEnvironmentValues);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveCSVFilePath"))
        {
            mstrSaveCSVFilePath = JGW_RealativePathToAbsPath(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SetExcelDataToPrimaryKey::TSE_Run()
    {
        rapidcsv::Document* pscvDoc = (rapidcsv::Document*)GetGlobalEnvironment()->GetInt(mstrCSVInstanceEnvironment);
        if (NULL == pscvDoc)
        {
            LogE(L"csv document is uninitialized");
            return false;
        }

        std::string strPrimaryValue = GetGlobalEnvironment()->GetAnsiString(mstrPrimaryValueEnvironment);
        if (mvstrColumnNames.size() != mvstrSetColumnEnvironmentValues.size())
        {
            LogE(L"Column Parameter configuration error");
            return false;
        }

        for (size_t index = 0;index < mvstrColumnNames.size();index++)
        {
            try
            {
                std::string value = GetGlobalEnvironment()->GetAnsiString(mvstrSetColumnEnvironmentValues[index]);
                pscvDoc->SetCell<std::string>(mvstrColumnNames[index], strPrimaryValue,value);
                LogI_F(L"Set %s\t%s",mvstrSetColumnEnvironmentValues[index].c_str(),JGW_A2W(value).c_str());
                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"Set %s\t%s",mvstrSetColumnEnvironmentValues[index].c_str(),JGW_A2W(value).c_str());
            }
            catch (...)
            {
                LogE(L"Set Column Environment Value Fail ..");
                return false;
            }
        }
        try
        {
            pscvDoc->Save(JGW_W2A(mstrSaveCSVFilePath,CP_UTF8));
        }
        catch (...)
        {
            LogE(L"save csv file Fail ..");
            return false;
        }
        return true;
    }
}

