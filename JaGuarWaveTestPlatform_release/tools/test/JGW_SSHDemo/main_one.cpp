#include "stdafx.h"
#include "ICMPDefine.h"
#define WLOGA //! 
/*#include "PublicDefine.h"*/
#pragma comment(lib,"ws2_32.lib")
#define LoadingSocketLib( wVersionRequired, lpWSAData )  WSAStartup (wVersionRequired , lpWSAData)

IP_HANDER *m_pRec = (IP_HANDER *)new BYTE[MAX_PACKET ];
ICMP_HANDER *m_pIcmp= (ICMP_HANDER *)new char[DEF_PACKET];
SOCKET m_socket = INVALID_SOCKET;
sockaddr_in m_sockAddr;
int m_nMsg = ICMP_ECHO;

//初始化socket
bool  Initialize()
{
    WSADATA wsadata ={0};
    int nResult =0;

    memset(m_pRec,0x00,MAX_PACKET);
    memset(m_pIcmp,0x00,DEF_PACKET);

    if(LoadingSocketLib (MAKEWORD(2,2),& wsadata))
    {
        WLOGA("Init LoadingSocketLib erro");
        return false ;
    }

    m_socket = WSASocket(AF_INET ,SOCK_RAW, IPPROTO_ICMP,NULL ,0,0);
    if(INVALID_SOCKET == m_socket)
    {
        WLOGA( "The network problems lead to SOCKET initialization error" );
        return FALSE ;
    }

    int nTimeout = 3000;
    setsockopt(m_socket ,SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout,sizeof (nTimeout));
    nTimeout = 3000;
    setsockopt(m_socket ,SOL_SOCKET, SO_SNDTIMEO,(char *)&nTimeout,sizeof (nTimeout));

    return TRUE ;

}

//卸载socket
void  Uninitialize()
{
    if(INVALID_SOCKET != m_socket )
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }

    :: WSACleanup();
}
//校验和
USHORT  CheckSum( USHORT *addr ,int size)
{
    unsigned long cksum = 0;
    while(size >1)
    {
        cksum += *addr ++;
        size -= sizeof (USHORT);
    }
    if(size )
    {
        cksum += *(UCHAR*)addr;
    }
    cksum = (cksum >>16) + (cksum&0xffff);
    cksum += (cksum >>16);

    return (USHORT )(~cksum);
}

extern int  SendICMPAddr( int nMsg ,sockaddr_in * pAddr,int nTimeOut = 1000);
//发送ICMP
bool  SendICMP( int nMsg ,char * pAddr,int nTimeOut/* = 1000*/)
{
    sockaddr_in addr ;
    hostent *pHost = NULL;

    if((addr .sin_addr. S_un.S_addr = inet_addr( pAddr)) == INADDR_NONE)
    {
        //域名转换IP地址
        pHost = gethostbyname( pAddr);

        if (!pHost )
        {
            WLOGA("ping The request could not find host!");
            return 0;
        }

        addr.sin_addr .S_un. S_addr = ((int**)pHost ->h_addr_list)[0][0];
    }

    addr.sin_family = AF_INET;
    addr.sin_port = 0;

    return SendICMPAddr (nMsg,& addr,nTimeOut ) == 1?true:false;
}

int  SendICMPAddr( int nMsg ,sockaddr_in * pAddr,int nTimeOut /*= 1000*/)
{
    char * p_data = NULL;
    static int i = 0;

    memset(m_pIcmp ,0x00,sizeof( ICMP_HANDER));
    m_pIcmp->i_type = nMsg;
    m_pIcmp->i_code = 0;
    m_pIcmp->i_id = i;
    m_pIcmp->i_seq = i;
    m_pIcmp->timestamp = ::GetTickCount();
    m_pIcmp->i_cksum = CheckSum((USHORT *)m_pIcmp, /*DEF_PACKET +*/ sizeof(ICMP_HANDER ));
    i ++;
    //QueryPerformanceCounter(&m_nBegainTime );

    int state = 0;
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500;
    DWORD s_time = GetTickCount();
    fd_set writefds ;

    while (1)
    {
        MSG msg ;
        PeekMessage(&msg ,NULL,0,0, PM_REMOVE);
        if (WM_QUIT == msg. message)
        {
            return 0;
        }

        FD_ZERO( &writefds );
        FD_SET(m_socket ,&writefds);
        if (SOCKET_ERROR == select( m_socket + 1/*0*/ ,NULL, & writefds, NULL , &timeout ))
        {
            return SOCKET_ERROR ;
        }

        DWORD e_time = GetTickCount();
        if (!FD_ISSET(m_socket,& writefds))
        {
            if (e_time - s_time > nTimeOut)
            {
                return -1 ;
            }
            else
                continue;
        }
        break;
    }
    state = sendto(m_socket, ( char *)m_pIcmp , DEF_PACKET, 0,
        ( sockaddr*)pAddr , sizeof( sockaddr));

    if (SOCKET_ERROR == state)
    {
        if(WSAETIMEDOUT == GetLastError())
        {
            WLOGA("连接因超时而失败(发送)" );
        }
        else
        {
            WLOGA("出现未知错误");
        }

        return -1 ;
    }

    if (state < DEF_PACKET)
    {
        WLOGA("send data error");
        return -1 ;
    }

    fd_set fd ;
    timeval tv = {2, 0};
    FD_ZERO(&fd );

    FD_SET(m_socket , &fd);
    INT nRet = select(0, & fd, NULL , NULL, & tv);

    if (nRet == 0)
    {
        WLOGA("PING: timeout。General failure.");
        return -1 ;
    }
    else if (nRet == SOCKET_ERROR)
    {
        WLOGA("PING: tran fail。General failure.");
        return -1 ;
    }

    m_nMsg = nMsg ;
    memcpy((void *)&m_sockAddr,( void*)pAddr ,sizeof( sockaddr_in));

    return 1 ;
}

