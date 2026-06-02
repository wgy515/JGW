#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
	class CTSE_SearchStringRegex : public CTSE_TestBase
	{
	public:
		CTSE_SearchStringRegex(void);
		~CTSE_SearchStringRegex(void);
	private:
		const wchar_t* TSE_GetParamDescription();
		bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
		bool TSE_Run();
	private:
		bool mbIgnoreCase;
		// 搜索正则表达式索引
		size_t mnSearchRegexIndex;
		// 原始字符串环境变量
		std::wstring mstrSrcStringEnvironment;
		// 搜索正则表达式
		std::wstring mstrSearchStringRegex;
		//
		std::wstring mstrSaveSearchStringEnvironment;
	};
}


