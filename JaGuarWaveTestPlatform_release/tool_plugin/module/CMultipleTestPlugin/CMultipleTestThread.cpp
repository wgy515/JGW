#include "StdAfx.h"
#include "CMultipleTestThread.h"
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

    CCMultipleTestThread::CCMultipleTestThread(void)
    {
    }


    CCMultipleTestThread::~CCMultipleTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCMultipleTestThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(DOWN_LOAD_THREAD_RUN_MSG,&CCMultipleTestThread::OnRunTestThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCMultipleTestThread::InitTestParam(PS_TSE_MULTIPLE_TEST_THREAD_PARAM psTSEMultipleTestParam)
    {
        mpsTSEMultipleTestParam = psTSEMultipleTestParam;
    }

    void CCMultipleTestThread::OnRunTestThread(WPARAM wParam,LPARAM lParam)
    {
#if 0
        jgw_timer testtim;
        for (int i = 0;i < 10;i ++)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"Test %d .... \n",i);
            mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
            Sleep( 1000);
        }
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Download Completed Index[%d]\n download time : %.2lfs\n",mpsTSEMultipleTestParam->mnSortIndex,testtim.elapsed());
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);

        mpsTSEMultipleTestParam->mbIsFinishedTSEMultipleTest = true;
        mpsTSEMultipleTestParam->meTSEMPTestStatus = E_TSEMP_PASS;
#else
        jgw_timer testtim;

        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.ClearText();
        if ( !StartTSEProcess() || !RecvTestInfo())
        {
            mpsTSEMultipleTestParam->mbIsFinishedTSEMultipleTest = true;
            mpsTSEMultipleTestParam->meTSEMPTestStatus = E_TSEMP_FAIL;
            SaveDownloadLog(false);
            return ;
        }
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(JGW_GetFormatWString(L"Test Completed Index[%d]\n download time : %.2lfs\n",mpsTSEMultipleTestParam->mnSortIndex,testtim.elapsed()));
        SaveDownloadLog(true);
        mpsTSEMultipleTestParam->mbIsFinishedTSEMultipleTest = true;
        mpsTSEMultipleTestParam->meTSEMPTestStatus = E_TSEMP_PASS;
#endif
    }

    std::wstring CCMultipleTestThread::GetTSEGeneralTestParam()
    {
        std::wstring strTSETestParam = L"";
        // TSE_SUITE_CONFIG_FILEPATH
        strTSETestParam += JGW_GetFormatWString(L" TSE_SUITE_CONFIG_FILE_PATH=\"%s\"",mpsTSEMultipleTestParam->mpsMultipleConfig->mstrTSESuiteConfigFilePath.c_str());
        // TSE_BINARYIMAGEPATH
        strTSETestParam += JGW_GetFormatWString(L" TSE_BINARYIMAGEPATH=\"%s\"",mpsTSEMultipleTestParam->mpsMultipleConfig->mstrBinaryImagePath.c_str());
        // TSE_SAVE_TESTLOG_FOLDER
        std::wstring strSaveTestLogFolder = mpsTSEMultipleTestParam->mpsMultipleConfig->mstrSaveTestLogFolder;
        JGW_EraseLastAndFristChars(strSaveTestLogFolder,'\\');
        strTSETestParam += JGW_GetFormatWString(L" TSE_SAVE_TESTLOG_FOLDER=\"%s\"",strSaveTestLogFolder.c_str());
        // TSE_HOST_IP
        strTSETestParam += JGW_GetFormatWString(L" TSE_HOST_IP=\"%s\"",mpsTSEMultipleTestParam->mpsMultipleConfig->msNetCardConfig.mstrHostIPAddr.c_str());
        // TSE_APS_UPGRADE_VERSION
        strTSETestParam += JGW_GetFormatWString(L" TSE_APS_UPGRADE_VERSION=\"%s\"",mpsTSEMultipleTestParam->mpsMultipleConfig->mstrUpgradeVersion.c_str());
        //! \"%s\"
        strTSETestParam += mpsTSEMultipleTestParam->mpsMultipleConfig->mstrFormatTSEParam;

        return strTSETestParam;
    }

    bool CCMultipleTestThread::StartTSEProcess()
    {
        std::wstring argv(g_szStartFlag);
        std::wstring strTemp;

        JGW_FormatWString(argv,L"%s Param:TSE_FRIST_DEVICE=\"%s\"&&TSE_SECOND_DEVICE=\"%s\"&&TSE_THREE_DEVICE=\"%s\"\n",g_szStartFlag,mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrFristDevice.c_str(),mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrSecondDevice.c_str(),mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrThreeDevice.c_str());
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
        //! 下载进程正在运行的时候
        if (mcDownloadPipe.CheckProcessIsRun() && mcDownloadPipe.timed_send(argv))
        {
            return true;
        }

        strTemp = GetTSEGeneralTestParam();
        JGW_FormatWString(argv,L" TSE_FRIST_DEVICE=\"%s\" TSE_SECOND_DEVICE=\"%s\" TSE_THREE_DEVICE=\"%s\"%s",mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrFristDevice.c_str(),mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrSecondDevice.c_str(),mpsTSEMultipleTestParam->msDeviceQueueInfo.mstrThreeDevice.c_str(),strTemp.c_str());

        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d],%s\n",mpsTSEMultipleTestParam->mnSortIndex,argv.c_str());
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);
        if(mcDownloadPipe.InitProcessPipe(mpsTSEMultipleTestParam->mpsMultipleConfig->mstrTSEExecApplicationFilePath.c_str(),argv,mpsTSEMultipleTestParam->mpsMultipleConfig->mstrTSEExecApplicationCurrentWorkDir))
        {
            return true;
        }
        JGW_FormatWString(strTemp,L"CCTSEMultipleTestThread::StartDownloadProcess[%d] InitProcessPipe Fail\n",mpsTSEMultipleTestParam->mnSortIndex);
        mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(strTemp);

        return false;
    }

    void CCMultipleTestThread::SaveDownloadLog(bool bTestResult)
    {
        if (!mpsTSEMultipleTestParam->mpsMultipleConfig->mbSaveTestLog)
        {
            return;
        }

        CCJGW_SaveTestLog::SaveTestLog(mpsTSEMultipleTestParam->mpsMultipleConfig->mstrSaveTestLogFolder,L"",bTestResult,mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.GetAllTestLog());
    }

    bool CCMultipleTestThread::RecvTestInfo()
    {
        jgw_timer tm;
        int readSize = 0;
        std::wstring strTemp;
        const wchar_t* p = NULL;

        //! 如果十分钟还未成功则表示超时
        while( tm.elapsed() < mpsTSEMultipleTestParam->mpsMultipleConfig->mnTestTimeOutSec)
        {
            readSize = mcDownloadPipe.recv(strTemp);
            if( -1 == readSize )
            {
                mcDownloadPipe.CloseProcessPipe();
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
        if (tm.elapsed() >= mpsTSEMultipleTestParam->mpsMultipleConfig->mnTestTimeOutSec)
        {
            mcDownloadPipe.CloseProcessPipe();
            mpsTSEMultipleTestParam->mstrTSEMultipleTestLog.AppendText(L"error: process closed!!!\n");
        }
        return false;
    }
}
