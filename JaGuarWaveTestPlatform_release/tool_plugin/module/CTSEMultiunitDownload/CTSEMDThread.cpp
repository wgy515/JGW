#include "StdAfx.h"
#include "CTSEMDThread.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
    const wchar_t g_szStartFlag[] = L"{D261BC0C-45F3-498D-AEE8-5E730691CF35}";

    const wchar_t g_szPassFlag[] = L"{A6A28E32-E91C-40D0-A79B-06CC8B441601}";

    const wchar_t g_szFailFlag[] = L"{241BC6EF-0CD0-49CE-9B39-EEA67691669C}";

    const wchar_t g_szExitFlag[] = L"{65FDA37A-8DFE-4540-9611-255A5C823649}";

    CCTSEMDThread::CCTSEMDThread(void) : mnSrcComPort(0)
    {
    }


    CCTSEMDThread::~CCTSEMDThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCTSEMDThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(DOWN_LOAD_THREAD_RUN_MSG,&CCTSEMDThread::OnRunDownloadThread)
    END_MYTHREAD_MESSAGE_MAP()


    void CCTSEMDThread::OnRunDownloadThread(WPARAM wParam,LPARAM lParam)
    {
        jgw_timer testtim;
        g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] = L"";
        //msDownloadProgressInfo.mdw64SendCounts += 200;
        if ( !StartDownloadProcess() || !RecvTestInfo())
        {
            msDownloadProgressInfo.meDownloadStatus = E_ERROR_DOWNLOAD;
            return ;
        } 
        std::wstring strTemp;
        JGW_FormatWString(strTemp,L"Download Completed Index[%d]\n download time : %.2lfs\n",msDownloadProgressInfo.mnIndex,testtim.elapsed());
        g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += strTemp;
        msDownloadProgressInfo.meDownloadStatus = E_SUCCESSFUL_DOWNLOAD;
    }

    bool CCTSEMDThread::StartDownloadProcess()
    {
        std::wstring argv(g_szStartFlag);
        std::wstring strTemp;
        //! 当端口不一致时
        if (0 == mnSrcComPort || mnSrcComPort != msDownloadProgressInfo.mnComPort)
        {
            JGW_FormatWString(argv,L"%s Port:%d\r\n",g_szStartFlag,msDownloadProgressInfo.mnComPort);
            g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += strTemp;
            mnSrcComPort = msDownloadProgressInfo.mnComPort;
        }
        //! 下载进程正在运行的时候
        if (m_pDownloadPipe.CheckProcessIsRun() && m_pDownloadPipe.timed_send(argv))
        {
            //msDownloadProgressInfo.mdw64SendCounts += 200;
            return true;
        }

        std::wstring arg;
        std::wstring strDownPath  = msDownloadProgressInfo.mpsTSEMDConfig->mstrDownloadPath;
        while (strDownPath.at(strDownPath.length()-1) == '\\' || strDownPath.at(strDownPath.length()-1) == '/')
        {
            strDownPath.erase(strDownPath.length()-1);
        }
        /*
        E_NULL_PARAM = 0,
        E_TSS_SUITE_CONFIG_PATH = 1,
        E_TSE_DOWNLOAD_PORT_PARAM = 2,
        E_TSE_DOWNLOAD_PATH_PARAM,
        E_TSE_SAHARA_FIREHOSE_IMAGE_PARAM,
        E_TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM, 
        E_TSE_RAWPROGRAM_XML_NAMES_PARAM,
        E_TSE_PATCH_XML_NAMES_PARAM,
        E_TSE_RESET_ENABLE_PARAM,
        E_TSE_ERROR_STOP
        */
        JGW_FormatWString(arg,L" \"%s\" %d \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" %d"
            ,msDownloadProgressInfo.mpsTSEMDConfig->mstrTSESuiteConfigPath.c_str()
            ,msDownloadProgressInfo.mnComPort
            ,strDownPath.c_str()
            ,msDownloadProgressInfo.mpsTSEMDConfig->mstrProgFirehoseLitembnFileName.c_str()
            ,msDownloadProgressInfo.mpsTSEMDConfig->mstrProvisionXml.c_str()
            ,msDownloadProgressInfo.mpsTSEMDConfig->mstrRawprograms.c_str()
            ,msDownloadProgressInfo.mpsTSEMDConfig->mstrPacths.c_str()
            ,msDownloadProgressInfo.mpsTSEMDConfig->mbResetAfterDownload?1:0
            );
        JGW_FormatWString(strTemp,L"CCTSEMDThread::StartDownloadProcess[%d],%s\n",msDownloadProgressInfo.mnIndex,arg.c_str());
        g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += strTemp;
        if(m_pDownloadPipe.InitProcessPipe(msDownloadProgressInfo.mpsTSEMDConfig->mstrDownloadProcessPath.c_str(),arg,msDownloadProgressInfo.mpsTSEMDConfig->mstrCurrentWorkDir))
        {
            return true;
        }

        JGW_FormatWString(strTemp,L"CCTSEMDThread::StartDownloadProcess[%d] InitProcessPipe Fail\n",msDownloadProgressInfo.mnIndex);
        return false;
    }

    bool CCTSEMDThread::RecvTestInfo()
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
                g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += L"error: process closed!!! \n";
                return false;
            }
            else if ( readSize > 0 )
            {
                //p = wcsstr(strTemp.c_str(),L"percent files transferred");
                //if (p)
                //{
                //    double nPos = _ttof(p + wcslen(L"percent files transferred"));
                //    if (nPos > 4) msDownloadProgressInfo.mdw64SendCounts = nPos * 100.00;
                //}
                //! 回调LOG函数
                g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += strTemp;
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
            g_strDownloadDubugLog[msDownloadProgressInfo.mnIndex] += L"error: process closed!!!\n";
        }
        return false;
    }
}