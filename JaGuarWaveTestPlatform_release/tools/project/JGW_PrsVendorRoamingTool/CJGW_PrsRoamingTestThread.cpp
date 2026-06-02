#include "StdAfx.h"
#include "CJGW_PrsRoamingTestThread.h"
#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/jgw_process_manage.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

//#include <Poco/Net/ICMPSocket.h>
//#include <Poco/Net/ICMPClient.h>

namespace JGW
{
    CCJGW_PrsRoamingTestThread::CCJGW_PrsRoamingTestThread(S_TestParams* psTestParams,CCJGW_AsyncWndMessage* pAsyncWndMessage) : mpsTestParams(psTestParams),mpAsyncWndMessage(pAsyncWndMessage),mbRunTestThread(false),mcIPQSshThread(mbRunTestThread,psTestParams,pAsyncWndMessage),_senderSocket("127.0.0.1",CEF3_DATA_UDP_PORT)
    {
        mcIperf3Command.RegisterCallbackIperf3RawTestLog(&CCJGW_PrsRoamingTestThread::Iperf3RawTestLogCallbackFunc,this);
         _socket.bind(Poco::Net::SocketAddress(), true);
    }


    CCJGW_PrsRoamingTestThread::~CCJGW_PrsRoamingTestThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_PrsRoamingTestThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_PrsRoamingTestThread::OnRunTestThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_PrsRoamingTestThread::StopThroughputTestThread()
    {
        mbRunTestThread = false;
        mcIperf3Command.StopIperf3();
        //mcPingTestThread.StopPingTestThread();
        //mcPrsVendorRoamingServer.StopPrsVendorRoamingServer();
        //mcPrsRoamingLogDebugSocket.StopPrsRoamingLogDebugSocket();
        JGW_KillProcessToName(L"iperf3.exe");
        JGW_KillProcessToName(L"iperf.exe");
        Log4WI(L"Stop Throughput Test Thread");
    }

    void CCJGW_PrsRoamingTestThread::SendCEF3DataUDPMessage(const std::string& strMsg)
    {
        _socket.sendTo(strMsg.c_str(),strMsg.length(),_senderSocket);
    }

