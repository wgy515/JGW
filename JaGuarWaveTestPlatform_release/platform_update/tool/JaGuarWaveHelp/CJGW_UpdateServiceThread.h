#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "CJGW_InstallThread.h"
#include "json/CJGW_HelpReqDownloadJson.h"

namespace JGW
{
    class CCJGW_UpdateServiceThread : public CCMessageThread
    {
    public:
        CCJGW_UpdateServiceThread(S_JGW_HELP_CONFIG& sHelpConfig);
        ~CCJGW_UpdateServiceThread(void);
    public:
        //! void OnTimerAutoDownloadThread(LPS_JGW_HELP_DOWNLOAD_THREAD_PARAM psHelpDownloadThreadParam);
        void StopUpdateServiceThread();
    private:
        //! 开始异步接收socket请求
        void StartAsyncAccept();
        //! 收到socket请求
        void AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        //! 接收到socket数据
        void RecvSocketData(const boost::system::error_code& ec/*,boost::shared_ptr<vector<char>> str*/,size_t rx_len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        //! 发送socket数据请求
        void SendSocketData(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);

        void SingleWriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        //!
        void CheckDeadline(const boost::system::error_code e);
        //! 发送心跳包
        void SendHeartbeatPacket(const boost::system::error_code e);
    private:
        //! 分发help请求
        void HandleHelpReq(const std::string& strRecv,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        //! 处理help download请求
        void HandleHelpDownloadReq( CCJGW_FTM_Json& ftmJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
        //! 发送JSON命令
        void SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
    private:
        //! 开启help 服务线程
        void OnStartServiceThread(WPARAM wParam,LPARAM lParam);
    public:
        //! 请求下载结果线程
        void OnRspDownloadThreadResultMsg(WPARAM wParam,LPARAM lParam);
    private:
        void LoadExecVersionConfig(const std::wstring& strExecFolder);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbStopAsio;//! 是否停止了IO完成端口
        char* mTxBuf;
        char* mRxBuf;
        CCJGW_AsyncWndMessage* mAsyncWndMessage;
        S_JGW_HELP_CONFIG& msHelpConfig;
        std::string mstrHeartbeatPacket;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::acceptor macceptorSocket;
        boost::asio::deadline_timer mdeadline_;
        //CCJGW_InstallThread mDownloadThread;
    };
}