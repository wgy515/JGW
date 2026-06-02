#include "StdAfx.h"
#include "CJGW_AsyncSocket.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#pragma comment(lib,"ws2_32.lib")

namespace JGW
{
    CCJGW_AsyncSocket::CCJGW_AsyncSocket(void)
    {
    }


    CCJGW_AsyncSocket::~CCJGW_AsyncSocket(void)
    {
    }

    bool CCJGW_AsyncSocket::ConnectSocketTimeOut(const std::string& server_name,size_t port,int send_recv_timeout /* = 10 * 1000 */,int connectTimeOut /* = 10 * 1000 */,const char* source_addr /* = NULL */,int family /* = AF_INET */,int socktype /* = SOCK_STREAM */)
    {
        std::string strPort;
        JGW_FormatString(strPort,"%d",port);
        return ConnectSocketTimeOut(server_name.c_str(),strPort.c_str(),send_recv_timeout,connectTimeOut,source_addr,family,socktype);
    }

    bool CCJGW_AsyncSocket::ConnectSocketTimeOut(const char* server_name,const char* port,int send_recv_timeout /* = 10 * 1000 */,int connectTimeOut /* = 10 * 1000 */,const char* source_addr /* = NULL */,int family /* = AF_INET */,int socktype /* = SOCK_STREAM */)
    {
        jgw_timer tm;
        while (tm.elapsed() <= (connectTimeOut / 1000.00))
        {
            if (CCJGW_SyncSocket::ConnectSocket(server_name,port,source_addr,family,socktype)) break;
            Sleep(200);
        }
        if (!CheckSocketIsConnected()) return false;
        setsockopt(mConnSocket ,SOL_SOCKET, SO_RCVTIMEO,(char *)&send_recv_timeout,sizeof (send_recv_timeout));
        setsockopt(mConnSocket ,SOL_SOCKET, SO_SNDTIMEO,(char *)&send_recv_timeout,sizeof (send_recv_timeout));
        return true;
    }

    bool CCJGW_AsyncSocket::AsyncWriteSocket(const std::string& strBuf)
    {
        return (SendSocket(strBuf.c_str(),strBuf.length()) > 0);
    }

    bool CCJGW_AsyncSocket::AsyncWriteSocket(const char* buf,int len)
    {
        return (SendSocket(buf,len) > 0);
    }

    bool CCJGW_AsyncSocket::AsyncReadSocket(char* buf,size_t buf_size)
    {
        return (RecvSocket(buf,buf_size) > 0);
    }
}