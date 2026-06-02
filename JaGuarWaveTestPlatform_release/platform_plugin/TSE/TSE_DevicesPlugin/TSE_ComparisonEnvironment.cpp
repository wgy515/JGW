#include "StdAfx.h"
#include "TSE_ComparisonEnvironment.h"
#include "TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_ComparisonEnvironment::CTSE_ComparisonEnvironment(void) : mstrFormat(L"Src:%s,DestS:%s，比对的变量不一致"),mbIgnoreString(false)
    {
    }


    CTSE_ComparisonEnvironment::~CTSE_ComparisonEnvironment(void)
    {
    }

    const wchar_t* CTSE_ComparisonEnvironment::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ComparisonEnvironment\",\
                \"TestName\": \"比较环境变量值\",\
                \"SrcEnvironment\": \"原环境变量\",\
                \"DestEnvironment\": \"目的环境变量\",\
                \"Format\": \"格式化提示\",\
                \"IgnoreString\": \"忽略指定字符串\"\
                }";
    }

    bool CTSE_ComparisonEnvironment::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SrcEnvironment",strParamName))
        {
            mstrSrcEnvironmentName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"DestEnvironment",strParamName))
        {
            mstrDestEnvironmentName = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"Format",strParamName))
        {
            mstrFormat = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"IgnoreString",strParamName))
        {
            mbIgnoreString = true;
            mstrIgnoreString = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ComparisonEnvironment::TSE_Run()
    {
        std::wstring strSrc = GetGlobalEnvironment()->GetString(mstrSrcEnvironmentName),strDest = GetGlobalEnvironment()->GetString(mstrDestEnvironmentName),strTemp;

        if (mbIgnoreString)
        {
            JGW_ReplaceStringW(strSrc,mstrIgnoreString,L"");
            JGW_ReplaceStringW(strDest,mstrIgnoreString,L"");
        }
        JGW_FormatWString(strTemp,L"%s,%s",strSrc.c_str(),strDest.c_str());
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());

        if (NULL == JGW_WStrComparenoCaseWStr(strSrc.c_str(),strDest.c_str()))
        {
            return  true;
        }
        LogE_F(mstrFormat.c_str(),strSrc.c_str(),strDest.c_str());
        return false;
    }
}