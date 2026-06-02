#include "StdAfx.h"
#include "CPEMultiDownloadThread.h"

#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
	const wchar_t g_szStartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";

	const wchar_t g_szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";

	const wchar_t g_szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";

	const wchar_t g_szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";

	CCPEMultiDownloadThread::CCPEMultiDownloadThread(void) : mpsMultiDownloadConfig(NULL),/*mpsMultiDownloadSortConfig(NULL),*/mpsMultiDownloadParam(NULL),mnDonwloadComPort(-1),mnSerialComPort(-1)
	{
	}


	CCPEMultiDownloadThread::~CCPEMultiDownloadThread(void)
	{
	}

	BEGIN_MYTHREAD_MESSAGE_MAP(CCPEMultiDownloadThread,CCMessageThread)
		ON_MYTHREAD_MESSGAE(DOWN_LOAD_THREAD_RUN_MSG,&CCPEMultiDownloadThread::OnRunDownloadThread)
	END_MYTHREAD_MESSAGE_MAP()

	void CCPEMultiDownloadThread::InitDownloadParam(LPS_MULTI_DOWNLOAD_CONFIG psMultiUpgradeConfig,LPS_MULTI_DOWNLOAD_VIEW_PARAM psMultiDownloadParam)
	{
		mpsMultiDownloadConfig = psMultiUpgradeConfig;
		/*mpsMultiDownloadSortConfig = psMultiDownloadSortConfig;*/
		mpsMultiDownloadParam = psMultiDownloadParam;
		mpsMultiDownloadSortConfig = &mpsMultiDownloadConfig->msMultiDownloadSortConfigs[mpsMultiDownloadParam->mnIndex - 1];
	}

	LPS_MULTI_DOWNLOAD_VIEW_PARAM CCPEMultiDownloadThread::GetMultiDownloadViewParam()
	{
		return mpsMultiDownloadParam;
	}

	void CCPEMultiDownloadThread::OnRunDownloadThread(WPARAM wParam,LPARAM lParam)
	{
		jgw_timer testtim;
		g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] = L"";

		if ( !StartDownloadProcess() || !RecvTestInfo())
		{
			mpsMultiDownloadParam->meDownloadStatus = E_ERROR_DOWNLOAD;
			return ;
		} 
		std::wstring strTemp;
		JGW_FormatWString(strTemp,L"Download Completed Index[%d]\n download time : %.2lfs\n",mpsMultiDownloadParam->mnIndex,testtim.elapsed());
        Log4WD(strTemp.c_str());
		g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += strTemp;
		mpsMultiDownloadParam->meDownloadStatus = E_SUCCESSFUL_DOWNLOAD;
	}

	bool CCPEMultiDownloadThread::StartDownloadProcess()
	{
		std::wstring argv(g_szStartFlag);
		std::wstring strTemp;
#if 0
		//! 当端口不一致时
		if (mstrSourceIP.empty() || 0 != mstrSourceIP.compare(mpsNetCardParam->mstrIPAddr))
		{
			JGW_FormatWString(argv,L"%s Param:TSE_SSH_SOURCE_IP=%s&&TSE_APS_UPGRADE_VERSION=\"%s\"\n",g_szStartFlag,mpsNetCardParam->mstrIPAddr.c_str(),mpsMultiUpgradeConfig->mstrUpgradeVersion.c_str());
			g_strDownloadDubugLog[mpsNetCardParam->mnIndex] += strTemp;
			mstrSourceIP = mpsNetCardParam->mstrIPAddr;
		}
		//! 下载进程正在运行的时候
		if (m_pDownloadPipe.CheckProcessIsRun() && m_pDownloadPipe.timed_send(argv))
		{
			//!mpsNetCardParam->mdw64SendCounts += 200;
			return true;
		}

		std::wstring arg;
		JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_SSH_HOST=\"%s\" TSE_SSH_SOURCE_IP=\"%s\" TSE_SFTP_LOCAL_FILE_PATH=\"%s\" TSE_APS_UPGRADE_VERSION=\"%s\""
			,mpsMultiUpgradeConfig->mstrTSESuiteConfigFilePath.c_str()
			,mpsMultiUpgradeConfig->mstrHostIPAddr.c_str()
			,mpsNetCardParam->mstrIPAddr.c_str()
			,mpsMultiUpgradeConfig->mstrDownloadFilePath.c_str()
			,mpsMultiUpgradeConfig->mstrUpgradeVersion.c_str()
			);
		JGW_FormatWString(strTemp,L"CCTSE_UpgradeThread::StartDownloadProcess[%d],%s\n",mpsNetCardParam->mnIndex,arg.c_str());
		g_strDownloadDubugLog[mpsNetCardParam->mnIndex] += strTemp;
		if(m_pDownloadPipe.InitProcessPipe(mpsMultiUpgradeConfig->mstrDownloadProcessPath.c_str(),arg,mpsMultiUpgradeConfig->mstrCurrentWorkDir))
		{
			return true;
		}
