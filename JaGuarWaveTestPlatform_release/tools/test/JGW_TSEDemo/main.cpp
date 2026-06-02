#include "stdafx.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CPUMemDiskStatus.h"
#include <functional>
#include <algorithm>
#include <unordered_map>
using namespace std;
using namespace JGW;

typedef struct  
{
    std::string mstrIperfLogFilePath;
}S_IPERF_LOG_INFO;

typedef struct
{
    int requestCode; //! 请求码 默认1001
    int class_num; //！身份级别。Windows为0级、linux根据下级数目实时计算
    int rssi; //! 本机信号强度
    int tx_mcs; //! 发送mcs
    int rx_mcs;//! 接收mcs
    int tx_sector; //!
    int rx_sector;//! 
    float tx_drop_rate;//! 发送丢包率 double
    float rx_drop_rate;//! 接收丢包率 double
    std::string mac; //! Dongle mac地址
    std::string ip; //! Dongle ip地址
    std::string tx_speed; //! TX速率
    std::string rx_speed;//! RX速率
}CCJGW_RepStatusJson;

typedef struct
{
    int requestCode;

    double cpuAll;
    double cpu0;
    double cpu1;
    double cpu2;
    double cpu3;
    ULONGLONG mem_total;
    ULONGLONG mem_used;
    ULONGLONG mem_free;

    std::string mac;
}CCJGW_ReqSystemInfoJson;

bool compare_string(const std::string& left, const std::string& right) {
    return std::equal(left.begin(), left.end(), right.begin(), right.end(),
        [] (const char& a, const char& b) {
            return std::tolower(a) == std::tolower(b);
    });
};

