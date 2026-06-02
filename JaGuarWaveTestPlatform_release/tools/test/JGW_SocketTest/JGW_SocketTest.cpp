// JGW_SocketTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

// #include <winsock2.h>
// 
// #include <ws2tcpip.h>
// #include <wspiapi.h>

/*#include "AdapterCommon.h"*/
/*#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>*/
#define DEFAULT_PORT            "9000"          // Default server port
#define DEFAULT_PROTO           SOCK_STREAM     // Default protocol (TCP)
#define DEFAULT_BUFFER_LEN      4096            // Default send/recv buffer length
#define  FLASH_PAGE_SIZE 256
#pragma comment(lib,"ws2_32.lib")


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Winsock2.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <string>
#include<ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib ")  //linking to the library
using namespace std;
int get_ip()
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    struct sockaddr_in6 *sinp6;
    PHOSTENT hostinfo;
    char hostname[255] = {0}; //主机名   
    char *port = "3294";      //端口号 
    const char *addr;
    int ilRc;
    gethostname(hostname, sizeof(hostname));
    if((hostinfo = gethostbyname(hostname)) == NULL) //获得本地ipv4地址
    {
        errno = GetLastError();
        fprintf(stderr,"gethostbyname Error:%d\n", errno);
        return 1;
    }
    LPCSTR ip;
    while(*(hostinfo->h_addr_list) != NULL) //输出ipv4地址
    {
        ip = inet_ntoa(*(struct in_addr *) *hostinfo->h_addr_list);
        printf("ipv4 addr = %s\n\n", ip);
        hostinfo->h_addr_list++;
    }
    hint.ai_family = AF_INET6;        //hint 的限定设置
    hint.ai_socktype = SOCK_STREAM;   //这里可是设置 socket type    比如  SOCK_DGRAM
    hint.ai_flags = AI_PASSIVE;       // flags 的标志很多。常用的有AI_CANONNAME;
    hint.ai_protocol = 0;             //设置协议  一般为0，默认
    hint.ai_addrlen = 0;              //下面不可以设置，为0，或者为NULL
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    ilRc = getaddrinfo(hostname, port, &hint, &ailist); //通过主机名获得地址信息
    if (ilRc < 0)
    {
        char str_error[100];
        strcpy(str_error, (char *)gai_strerror(errno));
        printf("str_error = %s", str_error);
        return 0;
    }
    if(ailist == NULL)
    {
        printf("sorry not find the IP address,please try again \n");
    }
    for (aip = ailist; aip != NULL; aip = aip->ai_next) //显示获取的信息
    {
        aip->ai_family == AF_INET6;
        sinp6 = (struct sockaddr_in6 *)aip->ai_addr;    //为什么是for 循环 ，先向下看
        int i;
        printf("ipv6 addr = ");
        for(i = 0; i < 16; i++)
        {
            if(((i-1)%2) && (i>0))
            {
                printf(":");
            }
            printf("%02x",sinp6->sin6_addr.u.Byte[i]);
        }
        printf(" \n");
        printf(" \n");
    }
    while(1);
}


