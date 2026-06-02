#pragma once
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>

#include "CJGW_HelpRspBaseJson.h"
/*
    每隔一段时间
*/
namespace JGW
{
    class CCJGW_UpdateClientThread : public CCMessageThread
    {
    public:
        CCJGW_UpdateClientThread(void);
        ~CCJGW_UpdateClientThread(void);
    public:
        void SetMainWindowsHwnd(HWND hMainHwnd);
        void StopClientThread();
    private:
        void CheckHelpServiceIsStart();
    private:
        void OnStartUpdateClientThread(WPARAM wParam,LPARAM lParam);
        // THREAD_HELP_UPLOAD_LOG_MSG
        void OnSendSocketMsgUploadLog(WPARAM wParam,LPARAM lParam);
    private:
        void ConnectedUpdateSocket(const boost::system::error_code& ec);

        void WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen);
        //!
        void ReadSocket(const boost::system::error_code& ec,size_t rxLen);

        void CloseSocket();
    private:
        //! 获取请求下载JSON 命令
        std::string GetReqDownloadJson(const std::string& strTaskNumber);
        //! 
        //! std::string GetReqUpdateJson(const std::string& strTaskNumber);
    private:
        //! 
        void HandleHelpRsp(const std::string& strRecv);
        //!
        void HandleHelpRspDownloadType(CCJGW_HelpRspBaseJson& helpRspBaseJson);
        //!
        void HandleHelpRspUpdateType(CCJGW_HelpRspBaseJson& helpRspBaseJson);
        //! 检查读写是否超时
        void CheckDeadline(const boost::system::error_code e);
        //！关闭客户端SOCKET
        void CloseClickSocket(const boost::system::error_code e);
        //!
        void TimeOut(const char* strErrorMsg);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP();
    private:
        bool mbConnected; //! 是否正在连接socket 如果正在连接则等待线程运行完成
        char* mszTxBuf;
        char* mszRxBuf;
        size_t mTimeOutSec;
        HWND mhMainWnd;
        bool mbFristConnect;
        boost::asio::io_service mios;
        boost::asio::ip::tcp::socket mSocket;
        boost::asio::deadline_timer mdeadline_;
        std::string mstrTaskNumber;
        LPS_JGW_HELP_CONFIG mpsHelpConfig;
    };
}