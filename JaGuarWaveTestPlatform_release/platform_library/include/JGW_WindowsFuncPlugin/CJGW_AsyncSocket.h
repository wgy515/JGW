#pragma once
#include "CJGW_SyncSocket.h"
namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_AsyncSocket : public CCJGW_SyncSocket
    {
    public:
        CCJGW_AsyncSocket(void);
        virtual ~CCJGW_AsyncSocket(void);

    public:
        bool ConnectSocketTimeOut(const std::string& server_name,size_t port,int send_recv_timeout = 10 * 1000,int connectTimeOut = 10 * 1000,const char* source_addr = NULL,int family = AF_INET,int socktype = SOCK_STREAM);

        bool ConnectSocketTimeOut(const char* server_name,const char* port,int send_recv_timeout = 10 * 1000,int connectTimeOut = 10 * 1000,const char* source_addr = NULL,int family = AF_INET,int socktype = SOCK_STREAM);

        bool AsyncReadSocket(char* buf,size_t buf_size);

        bool AsyncWriteSocket(const std::string& strBuf);

        bool AsyncWriteSocket(const char* buf,int len);
    };
}