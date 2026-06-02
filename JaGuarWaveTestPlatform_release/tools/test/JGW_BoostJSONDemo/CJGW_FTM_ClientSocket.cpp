#include "StdAfx.h"
#include "CJGW_FTM_ClientSocket.h"
#define LogE_F //
namespace JGW
{
#define PRINT_ERRORCODE(ec,msg)\
    if (ec) LogE_F(msg,ec.message().c_str());

    CCJGW_FTM_ClientSocket::CCJGW_FTM_ClientSocket() : mClientSocket(mios),mdeadline_(mios),mTimeOutSec(5),mbResult(true)
    {
        mRxBuf = (char*)calloc(0x01,TSE_FTM_MAX_READ_BUF);
    }


    CCJGW_FTM_ClientSocket::~CCJGW_FTM_ClientSocket(void)
    {
        CloseSocket();
        mios.stop();
        if (mRxBuf)
        {
            free(mRxBuf);
            mRxBuf = NULL;
        }
    }
    //! 
    bool CCJGW_FTM_ClientSocket::IsAdbTcpIP()
    {
        return true;
     //   std::wstring stFtmConnectMode;
//         stFtmConnectMode = GetGlobalEnvironment()->GetString(L"TSE_FTM_CONNECT_MODE");
//         return NULL == JGW_WStrComparenoCaseWStr(stFtmConnectMode.c_str(),L"ADB_TCPIP");
    }

    bool CCJGW_FTM_ClientSocket::AsyncAdbTcpIPConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec)
    {
//         if (IsAdbTcpIP())
//         {
//             CTSE_SendPipeCommand sendPipeCommand;
//             sendPipeCommand.TSE_AddParam(L"CommandCount",L"4");
// 
//             sendPipeCommand.TSE_AddParam(L"Command1",L"adb disconnect");
//             sendPipeCommand.TSE_AddParam(L"Recv1",L"disconnected everything");
//             sendPipeCommand.TSE_AddParam(L"SendCounts1",L"10");
//             sendPipeCommand.TSE_AddParam(L"TimeOut1",L"10");
// 
//             sendPipeCommand.TSE_AddParam(L"Command2",L"adb tcpip 5555");
//             sendPipeCommand.TSE_AddParam(L"Recv2",L"");
//             sendPipeCommand.TSE_AddParam(L"SendCounts2",L"10");
//             sendPipeCommand.TSE_AddParam(L"TimeOut2",L"10");
// 
//             sendPipeCommand.TSE_AddParam(L"Command3",L"adb connect $TSE_FTM_CLIENT_SOCKET_IP_ADDRESS$");
//             sendPipeCommand.TSE_AddParam(L"Recv3",L"connected to");
//             sendPipeCommand.TSE_AddParam(L"SendCounts3",L"10");
//             sendPipeCommand.TSE_AddParam(L"TimeOut4",L"10");
// 
//             std::wstring strTemp;
//             JGW_FormatWString(strTemp,L"adb forward tcp:%s tcp:49150", GetGlobalEnvironment()->GetString(L"TSE_FTM_CLIENT_PORT").c_str());
//             sendPipeCommand.TSE_AddParam(L"Command4",strTemp.c_str());
//             sendPipeCommand.TSE_AddParam(L"Recv4",L"");
//             sendPipeCommand.TSE_AddParam(L"SendCounts4",L"10");
//             sendPipeCommand.TSE_AddParam(L"TimeOut4",L"10");
// 
//             sendPipeCommand.TSE_Init();
//             sendPipeCommand.TSE_Run();
//             sendPipeCommand.TSE_Exit();
//         }
        
        return AsyncConnnectSocket(strSocketIP,socketPort,timeOutSec);
    }

    bool CCJGW_FTM_ClientSocket::AsyncConnnectSocket(const std::string& strSocketIP,const size_t socketPort,size_t timeOutSec)
    {
        try
        {
            boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(strSocketIP),socketPort);

            boost::system::error_code ec;
            mios.reset();
            mTimeOutSec = timeOutSec;
            mSocketPort = socketPort;
            mstrSocketIP = strSocketIP;
            mbResult = true;
            mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
            PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
            mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocket::CheckDeadline, this,_1));
            mClientSocket.async_connect(ep,boost::bind(&CCJGW_FTM_ClientSocket::ConnectedSocket,this,_1));
        } 
        catch (boost::system::system_error& ec)
        {
            LogE_F("(%s)\n",ec.what());
            return false;
        }
        mios.run();
        return mbResult;
    }

    void CCJGW_FTM_ClientSocket::ConnectedSocket(const boost::system::error_code& ec)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
            return ;
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
        }
