#include "StdAfx.h"
#include "CJGW_FTM_ClientSocket.h"
#include "TSE_FTM_Define.h"
#include "..\TSE_DevicesGlobalResource.h"
#include "..\TSE_SendPipeCommand.h"
#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>

namespace JGW
{
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) LogE_F(msg,ec.message().c_str());

    CCJGW_FTM_ClientSocket::CCJGW_FTM_ClientSocket() :mTimeOutSec(5),mbResult(true)
    {
    }


    CCJGW_FTM_ClientSocket::~CCJGW_FTM_ClientSocket(void)
    {
        CloseSocket();
    }
    //! 
    bool CCJGW_FTM_ClientSocket::IsAdbTcpIP()
    {
        std::wstring stFtmConnectMode;
        stFtmConnectMode = GetGlobalEnvironment()->GetString(L"TSE_FTM_CONNECT_MODE");
        return NULL == JGW_WStrComparenoCaseWStr(stFtmConnectMode.c_str(),L"ADB_TCPIP");
    }

    bool CCJGW_FTM_ClientSocket::AsyncAdbTcpIPConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec)
    {
        if (IsAdbTcpIP())
        {
            CTSE_SendPipeCommand sendPipeCommand;
            sendPipeCommand.TSE_AddParam(L"CommandCount",L"4");

            sendPipeCommand.TSE_AddParam(L"Command1",L"adb disconnect");
            sendPipeCommand.TSE_AddParam(L"Recv1",L"disconnected everything");
            sendPipeCommand.TSE_AddParam(L"SendCounts1",L"10");
            sendPipeCommand.TSE_AddParam(L"TimeOut1",L"10");

            sendPipeCommand.TSE_AddParam(L"Command2",L"adb tcpip 5555");
            sendPipeCommand.TSE_AddParam(L"Recv2",L"");
            sendPipeCommand.TSE_AddParam(L"SendCounts2",L"10");
            sendPipeCommand.TSE_AddParam(L"TimeOut2",L"10");

            sendPipeCommand.TSE_AddParam(L"Command3",L"adb connect $TSE_FTM_CLIENT_SOCKET_IP_ADDRESS$");
            sendPipeCommand.TSE_AddParam(L"Recv3",L"connected to");
            sendPipeCommand.TSE_AddParam(L"SendCounts3",L"10");
            sendPipeCommand.TSE_AddParam(L"TimeOut4",L"10");

            std::wstring strTemp;
            JGW_FormatWString(strTemp,L"adb forward tcp:%s tcp:49150", GetGlobalEnvironment()->GetString(L"TSE_FTM_CLIENT_PORT").c_str());
            sendPipeCommand.TSE_AddParam(L"Command4",strTemp.c_str());
            sendPipeCommand.TSE_AddParam(L"Recv4",L"");
            sendPipeCommand.TSE_AddParam(L"SendCounts4",L"10");
            sendPipeCommand.TSE_AddParam(L"TimeOut4",L"10");

            sendPipeCommand.TSE_Init();
            sendPipeCommand.TSE_Run();
            sendPipeCommand.TSE_Exit();
        }

        mbResult = false;
        if (!mAsyncSocket.ConnectSocketTimeOut(strSocketIP,socketPort,timeOutSec * 1000,timeOutSec * 1000))
        {
            return false;
        }
        char buf[80] = {0};
        if (!mAsyncSocket.AsyncReadSocket(buf,80))
        {
            return false;
        }
        return (mbResult = (NULL != strstr(buf,MSG_CLINET_CONNECT_SUCCESS)));
    }
   

    bool CCJGW_FTM_ClientSocket::AsyncWriterSocket(const std::string& strBuf,size_t timeOutSec)
    {
        return (mbResult = mAsyncSocket.AsyncWriteSocket(strBuf));
    }


    bool CCJGW_FTM_ClientSocket::AsyncReadSocket(char* rxBuf,size_t rxLen,size_t timeOutSec)
    {
        return (mbResult = mAsyncSocket.AsyncReadSocket(rxBuf,rxLen));
    }

    void CCJGW_FTM_ClientSocket::CloseSocket()
    {
        mAsyncSocket.CleanupSocket();
    }

    bool CCJGW_FTM_ClientSocket::CheckResult()
    {
        return mbResult;
    }
}