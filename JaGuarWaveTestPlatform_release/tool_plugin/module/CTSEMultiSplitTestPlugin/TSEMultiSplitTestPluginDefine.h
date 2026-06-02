#pragma once
#include <string>

#define TSE_MULTI_PLUGIN_APP_CONFIG_NAME L"TSEMultiPluginAppConfig.ini"
namespace JGW
{
	typedef enum
	{
		E_NUMBER_OF_MULTIPLE_TEST_1,
		E_NUMBER_OF_MULTIPLE_TEST_2,
		E_NUMBER_OF_MULTIPLE_TEST_4,
		E_NUMBER_OF_MULTIPLE_TEST_8,
	} E_NUMBER_OF_MULTIPLE_TEST;

	typedef struct
	{
		int mnNumberOfMultipleTests;
		//int mnViewLoaderID;
        //HWND mhMainWnd;
		//std::wstring mstrStationID;
		//std::wstring mstrClsid;
		std::wstring mstrModuleFolder;
		//std::wstring mstrPluginName;
		//std::wstring mstrStationName;
		std::wstring mstrSuiteName;
        std::wstring mstrSuiteXmlConfigPath;
	}S_MultiSplitTestConfig,*PS_MultiSplitTestConfig;

}
