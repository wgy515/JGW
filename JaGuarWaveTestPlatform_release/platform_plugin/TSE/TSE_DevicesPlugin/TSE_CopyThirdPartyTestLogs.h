#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
	class CTSE_CopyThirdPartyTestLogs : public CTSE_TestBase
	{
	public:
		CTSE_CopyThirdPartyTestLogs(void);
		~CTSE_CopyThirdPartyTestLogs(void);
	private:
		const wchar_t* TSE_GetParamDescription();
		bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
		bool TSE_Run();
	private:
		//! 是否开始记录
		static bool mbStartRecording;
		//! 是否停止记录
		//static bool mbEndRecording;
		//! 0 : 开始记录 1 : 停止记录
		int mnRecordingIndex;
		//! 原第三方测试日志目录环境变量
		std::wstring mstrSrcThirdPartyTestLogsFolderEnvironmentName;
		//! TestLog\$$_TSE_TEST_PROJECT_NAME_$$\2018-8-2\
		// std::wstring mstrSaveTestLogsFolder;
		//! ^result_.*?.csv$ 匹配的日志名称正则表达式
		std::wstring mstrMatchLogNameRegex;
		//! log日志名称替换 DUT1,$$_TSE_BRACODE_$$;DUT2,$$_TSE_BRACODE_$$
		//std::wstring mstrLogNameReplaceConfigCommand;
		std::map<std::wstring,std::wstring> mmapLogNameReplaceConfigCommand;
	};

}

