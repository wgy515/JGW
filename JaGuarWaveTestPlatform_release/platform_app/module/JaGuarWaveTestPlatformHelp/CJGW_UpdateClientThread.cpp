#include "StdAfx.h"
#include "CJGW_UpdateClientThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_StartHelpServiceDlg.h"


#define PRINT_ERRORCODE(ec,msg)\
    if (ec) PELOG4WW_F(msg,JGW_A2W(ec.message()).c_str());

namespace JGW
{
    CCJGW_UpdateClientThread::CCJGW_UpdateClientThread(void) : mSocket(mios),mdeadline_(mios),mhMainWnd(NULL),mbFristConnect(true),mbConnected(false)
    {
        mszTxBuf = (char*)calloc(1,SOCKET_TX_BUF_MAX);
        mszRxBuf = (char*)calloc(1,SOCKET_RX_BUF_MAX);
    }


    CCJGW_UpdateClientThread::~CCJGW_UpdateClientThread(void)
    {
        free(mszTxBuf);
        free(mszRxBuf);
    } 

    void CCJGW_UpdateClientThread::CloseClickSocket(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            boost::system::error_code ec;
            mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
            mdeadline_.cancel(ec);
            CloseSocket();
            PDLOG4WW_F(L"Close Client Help Socket");
        }  
    }

    void CCJGW_UpdateClientThread::StopClientThread()
    {
        boost::system::error_code ec_error;
        mdeadline_.expires_from_now(boost::posix_time::microsec(100),ec_error);
        PRINT_ERRORCODE(ec_error,L"deadline_.expires_from_now (%s)\n");
        mdeadline_.async_wait(boost::bind(&CCJGW_UpdateClientThread::CloseClickSocket, this,_1));
        //! mSocket.cancel(ec_error);
        while (mbConnected) Sleep(200);
    }

    void CCJGW_UpdateClientThread::SetMainWindowsHwnd(HWND hMainHwnd)
    {
        mhMainWnd = hMainHwnd;
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UpdateClientThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CCJGW_UpdateClientThread::OnStartUpdateClientThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_UpdateClientThread::CheckHelpServiceIsStart()
    {
        CCJGW_StartHelpServiceDlg dlg(mhMainWnd);
        dlg.DoModal();
    }


    void CCJGW_UpdateClientThread::OnStartUpdateClientThread(WPARAM wParam,LPARAM lParam)
    {
        LPS_JGW_HELP_CONFIG psHelpConfig = (LPS_JGW_HELP_CONFIG)wParam;
        mpsHelpConfig = psHelpConfig;
        mhMainWnd = (HWND)lParam;
        mTimeOutSec = psHelpConfig->mnSocketTimeoutSec;
        mstrTaskNumber = JGW_W2A(psHelpConfig->mstrTaskNumber);
        
        Log4WI_F(L"OnStartUpdateClientThread TaskNumber:%s,IP:%s,Port:%d",psHelpConfig->mstrTaskNumber.c_str(),JGW_A2W(psHelpConfig->mstrHelpServiceIPAddr).c_str(),psHelpConfig->mnUpdateServiceSocketPort);
        mios.reset();
        CheckHelpServiceIsStart();
        mbConnected = true;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(psHelpConfig->mstrHelpServiceIPAddr),psHelpConfig->mnUpdateServiceSocketPort);
        boost::system::error_code ec;

        mdeadline_.expires_from_now(boost::posix_time::seconds(10),ec);
        PRINT_ERRORCODE(ec,L"deadline_.expires_from_now (%s)\n");
        mdeadline_.async_wait(boost::bind(&CCJGW_UpdateClientThread::CheckDeadline, this,_1));

        mSocket.async_connect(ep,boost::bind(&CCJGW_UpdateClientThread::ConnectedUpdateSocket,this,_1));

        try
        {
            mios.run(ec);
        }
        catch (std::exception& e)
        {
            PELOG4WW_F(L"OnStartUpdateClientThread Run Error(%s)",JGW_A2W_A(e.what()).c_str());
        }
        mbConnected = false;
    }

    void CCJGW_UpdateClientThread::ConnectedUpdateSocket(const boost::system::error_code& ec)
    {
        if (ec) 
        {
            Log4WE_F(L"ConnectedUpdateSocket Error:%s",JGW_A2W(ec.message()).c_str());
            TimeOut(ec.message().c_str());
            CheckHelpServiceIsStart();
            return ;
        }

        boost::system::error_code ec_error;
        mdeadline_.expires_at(boost::posix_time::pos_infin,ec_error);
        //mdeadline_.expires_from_now(boost::posix_time::seconds(/*mTimeOutSec*/10),ec_error);
        PRINT_ERRORCODE(ec_error,L"deadline_.expires_from_now (%s)\n");
        mdeadline_.async_wait(boost::bind(&CCJGW_UpdateClientThread::CheckDeadline, this,_1));

        std::string strReqDownloadJson = GetReqDownloadJson(mstrTaskNumber);
        strcpy_s(mszTxBuf,SOCKET_TX_BUF_MAX,strReqDownloadJson.c_str());
        Log4WI_F(L"Send Msg:%s",JGW_A2W(strReqDownloadJson).c_str());
        mSocket.async_write_some(boost::asio::buffer(mszTxBuf,strReqDownloadJson.length()),boost::bind(&CCJGW_UpdateClientThread::WriteredSocket,this,_1,_2,strReqDownloadJson.length()));
        mbFristConnect = true;
    }

    void CCJGW_UpdateClientThread::WriteredSocket(const boost::system::error_code& ec,size_t txLenCompleted,size_t txLen)
    {
        if (ec) 
        {
            Log4WE_F(L"WriteredSocket Error:%s",JGW_A2W(ec.message()).c_str());
            TimeOut(ec.message().c_str());
        }
        else
        {
            mSocket.async_read_some(boost::asio::buffer(mszRxBuf,SOCKET_RX_BUF_MAX),boost::bind(&CCJGW_UpdateClientThread::ReadSocket,this,_1,_2));
        } 
    }

    void CCJGW_UpdateClientThread::ReadSocket(const boost::system::error_code& ec,size_t rxLen)
    {
        if (ec) 
        {
            Log4WE_F(L"ReadSocket Error:%s",JGW_A2W(ec.message()).c_str());
            TimeOut(ec.message().c_str());
        }
        else
        {
            mszRxBuf[rxLen] = '\0';
            Log4WI_F(L"ReadSocket:%s",JGW_A2W_A(mszRxBuf).c_str());
            HandleHelpRsp(mszRxBuf);
        }  
    }

    void CCJGW_UpdateClientThread::CheckDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            Log4WE_F(L"CheckDeadline Error: %s",JGW_A2W(e.message()).c_str());
            TimeOut("wait time out\n");	
            return;
        }  
    }

    void CCJGW_UpdateClientThread::TimeOut(const char* strErrorMsg)
    {
        boost::system::error_code ec;

        mdeadline_.expires_at(boost::posix_time::pos_infin,ec);
        mdeadline_.cancel(ec);
        CloseSocket();
        PELOG4WW(JGW_A2W_A(strErrorMsg).c_str());
        PostMessage(mhMainWnd,WM_HWND_RSP_HELP_CONNECT_ERROR_MSG,NULL,NULL);
    }

    void CCJGW_UpdateClientThread::HandleHelpRsp(const std::string& strRecv)
    {
        CCJGW_HelpRspBaseJson helpRspBaseJson;
        helpRspBaseJson.FromJosnString(strRecv);

        switch (helpRspBaseJson.helpRspType)
        {
        case E_HELP_RSP_HEARTBEAT_TYPE:
            break;
        case E_HELP_RSP_DOWNLOAD_TYPE:
            HandleHelpRspDownloadType(helpRspBaseJson);
            break;
        case E_HELP_RSP_UPDATE_TYPE:
            HandleHelpRspUpdateType(helpRspBaseJson);
            break;
        case E_HELP_RSP_FTP_CONNECT_TYPE:
            if (!helpRspBaseJson.status) PostMessage(mhMainWnd,WM_HWND_RSP_HELP_FTP_CONNECT_ERROR_MSG,NULL,NULL);
            break;
        case E_HELP_RSP_ERROR_MSG:
            if (!helpRspBaseJson.status) SendMessage(mhMainWnd,WM_HWND_RSP_HELP_ERROR_MSG,WPARAM(JGW_A2W(helpRspBaseJson.error_msg).c_str()),NULL);
            break;
        default:
            break;
        }
        mSocket.async_read_some(boost::asio::buffer(mszRxBuf,SOCKET_RX_BUF_MAX),boost::bind(&CCJGW_UpdateClientThread::ReadSocket,this,_1,_2));
    }

    void CCJGW_UpdateClientThread::HandleHelpRspDownloadType(CCJGW_HelpRspBaseJson& helpRspBaseJson)
    {
        if (helpRspBaseJson.bUpdate) //! 需要更新
        {
            PostMessage(mhMainWnd,WM_HWND_RSP_HELP_DOWNLOAD_MSG,NULL,NULL);
        }
        else //! 不需要更新
        {
            //             if (mbFristConnect)
            //             {
            //                 std::string strReqUpdateJson = GetReqUpdateJson(mstrTaskNumber);
            //                 strcpy_s(mszTxBuf,SOCKET_TX_BUF_MAX,strReqUpdateJson.c_str());
            //                 mSocket.async_write_some(boost::asio::buffer(mszTxBuf,strReqUpdateJson.length()),boost::bind(&CCJGW_UpdateClientThread::WriteredSocket,this,_1,_2,strReqUpdateJson.length()));
            //                 mbFristConnect = false;
            //             }
        }
    }

    void CCJGW_UpdateClientThread::HandleHelpRspUpdateType(CCJGW_HelpRspBaseJson& helpRspBaseJson)
    {
        if (helpRspBaseJson.bUpdate) //! 需要更新
        {
            mpsHelpConfig->msUpdateXMLConfig.mstrUpdateVersion = JGW_A2W(helpRspBaseJson.version);
            PostMessage(mhMainWnd,WM_HWND_RSP_HELP_UPDATE_MSG,NULL,NULL);
        }
        else //! 不需要更新
        {
            PostMessage(mhMainWnd,WM_HWND_RSP_HELP_CLOSE_MSG,NULL,NULL);
        }
    }


    //    std::string CCJGW_UpdateClientThread::GetReqUpdateJson(const std::string& strTaskNumber)
    //    {
    //        std::string strTemp;
    //        std::string strExecFolder;
    //#ifdef _DEBUG
    //        strExecFolder = "C:\\JW\\exec\\";
    //#else
    //        JGW_GetModuleWorkPathA(strExecFolder);
    //#endif 
    //        JGW_ReplaceStringA(strExecFolder,"\\","\\\\");
    //        JGW_FormatString(strTemp,"{\"helpReqType\":2,\"taskNumber\":\"%s\",\"execFolder\":\"%s\"}",strTaskNumber.c_str(),strExecFolder.c_str());
    //
    //        return strTemp;
    //    }
    //
    std::string CCJGW_UpdateClientThread::GetReqDownloadJson(const std::string& strTaskNumber)
    {
        std::string strTemp;
        std::string strExecFolder;
#ifdef _DEBUG
        strExecFolder = "C:\\JW\\exec\\";
#else
        JGW_GetModuleWorkPathA(strExecFolder);
#endif 
        JGW_ReplaceStringA(strExecFolder,"\\","\\\\");
        JGW_FormatString(strTemp,"{\"helpReqType\":0,\"taskNumber\":\"%s\",\"execFolder\":\"%s\"}",strTaskNumber.c_str(),strExecFolder.c_str());

        return strTemp;
    }

    void CCJGW_UpdateClientThread::CloseSocket()
    {
        boost::system::error_code ec;
        if (mSocket.is_open()) mSocket.close(ec);
        PRINT_ERRORCODE(ec,L"Close Socket(%s)\n");
    }
}