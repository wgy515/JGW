#include "StdAfx.h"
#include "TSE_CloseCSVDatabase.h"
#include <JGW_FoundationFunc/rapidcsv.hpp>
namespace JGW
{
    CTSE_CloseCSVDatabase::CTSE_CloseCSVDatabase(void) : mstrCSVInstanceEnvironment(L"TSE_CSV_INSTANCE")
    {
    }


    CTSE_CloseCSVDatabase::~CTSE_CloseCSVDatabase(void)
    {
    }

    const wchar_t* CTSE_CloseCSVDatabase::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_CloseCSVDatabase\",\
                \"TestName\": \"关闭CSV数据资源\",\
                \"CSVInstanceEnvironment\":\"保存解析的CSV DOC环境变量\"\
                }";
    }

    bool CTSE_CloseCSVDatabase::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"CSVInstanceEnvironment"))
        {
            mstrCSVInstanceEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CloseCSVDatabase::TSE_Run()
    {
        rapidcsv::Document* pscvDoc = (rapidcsv::Document*)GetGlobalEnvironment()->GetInt(mstrCSVInstanceEnvironment);
        if (NULL != pscvDoc)
        {
            pscvDoc->Clear();
            GetGlobalEnvironment()->PutInt(mstrCSVInstanceEnvironment,0);
        }
        return true;
    }
}