    void CCJGW_PrsRoamingTestThread::OnRunTestThread(WPARAM wParam,LPARAM lParam)
    {
        mbRunTestThread = true;
        mpsTestParams->mstrIperfIPAddrA = JGW_W2A(mpsTestParams->mstrIperfIPAddr);
        //! 1.开启TCP SERVER SOCKET
        SendWindowMsg(L"Start TCP Server Socket(%d) ...",PRS_VENDOR_APP_TCP_SOCKET_PORT);
        if (!mcPrsVendorRoamingServer.StartPrsVendorRoamingServer(PRS_VENDOR_APP_TCP_SOCKET_PORT))
        {
            goto T_END;
        }
        //! 3、开启LOG DEBUG调试功能
        if (!mpsTestParams->mstrLogDebugFilePath.empty())
        {
            SendWindowMsg(L"Start Prs Roaming Log Debug Socket(%d) ...",PRS_VENDOR_APP_TCP_SOCKET_PORT);
            if (!mcPrsRoamingLogDebugSocket.StartPrsRoamingLogDebugSocket(mpsTestParams->mstrLogDebugFilePath))
            {
                SendWindowMsg(L"Start Prs Roaming Log Debug Socket Error : %s",mcPrsRoamingLogDebugSocket.GetErrorMessage().c_str());
                goto T_END;
            }
        }
        //! roaming_graph 接收到消息后是否转发，如果为true 则不会接收UDP MSG
        if (!mpsTestParams->mbIsSendRoamingGraphMsg)
        {
            SendWindowMsg(L"Start Prs Roaming Graph UDP Thread ...");
            if (!mcPrsRoamingGraphUDPServer.StartPrsRoamingGraphUDPServer(mcPrsVendorRoamingServer.GetNotificationQueue()))
            {
                SendWindowMsg(mcPrsRoamingGraphUDPServer.GetErrorMessage());
                goto T_END;
            }
        }
        //! 
        SendWindowMsg(L"Start PrsVendorRoaming Data Handle Thread ...");
        if (!mcPrsVendorRoamingDataHandleThread.StartPrsVendorRoamingDataHandleThread(mpsTestParams->mstrLogFolder,mcPrsVendorRoamingServer.GetNotificationQueue(),mpsTestParams->mbIsSendRoamingGraphMsg,mpAsyncWndMessage))
        {
            SendWindowMsg(mcPrsVendorRoamingServer.GetErrorMessage());
            goto T_END;
        }
#ifndef _DEBUG
        //! 4、开启SSH服务
        if (!mcIPQSshThread.StartIPQSshThread())
        {
            goto T_END;
        }
#endif
#if 1
        //! 3.开启PING测试线程
        if (!mcPingTestThread.StartPingTestThread(mpsTestParams->mstrIperfIPAddrA,mpAsyncWndMessage))
        {
            goto T_END;
        }
#endif
        //! 4.开启IPERF服务端或者客户端
        while (mbRunTestThread)
        {
            if (E_PING_TYPE != mpsTestParams->meProtocolType)
            {
                if (E_SERVICE_TYPE == mpsTestParams->meSocketTerimalType)
                {
                    IperfServerFlowProcessing();
                }
                else if (E_CLIENT_TYPE == mpsTestParams->meSocketTerimalType)
                {
                    IperfClientFlowProcessing();
                }
            }
            Sleep(100);
        }
T_END:
        mbRunTestThread = false;
        //SendWindowMsg(L"Stop IPQ Ssh Thread");
        mcIPQSshThread.StopIPQSshThread();
        //SendWindowMsg(L"Stop Iperf3");
        mcIperf3Command.StopIperf3();
        //SendWindowMsg(L"Stop Ping Test Thread");
        mcPingTestThread.StopPingTestThread();
        //SendWindowMsg(L"StopPrsRoamingGraphUDPServer");
        mcPrsRoamingGraphUDPServer.StopPrsRoamingGraphUDPServer();
        //SendWindowMsg(L"StopPrsRoamingLogDebugSocket");
        mcPrsRoamingLogDebugSocket.StopPrsRoamingLogDebugSocket();
        //SendWindowMsg(L"StopPrsVendorRoamingServer");
        mcPrsVendorRoamingServer.StopPrsVendorRoamingServer();
        //SendWindowMsg(L"StopPrsVendorRoamingDataHandleThread");
        mcPrsVendorRoamingDataHandleThread.StopPrsVendorRoamingDataHandleThread();
        //SendWindowMsg(L"TerminateIPQSshThread");
        mcIPQSshThread.TerminateIPQSshThread();
        Sleep(2000);
        //SendWindowMsg(L"WM_TEST_PASS_MSG");
        //SendMessage(mpAsyncWndMessage->GetMainHwnd(),WM_TEST_PASS_MSG,NULL,NULL);
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
    }

    void CCJGW_PrsRoamingTestThread::IperfClientFlowProcessing()
    {
        //! 客户端 开始检查IPERF IP是否能连通，如果可以则开启IPERF进程(每次都重启IPERF进程)
        if (mcPingTestThread.GetPingSucessful())
        {
            if (!mcIperf3Command.CheckIper3IsRunning() && mbRunTestThread)
            {
                //! 清空吞吐率数据
                mpAsyncWndMessage->PutAsyncMessage(WM_THROUGHPUT_RATE_MSG,NULL);
                SendWindowMsg(L"Start Iperf3 Client");
                //! 2、检查对面IPERF IP是否能PING通
                //if (CheckNetworkConnection(mpsTestParams->mstrIperfIPAddr,L"",1))
                {
                    //mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
                    if (!mcIperf3Command.StartIperf3ClientToUserParameters(mpsTestParams->mstrIperfCommand,mpsTestParams->mstrIperfRelativePath))
                    {
                        SendWindowMsg(L"Start Iperf Client Fail");
                    }
                }
            }   
        }
        else
        {
            //SendWindowMsg(L"Stop Iperf3 Client");
            mcIperf3Command.StopIperf3();
            JGW_KillProcessToName(L"iperf3.exe");
            JGW_KillProcessToName(L"iperf.exe");
        }
    }