bool  RecvICMP()
{
    int nState = 0;
    int nLen = sizeof( sockaddr_in);
    nState = recvfrom(m_socket,( char *)m_pRec,MAX_PACKET ,0,(struct sockaddr*)&m_sockAddr ,&nLen);
    // nLen = sizeof(*m_pRec);
    //IP_HANDER_ICMP *p_recip = (IP_HANDER_ICMP *)((char*)m_pRec );
    //IP_HANDER* p_iphandle = (IP_HANDER *)((char*)m_pRec );

    if (SOCKET_ERROR == nState)
    {
        if (WSAETIMEDOUT == WSAGetLastError())
        {
            WLOGA("connect timeout fail(recv)" );
        }
        else
            WLOGA("unknow recv fail！");

        return false ;
    }
    int iphdlen = sizeof(unsigned long) * (m_pRec->h_lenver & 0xf);  

    if (nState < (iphdlen + ICMP_MIN))
    {
        WLOGA("The destination address of the response data is not correct" );
        return false ;
    }

    ICMP_HANDER *p_recicmp = (ICMP_HANDER *)((char*)m_pRec + iphdlen);
    if (p_recicmp ->i_type == ICMP_CANTTO)
    {
        WLOGA("The router can't forward！");
        return false ;
    }

    if (p_recicmp ->i_type == ICMP_DELAY)
    {
        WLOGA("The source station inhibition！");
        return false ;
    }

    if(m_nMsg == ICMP_ECHO)
    {
        if (p_recicmp ->i_type != ICMP_ECHOREPLY)//回显应答报文(Echo Reply)
        {
            WLOGA("Unknown message information！");
            return false ;
        }
    }

    // 	char *addr = inet_ntoa( m_sockAddr.sin_addr );
    // 
    // 	// 所有工作结束,打印信息
    // 	in_addr sourceIP ;
    // 	sourceIP.S_un .S_addr = m_pRec->sourceIP ;
    WLOGA(("ping %s\tbyte = %d\tttl = %d\ttime = %d msok"),inet_ntoa( m_sockAddr.sin_addr ),nState,m_pRec->ttl,GetTickCount() - m_pIcmp->timestamp);

    return true ;
}


unsigned long ResolveAddress(const char * szHost)
{ 
    unsigned long lAddr = inet_addr(szHost ); 

    if (lAddr == INADDR_NONE) { 
        hostent *pHE = gethostbyname( szHost); 
        if (!pHE ) 
            return INADDR_NONE ; 

        //                 for (int iHost=0;pHE->h_addr_list[iHost];iHost++) { 
        //                          //   CString ip; 
        //                          for (int i=0;i<4;i++) { 
        //                                    pHE->h_addr_list[iHost][i] & 0x00ff; 
        //                                    if (i > 0) { 
        //                                              printf("."); 
        //                                              } 
        //                                    printf("%d", pHE->h_addr_list[iHost][i] & 0x00ff); 
        //                                    if (3 == i) { 
        //                                              printf(""); 
        //                                              } 
        //                                    } 
        //                          } 
        lAddr = *((unsigned long*) pHE->h_addr_list[0]); 
    } 
    return lAddr ; 
} 


int _tmain(int argc, _TCHAR* argv[])
{
    if ( !Initialize() )
    {
        return false;
    }

    for (int i = 0;i < 40;i ++)
    {
        if ( SendICMP(ICMP_ECHO,"192.168.213.252",1000))
        {
            if ( RecvICMP() )
            {
                Uninitialize();
                return true;
            }
        }

        Sleep(1000);
    }

    Uninitialize();
}