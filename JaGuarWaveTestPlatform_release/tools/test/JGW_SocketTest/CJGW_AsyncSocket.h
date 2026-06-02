#pragma once
#include "CJGW_SyncSocket.h"
namespace JGW
{
    class CCJGW_AsyncSocket : public CCJGW_SyncSocket
    {
    public:
        CCJGW_AsyncSocket(void);
        virtual ~CCJGW_AsyncSocket(void);

    public:
        bool ConnectSocketTimeOut(const char* server_name,const char* port,int send_recv_timeout = 10 * 1000,int family = AF_INET,int socktype = SOCK_STREAM);
    };
}