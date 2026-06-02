#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_SyncSocket.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_SyncSocket::CCJGW_SyncSocket(void) : mAddrptr(NULL),mConnSocket(INVALID_SOCKET),mbConnected(false)
    {
        mbInitSocket = InitSocket();
    }

    CCJGW_SyncSocket::~CCJGW_SyncSocket(void)
    {
        CleanupSocket();
        if (mbInitSocket) WSACleanup();
    }

    SOCKET CCJGW_SyncSocket::GetSocket()
    {
        return mConnSocket;
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
                PELOG4WW_F(L"shutdown failed: %d", WSAGetLastError());
            }

            // Close the socket
            retval = closesocket(mConnSocket);
            if (retval == SOCKET_ERROR)
            {
                PELOG4WW_F(L"closesocket failed: %d", WSAGetLastError());
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
            PELOG4WW_F(L"WSAStartup failed with error %d",retval);
            WSACleanup();
            return false;
        }
        return true;
    }

    std::string CCJGW_SyncSocket::GetHostIP(std::string& strHost)
    {
        char host_name[256] = {0};

        if (strHost.empty())
        {
            gethostname(host_name,sizeof(host_name));
            strHost = host_name;
        }

        hostent* hostinfo= gethostbyname(strHost.c_str());

        while(*(hostinfo->h_addr_list) != NULL) //输出ipv4地址
        {
            LPCSTR ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
            //             printf("ipv4 addr = %s\n\n", ip);
            return ip;
            hostinfo->h_addr_list++;
        }

        //         for(size_t i = 0;i < (strlen((char*)*host->h_addr_list) - strlen(host->h_name)) / 4 && pAddr;i ++)
        //         {
        //             return inet_ntoa(pAddr[i]);
        //         }
        return "127.0.0.1";
    }

    void CCJGW_SyncSocket::GetHostIP(std::string& strHost,std::vector<std::string>& vListIP)
    {
        char host_name[256] = {0};
        if (strHost.empty())
        {
            gethostname(host_name,sizeof(host_name));
            strHost = host_name;
        }
        hostent* hostinfo=gethostbyname(strHost.c_str());
        while(*(hostinfo->h_addr_list) != NULL) //输出ipv4地址
        {
            LPCSTR ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
            hostinfo->h_addr_list++;
            vListIP.push_back(ip);
        }
//         in_addr* pAddr=(in_addr*)*host->h_addr_list;
//         for(size_t i = 0;i < (strlen((char*)*host->h_addr_list) - strlen(host->h_name)) / 4 && pAddr;i ++)
//         {
//             vListIP.push_back(inet_ntoa(pAddr[i]));
//         }
    }

    bool CCJGW_SyncSocket::ConnectSocket(const char* server_name,const char* port,const char* source_addr /* = NULL */,int family /* = AF_INET */,int socktype /* = SOCK_STREAM */)
    {
        struct addrinfo *results = NULL,*addrptr = NULL, hints = {0};
        int retval = 0;
        char hoststr[NI_MAXHOST] = {0},servstr[NI_MAXSERV] = {0};
        hints.ai_family = family;
        hints.ai_socktype = socktype;
        hints.ai_protocol = ((socktype == SOCK_STREAM) ? IPPROTO_TCP : IPPROTO_UDP);

        retval = getaddrinfo(server_name,port,&hints,&results);
        if (retval != 0)
        {
            PELOG4WW_F(L"getaddrinfo failed: %d", retval);
            return false;
        }

        // Make sure we got at least one address
        if (results == NULL)
        {
            PELOG4WW_F(L"Server (%s) name could not be resolved!", JGW_A2W_A(server_name).c_str());
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
                PELOG4WW_F(L"socket failed: %d", WSAGetLastError());
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
                PELOG4WW_F(L"getnameinfo failed: %d", retval);
                CleanupSocket();
                return false;
            }

            //绑定IP和端口
            if (source_addr)
            {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_port = 0;
                sin.sin_addr.S_un.S_addr = inet_addr(source_addr); 
                if (bind(mConnSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
                {
                    PELOG4WW(L"bind error !");
                    return false;
                }
            }

            retval = connect(mConnSocket, mAddrptr->ai_addr, (int)mAddrptr->ai_addrlen);
            if (retval == SOCKET_ERROR)
            {
                closesocket(mConnSocket);
                PELOG4WW_F(L"connect socket failed: %d", WSAGetLastError());
                mConnSocket = INVALID_SOCKET;
                mAddrptr = mAddrptr->ai_next;
            }
            else
            {
                break;
            }
        }
        mbConnected = (mConnSocket != INVALID_SOCKET);
        return mbConnected;
    }

    int CCJGW_SyncSocket::RecvSocket(char* recv_buf,int recv_siz)
    {
        int retval = recv(mConnSocket, recv_buf, recv_siz, 0);
        if (retval == SOCKET_ERROR)
        {
            PELOG4WW_F(L"recv failed: error %d", WSAGetLastError());
        }
        return retval;
    }

    int CCJGW_SyncSocket::SendSocket(const char* buf,int buf_size)
    {
        int retval = send(mConnSocket, buf, buf_size, 0);
        if (retval == SOCKET_ERROR)
        {
            PELOG4WW_F(L"send failed: error %d", WSAGetLastError());
        }
        return retval;
    }

    bool CCJGW_SyncSocket::CheckSocketIsConnected()
    {
        return mbConnected;
    }
}