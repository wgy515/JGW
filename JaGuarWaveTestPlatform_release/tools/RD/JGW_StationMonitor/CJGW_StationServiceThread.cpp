#include "StdAfx.h"
#include "CJGW_StationServiceThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

#define TSE_FTM_MAX_READ_BUF 1024 * 4

namespace JGW
{
    CCJGW_StationServiceThread::CCJGW_StationServiceThread(void) : macceptorSocket(mios,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),4050)),mdeadline_(mios)
    {
        mTxBuf = (char*)calloc(0x01,TSE_FTM_MAX_READ_BUF);
        mRxBuf = (char*)calloc(0x01,TSE_FTM_MAX_READ_BUF);
        msReqThreadParam.mpMessageThread = this;
        msReqThreadParam.mbThreadIsWorking = false;
    }


    CCJGW_StationServiceThread::~CCJGW_StationServiceThread(void)
    {
        free(mTxBuf);
        free(mRxBuf);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_StationServiceThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CCJGW_StationServiceThread::OnStartServiceThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_StationServiceThread::OnStartServiceThread(WPARAM wParam,LPARAM lParam)
    {
        mpsStationMonitorUiInfo = (PS_STATION_MONITOR_UI_INFO)wParam;
        try
        {
            StartAccept();
            mios.run();
            msReqThreadParam.mbStopReqThreadWork = true;
            msReqThreadParam.mClientSocket = NULL;
        }
        catch (std::exception& e)
        {
            Log4AE_F("asio run exception(%s)",e.what());
        }
    }

    void CCJGW_StationServiceThread::StartAccept()
    {
        msReqThreadParam.mbStopReqThreadWork = true;
        if (!macceptorSocket.is_open()) return ;
        boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(mios));
        macceptorSocket.async_accept(*sock,boost::bind(&CCJGW_StationServiceThread::AcceptHandle,this,boost::asio::placeholders::error,sock));
    }

    void CCJGW_StationServiceThread::AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        //! 请求 accept 出现异常
        if (ec) 
        {
            //! mAsyncWndMessage->PutAsyncMessage(WM_CLOSE);
            msReqThreadParam.mClientSocket = NULL;
            msReqThreadParam.mbStopReqThreadWork = true;
            Log4AE_F("Wlan Service Thread AcceptHandle Exception (%s)",ec.message().c_str());
        }
        else
        {
            //             boost::system::error_code ec;       
            //             mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
            msReqThreadParam.mClientSocket = sock;
            sock->async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_StationServiceThread::ReadSocket,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
        }
    }

    void CCJGW_StationServiceThread::ReadSocket(const boost::system::error_code& ec,size_t len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            std::string strError = ec.message().c_str();
            Log4AE_F("read socket fail(%s)",ec.message().c_str());
            StartAccept();
        }
        else
        {
            mRxBuf[len] = '\0';
            Log4AI_F("r: %s",mRxBuf);
            HandleRequest(mRxBuf);
            sock->async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_StationServiceThread::ReadSocket,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
        }
    }

    void CCJGW_StationServiceThread::WriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            std::string strError = ec.message().c_str();
            Log4AE_F("write socket fail(%s)",ec.message().c_str());
            StartAccept();
        }
        else
        {
            //             boost::system::error_code ec;       
            //             mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
            sock->async_read_some(boost::asio::buffer(mRxBuf,TSE_FTM_MAX_READ_BUF),boost::bind(&CCJGW_StationServiceThread::ReadSocket,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
        }
    }

    void CCJGW_StationServiceThread::WriteSocketOnly(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock,boost::shared_ptr<std::vector<char>> ptrBuf)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            std::string strError = ec.message().c_str();
            Log4AE_F("only write socket fail(%s)",ec.message().c_str());
            StartAccept();
        }
        else
        {
            Log4AI_F("w: %s",&(*ptrBuf)[0]);
        }
    }

    //! {"responseCode":1101,"cmdStr":"iperf3 -s","ip":"","time":"01:17:05"}
    //! {"responseCode":1101,"cmdStr":"iperf -s","ip":"192.168.133.43","time":"02:18:41"}
    void CCJGW_StationServiceThread::HandleRequest(const std::string& strRequest)
    {
        CCJGW_StationSocketRequestJson mStationSocketRequestJson;
        mStationSocketRequestJson.FromJsonToString(strRequest);

        if (STATION_SOCKET_RESPONSE_CODE == mStationSocketRequestJson.responseCode)
        {
            //! 终止信息上传
            if (JGW_StrCaseStr(mStationSocketRequestJson.cmdStr.c_str(),"stop"))
            {
                msReqThreadParam.mbStopReqThreadWork = true;
                return;
            }

            if (!mStationHandleRequestThread.IsThreadRun()) mStationHandleRequestThread.CreateMessageThread(0,0,true);
            msReqThreadParam.mstrSocketAddres = mStationSocketRequestJson.ip;
            msReqThreadParam.mstrCmd = mStationSocketRequestJson.cmdStr;
            //! 如果请求线程正在运行任务，则终止请求线程运行任务并等待线程当前任务结束
            while (msReqThreadParam.mbThreadIsWorking)
            {
                msReqThreadParam.mbStopReqThreadWork = true;
                Sleep(200);
            }
            //！ 重置线程运行任务标记位
            msReqThreadParam.mbStopReqThreadWork = false;
            msReqThreadParam.mbThreadIsWorking = true;
            mStationHandleRequestThread.PostThreadMessage(WM_TEST_THREAD_MSG_RUN,WPARAM(&msReqThreadParam),LPARAM(mpsStationMonitorUiInfo));
        }
    }

    bool CCJGW_StationServiceThread::SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (!sock || !sock->is_open()) return false;
        boost::shared_ptr<std::vector<char>> ptrBuf(new std::vector<char>(strJson.length() + 1,0));
        strcpy_s(&(*ptrBuf)[0],strJson.length() + 1,strJson.c_str());
        sock->async_write_some(boost::asio::buffer(*ptrBuf),boost::bind(&CCJGW_StationServiceThread::WriteSocketOnly,this,_1,_2,sock,ptrBuf));
        return true;
    }
} 
