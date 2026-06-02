#include "StdAfx.h"
#include "CDownloadFirewareThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CCDownloadFirewareThread::CCDownloadFirewareThread(void)
    {
    }


    CCDownloadFirewareThread::~CCDownloadFirewareThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCDownloadFirewareThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCDownloadFirewareThread::OnInitThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCDownloadFirewareThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCDownloadFirewareThread::OnInitThread( WPARAM wParam,LPARAM lParam )
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsDownloadFirewareParam = (LPS_DOWNLOAD_FIREWARE_PARAM)lParam;
    }

    void CCDownloadFirewareThread::OnRunThread(WPARAM wParam,LPARAM lParam)
    {
         SendTestMsgToWnd(L"Start Test ...\n");
#if 1
         if (!FindPerasoComPort()
             || !ExecuteCmdApplication()
             || !ReadDownloadFirewareResult()
             )
         {
             mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
             return ;
         }
#else
         if (!InitProductFolderPath()
             || !SaveTestLogFile()
             )
         {
             mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
             return ;
         }
#endif
         mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
         SendTestMsgToWnd(L"\nTest Finished ok");
    }

    void CCDownloadFirewareThread::SendTestMsgToWnd(const wchar_t* strMsg)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMsg,NULL);
    }

    bool CCDownloadFirewareThread::FindPerasoComPort()
    {
        std::wstring strComPort;
        for (int i = 0;i < 50;i ++)
        {
            SendTestMsgToWnd(L"Find Device ...\n");
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (!strComPort.empty()) 
            { 
                SendTestMsgToWnd(L"Found Device\n");
                return true;
            }
            Sleep(1000); 
        }
        SendTestMsgToWnd(L"No Found Device\n");

        return false;
    }

    bool CCDownloadFirewareThread::ExecuteCmdApplication()
    {
        std::wstring strCmdLine = DOWNLOAD_FIREWARE_TOOL_NAME;
        strCmdLine += L"\n";
        int recv_count = 0;

        if (!mcProecessPide.CheckProcessIsRun())
        {
            std::wstring strTemp;
            if (!mcProecessPide.InitProcessPipe(mpsDownloadFirewareParam->mstrCMDFilePath.c_str(),strTemp,mpsDownloadFirewareParam->mstrFirewareFolder))
            {
                SendTestMsgToWnd(L"ProductScreenTool.exe Execute Failed\n");
                return false;
            }
            while (1)
            {
                recv_count = mcProecessPide.recv(strTemp);
                if (-1 == recv_count)
                {
                    SendTestMsgToWnd(L"未知异常错误信息\n");
                    mcProecessPide.CloseProcessPipe();
                    return false;
                }
                else if (0 == recv_count)
                {
                    Sleep(100);
                    continue;
                }
                else
                {
                    if (L'>' == strTemp.at(strTemp.size() - 1))
                    {
                        break;
                    }
                }
            }   
        }

        if (!mcProecessPide.send(strCmdLine))
        {
            SendTestMsgToWnd(L"Send Command To cmd.exe Failed\n");
            return false;
        }

        return true;
    }

    bool CCDownloadFirewareThread::ReadDownloadFirewareResult()
    {
        std::wstring strRead;
        mstrReadTestResult = L"";
        int recv_count = 0;
        jgw_timer jgw_time;

        SendTestMsgToWnd(L"Start recv Download Fireware Result......\nPlease waiting....\n");
        while (DOWNLOAD_FIREWARE_TIME_OUT_SEC >= jgw_time.elapsed())
        {
            recv_count = mcProecessPide.recv(strRead);
            if (-1 == recv_count)
            {
                SendTestMsgToWnd(L"未知异常错误信息\n");
                mcProecessPide.CloseProcessPipe();
                return false;
            }
            else if (0 == recv_count)
            {
                Sleep(100);
                continue;
            }
            else
            {
                mstrReadTestResult += strRead;
                SendTestMsgToWnd(strRead.c_str());
                if (L'>' == strRead.at(strRead.size() - 1))
                {
                    return true;
                }
            }
        }
        SendTestMsgToWnd(L"recv time out\n");
        return false;
    }

    bool CCDownloadFirewareThread::CheckDownloadFirewareIsOk()
    {
        if (std::wstring::npos == mstrReadTestResult.find(L"SUCCESS"))
        {
            SendTestMsgToWnd(L"Download Fireware Failed\n");
            return false;
        }

        return true;
    }
}

