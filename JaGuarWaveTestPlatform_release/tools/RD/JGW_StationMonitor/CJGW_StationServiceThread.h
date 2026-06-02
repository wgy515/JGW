#pragma once
/**************************
Station服务线程处理上层网关发送的指令信息
**************************/
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "CJGW_StationMonitorDefine.h"
#include "CJGW_StationHandleRequestThread.h"

namespace JGW
{
    class CCJGW_StationServiceThread : public CCMessageThread
    {
    public:
        CCJGW_StationServiceThread(void);
        ~CCJGW_StationServiceThread(void);
    public:
        //! 
        bool SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
    private:
        void OnStartServiceThread(WPARAM wParam,LPARAM lParam);
    private:
        void StartAccept();
        void AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        void ReadSocket(const boost::system::error_code& ec/*,boost::shared_ptr<vector<char>> str*/,size_t len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        void WriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        void WriteSocketOnly(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock,boost::shared_ptr<std::vector<char>> ptrBuf);
    private:
        void HandleRequest(const std::string& strRequest);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        char* mTxBuf;
        char* mRxBuf;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::acceptor macceptorSocket;
        boost::asio::deadline_timer mdeadline_;
        S_STATION_HANDLE_REQ_THREAD_PARAM msReqThreadParam;
        PS_STATION_MONITOR_UI_INFO mpsStationMonitorUiInfo;
        CCJGW_StationHandleRequestThread mStationHandleRequestThread;
    };
}