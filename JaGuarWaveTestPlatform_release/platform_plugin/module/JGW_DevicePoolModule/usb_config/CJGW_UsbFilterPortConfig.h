#pragma once
#include <JGW_DevicePool/CJGW_DevicePoolModule_Define.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_UsbFilterPortConfig
    {
    public:
        CCJGW_UsbFilterPortConfig(void);
        ~CCJGW_UsbFilterPortConfig(void);
        //! 加载配置文件
        bool LoadUsbFilterPortConfig(const wchar_t* strUsbFilterPortConfigPath);
        //! 获取排序索引
        size_t GetUsbSortIndexToComPort(size_t nComport);
        //! 获取排序索引
        size_t GetUsbSortIndexToPhysicalAddress(const wchar_t* strPhysicalAddress);
        
        void ClearUsbSortIndex();

        bool IsFilterUsbPidVid(const wchar_t* strUsbInstance);
    private:
        void LoadUsbFilterPidVidConfig();
    private:
        size_t mMaxDevCounts;
        std::vector<S_USB_SORT_INFO> mvsUsbSortInfos;
        //! VID_05C6&PID_901D
        std::vector<std::wstring> mvFilterUsbPidVid;
        CCJGW_ConfigIni mConfigIni;
    };
}


