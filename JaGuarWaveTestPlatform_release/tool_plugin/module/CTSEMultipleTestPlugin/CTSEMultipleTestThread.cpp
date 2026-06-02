#include "StdAfx.h"
#include "CTSEMultipleTestThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>

namespace JGW
{
    const wchar_t g_szStartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";

    const wchar_t g_szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";

    const wchar_t g_szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";

    const wchar_t g_szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";

    CCTSEMultipleTestThread::CCTSEMultipleTestThread(void) : mpsTSEMultipleTestParam(NULL)
    {
    }


    CCTSEMultipleTestThread::~CCTSEMultipleTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEMultipleTestThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(DOWN_LOAD_THREAD_RUN_MSG,&CCTSEMultipleTestThread::OnRunDownloadThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCTSEMultipleTestThread::InitDownloadParam(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psTSEMultipleTestParam)
    {
        mpsTSEMultipleTestParam = psTSEMultipleTestParam;
    }

    void CCTSEMultipleTestThread::OnRunDownloadThread(WPARAM wParam,LPARAM lParam)
    {
        jgw_timer testtim;
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.ClearText();

        if ( !StartDownloadProcess() || !RecvTestInfo())
        {
            mpsTSEMultipleTestParam->mbIsFinishedTSEMultipleTest = true;
            mpsTSEMultipleTestParam->meTSEMPTestStatus = E_TSEMP_FAIL;
            SaveDownloadLog(false);
            return ;
        } 
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Download Completed Index[%d]\n download time : %.2lfs\n",mpsTSEMultipleTestParam->mnSortIndex,testtim.elapsed());
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
        SaveDownloadLog(true);
        mpsTSEMultipleTestParam->mbIsFinishedTSEMultipleTest = true;
        mpsTSEMultipleTestParam->meTSEMPTestStatus = E_TSEMP_PASS;
    }

    //! const wchar_t gsTSEEnvrionmentName[][36] ={
    //    L"TSE_IS_USEDEVICE",L"TSE_BINARY_IMAGEISFOLDER",L"TSE_ISSAVETEST_LOG",
    //        L"TSE_SAVE_TESTLOG_FOLDER",L"TSE_BINARYIMAGEPATH",L"TSE_UPGRADE_VERSION",
    //        L"TSE_IP_ADDRESS",L"TSE_NETCARD_GUID",L"TSE_COM_PORT",L"TSE_ADB_SERIALNUMBER",
    //        L"TSE_SUITE_CONFIG_FILEPATH",L"TSE_HOST_ADDRESS"
    //};
    std::wstring CCTSEMultipleTestThread::GetTSETestParam()
    {
        std::vector<std::wstring> vSourceTSEParam;
        mpsTSEMultipleTestParam->mstrTSEParam = L"";
        for (std::map<E_GSTSE_ENVNAME,std::wstring>::iterator it = mpsTSEMultipleTestParam->mpsMultipleConfig->mmapeSourceTSEParam.begin();it != mpsTSEMultipleTestParam->mpsMultipleConfig->mmapeSourceTSEParam.end();++ it)
        {
            switch (it->first)
            {
            case E_GSTSE_ISEUSEDEV:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=%d",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mbIsUsbDevice ? 1 : 0);
                break;
            case E_GSTSE_BINARY_IMAGEISFOLDER:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=%d",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mbBinaryImageIsFolder ? 1 : 0);
                break;
            case E_GSTSE_ISSAVETEST_LOG:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=%d",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mbSaveTestLog ? 1 : 0);
                break;
            case E_GSTSE_SAVE_TESTLOG_FOLDER:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mstrSaveTestLogFolder.c_str());
                break;
            case E_GSTSE_BINARYIMAGEPATH:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mstrBinaryImagePath.c_str());
                break;
            case E_GSTSE_UPGRADE_VERSION:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mstrUpgradeVersion.c_str());
                break;
            case E_GSTSE_IP_ADDRESS:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr.c_str());
                break;
            case E_GSTSE_NETCARD_GUID:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrNetGUID.c_str());
                break;
            case E_GSTSE_COM_PORT:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->msBindUsbDevInfo.mstrComPort.c_str());
                break;
            case E_GSTSE_ADB_SERIALNUMBER:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->msBindUsbDevInfo.mstrAdbSerialNumber.c_str());
                break;
            case E_GSTSE_SUITE_CONFIG_FILEPATH:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mstrTSESuiteConfigFilePath.c_str());
                break;
            case E_GSTSE_HOST_ADDRESS:
                mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=\"%s\"",it->second.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str());
                break;
