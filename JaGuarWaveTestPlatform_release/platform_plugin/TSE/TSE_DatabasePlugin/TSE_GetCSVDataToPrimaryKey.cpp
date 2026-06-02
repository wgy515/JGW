#include "StdAfx.h"
#include "TSE_GetCSVDataToPrimaryKey.h"
#include <JGW_FoundationFunc/rapidcsv.hpp>
namespace JGW
{
    CTSE_GetCSVDataToPrimaryKey::CTSE_GetCSVDataToPrimaryKey(void) : mstrCSVInstanceEnvironment (L"TSE_CSV_INSTANCE")
    {
    }


    CTSE_GetCSVDataToPrimaryKey::~CTSE_GetCSVDataToPrimaryKey(void)
    {
    }

    const wchar_t* CTSE_GetCSVDataToPrimaryKey::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_GetCSVDataToPrimaryKey\",\
                \"TestName\": \"获取CSV数据资源\",\
                \"PrimaryValueEnvironment\":\"主键对应的环境变量\",\
                \"CSVInstanceEnvironment\":\"保存解析的CSV DOC环境变量\",\
                \"ColumnNames\":\"列名数组\",\
                \"SaveColumnEnvironmentValues\":\"将列名的值保存到对应环境变量数组中\"\
                }";
    }

    bool CTSE_GetCSVDataToPrimaryKey::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveColumnEnvironmentValues"))
        {
            mvstrSaveColumnEnvironmentValues.clear();
            JGW_ParserStrW(strParamValue,L",",mvstrSaveColumnEnvironmentValues);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_GetCSVDataToPrimaryKey::TSE_Run()
    {
        rapidcsv::Document* pscvDoc = (rapidcsv::Document*)GetGlobalEnvironment()->GetInt(mstrCSVInstanceEnvironment);
        if (NULL == pscvDoc)
        {
            LogE(L"csv document is uninitialized");
            return false;
        }

        std::string strPrimaryValue = GetGlobalEnvironment()->GetAnsiString(mstrPrimaryValueEnvironment);
        if (mvstrColumnNames.size() != mvstrSaveColumnEnvironmentValues.size())
        {
            LogE(L"Column Parameter configuration error");
            return false;
        }
        std::wstring strNumberInfo;
        std::wstring value;
        for (size_t index = 0;index < mvstrColumnNames.size();index++)
        {
            try
            {
                value = JGW_A2W(pscvDoc->GetCell<std::string>(mvstrColumnNames[index], strPrimaryValue));
                GetGlobalEnvironment()->PutString(mvstrSaveColumnEnvironmentValues[index],value);
                LogI_F(L"%s\t%s",mvstrSaveColumnEnvironmentValues[index].c_str(),value.c_str());
                LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s\t%s",mvstrSaveColumnEnvironmentValues[index].c_str(),value.c_str());
            }
            catch (...)
            {
                LogE(L"Get Column Environment Value Fail ..");
                return false;
            }
            if (strNumberInfo.empty())
            {
                strNumberInfo = value;
            }
            else
            {
                strNumberInfo += L",";
                strNumberInfo += value;
            }
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strNumberInfo.c_str());

        return true;
    }
}

