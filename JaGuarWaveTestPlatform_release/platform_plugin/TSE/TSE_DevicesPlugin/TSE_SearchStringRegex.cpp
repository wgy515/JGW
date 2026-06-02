#include "StdAfx.h"
#include "TSE_SearchStringRegex.h"
#include <regex>
namespace JGW
{
	CTSE_SearchStringRegex::CTSE_SearchStringRegex(void) : mbIgnoreCase(false),mnSearchRegexIndex(0)
	{
	}


	CTSE_SearchStringRegex::~CTSE_SearchStringRegex(void)
	{
	}

	const wchar_t* CTSE_SearchStringRegex::TSE_GetParamDescription()
	{
		return L"";
	}

	bool CTSE_SearchStringRegex::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
		else if (TSE_PARAM_NAME_EQUAL(L"SearchRegexIndex",strParamName))
		{
			mnSearchRegexIndex = (size_t)_ttoi(strParamValue);
		}
		else if (TSE_PARAM_NAME_EQUAL(L"SearchStringRegex",strParamName))
		{
			mstrSearchStringRegex = strParamValue;
		}
		else if (TSE_PARAM_NAME_EQUAL(L"SaveSearchStringEnvironment",strParamName))
		{
			mstrSaveSearchStringEnvironment = strParamValue;
		}
		else
		{
			return false;
		}
		return true;
	}

	bool CTSE_SearchStringRegex::TSE_Run()
	{
		std::wstring strSrcString = GetGlobalEnvironment()->GetString(mstrSrcStringEnvironment),strSearchString;

		std::wsmatch match;
		std::wregex pattern(mstrSearchStringRegex,mbIgnoreCase ? std::regex_constants::icase : std::regex_constants::ECMAScript);
		if (!regex_search(strSrcString,match,pattern))
		{
			LogE_F(L"Error Read: %s",strSrcString.c_str());
			return false;
		}

		if (mnSearchRegexIndex < match.size())
		{
			strSearchString = match[mnSearchRegexIndex];
			LogI_F(L"%s : %s",mstrSaveSearchStringEnvironment.c_str(),strSearchString.c_str());
			LogU_F(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"%s : %s",mstrSaveSearchStringEnvironment.c_str(),strSearchString.c_str());
			LogU(E_TSE_SHOW_CONTENT_INFO,strSearchString.c_str());
			GetGlobalEnvironment()->PutString(mstrSaveSearchStringEnvironment,strSearchString);
		}
		else
		{
			LogE_F(L"Error Read: %s",strSrcString.c_str());
			return false;
		}
		return true;
	}
}
