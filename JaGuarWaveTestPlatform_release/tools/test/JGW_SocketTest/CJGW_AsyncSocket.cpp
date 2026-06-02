#include "StdAfx.h"
#include "CJGW_AsyncSocket.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
namespace JGW
{
    CCJGW_AsyncSocket::CCJGW_AsyncSocket(void)
    {
    }


    CCJGW_AsyncSocket::~CCJGW_AsyncSocket(void)
    {
    }

    bool CCJGW_AsyncSocket::ConnectSocketTimeOut(const char* server_name,const char* port,int send_recv_timeout /* = 10 * 1000 */,int family /* = AF_INET */,int socktype /* = SOCK_STREAM */)
    {
        jgw_timer tm;
        bool bConnect;

        while (tm.elapsed() > (send_recv_timeout / 1000.00))
        {
            if (!CCJGW_SyncSocket::ConnectSocket(server_name,port,family,socktype)) return false;
        }
        



        setsockopt(mConnSocket ,SOL_SOCKET, SO_RCVTIMEO,(char *)&send_recv_timeout,sizeof (send_recv_timeout));
        setsockopt(mConnSocket ,SOL_SOCKET, SO_SNDTIMEO,(char *)&send_recv_timeout,sizeof (send_recv_timeout));
        return true;
    }
}