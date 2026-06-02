#include "StdAfx.h"
#include "CJGW_SyncSocket.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_SyncSocket::CCJGW_SyncSocket(void) : mAddrptr(NULL),mConnSocket(INVALID_SOCKET),mbConnected(false)
    {
        mbInitSocket = InitSocket();
    }

    CCJGW_SyncSocket::~CCJGW_SyncSocket(void)
    {
        if (mbInitSocket) WSACleanup();
        CleanupSocket();
    }

    const std::string& CCJGW_SyncSocket::GetErrorMsg()
    {
        return mstrErrorMsg;
    }

    void CCJGW_SyncSocket::CleanupSocket()
    {
        if (mAddrptr)
        {
            freeaddrinfo(mAddrptr);
            mAddrptr = NULL;
        }
        //
        // clean up the client connection
        //
        int retval = 0;
        if (mConnSocket != INVALID_SOCKET)
        {
            // Indicate no more data to send
            retval = shutdown(mConnSocket, SD_SEND);
            if (retval == SOCKET_ERROR)
            {
                JGW_FormatString(mstrErrorMsg, "shutdown failed: %d", WSAGetLastError());
            }

            // Close the socket
            retval = closesocket(mConnSocket);
            if (retval == SOCKET_ERROR)
            {
                JGW_FormatString(mstrErrorMsg, "closesocket failed: %d", WSAGetLastError());
            }
            mConnSocket = INVALID_SOCKET;
            mbConnected = false;
        }
    }

    bool CCJGW_SyncSocket::InitSocket()
    {
        WSADATA wsadata ={0};
        int retval = 0;
        if ((retval = WSAStartup(MAKEWORD(2,2), &wsadata)) != 0)
        {
            JGW_FormatString(mstrErrorMsg,"WSAStartup failed with error %d",retval);
            WSACleanup();
            return false;
        }
        return true;
    }

    bool CCJGW_SyncSocket::ConnectSocket(const char* server_name,const char* port,int family /* = AF_INET */,int socktype /* = SOCK_STREAM */)
    {
        struct addrinfo *results = NULL,*addrptr = NULL, hints = {0};
        int retval = 0;
        char hoststr[NI_MAXHOST] = {0},servstr[NI_MAXSERV] = {0};
        hints.ai_family = family;
        hints.ai_socktype = socktype;
        hints.ai_protocol = ((socktype == SOCK_STREAM) ? IPPROTO_TCP : IPPROTO_UDP);

        CleanupSocket();

        retval = getaddrinfo(server_name,port,&hints,&results);
        if (retval != 0)
        {
            JGW_FormatString(mstrErrorMsg, "getaddrinfo failed: %d", retval);
            //! CleanupSocket();
            return false;
        }

        // Make sure we got at least one address
        if (results == NULL)
        {
            JGW_FormatString(mstrErrorMsg, "Server (%s) name could not be resolved!", server_name);
            //! CleanupSocket();
            return false;
        }
        //
        // Walk through the list of addresses returned and connect to each one.
        //    Take the first successful connection.
        //
        mAddrptr = results;
        while (mAddrptr)
        {
            mConnSocket = socket(mAddrptr->ai_family, mAddrptr->ai_socktype, mAddrptr->ai_protocol);
            if (mConnSocket == INVALID_SOCKET)
            {
                JGW_FormatString(mstrErrorMsg, "socket failed: %d\n", WSAGetLastError());
                CleanupSocket();
                return false;
            }

            //
            // Notice that nothing in this code is specific to whether we 
            // are using UDP or TCP.
            // We achieve this by using a simple trick.
            //    When connect() is called on a datagram socket, it does not 
            //    actually establish the connection as a stream (TCP) socket
            //    would. Instead, TCP/IP establishes the remote half of the
            //    ( LocalIPAddress, LocalPort, RemoteIP, RemotePort) mapping.
            //    This enables us to use send() and recv() on datagram sockets,
            //    instead of recvfrom() and sendto()

            retval = getnameinfo(
                mAddrptr->ai_addr,
                (socklen_t)mAddrptr->ai_addrlen,
                hoststr,
                NI_MAXHOST,
                servstr,
                NI_MAXSERV,
                NI_NUMERICHOST | NI_NUMERICSERV
                );
            if (retval != 0)
            {
                JGW_FormatString(mstrErrorMsg, "getnameinfo failed: %d", retval);
                CleanupSocket();
                return false;
            }

            //! printf("Client attempting connection to: %s port: %s\n", hoststr, servstr);

            retval = connect(mConnSocket, mAddrptr->ai_addr, (int)mAddrptr->ai_addrlen);
            if (retval == SOCKET_ERROR)
            {
                closesocket(mConnSocket);
                JGW_FormatString(mstrErrorMsg, "connect socket failed: %d", WSAGetLastError());
                mConnSocket = INVALID_SOCKET;
                mAddrptr = mAddrptr->ai_next;
            }
            else
            {
                break;
            }
        }
        mbConnected = (mConnSocket != INVALID_SOCKET);
        if (!mbConnected) CleanupSocket();
        return mbConnected;
    }

    int CCJGW_SyncSocket::RecvSocket(char* recv_buf,int recv_siz)
    {
        int retval = recv(mConnSocket, recv_buf, recv_siz, 0);
        if (retval == SOCKET_ERROR)
        {
            JGW_FormatString(mstrErrorMsg, "recv failed: error %d", WSAGetLastError());
        }
        return retval;
    }

    int CCJGW_SyncSocket::SendSocket(const char* buf,int buf_size)
    {
        int retval = send(mConnSocket, buf, buf_size, 0);
        if (retval == SOCKET_ERROR)
        {
            JGW_FormatString(mstrErrorMsg, "send failed: error %d", WSAGetLastError());
        }
        return retval;
    }

    bool CCJGW_SyncSocket::CheckSocketIsConnected()
    {
        return mbConnected;
    }
}