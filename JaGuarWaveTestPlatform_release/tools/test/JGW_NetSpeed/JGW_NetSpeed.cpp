// JGW_NetSpeed.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#if 0
#include "include/pcap.h"

#pragma comment(lib,"lib/Packet.lib")
#pragma comment(lib,"lib/wpcap.lib")

void usage();

void dispatcher_handler(u_char *, const struct pcap_pkthdr *, const u_char *);

void _tmain(int argc, _TCHAR* argv[])
{
pcap_t *fp;
char errbuf[PCAP_ERRBUF_SIZE];
struct timeval st_ts;
u_int netmask;
struct bpf_program fcode;
  
    /* 检查命令行参数的合法性 */


    /* 打开输出适配器 */
    if((fp= pcap_open_live("\\Device\\NPF_{DD6AF4EA-2A58-490B-A40A-8A135EF4C590}"/*我的网卡名*/,100,1,1000,errbuf))==NULL)
    {
        fprintf(stderr,"\nUnable to open adapter %s.\n", errbuf);
        return;
    }

    /* 不用关心掩码，在这个过滤器中，它不会被使用 */
    netmask=0xffffff; 

//     // 编译过滤器
//     if (pcap_compile(fp, &fcode, "ether proto 0x8864"/*我用的是ADSL这里是设置只接收PPPOE的包*/, 1, netmask) <0 )
//     {
//         fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
//         /* 释放设备列表 */
//         return;
//     }
    
    //设置过滤器
//     if (pcap_setfilter(fp, &fcode)<0)
//     {
//         fprintf(stderr,"\nError setting the filter.\n");
//         pcap_close(fp);
//         /* 释放设备列表 */
//         return;
//     }

    /* 将接口设置为统计模式 */
    if (pcap_setmode(fp, MODE_STAT)<0)
    {
        fprintf(stderr,"\nError setting the mode.\n");
        pcap_close(fp);
        /* 释放设备列表 */
        return;
    }


    printf("NetWork traffic summary:\n");

    /* 开始主循环 */
    pcap_loop(fp, 0, dispatcher_handler, (PUCHAR)&st_ts);

    pcap_close(fp);
    return;
}

void dispatcher_handler(u_char *state, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
    struct timeval *old_ts = (struct timeval *)state;
    u_int delay;
    LARGE_INTEGER Bps,Pps;
    struct tm *ltime;
    char timestr[16];
    time_t local_tv_sec;

    /* 以毫秒计算上一次采样的延迟时间 */
    /* 这个值通过采样到的时间戳获得 */
    delay=(header->ts.tv_sec - old_ts->tv_sec) * 1000000 - old_ts->tv_usec + header->ts.tv_usec;
    /* 获取每秒的比特数b/s */
    Bps.QuadPart=(((*(LONGLONG*)(pkt_data + 8))/* * 8 */* 1000000) / (delay));
    /*                                            ^      ^
                                                  |      |
                                                  |      | 
                                                  |      |
                              将字节转换成比特 --   |
                                                         |
                                       延时是以毫秒表示的 --
    */

    /* 得到每秒的数据包数量 */
    Pps.QuadPart=(((*(LONGLONG*)(pkt_data)) * 1000000) / (delay));

    /* 将时间戳转化为可识别的格式 */
    local_tv_sec = header->ts.tv_sec;
    ltime=localtime(&local_tv_sec);
    strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

    /* 打印时间戳*/
    printf("%s ", timestr);

    /* 打印采样结果 */
    printf("BPS=%I64u ", Bps.QuadPart / (1024));
    printf("PPS=%I64u\n", Pps.QuadPart / (1024));

    //存储当前的时间戳
    old_ts->tv_sec=header->ts.tv_sec;
    old_ts->tv_usec=header->ts.tv_usec;
}


void usage()
{
    
    printf("\nShows the TCP traffic load, in bits per second and packets per second.\nCopyright (C) 2002 Loris Degioanni.\n");
    printf("\nUsage:\n");
    printf("\t tcptop adapter\n");
    printf("\t You can use \"WinDump -D\" if you don't know the name of your adapters.\n");

    exit(0);
}
#endif

#if 1
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <iphlpapi.h>

