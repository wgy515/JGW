#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_ICMPSocket.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_CHiPerfTimer mHiPerfTimer;

    CCJGW_ICMPSocket::CCJGW_ICMPSocket(void) : mbInitSocket(false),mConnSocket(INVALID_SOCKET),_seq(0),m_pRec((IP_HANDER*)mszRecBuf),m_pIcmp((ICMP_HANDER*)mszICMPBuf)
    {
        mbInitSocket = InitSocket();
        memset(mszPingResult,0x00,256);
        memset(mszRecBuf,0x00,MAX_PACKET);
        memset(mszICMPBuf,0x00,DEF_PACKET);
    }


    CCJGW_ICMPSocket::~CCJGW_ICMPSocket(void)
    {
        CloseSocket();
        if (mbInitSocket) WSACleanup();
    }

    void CCJGW_ICMPSocket::ResetICMPSeq()
    {
        _seq = 0;
    }

    void CCJGW_ICMPSocket::CloseSocket()
    {
        if (INVALID_SOCKET != mConnSocket)
        {
            closesocket(mConnSocket);
            mConnSocket = INVALID_SOCKET;
        }
    }

    bool CCJGW_ICMPSocket::InitSocket()
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

    bool CCJGW_ICMPSocket::CreateSocket(const char* source_addr /* = NULL */)
    {
        mConnSocket = WSASocket(AF_INET ,SOCK_RAW, IPPROTO_ICMP,NULL ,0,0);
        if(INVALID_SOCKET == mConnSocket)
        {
            PELOG4WW(L"The network problems lead to SOCKET initialization error" );
            return false ;
        }

        int nTimeout = 3000;
        setsockopt(mConnSocket ,SOL_SOCKET, SO_RCVTIMEO,(char *)&nTimeout,sizeof (nTimeout));
        nTimeout = 3000;
        setsockopt(mConnSocket ,SOL_SOCKET, SO_SNDTIMEO,(char *)&nTimeout,sizeof (nTimeout));

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

        return true ;
    }

    bool CCJGW_ICMPSocket::Ping(const std::string& address,const char* source_addr /* = NULL */,int time_out /* = 1000 */)
    {
        mszPingResult[0] = '\0';
        memset(m_pRec,0x00,MAX_PACKET);
        memset(m_pIcmp,0x00,DEF_PACKET);

        if (!CreateSocket(source_addr) || !SendICMP(address,time_out) || !ReceiveICMP(time_out))
        {
            CloseSocket();
            return false;
        }
        CloseSocket();
        return true;
    }

    std::string CCJGW_ICMPSocket::GetPingResult()
    {
        return mszPingResult;
    }

    double CCJGW_ICMPSocket::GetPingTimeMs()
    {
        return mHiPerfTimer.GetTimeMs();
    }

    unsigned char CCJGW_ICMPSocket::GetPingTTL()
    {
        return m_pRec->ttl;
    }

    unsigned short CCJGW_ICMPSocket::GetPingSeq()
    {
        return _recv_seq;
    }

    void CCJGW_ICMPSocket::GetUrlToHost(sockaddr_in& addr,const std::string& address)
    {
        hostent* pHost = NULL;
        memset(&addr,0x00,sizeof(sockaddr_in));
        addr.sin_family = AF_INET;
        addr.sin_port = 0;

        if(INADDR_NONE == (addr.sin_addr.S_un.S_addr = inet_addr(address.c_str())))
        {
            //域名转换IP地址
            pHost = gethostbyname(address.c_str());
            if (!pHost) { PELOG4WW(L"ping The request could not find host!"); return ;}
            addr.sin_addr.S_un.S_addr = ((int**)pHost ->h_addr_list)[0][0];
        } 
    }

    USHORT CCJGW_ICMPSocket::CheckSum(USHORT *addr,int size)
    {
        int nleft = size;
        u_short* w   = addr;
        u_short answer;
        int sum = 0;

        while (nleft > 1)  
        {
            sum   += *w++;
            nleft -= sizeof(u_short);
        }

        if (nleft == 1) 
        {
            u_short u = 0;
            *(u_char*) (&u) = *(u_char*) w;
            sum += u;
        }

        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = ~sum;
        return answer;
    }

    bool CCJGW_ICMPSocket::SendICMP(const std::string& address,int time_out)
    {
        GetUrlToHost(msocketaddr,address);
        if (65535 == _seq) _seq = 0;
        memset(m_pIcmp ,0x00,sizeof( ICMP_HANDER));
        m_pIcmp->i_type = ICMP_ECHO;
        m_pIcmp->i_code = 0;
        m_pIcmp->i_id = (u_short)GetCurrentProcessId();
        m_pIcmp->i_seq = ++_seq;
        m_pIcmp->timestamp = ::GetTickCount();
        m_pIcmp->i_cksum = CheckSum((USHORT *)m_pIcmp, /*DEF_PACKET +*/ sizeof(ICMP_HANDER ));
        mHiPerfTimer.Start();
        //! time_out

        struct timeval timeout;
        timeout.tv_sec = time_out / 1000;
        timeout.tv_usec = time_out % 1000;
        fd_set writefds ;

        FD_ZERO(&writefds);
        FD_SET(mConnSocket,&writefds);

        if (SOCKET_ERROR == select(mConnSocket + 1/*0*/ ,NULL, & writefds, NULL , &timeout))  return false ;
        if (!FD_ISSET(mConnSocket,& writefds)) return false;

        int state = sendto(mConnSocket, ( char *)m_pIcmp , DEF_PACKET, 0,(sockaddr*)&msocketaddr , sizeof( sockaddr));
        /* if ((SOCKET_ERROR == state) || state < DEF_PACKET) return false;*/
        if (SOCKET_ERROR == state)
        {
            if(WSAETIMEDOUT == GetLastError())
            {
                PELOG4WW(L"Connection fails due to timeout (send)" );
                strcpy_s(mszPingResult,260,"connect time out");
            }
            else
            {
                PELOG4WW(L"unknow error");
                strcpy_s(mszPingResult,260,"unknow error");
            }

            return false;
        }

        if (state < DEF_PACKET)
        {
            PELOG4WW(L"send data error");
            strcpy_s(mszPingResult,260,"send data error");
            return false;
        }

        return true;
    }

    bool CCJGW_ICMPSocket::ReceiveICMP(int time_out)
    {
        fd_set fd ;
        timeval tv;

        tv.tv_sec = time_out / 1000;
        tv.tv_usec = time_out % 1000;
        FD_ZERO(&fd );
        FD_SET(mConnSocket , &fd);
        int nRet = select(0, & fd, NULL , NULL, & tv);
        if (nRet == 0)
        {
            PELOG4WW(L"PING: timeout. General failure.");
            strcpy_s(mszPingResult,260,"PING: timeout. General failure.");
            return false;
        }
        else if (nRet == SOCKET_ERROR)
        {
            PELOG4WW(L"PING: tran fail. General failure.");
            strcpy_s(mszPingResult,260,"PING: tran fail. General failure.");
            return false;
        }

        int nLen = sizeof(sockaddr_in);
        int nState = recvfrom(mConnSocket,( char *)m_pRec,MAX_PACKET ,0,(struct sockaddr*)&msocketaddr ,&nLen);

        if (SOCKET_ERROR == nState)
        {
            if (WSAETIMEDOUT == WSAGetLastError())
            {
                PELOG4WW(L"connect timeout fail(recv)");
                strcpy_s(mszPingResult,260,"connect timeout fail(recv)");
            }
            else
            {
                PELOG4WW(L"unknow recv fail!");
                strcpy_s(mszPingResult,260,"unknow recv fail!");
            }
            return false ;
        }

        int iphdlen = sizeof(unsigned long) * (m_pRec->h_lenver & 0xf);  
        if (nState < (iphdlen + ICMP_MIN))
        {
            PELOG4WW(L"The destination address of the response data is not correct");
            strcpy_s(mszPingResult,260,"The destination address of the response data is not correct");
            return false ;
        }

        ICMP_HANDER *p_recicmp = (ICMP_HANDER *)((char*)m_pRec + iphdlen);
        mHiPerfTimer.Stop();
        switch (p_recicmp->i_type)
        {
        case ICMP_CANTTO:
            PELOG4WW(L"The router can't forward!");
            strcpy_s(mszPingResult,260,"The router can't forward!");
            return false;
        case ICMP_DELAY:
            PELOG4WW(L"The source station inhibition!");
            strcpy_s(mszPingResult,260,"The source station inhibition!");
            return false;
        case ICMP_ECHOREPLY:
            _recv_seq = p_recicmp->i_seq;
            //! PDLOG4WA_F("ping %s\tbyte = %d\tttl = %d\ttime = %d msok time=%.4lfms",inet_ntoa(msocketaddr.sin_addr),nState,m_pRec->ttl,GetTickCount() - m_pIcmp->timestamp,mHiPerfTimer.GetTimeMs());
            sprintf_s(mszPingResult,"%d bytes from %s: icmp_seq=%u ttl=%u time=%.3lf ms",nState,inet_ntoa(msocketaddr.sin_addr),m_pIcmp->i_seq,m_pRec->ttl,mHiPerfTimer.GetTimeMs());
            PDLOG4WW_F(JGW_A2W_A(mszPingResult).c_str());
            return true;
        default:
            PELOG4WW(L"Unknown message information!");
            strcpy_s(mszPingResult,260,"Unknown message information!");
            return false;
        }
        return false;
    }
}