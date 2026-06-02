#pragma once
#include <winsock2.h>
#include <windows.h>

// namespace JGW
// {
#define DEF_PACKET  32
#define MAX_PACKET 1024
#define ICMP_MIN        8                  //ICMP包最小尺寸
#define ICMP_ECHO     8                  //回送请求
#define ICMP_ECHOREPLY    0         //回送应答
#define ICMP_NETUNREACHABLE 0 //网络不能到达
#define ICMP_PROTOCOLUNREACHABLE 1 //协议不可到达
#define ICMP_CANTTO         3                  //目的站不能到达
#define ICMP_DELAY  4                  //源站抑制
#define ICMP_SOURCEROUTEFAILED  5                            //源路由失败了
#define ICMP_TIME      13                //时间请求
#define ICMP_TIMEREPLY   14       //时间应答
#define ICMP_ADD       17                //地址掩码请求
#define ICMP_ADDREPLY    18       //地址掩码应答

#define MAX_PACK_LEN       65535 // The max IP packet to receive. 
#define MAX_ADDR_LEN       16    // The dotted addres's length. 
#define MAX_PROTO_TEXT_LEN 16    // The length of sub protocol name(like "TCP"). 
#define MAX_PROTO_NUM      12    // The count of sub protocols. 
#define MAX_HOSTNAME_LAN   256   // The max length of the host name. 


    typedef unsigned char u_char;
    typedef unsigned short u_short;
    // typedef struct ip_hander_icmp
    // {
    // 	unsigned int IHL:4; //用32位字表示报头长度
    // 	unsigned int ver:4; //协议版本
    // 	unsigned char level; //优先级
    // 	unsigned short len; //数据长度
    // 	unsigned short ID; //标识
    // 	unsigned short mflag; //其他标记
    // 	unsigned char tll; //生命期
    // 	unsigned char prot; //协议
    // 	unsigned short cksum; //检查和
    // 	unsigned int sourceIP; //源IP地址
    // 	unsigned int destIP; //目的IP地址
    // } IP_HANDER_ICMP;

    /* 
    // The IP packet is like this. Took from RFC791. 
    0                   1                   2                   3    
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1  
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |Version|  IHL  |Type of Service|          Total Length         | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |         Identification        |Flags|      Fragment Offset    | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |  Time to Live |    Protocol   |         Header Checksum       | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                       Source Address                          | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                    Destination Address                        | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                    Options                    |    Padding    | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    */ 
    typedef struct ip_hander//定义IP首部
    {
        unsigned char h_lenver; //4位首部长度+4位IP版本号
        unsigned char tos; //8位服务类型TOS 　
        unsigned short total_len; //16位总长度（字节）
        unsigned short ident; //16位标识
        unsigned short frag_and_flags; //3位标志位
        unsigned char ttl; //8位生存时间 TTL
        unsigned char proto; //8位协议 (TCP, UDP 或其他)
        unsigned short checksum; //16位IP首部校验和 　
        unsigned int sourceIP; //32位源IP地址 　
        unsigned int destIP; //32位目的IP地址 　
    }IP_HANDER;

    /* 
    // The TCP packet is like this. Took from RFC793. 
    0                   1                   2                   3    
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1  
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |          Source Port          |       Destination Port        | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                        Sequence Number                        | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                    Acknowledgment Number                      | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |  Data |           |U|A|P|R|S|F|                               | 
    | Offset| Reserved  |R|C|S|S|Y|I|            Window             | 
    |       |           |G|K|H|T|N|N|                               | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |           Checksum            |         Urgent Pointer        | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                    Options                    |    Padding    | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    |                             data                              | 
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
    */ 
    /*源端，目的端的端口号，用于寻找发端和收端应用进程。这两个值加上IP首部中的源端IP地址和目的端IP地址唯一确定一个TCP连接。一个IP地址和一个端口号有时也称为一个插口（Socket），插口对（Socket Pair）（包含客户IP地址、客户端口号、服务器 IP地址和服务器端口号的四元组）可唯一确定互联网络中每个TCP连接的双方。IP+TCP端口唯一确定一个TCP连接。
    TCP协议通过使用"端口"来标识源端和目标端的应用进程。端口号可以使用0到65535之间的任何数字。在收到服务请求时，操作系统动态地为客户端的应用程序分配端口号。在服务器端，每种服务在"众所周知的端口"（Well-Know Port）为用户提供服务。

    typedef struct _tcphdr //定义TCP首部 
    { 
    USHORT th_sport; //16位源端口 
    USHORT th_dport; //16位目的端口 
    unsigned int th_seq; //32位序列号 
    unsigned int th_ack; //32位确认号 
    unsigned char th_lenres;   //4位首部长度/6位保留字 
    unsigned char th_flag; //6位标志位 
    USHORT th_win; //16位窗口大小 
    USHORT th_sum; //16位校验和 
    USHORT th_urp; //16位紧急数据偏移量 
    }TCP_HEADER; */

    typedef struct _tagTCPHEADER   // 20 Bytes 
    { 
        unsigned short th_sport; //16位，标识主机上发起传送的应用程序
        unsigned short th_dport; //16位，标识主机上传送要到达的应用程序。
        unsigned int   th_seq; //32位，当SYN出现，序列码实际上是初始序列码（ISN），而第一个数据字节是ISN+1。用来标识从TCP源端向TCP目标端发送的数据字节流，它表示在这个报文段中的第一个数据字节。
        unsigned int   th_ack; //32位，如果设置了ACK控制位，这个值表示一个准备接收的包的序列码，只有ACK标志为1时，确认号字段才有效。它包含目标端所期望收到源端的下一个数据字节。
        unsigned char  th_lenres; //4位，也就是头部长度，指出TCP负载（数据）的开始位置。以4字节为单位，如"0101"表示20字节位置的数据为负载开始，也就是头部长度为20字节。
        unsigned char  th_flag; /*URG（Urgent data）：紧急指针（urgent pointer）有效。如果URG为1，表示这是一个携有紧急资料的封包。
                                ACK（Acknowledgment field significant）：确认序号有效。如果ACK为1，表示此封包属于一个要回应的封包。一般都会为1。
                                PSH（Push function）：接收方应该尽快将这个报文段交给应用层。如果PSH为1，此封包所携带的数据会直接上传给上层应用程序而无需经过TCP处理。
                                RST（Reset）：重建连接。如果RST为1，要求重传。表示要求重新设定封包再重新传递。
                                SYN（Synchronize sequence number）：发起一个连接。如果SYN为1，表示要求双方进行同步沟通。
                                FIN（Finish-No more data for sender）：释放一个连接。如果FIN为1，表示传送结束，然後双方发出结束回应进而正式终止一个TCP传送过程。*/
        unsigned short th_win; //16位，接收窗口大小。此字段用来进行流量控制，单位为字节数，这个值是本机期望一次接收的字节数。这里一般称为“滑动视窗(Sliding Window)”。
        unsigned short th_sum; //校验位
        unsigned short th_urp; //16位，它是一个偏移量。指向后面是优先数据的字节，紧急指针指出在本报文段中的紧急数据的最后一个字节的序号，和序号字段中的值相加表示紧急数据最后一个字节的序号。
    }TCP_HEADER; 

    /* 
    // The TCP's pseudo header is like this. Took from RFC793. 
    +--------+--------+--------+--------+ 
    |           Source Address          | 
    +--------+--------+--------+--------+ 
    |         Destination Address       | 
    +--------+--------+--------+--------+ 
    |  zero  |  PTCL  |    TCP Length   | 
    +--------+--------+--------+--------+ 
    */ 
    typedef struct _tagPSD_HEADER // 16 Bytes //定义TCP伪首部 
    { 
        unsigned long saddr; //源地址 
        unsigned long daddr; //目的地址 
        char mbz; 
        char ptcl; //协议类型 
        unsigned short tcpl; //TCP长度 
    }PSD_HEADER; 

    /* 
    // The UDP packet is lick this. Took from RFC768. 
    0      7 8     15 16    23 24    31   
    +--------+--------+--------+--------+  
    |     Source      |   Destination   |  
    |      Port       |      Port       |  
    +--------+--------+--------+--------+  
    |                 |                 |  
    |     Length      |    Checksum     |  
    +--------+--------+--------+--------+  
    |                                      
    |          data octets ...             
    +---------------- ...                  
    */ 
    typedef struct _tagUDPHEADER  // 8 Bytes 
    { 
        unsigned short uh_sport; 
        unsigned short uh_dport; 
        unsigned short uh_len; 
        unsigned short uh_sum; 
    } UDP_HEADER; 

    typedef struct icmp_hander
    {
        unsigned char i_type; //8位类型//类型
        unsigned char i_code; //8位代码//编码
        unsigned short i_cksum; //16位校验和, 从TYPE开始,直到最后一位用户数据,如果为字节数为奇数则补充一位 检查和
        unsigned short i_id ; //识别号（一般用进程号作为识别号）, 用于匹配ECHO和ECHO REPLY包 标识
        unsigned short i_seq ; //报文序列号, 用于标记ECHO报文顺序 计数
        unsigned int timestamp; //时间戳 时间
    }ICMP_HANDER;
    // The protocol's map. 
    typedef struct _tagPROTOMAP 
    { 
        int  ProtoNum; 
        char ProtoText[MAX_PROTO_TEXT_LEN]; 
    }PROTOMAP;
    // static PROTOMAP ProtoMap[MAX_PROTO_NUM]= 
    // { 
    // 	{ IPPROTO_IP   , "IP "  }, 
    // 	{ IPPROTO_ICMP , "ICMP" },  
    // 	{ IPPROTO_IGMP , "IGMP" }, 
    // 	{ IPPROTO_GGP  , "GGP " },  
    // 	{ IPPROTO_TCP  , "TCP " },  
    // 	{ IPPROTO_PUP  , "PUP " },  
    // 	{ IPPROTO_UDP  , "UDP " },  
    // 	{ IPPROTO_IDP  , "IDP " },  
    // 	{ IPPROTO_ND   , "NP "  },  
    // 	{ IPPROTO_RAW  , "RAW " },  
    // 	{ IPPROTO_MAX  , "MAX " }, 
    // 	{ NULL         , ""     } 
    // }; 

    /******************ARP HEAD*************************/
