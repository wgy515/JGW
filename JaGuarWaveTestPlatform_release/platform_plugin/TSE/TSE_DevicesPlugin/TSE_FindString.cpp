#include "StdAfx.h"
#include "TSE_FindString.h"

namespace JGW
{
    CTSE_FindString::CTSE_FindString(void) : mbIgnoreCase(true)
    {
    }


    CTSE_FindString::~CTSE_FindString(void)
    {

    }

    const wchar_t* CTSE_FindString::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FindString\",\
                \"TestName\": \"²éÕÒ×Ö·û´®\",\
                \"IgnoreCase\": \"IgnoreCase def:true\",\
                \"SrcStringEnvironment\": \"Ô´×Ö·û´®\",\
                \"FindString\": \"ÐèÒª²éÕÒµÄ×Ö·û´®\"\
                }";
    }

    bool CTSE_FindString::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"SrcStringEnvironment",strParamName))
        {
            mstrSrcStringEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"IgnoreCase",strParamName))
        {
            mbIgnoreCase = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(L"FindString",strParamName))
        {
            mstrFindString = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_FindString::TSE_Run()
    {
        std::wstring strSrcString = GetGlobalEnvironment()->GetString(mstrSrcStringEnvironment);
        if (mbIgnoreCase)
        {
            if (NULL == JGW_WStrCaseWStr(strSrcString.c_str(),mstrFindString.c_str()))
            {
                return false;
            }
            
        }
        else
        {
            if (std::wstring::npos == strSrcString.find(mstrFindString.c_str()))
            {
                return false;
            }
        }
        return true;
    }
}
