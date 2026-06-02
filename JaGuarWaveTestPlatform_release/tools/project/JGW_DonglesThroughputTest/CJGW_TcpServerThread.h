#pragma once
#include "CJGW_TcpStatsImplThread.h"
#include <HPSocket/HPSocket.h>
#include "Common/Src/STLHelper.h"
#include "JGW_DonglesThroughputTest_Define.h"

#ifdef _DEBUG
#pragma comment(lib,"HPSocket_UD.lib")
#else
#pragma comment(lib,"HPSocket_U.lib")
#endif

namespace JGW
{
    class CCJGW_TcpServerThread : public CCJGW_TcpStatsImplThread,public CTcpServerListener
    {
    public:
        CCJGW_TcpServerThread(const S_ThroughputTest_Confige& sThroughputTest);
        ~CCJGW_TcpServerThread(void);
    private:
        void StartTcpServer(WPARAM wParam,LPARAM lParam);

        void StopTcpServer(WPARAM wParam,LPARAM lParam);        
    private:
        E_SOCKET_TERIMALTYPE GetSocketThreadType() { return E_SERVICE_TYPE;}
        //! 准备监听 绑定监听地址前触发
        virtual EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen);
        //! 接受连接请求 客户连接请求到达时触发
        virtual EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient);
        //! 握手完成 握手完成时触发
        virtual EnHandleResult OnHandShake(ITcpServer* pSender, CONNID dwConnID);	
        //! 数据到达，接收到数据后触发
        /*virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength);*/
        virtual EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
        //! 数据已发送 数据发送成功后触发
        virtual EnHandleResult OnSend(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength);
        //!
        virtual EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
        //! 关闭通信组件 通信组件停止后触发
        virtual EnHandleResult OnShutdown(ITcpServer* pSender);
    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        CTcpServerPtr m_Server;
        CBufferPtr m_sendBuffer;
    };
}