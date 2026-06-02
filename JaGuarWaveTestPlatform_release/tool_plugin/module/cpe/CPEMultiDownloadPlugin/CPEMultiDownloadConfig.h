#pragma once
#include "MultiDownloadDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
	class CCPEMultiDownloadConfig
	{
	public:
		CCPEMultiDownloadConfig(void);
		~CCPEMultiDownloadConfig(void);
	public:
		void LoadMultiDownloadConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,const std::wstring& strDownloadConfigFilePath);
		void SettingTftp32BaseDirectory(const std::wstring& strBaseDirectory);

		void SaveMultiDownloadConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig);
	private:
        std::wstring mstrTftpd32ConfigFilePath;
		CCJGW_ConfigIni mConfigIni;
	};

}