#include <stdio.h>
#include <stdlib.h>

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

/* Note: could also use malloc() and free() */
/*
判断网线是否插入
可利用IPHELPAPI进行获取


MIB_IFROW Info ;    // 存放获取到的Adapter参数
memset(&Info ,0 ,sizeof(MIB_IFROW)) ; 
Info.dwIndex = dwIndex ; // dwIndex是需要获取的Adapter的索引，可以通过GetAdaptersInfo和其他相关函数获取

if(GetIfEntry(&Info) != NOERROR){
printf("ErrorCode = %d\n" ,GetLastError()) ;
return ;
}
MIB_IFROW  中有一项 dwOperStatus的参数，它表明当前接口的操作状态
其值如下：
Value	Meaning
IF_OPER_STATUS_NON_OPERATIONAL
LAN adapter has been disabled, for example because of an address conflict. 
局域网适配器禁用，例如地址冲突
IF_OPER_STATUS_UNREACHABLE
WAN adapter that is not connected.
WAN适配器未连接
IF_OPER_STATUS_DISCONNECTED
For LAN adapters: network cable disconnected. For WAN adapters: no carrier
局域网适配器：网线未插入。WAN适配器：无信号
IF_OPER_STATUS_CONNECTING
WAN adapter that is in the process of connecting. 
正在处理连接
IF_OPER_STATUS_CONNECTED 
WAN adapter that is connected to a remote peer.
已连接远端
IF_OPER_STATUS_OPERATIONAL
Default status for LAN adapters 默认状态
参考：ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.chs/iphlp/iphlp/getifentry.htm

经测试
拔下网线时，dwOperStatus 值为IF_OPER_STATUS_NON_OPERATIONAL 
连接网线时，dwOperStatus 值为IF_OPER_STATUS_OPERATIONAL
*/
int main()
{

    // Declare and initialize variables.

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i, j;

    /* variables used for GetIfTable and GetIfEntry */
    MIB_IFTABLE *pIfTable;
    MIB_IFROW *pIfRow;

    // Allocate memory for our pointers.
    pIfTable = (MIB_IFTABLE *) MALLOC(sizeof (MIB_IFTABLE));
    if (pIfTable == NULL) {
        printf("Error allocating memory needed to call GetIfTable\n");
        return 1;
    }
    // Make an initial call to GetIfTable to get the
    // necessary size into dwSize
    dwSize = sizeof (MIB_IFTABLE);
    if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        FREE(pIfTable);
        pIfTable = (MIB_IFTABLE *) MALLOC(dwSize);
        if (pIfTable == NULL) {
            printf("Error allocating memory needed to call GetIfTable\n");
            return 1;
        }
    }
    // Make a second call to GetIfTable to get the actual
    // data we want.
    if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
        printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
        for (i = 0; i < pIfTable->dwNumEntries; i++) {
            pIfRow = (MIB_IFROW *) & pIfTable->table[i];
            //! if (MIB_IF_OPER_STATUS_CONNECTING != pIfRow->dwOperStatus) continue;
            printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
            printf("\tInterfaceName[%d]:\t %ws", i, pIfRow->wszName);
            printf("\n");
            printf("\tDescription[%d]:\t ", i);
            //!dwInOctets
            printf("Recv[%d] :%ld\n",i,pIfRow->dwInOctets);
            for (j = 0; j < pIfRow->dwDescrLen; j++)
                printf("%c", pIfRow->bDescr[j]);
            printf("\n");
            printf("\tType[%d]:\t ", i);
            switch (pIfRow->dwType) {
            case IF_TYPE_OTHER:
                printf("Other\n");
                break;
            case IF_TYPE_ETHERNET_CSMACD:
                printf("Ethernet\n");
                break;
            case IF_TYPE_ISO88025_TOKENRING:
                printf("Token Ring\n");
                break;
            case IF_TYPE_PPP:
                printf("PPP\n");
                break;
            case IF_TYPE_SOFTWARE_LOOPBACK:
                printf("Software Lookback\n");
                break;
            case IF_TYPE_ATM:
                printf("ATM\n");
                break;
            case IF_TYPE_IEEE80211:
                printf("IEEE 802.11 Wireless\n");
                break;
            case IF_TYPE_TUNNEL:
                printf("Tunnel type encapsulation\n");
                break;
            case IF_TYPE_IEEE1394:
                printf("IEEE 1394 Firewire\n");
                break;
            default:
                printf("Unknown type %ld\n", pIfRow->dwType);
                break;
            }
            printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
            printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);
            printf("\tPhysical Addr:\t ");
            if (pIfRow->dwPhysAddrLen == 0)
                printf("\n");
            for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
                if (j == (pIfRow->dwPhysAddrLen - 1))
                    printf("%.2X\n", (int) pIfRow->bPhysAddr[j]);
                else
                    printf("%.2X-", (int) pIfRow->bPhysAddr[j]);
            }
            printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
            printf("\tOper Status[%d]:\t ", i);
            switch (pIfRow->dwOperStatus) {
            case IF_OPER_STATUS_NON_OPERATIONAL:
                printf("Non Operational\n");
                break;
            case IF_OPER_STATUS_UNREACHABLE:
                printf("Unreachable\n");
                break;
            case IF_OPER_STATUS_DISCONNECTED:
                printf("Disconnected\n");
                break;
            case IF_OPER_STATUS_CONNECTING:
                printf("Connecting\n");
                break;
            case IF_OPER_STATUS_CONNECTED:
                printf("Connected\n");
                break;
            case IF_OPER_STATUS_OPERATIONAL:
                printf("Operational\n");
                break;
            default:
                printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
                break;
            }
            printf("\n");
        }
    } else {
        printf("GetIfTable failed with error: \n", dwRetVal);
        if (pIfTable != NULL) {
            FREE(pIfTable);
            pIfTable = NULL;
        }  
        return 1;
        // Here you can use FormatMessage to find out why 
        // it failed.
    }
    if (pIfTable != NULL) {
        FREE(pIfTable);
        pIfTable = NULL;
    }
    return 0;
}
#endif

