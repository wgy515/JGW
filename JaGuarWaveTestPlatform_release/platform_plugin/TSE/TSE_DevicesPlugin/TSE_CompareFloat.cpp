#include "StdAfx.h"
#include "TSE_CompareFloat.h"

namespace JGW
{
    CTSE_CompareFloat::CTSE_CompareFloat(void)
    {
    }


    CTSE_CompareFloat::~CTSE_CompareFloat(void)
    {
    }

    const wchar_t* CTSE_CompareFloat::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_CompareFloat\",\
				\"TestName\": \"Compare Float String\",\
				\"CompareFloatRangeList\": \"Float String Range List Example:1.0~5.0 || 8.0~10.0\",\
				\"CompareFloatEnvironment\": \"Float String Environment\"\
				}";
    }

    bool CTSE_CompareFloat::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"CompareFloatRangeList",strParamName))
        {
            mstrCompareFloatRangeList = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"CompareFloatEnvironment",strParamName))
        {
            mstrCompareFloatEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CompareFloat::TSE_Run()
    {
        float nCompareFloat = GetGlobalEnvironment()->GetFloat(mstrCompareFloatEnvironment);
        std::vector<std::wstring> vCompareIntRangeList;
        JGW_ParserStrW(mstrCompareFloatRangeList.c_str(),L"||",vCompareIntRangeList);
        float min = 0,max  = 0;

        for (size_t i = 0;i < vCompareIntRangeList.size();i ++)
        {
            if (std::wstring::npos != vCompareIntRangeList[i].find(L"~"))
            {
                _stscanf_s(vCompareIntRangeList[i].c_str(),L"%f~%f",&min,&max);
                LogU_F(E_TSE_SHOW_CONTENT_INFO,L"%.2f(%.2f-%.2f)",nCompareFloat,min,max);
                if (nCompareFloat < min || nCompareFloat > max)
                {
                    LogE_F(L"out of range ,Value:%.2f,Min:%.2f,Max:%.2f",nCompareFloat,min,max);
                    return false;
                }
            }
            else
            {
                LogE(L"CompareFloatRangeList Config Fail");
                return false;
            }
        }
        return true;
    }

}
