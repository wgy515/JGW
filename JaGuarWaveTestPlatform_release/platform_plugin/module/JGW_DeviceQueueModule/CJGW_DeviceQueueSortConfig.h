#pragma once
#include <JGW_DeviceQueueModule/CJGW_DeviceQueueImpl.h>
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

#define MAX_DEVICE_QUEUE_SORT 1
namespace JGW
{
    class CCJGW_DeviceQueueSortConfig
    {
    public:
        CCJGW_DeviceQueueSortConfig(void);
        ~CCJGW_DeviceQueueSortConfig(void);
    public:
        //! 
        bool LoadDeviceQueueSortConfig(const std::wstring& strDeviceQueueSortConfig,S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo);
        // 添加设备排序信息
        void AddDeviceQueueSortConfig(const S_DEVICESORT_CONFIG& sDeviceSortConfig);
        //!
        void UpdateDeviceQueueSortConfig(int nViewIndex);
        //! 保存设备排序配置信息
        void SaveDeviceQueueSortConfig(S_DEVICEQUEUE_PROGRAM_INFO& sDeviceQueueProgramInfo);
        //! 清空排序信息
        void ClearDeviceQueueSortConfig();
    public:
        //! 获取设备排序索引或者视图索引
        int GetFritDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice);
        //! 根据视图索引或者设备排序信息
        bool GetDeviceSortConfigToFristSortIndex(int nViewIndex,S_DEVICESORT_CONFIG& sDeviceSortConfig);
        //! 获取第二设备排序索引或者视图索引
        int GetSencondDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice);
        //! 获取第三设备排序索引或者视图索引
        int GetThreeDeviceViewIndexToSortIndex(int nSortIndex,const std::wstring& strDevice);
    public:
        //! 获取设备排序信息
        std::vector<S_DEVICESORT_CONFIG>& GetDeviceSortConfigInfo();
        //! 根据视图索引获取对应排序信息
        S_DEVICESORT_CONFIG& GetDeviceSortConfigToViewIndex(int nViewIndex);
    private:
        E_DEVICE_QUEUE_PROGRAM meDeviceQueueProgram;
        CCJGW_ConfigIni mcConfigIni;
        std::vector<S_DEVICESORT_CONFIG> mvsDeviceSortConfigs;
    };
}