#else
		//! 当端口不一致时
		/*if (mnDonwloadComPort != mpsMultiDownloadConfig->msMultiDownloadSortConfigs[mpsMultiDownloadParam->mnIndex].mnDonwloadComPort || mnSerialComPort != mpsMultiDownloadConfig->msMultiDownloadSortConfigs[mpsMultiDownloadParam->mnIndex].mnSerialComPort)
		{
			JGW_FormatWString(argv,L"%s Param:TSE_SSH_SOURCE_IP=%s&&TSE_APS_UPGRADE_VERSION=\"%s\"\n",g_szStartFlag,mpsNetCardParam->mstrIPAddr.c_str(),mpsMultiUpgradeConfig->mstrUpgradeVersion.c_str());
			g_strDownloadDubugLog[mpsNetCardParam->mnIndex] += strTemp;
			mstrSourceIP = mpsNetCardParam->mstrIPAddr;
		}*/

		JGW_FormatWString(argv,L"%s Param:TSE_DOWNLOAD_PORT_PARAM=%d&&TSE_SERIAL_PORT=%d\n",g_szStartFlag,mpsMultiDownloadSortConfig->mnDonwloadComPort,mpsMultiDownloadSortConfig->mnSerialComPort);
		g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += strTemp;

		//! 下载进程正在运行的时候
		if (m_pDownloadPipe.CheckProcessIsRun() && m_pDownloadPipe.timed_send(argv))
		{
			return true;
		}

        std::wstring strDownloadParam;
        for (std::map<std::wstring,std::wstring>::iterator it = mpsMultiDownloadConfig->mmapParams.begin();
            it != mpsMultiDownloadConfig->mmapParams.end();
            ++ it)
        {
            strDownloadParam += it->first;
            strDownloadParam += JGW_GetFormatWString(L"=\"%s\"  ",it->second.c_str());
        }

		std::wstring arg;
        JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_DOWNLOAD_PATH_PARAM=\"%s\" %sTFTPD_SERVICE_IP_ADDR=\"%s\" CPE_HOST_IP_ADDR=\"%s\" TSE_DOWNLOAD_PORT_PARAM=%d TSE_SERIAL_PORT=%d"
            ,mpsMultiDownloadConfig->mstrTSESuiteConfigFilePath.c_str()
            ,mpsMultiDownloadConfig->mstrDownloadPath.c_str()
            ,strDownloadParam.c_str()
            ,mpsMultiDownloadConfig->mstrTFtpdIPAddr.c_str()
            ,mpsMultiDownloadSortConfig->mstrCPEIPAddr.c_str()
            ,mpsMultiDownloadSortConfig->mnDonwloadComPort
            ,mpsMultiDownloadSortConfig->mnSerialComPort
            );

        /*JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_DOWNLOAD_PATH_PARAM=\"%s\" TSE_SAHARA_FIREHOSE_IMAGE_PARAM=\"%s\" TZ_MBN=\"%s\" DEVCFG_MBN=\"%s\" RPM_MBN=\"%s\" BOOT_MBN=\"%s\" CDT_DDR4=\"%s\" TFTPD_SERVICE_IP_ADDR=\"%s\" CPE_HOST_IP_ADDR=\"%s\" TSE_DOWNLOAD_PORT_PARAM=%d TSE_SERIAL_PORT=%d"
        ,mpsMultiDownloadConfig->mstrTSESuiteConfigFilePath.c_str()
        ,mpsMultiDownloadConfig->mstrDownloadPath.c_str()
        ,mpsMultiDownloadConfig->mstrSBL.c_str()
        ,mpsMultiDownloadConfig->mstrTZ.c_str()
        ,mpsMultiDownloadConfig->mstrDEV.c_str()
        ,mpsMultiDownloadConfig->mstrRPM.c_str()
        ,mpsMultiDownloadConfig->mstrBOOT.c_str()
        ,mpsMultiDownloadConfig->mstrCDT.c_str()
        ,mpsMultiDownloadConfig->mstrTFtpdIPAddr.c_str()
        ,mpsMultiDownloadSortConfig->mstrCPEIPAddr.c_str()
        ,mpsMultiDownloadSortConfig->mnDonwloadComPort
        ,mpsMultiDownloadSortConfig->mnSerialComPort
        );*/
		JGW_FormatWString(strTemp,L"CCTSE_UpgradeThread::StartDownloadProcess[%d],%s\n",mpsMultiDownloadParam->mnIndex,arg.c_str());
		g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += strTemp;
		if(m_pDownloadPipe.InitProcessPipe(mpsMultiDownloadConfig->mstrDownloadProcessPath.c_str(),arg,mpsMultiDownloadConfig->mstrCurrentWorkDir))
		{
			return true;
		}
#endif
		JGW_FormatWString(strTemp,L"CCTSE_UpgradeThread::StartDownloadProcess[%d] InitProcessPipe Fail\n",mpsMultiDownloadParam->mnIndex);
		return false;
	}

	bool CCPEMultiDownloadThread::RecvTestInfo()
	{
		jgw_timer tm;
		int readSize = 0;
		std::wstring strTemp;
		const wchar_t* p = NULL;

		//! 如果十分钟还未成功则表示超时
		while( tm.elapsed() < PIPE_TIMEOUT_  )
		{
			readSize = m_pDownloadPipe.recv(strTemp);
			if( -1 == readSize )
			{
				m_pDownloadPipe.CloseProcessPipe();
				g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += L"error: process closed!!! \n";
				return false;
			}
			else if ( readSize > 0 )
			{
				//                 p = wcsstr(strTemp.c_str(),L"percent files transferred");
				//                 if (p)
				//                 {
				//                     double nPos = _ttof(p + wcslen(L"percent files transferred"));
				//                     if (nPos > 4) mpsNetCardParam->mdw64SendCounts = nPos * 100.00;
				//                 }
				Log4WD(strTemp.c_str());
                //! 回调LOG函数
				g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += strTemp;
				if (std::wstring::npos != strTemp.find(g_szPassFlag))
				{
					return true;
				}	
				else if (std::wstring::npos != strTemp.find(g_szFailFlag))
				{
					return false;
				}
			}
			Sleep(50);
		}
		if ( tm.elapsed() >= PIPE_TIMEOUT_ )
		{
			m_pDownloadPipe.CloseProcessPipe();
			g_strDownloadDubugLog[mpsMultiDownloadParam->mnIndex - 1] += L"error: process closed!!!\n";
		}
		return false;
	}
}

