#pragma once
#include <vector>
#include <string>
#include <pdh.h>
#include "JGW_WindowsFuncPlugin_Define.h"

namespace JGW
{
    class CCJGW_CPUMemDiskStatusImp
    {
    public:
        CCJGW_CPUMemDiskStatusImp(){}
        virtual ~CCJGW_CPUMemDiskStatusImp(){}
        virtual void GetSystemDiskStatus(ULONGLONG& AllDiskTotal, ULONGLONG& AllDiskFree) = 0;
        virtual void GetSystemCurrentDiskStatus(ULONGLONG& TatolMB, ULONGLONG& FreeCaller) = 0;
        virtual double GetSystemCurrentDiskUsage() = 0;
        //! \\Processor Information(_Total)\\% Processor Utility 
        //! \\Processor Information(0,0)\\% Processor Utility
        //! \\Processor Information(0,1)\\% Processor Utility
        //! \\Processor Information(0,2)\\% Processor Utility
        //! \\Processor Information(0,3)\\% Processor Utility
        //! \\Processor Information(0,4)\\% Processor Utility
        virtual bool SystemCpuInit(const std::vector<std::wstring>& vFullCounterPath) = 0;			//系统CPU使用率 设置检测开始点
        virtual void SystemCpuUnInit() = 0;			//系统CPU使用率
        virtual bool GetSystemCpuCurrentUsage(std::vector<double>& vDoubleValue) = 0;	//系统CPU使用率 检测结束点
        virtual BOOL GetPhysicalMemoryState(ULONGLONG& totalPhysMem, ULONGLONG& physMemUsed) = 0;
        virtual double GetTotalPhysicalMemory() = 0;
        virtual double GetTotalPhysicalMemoryUsed() = 0;
        virtual double GetPhysicalMemoryUsage() = 0;
        virtual float TransPercentToFloatValue(const double dValue) = 0;
    };

#ifdef JGW_EXTERN_
    extern "C" 
    {
#endif
        JGWWINDOWSFUNCPLUGIN_API CCJGW_CPUMemDiskStatusImp* GetCPUMemDiskStatusImp();

        JGWWINDOWSFUNCPLUGIN_API void ReleaseCPUMemDiskStatusImp(CCJGW_CPUMemDiskStatusImp* pCPUMemDiskStatusImp);
#ifdef JGW_EXTERN_
    }
#endif
}