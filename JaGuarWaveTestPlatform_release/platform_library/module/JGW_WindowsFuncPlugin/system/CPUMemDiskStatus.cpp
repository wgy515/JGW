#include "stdafx.h"
#include <JGW_WindowsFuncPlugin/CPUMemDiskStatus.h>
#include <Shlwapi.h>
#include <windows.h>
#include <string>
#include <tchar.h>
using namespace std;

#pragma comment(lib, "Pdh.lib")
#pragma comment(lib, "Shlwapi.lib")


namespace JGW
{
    CCPUMemDiskStatus::CCPUMemDiskStatus()
    {
        m_CpuQuery = NULL;
        //m_CpuTotal = NULL;
    }

    CCPUMemDiskStatus::~CCPUMemDiskStatus()
    {
        if(m_CpuQuery){
            PdhCloseQuery(m_CpuQuery);
            m_CpuQuery = NULL;
        }
        SystemCpuUnInit();
    }

    // 获取系统所有硬盘使用情况		
    void CCPUMemDiskStatus::GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree)
    {
        int DType = 0;
        int si = 0;
        BOOL bResult = FALSE;
        int DiskCount = 0;
        ULONGLONG nTempTotal = 0;
        ULONGLONG nTempFree = 0;

        ULONGLONG i64FreeBytesToCaller;  
        ULONGLONG i64TotalBytes;  
        ULONGLONG i64FreeBytes;

        //枚举磁盘数量
        DWORD DiskInfo = GetLogicalDrives();
        while (DiskInfo){
            if (DiskInfo & 1){
                ++DiskCount;
            }
            DiskInfo = DiskInfo >> 1;
        }
        int test = DiskCount;
        int DSLength = GetLogicalDriveStrings(0, NULL);

        TCHAR* DStr = NULL;
        while (!DStr){
            DStr = new TCHAR[DSLength];
        }
        if (NULL == DStr){
            return;
        }
        int nRet = GetLogicalDriveStrings(DSLength, (LPTSTR)DStr);

        //枚举磁盘名称
        for (int i = 0;i < DSLength / 4; i++){
            TCHAR strDisk[3] = {0};
            _stprintf_s(strDisk,3, _T("%c:"), DStr[si]);
            DType = GetDriveType(DStr + i * 4);
            bResult = GetDiskFreeSpaceEx(
                strDisk, 
                (PULARGE_INTEGER)&i64FreeBytesToCaller,
                (PULARGE_INTEGER)&i64TotalBytes,
                (PULARGE_INTEGER)&i64FreeBytes);

            if (bResult){
                nTempTotal += (ULONGLONG)i64TotalBytes / 1024 / 1024 / 1024;
                nTempFree += (ULONGLONG)i64FreeBytesToCaller / 1024 / 1024 / 1024;
            }
            else{
                //OutputDebugString(_T("设备未准备..."));
            }
            si += 4;
        }
        AllDiskTotal = nTempTotal;
        AllDiskFree = nTempFree;

        delete[] DStr;
    }

    //获取系统当前磁盘状态
    void CCPUMemDiskStatus::GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller)
    {
        BOOL bResult = FALSE;
        ULONGLONG nTempTotal = 0;
        ULONGLONG nTempFree = 0;

        ULONGLONG ui64FreeBytesToCaller;  
        ULONGLONG ui64TotalBytes;  
        ULONGLONG ui64FreeBytes;

        TCHAR szModule[MAX_PATH*2] = {0};
        GetModuleFileName(NULL, szModule, MAX_PATH*2);
#ifdef _UNICODE
        wstring strPath(szModule);
        wstring strDirve = strPath.substr(0, 1);
#else
        string strPath(szModule);
        string strDirve = strPath.substr(0, 1);
#endif
        TCHAR szDisk[3] = {0};
        _stprintf_s(szDisk,3, _T("%c:"), strDirve[0]);
        bResult = GetDiskFreeSpaceEx(
            szDisk, 
            (PULARGE_INTEGER)&ui64FreeBytesToCaller,	//用户可用的磁盘空间
            (PULARGE_INTEGER)&ui64TotalBytes,			//磁盘总共的空间
            (PULARGE_INTEGER)&ui64FreeBytes);			//磁盘空闲的空间.以上都是字节为单位。
        if (bResult){
            nTempTotal = ui64TotalBytes / 1024 / 1024 / 1024;
            nTempFree = ui64FreeBytesToCaller / 1024 / 1024 / 1024;
            TatolMB = nTempTotal;
            FreeCaller = nTempFree;
        }
    }

    //获取系统当前磁盘使用率
    double CCPUMemDiskStatus::GetSystemCurrentDiskUsage()
    {
        BOOL bResult = FALSE;
        ULONGLONG ui64FreeBytesToCaller;  
        ULONGLONG ui64TotalBytes;  
        ULONGLONG ui64FreeBytes;

        TCHAR szModule[MAX_PATH*2] = {0};
        GetModuleFileName(NULL, szModule, MAX_PATH*2);
        ::PathRemoveFileSpec(szModule);
#ifdef _UNICODE
        wstring strPath(szModule);
        wstring strDirve = strPath.substr(0, 1);
#else
        string strPath(szModule);
        string strDirve = strPath.substr(0, 1);
#endif

        TCHAR szDisk[3] = {0};
        _stprintf_s(szDisk,3, _T("%c:"), strDirve[0]);
        bResult = GetDiskFreeSpaceEx(
            szDisk, 
            (PULARGE_INTEGER)&ui64FreeBytesToCaller,	//用户可用的磁盘空间
            (PULARGE_INTEGER)&ui64TotalBytes,			//磁盘总共的空间
            (PULARGE_INTEGER)&ui64FreeBytes);			//磁盘空闲的空间.以上都是字节为单位。
        if (bResult){
            double TempNum  = ((ui64TotalBytes / 1024 / 1024 / 1024)-(ui64FreeBytesToCaller/ 1024 / 1024 / 1024))* 100.0 / (ui64TotalBytes/ 1024 / 1024 / 1024);
            return TempNum;
        }
        return 0;
    }

    //初始化CPU
    bool CCPUMemDiskStatus::SystemCpuInit(const std::vector<std::wstring>& vFullCounterPath)
    {
        PDH_STATUS state;
        PDH_HCOUNTER phCounter = NULL;
        state = PdhOpenQuery(NULL, NULL, &m_CpuQuery);
        if (state != ERROR_SUCCESS) return false;

        for (size_t i = 0;i < vFullCounterPath.size();i ++)
        {
            state = PdhAddCounter(m_CpuQuery, vFullCounterPath[i].c_str(), NULL, &phCounter);
            if (ERROR_SUCCESS != state) return false;
            mvPDHHCounter.push_back(phCounter);
        }
        state = PdhCollectQueryData(m_CpuQuery);
        return (state == ERROR_SUCCESS);
    }

    //获取CPU使用值
    bool CCPUMemDiskStatus::GetSystemCpuCurrentUsage(std::vector<double>& vDoubleValue)
    {
        PDH_FMT_COUNTERVALUE counterVal;
        PDH_STATUS state = PdhCollectQueryData(m_CpuQuery);
        if (ERROR_SUCCESS != state) return false;
        vDoubleValue.clear();

        for (size_t i = 0;i < mvPDHHCounter.size();i ++)
        {
            state = PdhGetFormattedCounterValue(mvPDHHCounter[i], PDH_FMT_DOUBLE, NULL, &counterVal);
            if (ERROR_SUCCESS == state) vDoubleValue.push_back(counterVal.doubleValue);
            else vDoubleValue.push_back(0.00);
        }
        return true; 
    }

    //反初始化CPU
    void CCPUMemDiskStatus::SystemCpuUnInit()
    {
        for (size_t i = 0;i < mvPDHHCounter.size();i ++)
        {
            if (mvPDHHCounter[i]){
                PdhRemoveCounter(mvPDHHCounter[i]);
                mvPDHHCounter[i] = NULL;
            }
        }
        mvPDHHCounter.clear();
        if(m_CpuQuery){
            PdhCloseQuery(m_CpuQuery);
            m_CpuQuery = NULL;
        }
    }

    //物理内存和使用
    BOOL CCPUMemDiskStatus::GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed)
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        totalPhysMem = memInfo.ullTotalPhys;
        physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
        return TRUE;
    }

    //机器物理内存容量
    double CCPUMemDiskStatus::GetTotalPhysicalMemory()
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        double totalPhysMem = ((memInfo.ullTotalPhys  / 1024 / 1024)  * 100)/ 1024.00;
        double fTemptotal = totalPhysMem / 100.0;
        return fTemptotal;
    }

    //机器物理内存使用
    double CCPUMemDiskStatus::GetTotalPhysicalMemoryUsed()
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        double physMemUsed = (((memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024 / 1024) * 100) / 1024.00;
        double fTemMemUsed = physMemUsed / 100.0;
        return fTemMemUsed;
    }

    // 机器物理内存使用率 
    double CCPUMemDiskStatus::GetPhysicalMemoryUsage()
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);
        double MemUsage = memInfo.dwMemoryLoad;
        return MemUsage;
    }

    //使用率百分数转换浮点数
    float CCPUMemDiskStatus::TransPercentToFloatValue(const double dValue)
    {
        float fResult = 0.00;
        if (dValue == 0){
            return fResult;
        }
        double dTemp = dValue * 10;
        int iTemp = (int)dValue;
        float fCmp =  static_cast<float>(dTemp - iTemp);
        fCmp = fCmp * 10;
        iTemp = static_cast<int>(iTemp + (int)fCmp / 10.0);
        fResult = iTemp / 1000.0f;
        return fResult;
    }

    CCJGW_CPUMemDiskStatusImp* GetCPUMemDiskStatusImp()
    {
        return new CCPUMemDiskStatus;
    }

    void ReleaseCPUMemDiskStatusImp(CCJGW_CPUMemDiskStatusImp* pCPUMemDiskStatusImp)
    {
        delete pCPUMemDiskStatusImp;
    }
}