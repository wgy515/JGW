#pragma once
#include "CJGW_TcpStatsImplThread.h"
#include <HPSocket/HPSocket.h>
#include "Common/Src/STLHelper.h"
#include "Common/Src/WaitFor.h"
#include "JGW_DonglesThroughputTest_Define.h"
#include <vector>

namespace JGW
{
    class CCJGW_TcpClientThread : public CCJGW_TcpStatsImplThread,public CTcpClientListener
    {
    public:
        CCJGW_TcpClientThread(const S_ThroughputTest_Confige& sThroughputTest);
        virtual ~CCJGW_TcpClientThread(void);
    public:
        void StartClient(WPARAM wParam,LPARAM lParam);

        void StopClient(WPARAM wParam,LPARAM lParam);
    private:
        void SendBufferToServer(); 

        E_SOCKET_TERIMALTYPE GetSocketThreadType() { return E_CLIENT_TYPE;}

        virtual EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket);
        virtual EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID);
        virtual EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID);
        virtual EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
        virtual EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
        virtual EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        CBufferPtr m_sendBuffer;
        VectorWrapper<std::vector<CTcpClientPtr*>, PtrSet_Cleaner<std::vector<CTcpClientPtr*>>> m_vtClients;
    };
}