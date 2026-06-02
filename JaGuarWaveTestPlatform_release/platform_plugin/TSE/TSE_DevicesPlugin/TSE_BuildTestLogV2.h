#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
	class CTSE_BuildTestLogV2 : public CTSE_TestBase
	{
	public:
		CTSE_BuildTestLogV2(void);
		~CTSE_BuildTestLogV2(void);
	private:
		const wchar_t* TSE_GetParamDescription();
		bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
		bool TSE_Run();
	private:
		int mnLineMaxStringLength;
		wchar_t* mszLineStringBuffer;
		//! xx.csv
		std::wstring mstrTemplateTestLogFilePath;
		//! TestLog/$$_TSE_TEST_PROJECT_NAME_$$/$$_TIME_YEAR_MON_DAY_$$/$$_TSE_TEST_RESULT_STRING_$$_$$_TSE_BARCODE_$$_$$_TSE_TIMESTAMP_$$.csv
		std::wstring mstrSaveTestLogFilePathCommand;
		//! TSE_SVAE_TEST_LOG_FILE_PATH
		std::wstring mstrSaveTestLogFilePathEnvironmentName;
	};
}
