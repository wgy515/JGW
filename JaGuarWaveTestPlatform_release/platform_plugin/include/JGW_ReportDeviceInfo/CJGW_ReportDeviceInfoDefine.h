#pragma once
#include <string>
#include <JGW_FoundationFunc/CJGW_ReferenceCount.hpp>
#define NETCARD_SORT_CONFIG "NetCardSortConfig.data"
//#define MAX_NETCARD_DEFAULT_COUNT 100
/*
接收到设备消息，通过POST 方式发送给对应的窗口
*/
namespace JGW
{
    enum E_DEVICE_TYPE
    {
        E_USB_TYPE,
        E_NETCARD_TYPE
    };

    enum E_NETCARD_PHYSICAL_STATE
    {
        //! 网卡物理断开连接
        E_NETCARD_PHYSICAL_DISCONNECT = 0,
        //! 网卡物理插入
        E_NETCARD_PHYSICAL_CONNECT 
    };

    typedef struct
    {
        //! 默认 NETCARD TYPE
        E_DEVICE_TYPE meDeviceType; 
        //! 网卡物理连接状态
        E_NETCARD_PHYSICAL_STATE meNetCardPhysicalState;
        //! 平台排序索引 -1 未排序
        int	mnSortIndex;
        //! 引用计数
        //! CCJGW_ReferenceCount mReferenceCount;
        //! IP 地址
        std::wstring mstrIPAddr;
        //! 描述信息
        std::wstring mstrDescription;
        //! 适配器名称
        std::wstring mstrAdapterName;
        //! 适配器GUID
        std::wstring mstrNetGUID;
    }S_NETCARD_INFO,*PS_NETCARD_INFO;

    typedef struct
    {
        bool mbIsWindwHwnd;
        union
        {
            HWND mhWindow;
            DWORD mhThreadID;
        };
        std::wstring mstrIPAddressSegment;
    }S_REG_NETCARD_MSG,*PS_REG_NETCARD_MSG;
}