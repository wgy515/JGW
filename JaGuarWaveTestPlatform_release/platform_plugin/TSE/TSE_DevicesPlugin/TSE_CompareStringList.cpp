#include "StdAfx.h"
#include "TSE_CompareStringList.h"

namespace JGW
{
    CTSE_CompareStringList::CTSE_CompareStringList(void) : mbIgnoreCase(true)
    {
    }


    CTSE_CompareStringList::~CTSE_CompareStringList(void)
    {
    }

    const wchar_t* CTSE_CompareStringList::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_CompareStringList\",\
				\"TestName\": \"Compare String List\",\
				\"IgnoreCase\": \"IgnoreCase def:true\",\
				\"CompareStrEnvironment\": \"String Environment\",\
				\"StringListCommand\": \"String List Command Example:good,cool\"\
				}";
    }

    bool CTSE_CompareStringList::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"CompareStrEnvironment",strParamName))
        {
            mstrCompareStrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"IgnoreCase",strParamName))
        {
            mbIgnoreCase = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"StringListCommand",strParamName))
        {
            mstrStringListCommand = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_CompareStringList::TSE_Run()
    {
        std::wstring strStringList (mstrStringListCommand);
        GetEnvironmentCommandString(strStringList);
        std::wstring strCompareStr = GetGlobalEnvironment()->GetString(mstrCompareStrEnvironment),strTemp;

        std::vector<std::wstring> vstrStringList;
        JGW_ParserStrW(strStringList.c_str(),L",",vstrStringList);
        for (size_t i = 0;i < vstrStringList.size();i ++)
        {
            if (mbIgnoreCase)
            {
                if (0 == JGW_WStrComparenoCaseWStr(strCompareStr.c_str(),vstrStringList[i].c_str()))
                {
                    JGW_FormatWString(strTemp,L"%s,%s",vstrStringList[i].c_str(),strCompareStr.c_str());
                    LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
                    return true;
                }
            }
            else
            {
                if (0 == vstrStringList[i].compare(strCompareStr))
                {
                    JGW_FormatWString(strTemp,L"%s,%s",vstrStringList[i].c_str(),strCompareStr.c_str());
                    LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
                    return true;
                }
            }
        }
        LogE_F(L"Compare String List Fail src:%s,dest:%s",strStringList.c_str(),strCompareStr.c_str());

        return false;
    }
}