int _tmain(int argc, _TCHAR* argv[])
{
    compare_string("a","a");
    std::string strStatusLog;
    size_t mnStatusTime = 1;
    CCJGW_RepStatusJson reqStatusJson;
    CCJGW_ReqSystemInfoJson mReqSystemInfoJson;
    //! %8I64d %7I64d %7I64d  %-15s %-15s
    //! mReqSystemInfoJson.mem_total,mReqSystemInfoJson.mem_used,mReqSystemInfoJson.mem_free,reqStatusJson.tx_speed.c_str(),reqStatusJson.rx_speed.c_str()
    JGW::JGW_FormatString(strStatusLog,"%4d %4d %5d %5d %5d %5d %10.2f %10.2f %6.2f  %5.1lf  %5.1lf  %5.1lf  %5.1lf  %8I64u %7I64u %7I64u  %-15s %-15s\n",mnStatusTime++,reqStatusJson.rssi,reqStatusJson.tx_mcs,reqStatusJson.rx_mcs,reqStatusJson.tx_sector,reqStatusJson.rx_sector,reqStatusJson.tx_drop_rate,reqStatusJson.rx_drop_rate,mReqSystemInfoJson.cpuAll,mReqSystemInfoJson.cpu0,mReqSystemInfoJson.cpu1,mReqSystemInfoJson.cpu2,mReqSystemInfoJson.cpu3,mReqSystemInfoJson.mem_total,mReqSystemInfoJson.mem_used,mReqSystemInfoJson.mem_free,reqStatusJson.tx_speed.c_str(),reqStatusJson.rx_speed.c_str());


    std::string strCmd = "iperf3 -s -p 5021:3";
    char szCmd[260] = {0};
    char* pBuf = NULL,*pSocketPort;
    strcpy_s(szCmd,260,strCmd.c_str());

    std::vector<std::string> vstrIperfCmd;
    std::string strIperfCmd,strIperfBaseCmd,strTemp;
    S_IPERF_LOG_INFO sIperfLogInfo;
    std::string strApplicationName = strtok(szCmd," ");
    strcpy_s(szCmd,260,strCmd.c_str());

    pBuf = strstr(szCmd,"-p");
    //mvIperfLogInfo.clear();
    //! 如果未找到-p 参数则默认启动单个iperf
    if (NULL == pBuf || NULL == strstr(szCmd,":"))
    {
        sIperfLogInfo.mstrIperfLogFilePath = JGW_RealativePathToAbsPathA("iperf.log");
        JGW_FormatString(strIperfCmd,"%s --logfile \"%s\"",strCmd.c_str(),sIperfLogInfo.mstrIperfLogFilePath.c_str());
        vstrIperfCmd.push_back(strIperfCmd);
    }//! 找到-P参数则解析
    else
    {
        strIperfBaseCmd = strCmd.substr(0,pBuf - szCmd);
        int nStartSocketPort = atoi(pBuf + 2),nSocketPortCount = 0;
        pSocketPort = strstr(pBuf + 2,":");
        nSocketPortCount = atoi(pSocketPort + 1);
        for (int i = 0;i < nSocketPortCount;i ++)
        {
            JGW_FormatString(strTemp,"iperf_%d.log",nStartSocketPort + i);
            sIperfLogInfo.mstrIperfLogFilePath = JGW_RealativePathToAbsPathA(strTemp);
            JGW_FormatString(strIperfCmd,"%s -p %d --logfile \"%s\"",strIperfBaseCmd.c_str(),nStartSocketPort + i,sIperfLogInfo.mstrIperfLogFilePath.c_str());
            vstrIperfCmd.push_back(strIperfCmd);
        }
    }

    std::string strFolder(JGW_W2A_W(JGW_GetApplicationFolder()));
    strFolder += "tool\\";

    for (size_t i = 0;i < vstrIperfCmd.size();i ++)
    {
        ShellExecuteA(NULL,"open",strApplicationName.c_str(),vstrIperfCmd[i].c_str(),strFolder.c_str(),SW_HIDE);
    }
    return 0;
#if 0
    //while( !_kbhit() )
    //{
    //    HQUERY          hQuery;
    //    HCOUNTER        *pCounterHandle;
    //    PDH_STATUS      pdhStatus;
    //    PDH_FMT_COUNTERVALUE  fmtValue;
    //    DWORD          ctrType;
    //    char            szPathBuffer[260] = {0};
    //    int				nCPUUsage;

    //    // Open the query object.
    //    pdhStatus = PdhOpenQuery (0, 0, &hQuery);
    //    pCounterHandle = (HCOUNTER *)GlobalAlloc(GPTR, sizeof(HCOUNTER));
    //    // Get the CPU used information
    //    strcat(szPathBuffer,"\\Processor Information(_Total)\\% Processor Time"); 
    //    pdhStatus = PdhAddCounterA (hQuery,
    //        szPathBuffer, 
    //        0, 
    //        pCounterHandle);

    //    pdhStatus = PdhCollectQueryData (hQuery);

    //    // Get the current value of this counter.
    //    pdhStatus = PdhGetFormattedCounterValue (*pCounterHandle,
    //        PDH_FMT_DOUBLE,
    //        &ctrType,
    //        &fmtValue);

    //    nCPUUsage = (int)fmtValue.doubleValue;
    //    //fmtValue.doubleValue
    //    if (pdhStatus == ERROR_SUCCESS) {
    //        printf (",\"%.20g\"/n", fmtValue.doubleValue);
    //    }
    //    else {
    //        // Print the error value.
    //        printf ("error.\"-1\""); 
    //    }

    //    // Close the query.
    //    pdhStatus = PdhCloseQuery (hQuery);	
    //    // Output the informaion
    //    // printf("CPU Usage is %d/n", 100-nCPUUsage );
    //    Sleep( 500 );
    //}

    CCPUMemDiskStatus m_CpuMemDiskStatus;
    std::vector<std::wstring> vFullCounterPath;
    std::vector<double> vDoubleValue;

    vFullCounterPath.push_back(L"\\Processor Information(_Total)\\% Processor Utility");
    vFullCounterPath.push_back(L"\\Processor Information(0,0)\\% Processor Utility");
    vFullCounterPath.push_back(L"\\Processor Information(0,1)\\% Processor Utility");
    vFullCounterPath.push_back(L"\\Processor Information(0,2)\\% Processor Utility");
    vFullCounterPath.push_back(L"\\Processor Information(0,3)\\% Processor Utility");
    m_CpuMemDiskStatus.SystemCpuInit(vFullCounterPath);
    WCHAR cData[MAX_PATH] = {0};
    ULONGLONG AllDiskTotal,AllDiskFree;
    while (1)
    {

        
        Sleep (1000);
        vDoubleValue.clear();
        /*double dCpuUsage = */m_CpuMemDiskStatus.GetSystemCpuCurrentUsage(vDoubleValue);
        
        //float fCpuUsage =  m_CpuMemDiskStatus.TransPercentToFloatValue(dCpuUsage);
       
        //m_CpuMemDiskStatus.SystemCpuUnInit();
        m_CpuMemDiskStatus.GetSystemDiskStatus(AllDiskTotal,AllDiskFree);

        double fMemSize =  m_CpuMemDiskStatus.GetTotalPhysicalMemory();
        float fMemUsage =  m_CpuMemDiskStatus.TransPercentToFloatValue(m_CpuMemDiskStatus.GetPhysicalMemoryUsage());

        ULONGLONG ulCurDiskSize = 0,ulCurDiskFree = 0;
        m_CpuMemDiskStatus.GetSystemCurrentDiskStatus(ulCurDiskSize, ulCurDiskFree);
        float fCurDiskUsage = m_CpuMemDiskStatus.TransPercentToFloatValue(m_CpuMemDiskStatus.GetSystemCurrentDiskUsage());

        for (size_t i = 0;i < vDoubleValue.size();i ++)
        {
            memset(cData, 0x00, sizeof(cData)/sizeof(char));
            swprintf_s(cData,260, L"%0.6f", vDoubleValue[i]);
            wstring wstrCpuUsage = cData;
            wcout << L"Cpu" << i << L" " << wstrCpuUsage << std::endl;
        }
        

        memset(cData, 0x00, sizeof(cData)/sizeof(char));
        swprintf_s(cData,260, L"%0.2f", fMemSize);
        wstring wstrMemSize = cData;

        memset(cData, 0x00, sizeof(cData)/sizeof(char));
        swprintf_s(cData,260, L"%0.2f", fMemUsage);
        wstring wstrMemUsage = cData;
// 
//         memset(cData, 0x00, sizeof(cData)/sizeof(char));
//         swprintf(cData, L"%d", ulCurDiskSize);
//         wstring wstrDiskSize = cData;
// 
//         memset(cData, 0x00, sizeof(cData)/sizeof(char));
//         swprintf(cData, L"%0.2f", fCurDiskUsage);
//         wstring wstrDiskUsage = cData;

       
    }

     m_CpuMemDiskStatus.SystemCpuUnInit();
#endif
    return 0;
}