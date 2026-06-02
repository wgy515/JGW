#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <string>

namespace JGW
{
    class CCJGW_SyncSocket
    {
    public:
        CCJGW_SyncSocket(void);
        virtual ~CCJGW_SyncSocket(void);
    public:
        bool ConnectSocket(const char* server_name,const char* port,int family = AF_INET,int socktype = SOCK_STREAM);

        int RecvSocket(char* recv_buf,int recv_siz);

        int SendSocket(const char* buf,int buf_size);

        const std::string& GetErrorMsg();

        void CleanupSocket();

        bool CheckSocketIsConnected();
    private:
        bool InitSocket();
        
    protected:
        bool mbInitSocket;
        bool mbConnected;
        struct addrinfo* mAddrptr;
        SOCKET mConnSocket;
        std::string mstrErrorMsg;
    };
}