#include "StdAfx.h"
#include "CJGW_UpdateServiceThread.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/jgw_single_application_instance.h>

#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JaGuarWaveHelp/JaGuarWaveHelpDefine.h>

namespace JGW
{
    CCJGW_UpdateServiceThread::CCJGW_UpdateServiceThread(S_HELP_V3_CONFIG& sHelpV3Config) : msHelpV3Config(sHelpV3Config),macceptorSocket(mios,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),sHelpV3Config.mnUpdateServiceSocketPort)),/*mDownloadThread(sHelpConfig),*/mdeadline_(mios),mstrHeartbeatPacket("{\"helpRspType\":-1,\"status\":\"true\",\"bUpdate\":\"false\",\"error_msg\":\"\"}")
    {
        mTxBuf = (char*)calloc(0x01,SOCKET_TX_BUF_MAX);
        mRxBuf = (char*)calloc(0x01,SOCKET_RX_BUF_MAX); 
        Log4WI_F(L"Server Port:%d",sHelpV3Config.mnUpdateServiceSocketPort);
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_UpdateServiceThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_THREAD_TEST_START_MSG,&CCJGW_UpdateServiceThread::OnStartServiceThread)
    END_MYTHREAD_MESSAGE_MAP()

    CCJGW_UpdateServiceThread::~CCJGW_UpdateServiceThread(void)
    {
        free(mTxBuf);
        free(mRxBuf);
    }

    void CCJGW_UpdateServiceThread::CheckDeadline(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            boost::system::error_code ecclose;
            macceptorSocket.close(ecclose);
            msHelpV3Config.msDownloadThreadParam.mClientSock = NULL;
            mdeadline_.cancel(ecclose);
            return;
        }  
    }

    void CCJGW_UpdateServiceThread::SendHeartbeatPacket(const boost::system::error_code e)
    {
        if (e) return;
        //! 真正的等待超时
        if (mdeadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())  
        {
            if (msHelpV3Config.msDownloadThreadParam.mClientSock && msHelpV3Config.msDownloadThreadParam.mClientSock->is_open())
            {
                mAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W(mstrHeartbeatPacket).c_str());   
                msHelpV3Config.msDownloadThreadParam.mClientSock->async_write_some(boost::asio::buffer(mstrHeartbeatPacket),boost::bind(&CCJGW_UpdateServiceThread::SendSocketData,this,_1,_2,msHelpV3Config.msDownloadThreadParam.mClientSock));
                boost::system::error_code ec_error;
                mdeadline_.expires_from_now(boost::posix_time::minutes(1),ec_error);
                mdeadline_.async_wait(boost::bind(&CCJGW_UpdateServiceThread::SendHeartbeatPacket, this,_1));
            }    
        }
    }

    void CCJGW_UpdateServiceThread::StopUpdateServiceThread()
    {
        boost::system::error_code ec_error;
        mdeadline_.expires_from_now(boost::posix_time::microsec(100),ec_error);
        mdeadline_.async_wait(boost::bind(&CCJGW_UpdateServiceThread::CheckDeadline, this,_1));
        while (mbStopAsio) Sleep(200);
    }

    void CCJGW_UpdateServiceThread::OnStartServiceThread(WPARAM wParam,LPARAM lParam)
    {
        mAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        HWND hWnd = (HWND)lParam;
        mbStopAsio = true;
        try
        {
            StartAsyncAccept();
            mios.run();
        }
        catch (std::exception&)
        {

        }
        mbStopAsio = false;
        //! JaGuarWaveHelp守护进程不关闭
#if 1
        {
#if 0
            ShellExecute(NULL,L"open",JGW_GetApplicationName2HModule(ghInstance).c_str(),NULL,JGW_GetApplicationFolder(),SW_NORMAL);
#endif
            PostMessage(hWnd,WM_CLOSE,NULL,NULL);
        }  
#endif
    }

    void CCJGW_UpdateServiceThread::StartAsyncAccept()
    {
        boost::shared_ptr<boost::asio::ip::tcp::socket> sock(new boost::asio::ip::tcp::socket(mios));
        macceptorSocket.async_accept(*sock,boost::bind(&CCJGW_UpdateServiceThread::AcceptHandle,this,boost::asio::placeholders::error,sock));
    }

    void CCJGW_UpdateServiceThread::AcceptHandle(const boost::system::error_code& ec,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec) return;
        msHelpV3Config.msDownloadThreadParam.mClientSock = sock;
        sock->async_read_some(boost::asio::buffer(mRxBuf,SOCKET_RX_BUF_MAX),boost::bind(&CCJGW_UpdateServiceThread::RecvSocketData,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
        boost::system::error_code ec_error;
        mdeadline_.expires_from_now(boost::posix_time::minutes(1),ec_error);
        mdeadline_.async_wait(boost::bind(&CCJGW_UpdateServiceThread::SendHeartbeatPacket, this,_1));
        StartAsyncAccept();
    }

    void CCJGW_UpdateServiceThread::RecvSocketData(const boost::system::error_code& ec,size_t rx_len,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            return ;
        }
        else
        {
            mRxBuf[rx_len] = '\0';
            mAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W_A(mRxBuf).c_str());
            HandleHelpReq(mRxBuf,sock);
        }
    }

    void CCJGW_UpdateServiceThread::OnRspDownloadThreadResultMsg(WPARAM wParam,LPARAM lParam)
    {
        LPS_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM psDownloadThreadParam = (LPS_JGW_HELP_V2_DOWNLOAD_THREAD_PARAM)wParam;
        bool bResult = (1 == lParam);
        if (!psDownloadThreadParam->mClientSock || !psDownloadThreadParam->mClientSock->is_open()) return ;
        SendJsonCommand(psDownloadThreadParam->mstrJsonCommand,psDownloadThreadParam->mClientSock);
    }

    void CCJGW_UpdateServiceThread::SingleWriteSocket(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            return ;
        }
    }

    void CCJGW_UpdateServiceThread::SendSocketData(const boost::system::error_code& ec,size_t txLenCompleted,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        if (ec)
        {
            boost::system::error_code ecclose;
            sock->close(ecclose);
            return ;
        }
        else
        {
            sock->async_read_some(boost::asio::buffer(mRxBuf,SOCKET_RX_BUF_MAX),boost::bind(&CCJGW_UpdateServiceThread::RecvSocketData,this,boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred,sock));
        }
    }

    void CCJGW_UpdateServiceThread::HandleHelpReq(const std::string& strRequest,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        CCJGW_HelpReqBaseJson requestBaseJson;
        CCJGW_FTM_Json ftmJson;
        ftmJson.FromJosn(strRequest);
        requestBaseJson.FromJosn(ftmJson);

        switch (requestBaseJson.helpReqType)
        {
        case E_HELP_REQ_CHECK_SOFTWARE_UPDATE_TYPE:
            HandleHelpDownloadReq(ftmJson,sock);
            break;
#if 0
        case E_HELP_REQ_UPDATE_TYPE:
            HandleHelpUpdateReq(ftmJson,sock);
            break;
#endif
        }
    }

    void CCJGW_UpdateServiceThread::HandleHelpDownloadReq(CCJGW_FTM_Json& ftmJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        CCJGW_HelpReqDownloadJson helpReqDownloadJson;
        helpReqDownloadJson.FromJosn(ftmJson);
        //! 
        //! msHelpConfig.msDownloadThreadParam.mClientSock = sock;
        msHelpV3Config.msDownloadThreadParam.mpParam = this;
        //! 新增检查ExecFolder与当前JaGuarWaveHelpV3的目录是否一致，如果不一致则JaGuarWaveHelpV3程序自行退出
        std::wstring strTemp = JGW_A2W(helpReqDownloadJson.execFolder,CP_UTF8);
        JGW_ReplaceStringW(strTemp,L"\\\\",L"\\");
        if (0 != JGW_WStrComparenoCaseWStr(strTemp.c_str(),msHelpV3Config.msDownloadThreadParam.mstrExecFolder.c_str()))
        {
            PostMessage(gMainHwnd,WM_CLOSE,NULL,NULL);
            return ;
        }

        //msHelpV3Config.msDownloadThreadParam.mstrExecFolder = JGW_A2W(helpReqDownloadJson.execFolder,CP_UTF8);
        std::wstring strTask = JGW_A2W(helpReqDownloadJson.taskNumber,CP_UTF8);
        if (0 == JGW_WStrComparenoCaseWStr(strTask.c_str(),msHelpV3Config.msDownloadThreadParam.mstrTaskNumber.c_str()))
        {
            msHelpV3Config.msDownloadThreadParam.mbIsUpdateTaskNumber = false;
        }
        else
        {
            msHelpV3Config.msDownloadThreadParam.mbIsUpdateTaskNumber = true;
        }
        msHelpV3Config.msDownloadThreadParam.mstrTaskNumber = strTask;

        LoadExecVersionConfig(JGW_A2W(helpReqDownloadJson.execFolder));
        //! 发送请求线程下载消息
        PostMessage(gMainHwnd,WM_HWND_HELP_REQ_DOWNLOAD_MSG,NULL,NULL);
    }

    void CCJGW_UpdateServiceThread::SendJsonCommand(const std::string& strJson,boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
    {
        memset(mTxBuf,0x00,SOCKET_TX_BUF_MAX);
        strcpy_s(mTxBuf,SOCKET_TX_BUF_MAX,strJson.c_str());
        mAsyncWndMessage->PutAsyncStringMessage(WM_HWND_RICHEDIT_APPEND_TEST_MSG,JGW_A2W_A(mTxBuf).c_str());
        sock->async_write_some(boost::asio::buffer(mTxBuf,strlen(mTxBuf)),boost::bind(&CCJGW_UpdateServiceThread::SendSocketData,this,_1,_2,sock));
    }


    void CCJGW_UpdateServiceThread::LoadExecVersionConfig(const std::wstring& strExecFolder)
    {
        std::wstring strVersionFilePath(strExecFolder);
        CCJGW_ConfigIni versionIni;
        strVersionFilePath += _T("\\version.ini");

        versionIni.InitIniFilePath(strVersionFilePath.c_str());
        msHelpV3Config.msVersionConfig.mstrIntsallTaskNumber = L"";
        msHelpV3Config.msVersionConfig.mstrInstallVersion = versionIni.GetIniKeyValue(L"install",L"version");
        msHelpV3Config.msVersionConfig.mstrUpdateVersion = versionIni.GetIniKeyValue(L"update",L"version");
        msHelpV3Config.msVersionConfig.mstrConfigVersion = versionIni.GetIniKeyValue(L"config",L"version");

        if (std::wstring::npos != msHelpV3Config.msVersionConfig.mstrInstallVersion.find(L"-"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(msHelpV3Config.msVersionConfig.mstrInstallVersion.c_str(),L"-",vstrTemp);
            if (2 == vstrTemp.size())
            {
                msHelpV3Config.msVersionConfig.mstrInstallVersion = vstrTemp[0];
                msHelpV3Config.msVersionConfig.mstrIntsallTaskNumber = vstrTemp[1];
            }
        }
    }
}