//             default:
//                 continue;
            }
            //mpsTSEMultipleTestParam->mstrTSEParam += JGW_GetFormatWString(L" %s=%s",it->second.c_str(),strValue.c_str());
        }
        return mpsTSEMultipleTestParam->mstrTSEParam;
    }

    bool CCTSEMultipleTestThread::StartDownloadProcess()
    {
        if (mpsTSEMultipleTestParam->mpsMultipleConfig->mbIsUsbDevice)
        {
            return StartUSBDeviceTSEProcess();
        }

        return StartNetCardTSEProcess();
#if 0
        std::wstring argv(g_szStartFlag);
        std::wstring strTemp;
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
        if (mpsMultiUpgradeConfig->mbIsDownloadFolder)
        {
            JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_SSH_HOST=\"%s\" TSE_SSH_SOURCE_IP=\"%s\" TSE_SFTP_LOCAL_FILE_PATH=\"%s\" TSE_APS_UPGRADE_VERSION=\"%s\"%s"
                ,mpsMultiUpgradeConfig->mstrTSESuiteConfigFilePath.c_str()
                ,mpsMultiUpgradeConfig->mstrHostIPAddr.c_str()
                ,mpsNetCardParam->mstrIPAddr.c_str()
                ,mpsMultiUpgradeConfig->mstrDownloadFilePath.c_str()
                ,mpsMultiUpgradeConfig->mstrUpgradeVersion.c_str()
                ,mpsMultiUpgradeConfig->mstrFolderTSEParam.c_str()
                );
        }
        else
        {
            JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_SSH_HOST=\"%s\" TSE_SSH_SOURCE_IP=\"%s\" TSE_SFTP_LOCAL_FILE_PATH=\"%s\" TSE_APS_UPGRADE_VERSION=\"%s\""
                ,mpsMultiUpgradeConfig->mstrTSESuiteConfigFilePath.c_str()
                ,mpsMultiUpgradeConfig->mstrHostIPAddr.c_str()
                ,mpsNetCardParam->mstrIPAddr.c_str()
                ,mpsMultiUpgradeConfig->mstrDownloadFilePath.c_str()
                ,mpsMultiUpgradeConfig->mstrUpgradeVersion.c_str()
                );
        }


        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d],%s\n",mpsNetCardParam->mnIndex,arg.c_str());
        g_strDownloadDubugLog[mpsNetCardParam->mnIndex] += strTemp;
        if(m_pDownloadPipe.InitProcessPipe(mpsMultiUpgradeConfig->mstrDownloadProcessPath.c_str(),arg,mpsMultiUpgradeConfig->mstrCurrentWorkDir))
        {
            return true;
        }

        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d] InitProcessPipe Fail\n",mpsNetCardParam->mnIndex);
        return false;
#endif
    }

    bool CCTSEMultipleTestThread::StartUSBDeviceTSEProcess()
    {
        return false;
    }

    bool CCTSEMultipleTestThread::StartNetCardTSEProcess()
    {
        std::wstring argv(g_szStartFlag);
        std::wstring strTemp;
        //! 当端口不一致时
        if (mstrSourceIP.empty() || 0 != mstrSourceIP.compare(mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr))
        {
            JGW_FormatWString(argv,L"%s Param:TSE_SSH_SOURCE_IP=%s&&TSE_APS_UPGRADE_VERSION=\"%s\"\n",g_szStartFlag,mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr.c_str(),mpsTSEMultipleTestParam->mpsMultipleConfig->mstrUpgradeVersion.c_str());
            mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
            mstrSourceIP = mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr;
        }
        //! 下载进程正在运行的时候
        if (m_pDownloadPipe.CheckProcessIsRun() && m_pDownloadPipe.timed_send(argv))
        {
            return true;
        }
        GetTSETestParam();
        std::wstring arg;
        if (mpsTSEMultipleTestParam->mpsMultipleConfig->mbBinaryImageIsFolder)
        {
            JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_SSH_HOST=\"%s\" TSE_SSH_SOURCE_IP=\"%s\" TSE_SFTP_LOCAL_FILE_PATH=\"%s\" TSE_APS_UPGRADE_VERSION=\"%s\"%s%s"
                ,mpsTSEMultipleTestParam->mstrTSESuiteConfigFilePath.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str()
                ,mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrBinaryImagePath.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrUpgradeVersion.c_str()
                ,mpsTSEMultipleTestParam->mstrTSEParam.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrDownloadUpgradeFileTSEParam.c_str()
                );
        }
        else
        {
            JGW_FormatWString(arg,L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\" TSE_SSH_HOST=\"%s\" TSE_SSH_SOURCE_IP=\"%s\" TSE_SFTP_LOCAL_FILE_PATH=\"%s\" TSE_APS_UPGRADE_VERSION=\"%s\""
                ,mpsTSEMultipleTestParam->mstrTSESuiteConfigFilePath.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str()
                ,mpsTSEMultipleTestParam->mpsBindNetCardInfo->mstrIPAddr.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrBinaryImagePath.c_str()
                ,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrUpgradeVersion.c_str()
                );
        }


        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d],%s\n",mpsTSEMultipleTestParam->mnSortIndex,arg.c_str());
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
        if(m_pDownloadPipe.InitProcessPipe(mpsTSEMultipleTestParam->mpsMultipleConfig->mstrTSEExecApplicationFilePath.c_str(),arg,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrTSEExecApplicationCurrentWorkDir))
        {
            return true;
        }

        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d] InitProcessPipe Fail\n",mpsTSEMultipleTestParam->mnSortIndex);
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
        return false;
    }

    bool CCTSEMultipleTestThread::RecvTestInfo()
    {
        jgw_timer tm;
        int readSize = 0;
        std::wstring strTemp;
        const wchar_t* p = NULL;

        //! 如果十分钟还未成功则表示超时
        while( tm.elapsed() < mpsTSEMultipleTestParam->mpsMultipleConfig->mnDownloadTimeOutSec)
        {
            readSize = m_pDownloadPipe.recv(strTemp);
            if( -1 == readSize )
            {
                m_pDownloadPipe.CloseProcessPipe();
                mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(L"error: process closed!!! \n");
                return false;
            }
            else if ( readSize > 0 )
            {
                //! 回调LOG函数
                mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
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
        if (tm.elapsed() >= mpsTSEMultipleTestParam->mpsMultipleConfig->mnDownloadTimeOutSec)
        {
            m_pDownloadPipe.CloseProcessPipe();
            mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(L"error: process closed!!!\n");
        }
        return false;
    }

    void CCTSEMultipleTestThread::SaveDownloadLog(bool bTestResult)
    {
        if (!mpsTSEMultipleTestParam->mpsMultipleConfig->mbSaveTestLog)
        {
            return;
        }

        CCJGW_SaveTestLog::SaveTestLog(mpsTSEMultipleTestParam->mpsMultipleConfig->mstrSaveTestLogFolder,L"",bTestResult,mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.GetAllTestLog());
    }
}