int _tmain(int argc, _TCHAR* argv[])
{
    std::string strWlanGateWay("192.168.133.18");
    std::string strWlanGateWaySegment = strWlanGateWay.substr(0,strWlanGateWay.rfind('.') + 1);



    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD( 1, 1 );
    err = WSAStartup( wVersionRequested, &wsaData );//initiate the ws2_32.dll and match the version
    if ( err != 0 )
    {
        return 0;
    }
    if ( LOBYTE( wsaData.wVersion ) != 1 ||   //if the version is not matched ,then quit and terminate the ws3_32.dll 
        HIBYTE( wsaData.wVersion ) != 1 )
    {
        WSACleanup( );
        return 0;
    }
    get_ip();
    WSACleanup( );
    return 0;
//     std::vector<NetWorkConection> adapters;
//     CAdapterCommon::GetAdapterInfo(adapters);
//     return 0;
#if 0
    //! 1048576
    size_t mnCheckBlankLength = (665872+(FLASH_PAGE_SIZE-1)) & (~(FLASH_PAGE_SIZE-1)); //! 0xff
    //! mnCheckBlankLength = 666112

    JGW::CCJGW_AsyncSocket asyncSocket;
    char Buffer[DEFAULT_BUFFER_LEN] = {0};
    if (!asyncSocket.ConnectSocketTimeOut("127.0.0.1",DEFAULT_PORT))
    {
        fprintf(stderr,"WSAStartup failed with error \n");
        return 0;
    }

    if (-1 == asyncSocket.RecvSocket(Buffer,DEFAULT_BUFFER_LEN))
    {
        asyncSocket.CleanupSocket();
        return 0;
    }

#if 0
    WSADATA wsadata ={0};
    struct addrinfo *results = NULL,*addrptr = NULL, hints;
    char *server_name = "192.168.133.40",
        Buffer[DEFAULT_BUFFER_LEN] = {0},
    *port = DEFAULT_PORT;
    SOCKET conn_socket = INVALID_SOCKET;
    char hoststr[NI_MAXHOST] = {0},servstr[NI_MAXSERV] = {0};
    int retval = 0;

    if(WSAStartup (MAKEWORD(2,2),& wsadata))
    {
        fprintf(stderr,"WSAStartup failed with error %d\n",retval);
        WSACleanup();
        return -1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    retval = getaddrinfo(
        server_name,
        port,
        &hints,
        &results
        );
    if (retval != 0)
    {
        fprintf(stderr, "getaddrinfo failed: %d\n", retval);
        goto cleanup;
    }

    // Make sure we got at least one address
    if (results == NULL)
    {
        fprintf(stderr, "Server (%s) name could not be resolved!\n", server_name);
        goto cleanup;
    }

//     addrptr->ai_family = AF_INET;
//     addrptr->ai_socktype = SOCK_STREAM;
//     addrptr->ai_protocol = IPPROTO_TCP;
//     addrptr->ai_addr = &sock_addr;
//      sock_addr.sa_family = AF_INET;
//     strcpy_s(sock_addr.sa_data,14,"192.168.133.40");
    //!addrptr->ai_addr->sa_data = inet_addr("192.168.133.40");

    //
    // Walk through the list of addresses returned and connect to each one.
    //    Take the first successful connection.
    //
    addrptr = results;
    while (addrptr)
    {
        conn_socket = socket(addrptr->ai_family, addrptr->ai_socktype, addrptr->ai_protocol);
        if (conn_socket == INVALID_SOCKET)
        {
            fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
            goto cleanup;
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
            addrptr->ai_addr,
            (socklen_t)addrptr->ai_addrlen,
            hoststr,
            NI_MAXHOST,
            servstr,
            NI_MAXSERV,
            NI_NUMERICHOST | NI_NUMERICSERV
            );
        if (retval != 0)
        {
            fprintf(stderr, "getnameinfo failed: %d\n", retval);
            goto cleanup;
        }

        printf("Client attempting connection to: %s port: %s\n", hoststr, servstr);

        retval = connect(conn_socket, addrptr->ai_addr, (int)addrptr->ai_addrlen);
        if (retval == SOCKET_ERROR)
        {
            closesocket(conn_socket);
            conn_socket = INVALID_SOCKET;

            addrptr = addrptr->ai_next;
        }
        else
        {
            break;
        }
    }

//     int nTimeout = 3000;
//     setsockopt(conn_socket ,SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout,sizeof (nTimeout));
//     nTimeout = 3000;
//     setsockopt(conn_socket ,SOL_SOCKET, SO_SNDTIMEO,(char *)&nTimeout,sizeof (nTimeout));

    retval = recv(conn_socket, Buffer, DEFAULT_BUFFER_LEN, 0);
    if (retval == SOCKET_ERROR)
    {
        fprintf(stderr, "recv failed: error %d\n", WSAGetLastError());
        goto cleanup;
    }
cleanup:
    //
    // clean up the client connection
    //
    if (conn_socket != INVALID_SOCKET)
    {
        // Indicate no more data to send
        retval = shutdown(conn_socket, SD_SEND);
        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr, "shutdown failed: %d\n", WSAGetLastError());
        }

        // Close the socket
        retval = closesocket(conn_socket);
        if (retval == SOCKET_ERROR)
        {
            fprintf(stderr, "closesocket failed: %d\n", WSAGetLastError());
        }

        conn_socket = INVALID_SOCKET;
    }
    if (results != NULL)
    {
        freeaddrinfo(results);
        results = NULL;
    }
    WSACleanup();
#endif
#endif
    return 0;
}

