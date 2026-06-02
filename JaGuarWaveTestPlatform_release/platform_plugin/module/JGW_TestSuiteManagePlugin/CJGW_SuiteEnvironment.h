#pragma once
#include <string>

namespace JGW
{
	class CCJGW_SuiteEnvironment
	{
	public:
		static void SetSuiteEnvironmentVar( const wchar_t* envirname,const wchar_t* envirvalue );
		static std::wstring GetSuiteEnvironmentVar( const wchar_t* envirname );
	};
};