#if 0
#include "stdafx.h"
#include <Windows.h>
#include <IPHlpApi.h>

#pragma comment(lib,"IPHlpApi.lib")

/*typedef std::vector<>*/

int WIN_GetIfTable(/*WIN_IfTable& lstIf*/)
{
    // GetAdaptersAddresses

    // 网口信息, 准备1个足够大的buf
    struct
    {
        DWORD dwNumEntries;
        MIB_IFROW table[48];
    }myIfTable;
    memset(&myIfTable, 0, sizeof(myIfTable));

      DWORD nNumOfIf = 0;
      DWORD ret = GetNumberOfInterfaces(&nNumOfIf);

    ULONG nBufSize = sizeof(myIfTable);
     ret = GetIfTable((MIB_IFTABLE*)&myIfTable, &nBufSize, TRUE);
    if( ret == 0)
    {
        for(int i=0; i<myIfTable.dwNumEntries; i++)
        {
            /*WIN_IfEntry entry;*/
            MIB_IFROW* pRow = &myIfTable.table[i];
            //             strcpy(entry.strName,"");
            //             strcpy(entry.strDescr, (char*)pRow->bDescr);
            //             entry.nIndex = pRow->dwIndex;
            //             entry.nType = pRow->dwType;
            //             entry.nInOctets = pRow->dwInOctets; //pRow->dwInUcastPkts + pRow->dwInNUcastPkts;
            //             entry.nOutOctes = pRow->dwOutOctets; //pRow->dwOutUcastPkts + pRow->dwOutNUcastPkts;
            //             entry.nAdminStatus = pRow->dwAdminStatus;
            //             entry.nSpeed = pRow->dwSpeed;

            if(pRow->dwType == 6)
            {

            }
        }
    }
    return 0;
}

//!由于InOctets字段是32bit整数，增至0xFFFFFFFF后会掉头到0，所以要注意减法运算。

inline unsigned int GetWrappedDelta(unsigned int a, unsigned int b)
{
    if(a >= b)
        return a - b;
    else
        return (0xFFFFFFFF-b) + a;
}
int _tmain(int argc, _TCHAR* argv[])
{
    WIN_GetIfTable();
    return 0;
}

#endif