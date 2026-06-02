#pragma once
#include "MultiDownloadDefine.h"
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
	class CCPEMultiDownloadSortConfig
	{
	public:
		CCPEMultiDownloadSortConfig(void);
		~CCPEMultiDownloadSortConfig(void);
	public:
		//! 初始化下载排序索引配置文件
		void InitDownloadPortSortConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,const std::wstring& strDownloadPortSortConfigFilePath);
		//! 根据设备信息，或者当前多路下载排序索引
		int GetMultiDownloadSortIndexToDeviceMsg(SDeviceMsg_Info* psDeviceMsgInfo);
		//! 保存排序相关信息
		void SaveMultiDownloadSortConfig();
	private:
		LPS_MULTI_DOWNLOAD_CONFIG mpsMultiDownloadConfig;
		CCJGW_ConfigIni	m_cInifConfig;
	};
}

