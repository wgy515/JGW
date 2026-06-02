#include "StdAfx.h"
#include "CJGW_IfTableTrafficMonitor.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_IfTableTrafficMonitor::CCJGW_IfTableTrafficMonitor(std::string& strConnectionName) : m_pIfTable(0),mConnectionSelected(0),mSelectAll(false),m_dwSize(0),mstrConnectionName(strConnectionName),mdwLastSent(0),mdwLastReceiver(0),mdwLastPps(0)
    {
    }


    CCJGW_IfTableTrafficMonitor::~CCJGW_IfTableTrafficMonitor(void)
    {
        if (m_pIfTable) free(m_pIfTable);
    }

    void CCJGW_IfTableTrafficMonitor::SetNetCardConnectName(std::string& strConnectName)
    {
        mstrConnectionName = strConnectName;
    }

    void CCJGW_IfTableTrafficMonitor::SelectNetCardInfo()
    {
        if (0 == mstrConnectionName.compare("All"))
        {
            mSelectAll = true;
            return;
        }

        for (size_t i = 0; i< mvConnections.size(); i++)
        {
            if (mstrConnectionName == mvConnections.at(i).description)
            {
                mConnectionSelected = i;
                return ;
            }
        }
        mConnectionSelected = -1;
    }

    std::vector<NetWorkConection>& CCJGW_IfTableTrafficMonitor::GetNetCardInfo()
    {
        mvConnections.clear();
        //为m_pIfTable开辟所需大小的内存
        if (m_pIfTable) free(m_pIfTable);
        m_dwSize = sizeof(MIB_IFTABLE);
        m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);

        //! 如果函数返回值为ERROR_INSUFFICIENT_BUFFER，说明m_pIfTable的大小不够
        if (ERROR_INSUFFICIENT_BUFFER == GetIfTable(m_pIfTable, &m_dwSize, FALSE))	
        {
            free(m_pIfTable);
            //! 用新的大小重新开辟一块内存
            m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);	
        }
        //! 获取当前所有的连接，并保存到m_connections容器中
        CAdapterCommon::GetAdapterInfo(mvConnections);
        GetIfTable(m_pIfTable, &m_dwSize, FALSE);
        CAdapterCommon::GetIfTableInfo(mvConnections, m_pIfTable);

        return mvConnections;
    }

    bool CCJGW_IfTableTrafficMonitor::GetTxRxDropRatePhyAddress(ULONGLONG& ulUploadSpeed,ULONGLONG& ulDownloadSpeed,float& txDropRate,float& rxDropRate,std::string& mac)
    {
        GetNetCardInfo();
        SelectNetCardInfo();
        DWORD dwInPkts = 0,dwOutPkts;

        int rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
        if (NO_ERROR != rtn) return false;

        PMIB_IFROW pMibTmp = &m_pIfTable->table[mvConnections[mConnectionSelected].index];
        dwInPkts = pMibTmp->dwInUcastPkts;
        dwInPkts += pMibTmp->dwInNUcastPkts;
        rxDropRate = (float)pMibTmp->dwInDiscards / (float)dwInPkts;

        dwOutPkts = pMibTmp->dwOutNUcastPkts;
        dwOutPkts += pMibTmp->dwOutUcastPkts;
        txDropRate = (float)pMibTmp->dwOutDiscards / (float)dwOutPkts;

        mdwReceiver = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwInOctets;
        mdwSent = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwOutOctets;

        //如果发送和接收的字节数为0或上次发送和接收的字节数为0或当前连接已改变时，网速无效
        if ((mdwReceiver == 0 && mdwSent == 0) || (mdwLastReceiver == 0 && mdwLastSent))
        {
            mdwSent = 0;
            mdwReceiver = 0;
        }
        else
        {
            ulDownloadSpeed = static_cast<unsigned int>(mdwReceiver - mdwLastReceiver);
            ulUploadSpeed = static_cast<unsigned int>(mdwSent - mdwLastSent);
        }

        if (0 == mdwLastSent && 0 == mdwLastReceiver && 0 == mdwLastPps)
        {
            ulDownloadSpeed = 0;
            ulUploadSpeed = 0;
        }

        mdwLastReceiver = mdwReceiver;
        mdwLastSent = mdwSent;
        mdwLastPps = mdwPps;
        //如果大于1GB/s，说明可能产生了异常，网速无效
        if (mdwReceiver > 1073741824) mdwReceiver = 0;
        if (mdwSent > 1073741824) mdwSent = 0;

        JGW_FormatString(mac,"%02x:%02x:%02x:%02x:%02x:%02x",pMibTmp->bPhysAddr[0],pMibTmp->bPhysAddr[1],pMibTmp->bPhysAddr[2],pMibTmp->bPhysAddr[3],pMibTmp->bPhysAddr[4],pMibTmp->bPhysAddr[5]);
        return true;
    }

    ULONGLONG CCJGW_IfTableTrafficMonitor::GetTotalSentRecvPps(LONGLONG& uPps)
    {
        ULONGLONG uTotalSendRecv = 0,uTotalPps = 0;
        uPps = 0;
        if (!mSelectAll && -1 == mConnectionSelected) return 0;
        //获取网络连接速度
        int rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
        if (NO_ERROR != rtn) return 0;

        if (!mSelectAll)		//获取当前选中连接的网速
        {
            
            mdwReceiver = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwInOctets;
            mdwSent = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwOutOctets;
            mdwPps = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwInUcastPkts + m_pIfTable->table[mvConnections[mConnectionSelected].index].dwOutUcastPkts;
        }
        else		//获取全部连接的网速
        {
            mdwReceiver = 0;
            mdwSent = 0;
            mdwPps = 0;
            for (size_t i = 0; i< mvConnections.size(); i++)
            {
                //if (i > 0 && m_pIfTable->table[m_connections[i].index].dwInOctets == m_pIfTable->table[m_connections[i - 1].index].dwInOctets
                //	&& m_pIfTable->table[m_connections[i].index].dwOutOctets == m_pIfTable->table[m_connections[i - 1].index].dwOutOctets)
                //	continue;		//连接列表中可能会有相同的连接，统计所有连接的网速时，忽略掉已发送和已接收字节数完全相同的连接
                mdwReceiver += m_pIfTable->table[mvConnections[i].index].dwInOctets;
                mdwSent += m_pIfTable->table[mvConnections[i].index].dwOutOctets;
                mdwPps += m_pIfTable->table[mvConnections[i].index].dwInUcastPkts;
                mdwPps += m_pIfTable->table[mvConnections[i].index].dwOutUcastPkts;
            }
        }

        //如果发送和接收的字节数为0或上次发送和接收的字节数为0或当前连接已改变时，网速无效
        if ((mdwReceiver == 0 && mdwSent == 0) || (mdwLastReceiver == 0 && mdwLastSent))
        {
            mdwSent = 0;
            mdwReceiver = 0;
            mdwPps = 0;
            GetNetCardInfo();
            SelectNetCardInfo();
        }
        else
        {
            uTotalSendRecv = static_cast<unsigned int>(mdwReceiver - mdwLastReceiver);
            uTotalSendRecv += static_cast<unsigned int>(mdwSent - mdwLastSent);
            uTotalPps = static_cast<unsigned int>(mdwPps - mdwLastPps);
            uPps = uTotalPps;
        }

        if (0 == mdwLastSent && 0 == mdwLastReceiver && 0 == mdwLastPps)
        {
             uTotalSendRecv = 0;
             uTotalPps = 0;
             uPps = 0;
        }

        mdwLastReceiver = mdwReceiver;
        mdwLastSent = mdwSent;
        mdwLastPps = mdwPps;
        //如果大于1GB/s，说明可能产生了异常，网速无效
        if (mdwReceiver > 1073741824)
            mdwReceiver = 0;
        if (mdwSent > 1073741824)
            mdwSent = 0;

        return uTotalSendRecv;
    }

