#pragma once
#include "Common/AdapterCommon.h"

namespace JGW
{
    class CCJGW_IfTableTrafficMonitor
    {
    public:
        CCJGW_IfTableTrafficMonitor(std::string& strConnectionName);
        ~CCJGW_IfTableTrafficMonitor(void);

        std::vector<NetWorkConection>& GetNetCardInfo();
        //! 获取发送接收
        ULONGLONG GetTotalSentReceived();
        //!
        ULONGLONG GetTotalSentRecvPps(LONGLONG& uTotalPps);
        //! 清空发送接收
        void ClearSentReceived();
        //!
        void SelectNetCardInfo();
    private:
        bool    mSelectAll;		//! 统计所有连接的网速
        int     mConnectionSelected;	//! 要显示流量的连接的序号
        DWORD   m_dwSize;	//! m_pIfTable的大小
        ULONGLONG mdwSent; //! 当前发送
        ULONGLONG mdwReceiver;//! 当前接收
        ULONGLONG mdwLastSent; //! 上一次发送
        ULONGLONG mdwLastReceiver;//! 上一次接收
        ULONGLONG mdwLastPps; //! 上一次数据包总量
        ULONGLONG mdwPps;
        MIB_IFTABLE* m_pIfTable;
        std::string& mstrConnectionName;	//当前选择网络的名称
        std::vector<NetWorkConection> mvConnections;	//保存获取到的要显示到“选择网卡”菜单项中的所有网络连接
    };
}