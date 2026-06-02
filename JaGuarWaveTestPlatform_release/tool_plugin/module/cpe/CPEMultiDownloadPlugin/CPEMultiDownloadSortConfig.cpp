#include "StdAfx.h"
#include "CPEMultiDownloadSortConfig.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
	CCPEMultiDownloadSortConfig::CCPEMultiDownloadSortConfig(void) : mpsMultiDownloadConfig(NULL)
	{
	}


	CCPEMultiDownloadSortConfig::~CCPEMultiDownloadSortConfig(void)
	{
	}

	void CCPEMultiDownloadSortConfig::InitDownloadPortSortConfig(LPS_MULTI_DOWNLOAD_CONFIG psMultiDownloadConfig,const std::wstring& strDownloadPortSortConfigFilePath)
	{
		std::wstring strTemp;
		mpsMultiDownloadConfig = psMultiDownloadConfig;
        m_cInifConfig.InitIniFilePath(strDownloadPortSortConfigFilePath.c_str());

		if (INVALID_FILE_ATTRIBUTES == GetFileAttributesW(strDownloadPortSortConfigFilePath.c_str()))
		{
			for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
			{
				JGW_FormatWString(strTemp,L"DEV_%d",i + 1);
				m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"DownloadComPort",-1);
				m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"DownloadSortIndex",-1);
				m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"SerialComPort",-1);
				m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"SerialSortIndex",-1);
				m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"CPEIPAddr",JGW_GetFormatWString(L"192.168.1.%d",i + 102).c_str());
			}
		}

		for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
		{
			JGW_FormatWString(strTemp,L"DEV_%d",i + 1);
			psMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDonwloadComPort = m_cInifConfig.GetIniKeyIntValue(strTemp.c_str(),L"DownloadComPort",-1);
			psMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDonwloadSortIndex = m_cInifConfig.GetIniKeyIntValue(strTemp.c_str(),L"DownloadSortIndex",-1);
			psMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnSerialComPort = m_cInifConfig.GetIniKeyIntValue(strTemp.c_str(),L"SerialComPort",-1);
			psMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnSerialSortIndex = m_cInifConfig.GetIniKeyIntValue(strTemp.c_str(),L"SerialSortIndex",-1);
			psMultiDownloadConfig->msMultiDownloadSortConfigs[i].mstrCPEIPAddr = m_cInifConfig.GetIniKeyValue(strTemp.c_str(),L"CPEIPAddr",JGW_GetFormatWString(L"192.168.1.%d",i + 102).c_str());
		}
	}
	
	int CCPEMultiDownloadSortConfig::GetMultiDownloadSortIndexToDeviceMsg(SDeviceMsg_Info* psDeviceMsgInfo)
	{
		if (psDeviceMsgInfo->m_nSortIndex< 0)
		{
			return -1;
		}

		for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
		{
			if (psDeviceMsgInfo->m_nSortIndex == mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDonwloadSortIndex
				/*|| psDeviceMsgInfo->m_nSortIndex == mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnSerialSortIndex*/)
			{
				return mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDownLoadIndex;
			}
		}
		return -1;
	}

	void CCPEMultiDownloadSortConfig::SaveMultiDownloadSortConfig()
	{
		std::wstring strTemp;
		for (int i = 0;i < DOWNLOAD_THREAD_MAX;i ++)
		{
			JGW_FormatWString(strTemp,L"DEV_%d",i + 1);
			m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"DownloadComPort",mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDonwloadComPort);
			m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"DownloadSortIndex",mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnDonwloadSortIndex);
			m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"SerialComPort",mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnSerialComPort);
			m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"SerialSortIndex",mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mnSerialSortIndex);
			m_cInifConfig.SetIniKeyValue(strTemp.c_str(),L"CPEIPAddr",mpsMultiDownloadConfig->msMultiDownloadSortConfigs[i].mstrCPEIPAddr.c_str());
		}
	}
}
