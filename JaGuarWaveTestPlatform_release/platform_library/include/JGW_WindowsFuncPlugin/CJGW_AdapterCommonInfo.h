#pragma once
#include "JGW_WindowsFuncPlugin_Define.h"
#include <vector>
#include <string>
#include <iphlpapi.h>

namespace JGW
{
    struct NetWorkConection
    {
        int index;			//该连接在MIB_IFTABLE中的索引
        DWORD in_bytes;	//初始时已接收字节数
        DWORD out_bytes;	//初始时已发送字节数
        std::string description;		//网络描述
        std::wstring ip_address;		//IP地址
        std::wstring subnet_mask;	//子网掩码
        std::wstring default_gateway;	//默认网关
        std::wstring adapter_name;
    };
    //! Network traffic
    typedef struct 
    {
        bool mbConnectNet;
        float mfRxDropRate; //! 接收丢包率
        float mfTxDropRate; //! 发送丢包率
        ULONGLONG mulPkts; //! 上次调用到现在接收包数量
        ULONGLONG mulInOctets; //! 上次调用到现在接收（字节）
        ULONGLONG mulOutOctets; //! 上次调用到现在发送(字节)
        std::string mstrMac;
    }S_NETWORK_TRAFFIC_INFO;

    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_AdapterCommonInfo
    {
    public:
        CCJGW_AdapterCommonInfo(void);
        ~CCJGW_AdapterCommonInfo(void);
    public:
        //! 获取所有网络适配器信息(包括使用和未使用)
        std::vector<NetWorkConection>& GetNetAdaptersInfo();
        //! 获取有效的网络适配器信息
        //! std::vector<NetWorkConection>& GetValidNetAdaptersInfo();
        //! The speed of the interface in bits per second.
        DWORD GetNetAdaptersSpeedToDescription(const std::string& strAdapterDescription);
        //! 获取网络流量信息
        void GetNetWorkTrafficInfoToDescription(const std::string& strAdapterDescription,S_NETWORK_TRAFFIC_INFO& sNetWorkTrafficInfo);
    private:
        //!
        bool GetNetIfTableInfo();
    private:
        DWORD  m_dwSize;	//! m_pIfTable的大小
        MIB_IFTABLE* m_pIfTable;
        ULONGLONG mulSent; //! 当前发送
        ULONGLONG mulReceiver;//! 当前接收
        ULONGLONG mulPps; //! 当前PPS
        ULONGLONG mulLastSent; //! 上一次发送
        ULONGLONG mulLastReceiver;//! 上一次接收
        ULONGLONG mulLastPps; //! 上一次数据包总量
    };
}