#define SIOCGARP 0x00008951
#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN 6
#endif

#ifndef PCAP_OPENFLAG_PROMISCUOUS
#define PCAP_OPENFLAG_PROMISCUOUS 1
#endif

    struct ether_header {
        u_char ether_dhost[ETHER_ADDR_LEN];
        u_char ether_shost[ETHER_ADDR_LEN];
        u_short ether_type;
    };
    //ARP头部 
    struct ARP_HEADER{
        unsigned short arp_hdr;   /*ARP分组中的硬件类型，2字节，定义运行ARP的网络的类型，以太网是类型1*/
        unsigned short arp_pro;  /*协议类型，2字节，定义上层协议类型，对于IPV4协议，该字段值为0800*/
        unsigned char arp_hln;  /*硬件长度，8位字段，定义对应物理地址长度，以太网中这个值为6*/
        unsigned char apr_pln; /*协议长度，8位字段，定义以字节为单位的逻辑地址长度，对IPV4协议这个值为4*/
        unsigned  short arp_opt;  /*16位字段，定义分组类型，是ARP请求（值为1），或者ARP应答（值为2）*/
        unsigned char arp_sha[6]; /*发送端硬件地址，可变长度字段，对以太网这个字段是6字节长*/
        unsigned char arp_spa[4]; /*发送端协议地址，可变长度字段，对IP协议，这个字段是4字节长*/
        unsigned char arp_tha[6]; /*接受端硬件地址*/
        unsigned long arp_tpa[4];/*接收端协议地址*/
    };
/*}*/