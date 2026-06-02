#include "StdAfx.h"
#include "TSE_BuildString.h"
#include <regex>
namespace JGW
{
    CTSE_BuildString::CTSE_BuildString(void)
    {
    }


    CTSE_BuildString::~CTSE_BuildString(void)
    {
    }

    const wchar_t* CTSE_BuildString::TSE_GetParamDescription()
    {
		return L"{\
				\"RealName\": \"CTSE_BuildString\",\
				\"TestName\": \"Build String\",\
				\"EraseStringSplit\": \"delete string split example:AT+GSN,OK,\\r,\\n, \",\
				\"SrcStringCommand\": \"src string environment commad\",\
				\"RegexRule\": \"regex rule \",\
				\"SaveEnvironment\": \"save string environment\"\
				}";
    }

    bool CTSE_BuildString::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if( TSE_PARAM_NAME_EQUAL(_T("EraseStringSplit"),strParamName) )
        {
            mstrEraseStringSplit = strParamValue;
        }
        else if( TSE_PARAM_NAME_EQUAL(_T("SrcStringCommand"),strParamName) )
        {
            mstrSrcStringCommand = strParamValue;
        }
		else if (TSE_PARAM_NAME_EQUAL(_T("RegexRule"),strParamName))
		{
			mstrRegexRule = strParamValue;
		}
        else if( TSE_PARAM_NAME_EQUAL(_T("SaveEnvironment"),strParamName) )
        {
            mstrSaveEnvironment = strParamValue;
        }
        else
        {
            return false;
        }      
        return true;
    }

    bool CTSE_BuildString::TSE_Init()
    {
        std::wstring strEraseStringSplit (mstrEraseStringSplit);
        JGW_ReplaceStringW(strEraseStringSplit,L"\\\\r",L"\r");
        JGW_ReplaceStringW(strEraseStringSplit,L"\\\\n",L"\n");
        JGW_ReplaceStringW(strEraseStringSplit,L"\\\\t",L"\t");
        JGW_ReplaceStringW(strEraseStringSplit,L"\\\\,",L"\''");
        mvstrEraseString.clear();
        JGW_ParserStrW(strEraseStringSplit.c_str(),L",",mvstrEraseString);
        for (size_t i = 0;i < mvstrEraseString.size();i ++)
        {
            JGW_ReplaceStringW(mvstrEraseString[i],L"\''",L",");
        }
        return true;
    }

    bool CTSE_BuildString::TSE_Run()
    {
        std::wstring strSrcString(mstrSrcStringCommand);

        GetEnvironmentCommandString(strSrcString);
        for (size_t i = 0;i < mvstrEraseString.size();i ++)
        {
            JGW_ReplaceStringW(strSrcString,mvstrEraseString[i],L"");
        }
		if (!mstrRegexRule.empty())
		{
			std::wregex partten(mstrRegexRule);
			if (!regex_match(strSrcString,partten))
			{
				LogE_F(L"rule:%s,value:%s,regex_match fail",mstrRegexRule.c_str(),strSrcString.c_str());
				return false;
			}
		}
        GetGlobalEnvironment()->PutString(mstrSaveEnvironment,strSrcString);
        LogI_F(L"%s : %s",mstrSaveEnvironment.c_str(),strSrcString.c_str());
        return true;
    }
}