//     ULONGLONG CCJGW_IfTableTrafficMonitor::GetTotalSentReceived()
//     {
//         //获取网络连接速度
//         int rtn = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
//         if (NO_ERROR != rtn) return 0;
// 
//         if (!mSelectAll)		//获取当前选中连接的网速
//         {
//             mdwReceiver = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwInOctets;
//             mdwSent = m_pIfTable->table[mvConnections[mConnectionSelected].index].dwOutOctets;
//         }
//         else		//获取全部连接的网速
//         {
//             mdwReceiver = 0;
//             mdwSent = 0;
//             for (size_t i = 0; i< mvConnections.size(); i++)
//             {
//                 //if (i > 0 && m_pIfTable->table[m_connections[i].index].dwInOctets == m_pIfTable->table[m_connections[i - 1].index].dwInOctets
//                 //	&& m_pIfTable->table[m_connections[i].index].dwOutOctets == m_pIfTable->table[m_connections[i - 1].index].dwOutOctets)
//                 //	continue;		//连接列表中可能会有相同的连接，统计所有连接的网速时，忽略掉已发送和已接收字节数完全相同的连接
//                 mdwReceiver += m_pIfTable->table[mvConnections[i].index].dwInOctets;
//                 mdwSent += m_pIfTable->table[mvConnections[i].index].dwOutOctets;
//             }
//         }
// 
//         ULONGLONG dwCount = 0;
//         //如果发送和接收的字节数为0或上次发送和接收的字节数为0或当前连接已改变时，网速无效
//         if ((mdwReceiver == 0 && mdwSent == 0) || (mdwLastReceiver == 0 && mdwLastSent))
//         {
//             mdwSent = 0;
//             mdwReceiver = 0;
//         }
//         else
//         {
//             dwCount = static_cast<unsigned int>(mdwReceiver - mdwLastReceiver);
//             dwCount += static_cast<unsigned int>(mdwSent - mdwLastSent);
//         }
// 
//         if (0 == mdwLastSent && 0 == mdwLastReceiver)
//         {
//             dwCount = 0;
//         }
// 
//         mdwLastReceiver = mdwReceiver;
//         mdwLastSent = mdwSent;
// 
//         //如果大于1GB/s，说明可能产生了异常，网速无效
//         if (mdwReceiver > 1073741824)
//             mdwReceiver = 0;
//         if (mdwSent > 1073741824)
//             mdwSent = 0;
// 
//         return dwCount;
//     }

    void CCJGW_IfTableTrafficMonitor::ClearSentReceived()
    {
        mdwLastSent = 0;
        mdwLastReceiver = 0;
    }
}

