#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <string>
#include <vector>
//#include "JGW_WindowsFuncPlugin_Define.h"
namespace JGW
{
    class CCJGW_SyncSocket
    {
    public:
        CCJGW_SyncSocket(void);
        virtual ~CCJGW_SyncSocket(void);
    public:
        //! 连接
        bool ConnectSocket(const char* server_name,const char* port,const char* source_addr = NULL,int family = AF_INET,int socktype = SOCK_STREAM);
        //! 接收
        int RecvSocket(char* recv_buf,int recv_siz);
        //! 发送
        int SendSocket(const char* buf,int buf_size);
        //! 清空
        void CleanupSocket();
        //! 检查连接状态
        bool CheckSocketIsConnected();
        //!
        std::string GetHostIP(std::string& strHost);
        //!
        void GetHostIP(std::string& strHost,std::vector<std::string>& vListIP);
        //!
        SOCKET GetSocket();
    private:
        bool InitSocket();
    protected:
        bool mbInitSocket;
        bool mbConnected;
        struct addrinfo* mAddrptr;
        SOCKET mConnSocket;
    };
}