//         
//         mClientSocket.async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_FTM_ClientSocket::ReadSerialNoID,this,_1,_2,mRxBuf));
    }

    void CCJGW_FTM_ClientSocket::ReadSerialNoID(const boost::system::error_code& ec,size_t rxLen,char* rxBuf)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
            CloseSocket();
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
//             if (rxLen < 7) mbResult = false;
//             else mbResult = (NULL != strstr(rxBuf,MSG_CLINET_CONNECT_SUCCESS));
        }   
    }

    bool CCJGW_FTM_ClientSocket::AsyncWriterSocket(const std::string& strBuf,size_t timeOutSec)
    {
        if (!mClientSocket.is_open() && !AsyncAdbTcpIPConnnectSocket(mstrSocketIP,mSocketPort,timeOutSec))  return false;
        boost::system::error_code ec;
        mTimeOutSec = timeOutSec;
        mbResult = true;
        mios.reset();
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
        mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocket::CheckDeadline, this,_1));
        mClientSocket.async_write_some(boost::asio::buffer(strBuf),boost::bind(&CCJGW_FTM_ClientSocket::WriteredSocket,this,_1,_2,strBuf.length()));
        mios.run();
        return mbResult;
    }

    void CCJGW_FTM_ClientSocket::WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
            CloseSocket();
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
        }  
    }

    bool CCJGW_FTM_ClientSocket::AsyncReadSocket(char* rxBuf,size_t rxLen,size_t timeOutSec)
    {
        if (!mClientSocket.is_open() && !AsyncAdbTcpIPConnnectSocket(mstrSocketIP,mSocketPort,timeOutSec))  return false;
        boost::system::error_code ec;
        mTimeOutSec = timeOutSec;
        mbResult = true;
        mios.reset();
        mdeadline_.expires_from_now(boost::posix_time::seconds(timeOutSec),ec);
        PRINT_ERRORCODE(ec,"deadline_.expires_from_now (%s)\n");
        mdeadline_.async_wait(boost::bind(&CCJGW_FTM_ClientSocket::CheckDeadline, this,_1));
        mClientSocket.async_read_some(boost::asio::buffer(rxBuf,rxLen),boost::bind(&CCJGW_FTM_ClientSocket::ReadSocket,this,_1,_2,rxBuf));
        mios.run();
        return mbResult;
    }

    void CCJGW_FTM_ClientSocket::ReadSocket(const boost::system::error_code& ec,size_t rxLen,char* rxBuf)
    {
        if (ec) 
        {
            TimeOut(ec.message().c_str());
            CloseSocket();
        }
        else
        {
            boost::system::error_code ecinfin;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ecinfin);
            rxBuf[rxLen] = '\0';
        }  
    }

    void CCJGW_FTM_ClientSocket::CheckDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            TimeOut("wait time out\n");	
            return;
        }  
    }

    void CCJGW_FTM_ClientSocket::TimeOut(const char* strErrorMsg)
    {
        boost::system::error_code ec;
        mbResult = false;        
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        PRINT_ERRORCODE(ec,"TimeOut(%s)\n");
        LogE_F("%s",strErrorMsg);
        //! CloseSocket();
        mios.reset();
    }

    void CCJGW_FTM_ClientSocket::CloseSocket()
    {
        boost::system::error_code ec;
        if (mClientSocket.is_open())
        {
            mClientSocket.close(ec);
           // LogI_F("断开连接\n");
        }
    }

    bool CCJGW_FTM_ClientSocket::CheckResult()
    {
        return mbResult;
    }
}