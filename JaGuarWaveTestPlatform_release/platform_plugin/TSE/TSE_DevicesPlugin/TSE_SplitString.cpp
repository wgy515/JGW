#include "StdAfx.h"
#include "TSE_SplitString.h"
#include <regex>
namespace JGW
{
    CTSE_SplitString::CTSE_SplitString(void) : mstrSeparator(L",")
    {
    }


    CTSE_SplitString::~CTSE_SplitString(void)
    {
    }

    const wchar_t* CTSE_SplitString::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SplitString\",\
                \"TestName\": \"Split String\",\
                \"Separator\": \"def:,\",\
                \"SrcStringEnvironment\": \"Source String Environment\",\
                \"ParamValueRegexRule%d\": \"正则表达式规则 ParamValueRegexRule1,ParamValueRegexRule2....\",\
                \"SaveStringEnvironmentList\": \"Save String Environmen Example:TSE_NUMBER,TSE_MAC\"\
                }";
    }

    bool CTSE_SplitString::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(L"Separator",strParamName))
        {
            mstrSeparator = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SrcStringEnvironment",strParamName))
        {
            mstrSrcStringEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(L"SaveStringEnvironmentList",strParamName))
        {
            mstrSaveStringEnvironmentList = strParamValue;
        }
        else
        {
            if (NULL != strParamName && strParamValue != NULL && NULL != JGW_WStrCaseWStr(strParamName,L"ParamValueRegexRule"))
            {
                mmapstrParamValuesRegexRule[strParamName] = strParamValue;
                return true;
            }
            return false;
        }
        return true;
    }

    bool CTSE_SplitString::TSE_Run()
    {
        std::vector<std::wstring> vstrEnvName,vstrEnvValue;

        std::wstring strSrcString = GetGlobalEnvironment()->GetString(mstrSrcStringEnvironment);
        JGW_ParserStrW(strSrcString.c_str(),mstrSeparator.c_str(),vstrEnvValue);
        JGW_ParserStrW(mstrSaveStringEnvironmentList.c_str(),L",",vstrEnvName);
        /*if (vstrEnvName.size() != vstrEnvValue.size())
        {
        LogE(L"Environment Name Count not equal to Environment Value Count");
        return false;
        }*/

        std::wstring strTemp,strValue;
        for (size_t i = 0;i < vstrEnvName.size();i++)
        {
            //! 
            if (i < vstrEnvValue.size())
            {
                strValue = vstrEnvValue[i];
                GetGlobalEnvironment()->PutString(vstrEnvName[i],strValue); 
            }
            else
            {
                strValue = L"";
                GetGlobalEnvironment()->PutString(vstrEnvName[i],strValue);
            }
            LogI_F(L"%s : %s",vstrEnvName[i].c_str(),strValue.c_str());

            JGW_FormatWString(strTemp,L"ParamValueRegexRule%d",i + 1);
            std::map<std::wstring,std::wstring>::iterator it = mmapstrParamValuesRegexRule.find(strTemp);
            if (it != mmapstrParamValuesRegexRule.end() && !it->second.empty())
            {
                std::wregex partten(it->second);
                if (!regex_match(strValue,partten))
                {
                    LogE_F(L"param:%s,rule:%s,value:%s,regex_match fail",vstrEnvName[i].c_str(),it->second.c_str(),strValue.c_str());
                    return false;
                }
            }
        }

        return true;
    }
}
