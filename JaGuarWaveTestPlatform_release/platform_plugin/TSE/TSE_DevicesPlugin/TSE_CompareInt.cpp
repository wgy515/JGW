#include "StdAfx.h"
#include "TSE_CompareInt.h"

namespace JGW
{
    CTSE_CompareInt::CTSE_CompareInt(void)
    {
    }


    CTSE_CompareInt::~CTSE_CompareInt(void)
    {
    }

    const wchar_t* CTSE_CompareInt::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_CompareInt\",\
				\"TestName\": \"Compare Int String\",\
				\"IntRangeListCommand\": \"Int String Range List Example:1~5 || 8~10\",\
				\"CompareIntEnvironment\": \"Int String Environment\"\
				}";
    }

    bool CTSE_CompareInt::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"IntRangeListCommand",strParamName))
        {
            mstrIntRangeListCommand = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CompareIntEnvironment",strParamName))
        {
            mstrCompareIntEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CompareInt::TSE_Run()
    {
        __int64 nCompareInt = GetGlobalEnvironment()->GetInt64(mstrCompareIntEnvironment);
        std::wstring strIntRangeList (mstrIntRangeListCommand);
        GetEnvironmentCommandString(strIntRangeList);
        std::wstring strRate = GetGlobalEnvironment()->GetString(mstrCompareIntEnvironment + L"_RATE");
        double dRate = _ttof(strRate.c_str());
        
        std::vector<std::wstring> vCompareIntRangeList;
        JGW_ParserStrW(strIntRangeList.c_str(),L"||",vCompareIntRangeList);
        if (0 == vCompareIntRangeList.size())
        {
            LogE(L"CompareIntRangeList Config Fail");
            return false;
        }

        __int64 min = 0,max  = 0;
        for (size_t i = 0;i < vCompareIntRangeList.size();i ++)
        {
            if (std::wstring::npos != vCompareIntRangeList[i].find(L"~"))
            {
                _stscanf_s(vCompareIntRangeList[i].c_str(),L"%I64d~%I64d",&min,&max);
				GetGlobalEnvironment()->PutInt64(mstrCompareIntEnvironment + L"_MIN",min);
				GetGlobalEnvironment()->PutInt64(mstrCompareIntEnvironment + L"_MAX",max);
                if (strRate.empty())
                {
                    LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%I64d(%I64d-%I64d)",nCompareInt,min,max);
                }
                else
                {
                    //! divide
                    GetGlobalEnvironment()->PutDouble(mstrCompareIntEnvironment + L"_DIVIDE_RATE",nCompareInt / dRate);
                    GetGlobalEnvironment()->PutDouble(mstrCompareIntEnvironment + L"_DIVIDE_RATE_MIN",min / dRate);
                    GetGlobalEnvironment()->PutDouble(mstrCompareIntEnvironment + L"_DIVIDE_RATE_MAX",max / dRate);
                    LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%f(%f-%f)",nCompareInt / dRate,min / dRate,max / dRate);
                }
                if (nCompareInt < min || nCompareInt > max)
                {
#if 0
                    LogE_F(L"out of range ,Value:%I64d,Min:%I64d,Max:%I64d",nCompareInt,min,max);
#else
                    
                    if (strRate.empty())
                    {
                        LogE_F(L"out of range ,Value:%I64d,Min:%I64d,Max:%I64d",nCompareInt,min,max);
                    }
                    else
                    {
                        LogE_F(L"out of range ,Value:%f,Min:%f,Max:%f",nCompareInt / dRate,min / dRate,max / dRate);
                    }
#endif
                    return false;
                }
            }
            else
            {
                LogE(L"CompareIntRangeList Config Fail");
                return false;
            }
        }
        return true;
    }
}

