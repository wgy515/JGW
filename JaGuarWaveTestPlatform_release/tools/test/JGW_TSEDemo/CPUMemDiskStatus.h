#ifndef __CPUMemDiskStatus_h__
#define __CPUMemDiskStatus_h__
#include <vector>
#include <string>
//每个磁盘的状态
typedef struct tagDISKSTATUS 
{
    DWORD	_FreeAvalidToCaller;	//用于容纳调用者可用的字节数量 BYTE
    DWORD	_TotalNummber;			//用于容纳磁盘上的总字节数	BYTE
    DWORD	_TotalNummberOfFree;	//用于容纳磁盘上可用的字节数	BYTE
}DISKSTATUS, *LPDISKSTATUS;
//所有磁盘的状态
typedef struct tagAllDISKSTATUS
{
    UINT	_DiskCount;				//磁盘数量
    DWORD	_Total;					//所有磁盘总容量MB
    DWORD	_OfFree;				//所有磁盘剩余容量MB
}AllDISKSTATUS, *LPAllDISKSTATUS;


#include "pdh.h"

class CCPUMemDiskStatus
{
public:
    CCPUMemDiskStatus();
    ~CCPUMemDiskStatus();
public:
    void		GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree);
    void		GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller);
    double		GetSystemCurrentDiskUsage();
    //! \\Processor Information(_Total)\\% Processor Utility 
    //! \\Processor Information(0,0)\\% Processor Utility
    //! \\Processor Information(0,1)\\% Processor Utility
    //! \\Processor Information(0,2)\\% Processor Utility
    //! \\Processor Information(0,3)\\% Processor Utility
    //! \\Processor Information(0,4)\\% Processor Utility
    bool		SystemCpuInit(std::vector<std::wstring>& vFullCounterPath);			//系统CPU使用率 设置检测开始点
    void		SystemCpuUnInit();			//系统CPU使用率
    bool		GetSystemCpuCurrentUsage(std::vector<double>& vDoubleValue);	//系统CPU使用率 检测结束点



    BOOL		GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed);
    double		GetTotalPhysicalMemory();
    double		GetTotalPhysicalMemoryUsed();
    double		GetPhysicalMemoryUsage();
    float		TransPercentToFloatValue(const double dValue);
private:
    PDH_HQUERY		m_CpuQuery;
    //!PDH_HCOUNTER	m_CpuTotal[];
    std::vector<PDH_HCOUNTER> mvPDHHCounter;
};


#endif
