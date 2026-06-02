#include "StdAfx.h"
#include "CJGW_DonglesMonitorDlg.h"
#include "CJGW_NetCardMonitorThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#define GB_UNIT_DEFINE (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024 * (ULONGLONG)1024
namespace JGW
{
    //! std::wstring mstrIPAddress;
    const LONGLONG gGBUint = GB_UNIT_DEFINE;
    CCJGW_AsyncWndMessage* gpAsyncWndMessage;

    CCJGW_NetCardMonitorThread::CCJGW_NetCardMonitorThread(std::string& strNetCardInfo):mstrNetCardInfo(strNetCardInfo)
    {

    }


    CCJGW_NetCardMonitorThread::~CCJGW_NetCardMonitorThread(void)
    {

    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_NetCardMonitorThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_NetCardMonitorThread::OnMonitorThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_NetCardMonitorThread::OnBreakLoop()
    {
       /* if (mpsPcap_t) pcap_breakloop(mpsPcap_t);*/
    }
// 
//     void dispatcher_handler(u_char *state, const struct pcap_pkthdr *header, const u_char *pkt_data)
//     {
//         struct timeval *old_ts = (struct timeval *)state;
//         u_int delay,mstime;
//         ULARGE_INTEGER Bps,Pps;
//         struct tm *ltime;
//         time_t local_tv_sec;
// 
//         /* 以毫秒计算上一次采样的延迟时间 */
//         /* 这个值通过采样到的时间戳获得 */
//         delay = (header->ts.tv_sec - old_ts->tv_sec) * 1000000 - old_ts->tv_usec + header->ts.tv_usec;
//         /* 获取每秒的比特数b/s */
//         Bps.QuadPart=(((*(LONGLONG*)(pkt_data + 8)) * 8 * 1000000) / (delay));
//         /*                                            ^      ^
//         |      |
//         |      | 
//         |      |
//         将字节转换成比特 --   |
//         |
//         延时是以毫秒表示的 --
//         */
//         /* 得到每秒的数据包数量 */
//         Pps.QuadPart=(((*(LONGLONG*)(pkt_data)) * 1000000) / (delay));
// 
//         /* 将时间戳转化为可识别的格式 */
//         local_tv_sec = header->ts.tv_sec;
//         ltime = localtime(&local_tv_sec);
//         mstime = header->ts.tv_usec * 1000;
// 
// #if 0
//         /* 打印时间戳*/
//         printf("%s ", timestr);
// 
//         /* 打印采样结果 */
//         printf("BPS=%I64u ", Bps.QuadPart / (1024));
//         printf("PPS=%I64u\n", Pps.QuadPart / (1024));
// #else  
// #if 0
//         std::wstring strTemp;
//         //! B/s
//         if (Bps.QuadPart < 1024)
//         {
//             //! yyyyMMddHHmmssSSS
//             JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%I64u B/s,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,Bps.QuadPart,Pps.QuadPart);
//         }
//         else if (Bps.QuadPart >= 1024 && Bps.QuadPart < 1024 * 1024)
//         {
//             double speed = (double)Bps.QuadPart / 1024.00;
//             JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%.2lf KB/s,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,speed/*Bps.QuadPart / 1024*/,Pps.QuadPart);
//         }
//         else if (Bps.QuadPart >= 1024 * 1024 && Bps.QuadPart < 1024 * 1024 * 1024)
//         {
//             double speed = (double)Bps.QuadPart / (1024.00 * 1024.00);
//             JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%.2lf MB/s,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,speed/*Bps.QuadPart / 1024*/,Pps.QuadPart);
//         }
//         else if (Bps.QuadPart >= 1024 * 1024 * 1024 && Bps.QuadPart < gGBUint)
//         {
//             double speed = (double)Bps.QuadPart / (1024.00 * 1024.00 * 1024.00);
//             JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%.2lf GB/s,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,speed/*Bps.QuadPart / 1024*/,Pps.QuadPart);
//         }
//         else
//         {
//             double speed = (double)Bps.QuadPart / (1024.00 * 1024.00 * 1024.00 * 1024.00);
//             JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%.2lf TB/s,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,speed/*Bps.QuadPart / 1024*/,Pps.QuadPart);
//         }
// #else
//         std::wstring strTemp;
//         JGW_FormatWString(strTemp,L"%02d:%02d:%02d:%03d,%I64u,%I64u",ltime->tm_hour,ltime->tm_min,ltime->tm_sec,mstime,Bps.QuadPart,Pps.QuadPart);
// 
// #endif
// 
//         gpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strTemp.c_str());
// #endif
//         //存储当前的时间戳
//         old_ts->tv_sec=header->ts.tv_sec;
//         old_ts->tv_usec=header->ts.tv_usec;
//     }

    void CCJGW_NetCardMonitorThread::OnMonitorThread(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        gpAsyncWndMessage = mpAsyncWndMessage;
        std::string strName = mstrNetCardInfo;
        //char errbuf[PCAP_ERRBUF_SIZE * 4+ 1] = {0};

//         if((mpsPcap_t= pcap_open_live(strName.c_str(),100,/*PCAP_OPENFLAG_PROMISCUOUS*/1,1000,errbuf))==NULL)
//         {
//             JGW_FormatString(strName,"Unable to open adapter %s.",errbuf);
//             MessageBoxA(FindWindow(NULL,NULL),strName.c_str(),"错误",MB_ICONERROR);
//             mpAsyncWndMessage->PutAsyncMessage(WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS);
//             return;
//         }
// 
//         /* 将接口设置为统计模式 */
//         if (pcap_setmode(mpsPcap_t, MODE_STAT) < 0)
//         {
//             MessageBoxA(FindWindow(NULL,NULL),"Error setting the mode.","错误",MB_ICONERROR);
//             mpAsyncWndMessage->PutAsyncMessage(WM_HWND_CLOSE_SUB_PLUGIN_WINDOWS);
//             pcap_close(mpsPcap_t);
//             mpsPcap_t = NULL;
//             return;
//         }
//         //! 采样
//         pcap_loop(mpsPcap_t, 0, dispatcher_handler, (PUCHAR)&st_ts);
    }
}
