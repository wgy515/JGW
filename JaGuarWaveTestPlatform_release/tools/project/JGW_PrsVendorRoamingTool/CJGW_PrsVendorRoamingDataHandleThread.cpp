#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingDataHandleThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_PrsVendorRoamingServer.h"

namespace JGW
{
    CCJGW_PrsVendorRoamingDataHandleThread::CCJGW_PrsVendorRoamingDataHandleThread(void) : mbRuningThread(false),_senderSocket("127.0.0.1",PRS_ROAMING_GRAPH_UDP_SOCKET_PORT),_span(250000)
    {
        _socket.bind(Poco::Net::SocketAddress(), true);
    }


    CCJGW_PrsVendorRoamingDataHandleThread::~CCJGW_PrsVendorRoamingDataHandleThread(void)
    {
    }

    bool CCJGW_PrsVendorRoamingDataHandleThread::StartPrsVendorRoamingDataHandleThread(const std::wstring& strLogFolder,Poco::NotificationQueue* pPrsRoamingMessageNotificationQueue,bool bIsSendRoamingGraphMsg,CCJGW_AsyncWndMessage* pAsyncWndMessage)
    {
        std::wstring strPrsVendorAppLog;
        mpPrsRoamingMessageNotificationQueue = pPrsRoamingMessageNotificationQueue;
        mptrAsyncWndMessageHelp.reset(new CCJGW_AsyncWndMessageHelp(pAsyncWndMessage));
        mcPrsVendorRoamingDataHandle.SetAsyncWndMessageHelp(mptrAsyncWndMessageHelp);
        mbIsSendRoamingGraphMsg = bIsSendRoamingGraphMsg;
        //! 
        try
        {
            JGW_FormatWString(strPrsVendorAppLog,L"%s\\prs_vendor_app_%u.log",strLogFolder.c_str(),JGW_GetTimeOfDay());
            mptrPrsVendorLogStream.reset(new Poco::FileOutputStream(JGW_W2A(strPrsVendorAppLog,CP_UTF8)));
            if (!mptrPrsVendorLogStream->good()) return false;
        }
        catch (Poco::Exception ec)
        {
            JGW_FormatWString(mstrErrorMessage,L"Create File(%s) Exception : %s",strPrsVendorAppLog.c_str(),JGW_A2W(ec.displayText(),CP_UTF8).c_str());
            return false;
        }

        //! 
        mbRuningThread = true;
        if (!mcPrsVendorRoamingDataHandleThread.CreateMessageThread(OnRuningPrsVendorRoamingDataHandleThreadCallback,this,true))
        {
            JGW_FormatWString(mstrErrorMessage,L"Error : Create PrsVendorRoamingDataHandle Thread");
            return false;
        }

        return true;
    }

    void CCJGW_PrsVendorRoamingDataHandleThread::OnRuningPrsVendorRoamingDataHandleThreadCallback(void *lpParam)
    {
        CCJGW_PrsVendorRoamingDataHandleThread* pPrsVendorRoamingDataHandleThread = (CCJGW_PrsVendorRoamingDataHandleThread*)lpParam;
        if (pPrsVendorRoamingDataHandleThread)
        {
            pPrsVendorRoamingDataHandleThread->OnRuningPrsVendorRoamingDataHandleThreadThread();
        }
    }

    void CCJGW_PrsVendorRoamingDataHandleThread::OnRuningPrsVendorRoamingDataHandleThreadThread()
    {
        while (mbRuningThread)
        {
            Poco::AutoPtr<CCJGW_PrsRoamingMessageNotification> pNf = static_cast<CCJGW_PrsRoamingMessageNotification*>(mpPrsRoamingMessageNotificationQueue->waitDequeueNotification(1000));
            if (pNf)
            {
                WritePrsVendorAppLog(pNf->GetPrsRoamingMessage());
                if (mbIsSendRoamingGraphMsg)
                {
                    try
                    {
                        if (_socket.poll(_span, Poco::Net::Socket::SELECT_WRITE))
                        {
                            _socket.sendTo(pNf->GetPrsRoamingMessage().c_str(),pNf->GetPrsRoamingMessage().length() > PYTHON_ROAMING_GRAPH_PY_RECV_BUFFER_MAX_LENGTH ? PYTHON_ROAMING_GRAPH_PY_RECV_BUFFER_MAX_LENGTH : pNf->GetPrsRoamingMessage().length(),_senderSocket);
                        }
                    }
                    catch (Poco::Exception ec)
                    {
                        mptrAsyncWndMessageHelp->SendWindowMsg(L"UDP Send Error : %s",JGW_A2W(ec.displayText(),CP_UTF8).c_str());
                    }
                }
                mcPrsVendorRoamingDataHandle.OnHandlePrsVendorRoamingMessage(pNf->GetPrsRoamingMessage());
            }
        }
    }

    void CCJGW_PrsVendorRoamingDataHandleThread::WritePrsVendorAppLog(const std::string& strLog)
    {
        if (!!mptrPrsVendorLogStream && mptrPrsVendorLogStream->good())
        {
            *mptrPrsVendorLogStream << strLog << std::endl;
        }
    }

    std::wstring CCJGW_PrsVendorRoamingDataHandleThread::GetErrorMessage() const
    {
        return mstrErrorMessage;
    }

    bool CCJGW_PrsVendorRoamingDataHandleThread::StopPrsVendorRoamingDataHandleThread()
    {
        mptrPrsVendorLogStream.reset();
        mbRuningThread = false;
        while (mcPrsVendorRoamingDataHandleThread.IsThreadRun()) Sleep(100);
        mptrAsyncWndMessageHelp.reset();
        return true;
    }
}