    void CCJGW_PrsRoamingTestThread::IperfServerFlowProcessing()
    {
        //! 服务端，则开启IPERF服务程序,如果上次退出的时候有iperf3: the client has terminated提示以及Server listening on 5201 提示则表示IPERF3 SERVER不需要重新启动
        if (mcPingTestThread.GetPingSucessful())
        {
            mbStopIperf3Server = true;
            //! 检查IPERF进程是否正在运行以及测试线程是否运行
            if (!mcIperf3Command.CheckIper3IsRunning() && mbRunTestThread)
            {
                //! 清空吞吐率数据
                mpAsyncWndMessage->PutAsyncMessage(WM_THROUGHPUT_RATE_MSG,NULL);
                //! mpAsyncWndMessage->PutAsyncStringMessage(WM_TIME_DELAY_MSG,L"");
                SendWindowMsg(L"Start Iperf3 Service");
                //! 重启IPERF3进程
                if (!mcIperf3Command.StartIper3ServerToUserParameters(mpsTestParams->mstrIperfCommand,mpsTestParams->mstrIperfRelativePath))
                {
                    SendWindowMsg(L"Start Iperf server Fail");
                }
            }
        }
        else
        {
            if (mbStopIperf3Server)
            {
                //SendWindowMsg(L"Stop Iperf3 Service");
                mcIperf3Command.StopIperf3();
                JGW_KillProcessToName(L"iperf3.exe");
                JGW_KillProcessToName(L"iperf.exe");
                mbStopIperf3Server = false;
            }
        }
    }

    void CCJGW_PrsRoamingTestThread::SendWindowMsg(const std::wstring& strMessage)
    {
        Log4WI(strMessage.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strMessage.c_str());
    }

    void CCJGW_PrsRoamingTestThread::SendWindowMsg(const wchar_t* szFormat,...)
    {
        std::wstring strLog;
        FORMAT_WSTRING(strLog,szFormat);
        Log4WI(strLog.c_str());
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,strLog.c_str());
    }

    bool HandIperfTestLogSpeed(const std::wstring& strIperfLog,std::wstring& strID,size_t& start,size_t& end,float& speed)
    {
        //! [SUM]   0.00-1.00   sec  1.40 GBytes  12062 Mbits/sec
        if (std::wstring::npos != strIperfLog.find(L"sec") && std::wstring::npos != strIperfLog.find(L"s/sec") && strIperfLog[0] == '[')
        {
            int id = 0;
            id = strIperfLog.find(L']',2) - 1;
            strID = strIperfLog.substr(1,id);
            start = _ttoi(strIperfLog.substr(id + 2).c_str());
            id = strIperfLog.find(L'-',id);
            end = _ttoi(strIperfLog.substr(id + 1).c_str());
            id = strIperfLog.find(L"s/sec",id + 1);
            id = strIperfLog.rfind(L's',id - 1);
            speed = static_cast<float>(_ttof (strIperfLog.substr(id + 1).c_str()));
            return true;
        }
        return false;
    }

    //! 原生LOG
    //! iperf3: interrupt - the client has terminated
    //! iperf3: error - the server has terminated
    //! iperf3: interrupt - the server has terminated
    void CCJGW_PrsRoamingTestThread::Iperf3RawTestLogCallbackFunc(const std::wstring& strRawLog,void* pContext)
    {
        CCJGW_PrsRoamingTestThread* pThroughputTestThread = (CCJGW_PrsRoamingTestThread*)pContext;
        if (NULL == pThroughputTestThread || !pThroughputTestThread->mbRunTestThread) return ;

        static int mnConnectedCount;
        static bool mbMultiThread = false;
        std::wstring strID;
        size_t start = 0,end = 0;
        float fSpeedMbitssec = 0.00f;
#ifdef _DEBUG
        OutputDebugStringW(strRawLog.c_str());
        OutputDebugStringW(L"\n");
#endif  
        Log4WI(strRawLog.c_str());
        pThroughputTestThread->SendWindowMsg(strRawLog);
        //if (NULL != JGW_WStrCaseWStr(strRawLog.c_str(),L"Server listening on"))
        //{
        //    pThroughputTestThread->mbStopIperf3Server = false;
        //}

        if (NULL != JGW_WStrCaseWStr(strRawLog.c_str(),L"connected to")) mnConnectedCount++;
        if (mnConnectedCount > 1) mbMultiThread = true;
        if (1 == mnConnectedCount) mbMultiThread = false;
        if (mbMultiThread && NULL == JGW_WStrCaseWStr(strRawLog.c_str(),L"[SUM]")) return;
        if (!HandIperfTestLogSpeed(strRawLog,strID,start,end,fSpeedMbitssec)) return;
        mnConnectedCount = 0;

        if (end == start + 1)
        {
            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"%.2f",fSpeedMbitssec);
            pThroughputTestThread->mpAsyncWndMessage->PutAsyncStringMessage(WM_THROUGHPUT_RATE_MSG,strTemp.c_str());
            pThroughputTestThread->mbStopIperf3Server = true;
        }
        else
        {
            pThroughputTestThread->mbStopIperf3Server = false;
        }

    }
}
