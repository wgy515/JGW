#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <wspiapi.h>
#include <string>
#include "JGW_WindowsFuncPlugin_Define.h"
#include "ICMPDefine.h"


namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_ICMPSocket
    {
    public:
        CCJGW_ICMPSocket(void);
        ~CCJGW_ICMPSocket(void);
    public:
        //! 
        void ResetICMPSeq();
        bool Ping(const std::string& address,const char* source_addr = NULL,int time_out = 1000);
        double GetPingTimeMs();
        std::string GetPingResult();
        unsigned char GetPingTTL();
        unsigned short GetPingSeq();
    private:
        bool CreateSocket(const char* source_addr = NULL);
        bool InitSocket();
        bool SendICMP(const std::string& address,int time_out);
        bool ReceiveICMP(int time_out);
        void GetUrlToHost(sockaddr_in& addr,const std::string& address);
        USHORT CheckSum(USHORT *addr,int size);
        void CloseSocket();
    protected:
        bool mbInitSocket;
        bool mbConnected;
        u_short _seq;
        u_short _recv_seq;
        struct addrinfo* mAddrptr;
        SOCKET mConnSocket;
        IP_HANDER* m_pRec;
        ICMP_HANDER* m_pIcmp;
        sockaddr_in msocketaddr;  
        char mszPingResult[256];
        char mszRecBuf[MAX_PACKET];
        char mszICMPBuf[DEF_PACKET];
    };
}