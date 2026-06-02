#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_AdapterCommonInfo.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    std::vector<NetWorkConection> mvNetWorkConnection;

    CCJGW_AdapterCommonInfo::CCJGW_AdapterCommonInfo(void) : mulSent(0),mulLastReceiver(0),mulReceiver(0),mulLastSent(0),mulLastPps(0),mulPps(0),m_pIfTable(NULL),m_dwSize(0)
    {
    }


    CCJGW_AdapterCommonInfo::~CCJGW_AdapterCommonInfo(void)
    {
        if (m_pIfTable)
        {
            m_dwSize = 0;
            free(m_pIfTable);
            m_pIfTable = NULL;
        }
    }

    DWORD CCJGW_AdapterCommonInfo::GetNetAdaptersSpeedToDescription(const std::string& strAdapterDescription)
    {
        if (!GetNetIfTableInfo())
        {
            return 0;
        }

        for (unsigned int i = 0; i < m_pIfTable->dwNumEntries; i++)
        {
            std::string strTempDescription = (const char*)m_pIfTable->table[i].bDescr;
            if (0 == strTempDescription.compare(strAdapterDescription))
            {
                //! 判断当前适配器网络是否处于连接状态
                if (IF_OPER_STATUS_OPERATIONAL == m_pIfTable->table[i].dwOperStatus ||  m_pIfTable->table[i].dwOperStatus == IF_OPER_STATUS_CONNECTED)
                {
                    return m_pIfTable->table[i].dwSpeed;
                }
            }
        }
        return 0;
    }

    std::vector<NetWorkConection>& CCJGW_AdapterCommonInfo::GetNetAdaptersInfo()
    {
        unsigned long stSize = sizeof(IP_ADAPTER_INFO);
        PIP_ADAPTER_INFO pIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,sizeof(IP_ADAPTER_INFO)),pIpAdapterInfoTmp = NULL;;
        int res = GetAdaptersInfo(pIpAdapterInfo, &stSize);
        mvNetWorkConnection.clear();
        //! 如果缓冲区太小则扩大缓冲区
        if (ERROR_BUFFER_OVERFLOW == res)
        {
            free(pIpAdapterInfo);
            pIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,stSize);
            res = GetAdaptersInfo(pIpAdapterInfo, &stSize);
        }
        pIpAdapterInfoTmp = pIpAdapterInfo;
        if (ERROR_SUCCESS != res) goto TEST_END;

        while (pIpAdapterInfoTmp)
        {
            NetWorkConection connection;
            connection.description = pIpAdapterInfoTmp->Description;
            connection.ip_address = JGW_A2W_A(pIpAdapterInfoTmp->IpAddressList.IpAddress.String);
            connection.subnet_mask = JGW_A2W_A(pIpAdapterInfoTmp->IpAddressList.IpMask.String);
            connection.default_gateway = JGW_A2W_A(pIpAdapterInfoTmp->GatewayList.IpAddress.String);
            connection.adapter_name = JGW_A2W_A(pIpAdapterInfoTmp->AdapterName);
            mvNetWorkConnection.push_back(connection);
            pIpAdapterInfoTmp = pIpAdapterInfoTmp->Next;
        }
TEST_END:
        if (pIpAdapterInfo)
        {
            free(pIpAdapterInfo);
            pIpAdapterInfo = NULL;
        }
        return mvNetWorkConnection;
    }

    bool CCJGW_AdapterCommonInfo::GetNetIfTableInfo()
    {
        if (!m_pIfTable)
        {
            m_dwSize = sizeof(MIB_IFTABLE);
            m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);
        }

        int res = GetIfTable(m_pIfTable, &m_dwSize, FALSE);
        if (ERROR_SUCCESS == res)
        {
            return true;
        }

        if (ERROR_INSUFFICIENT_BUFFER == res)	
        {
            free(m_pIfTable);
            m_pIfTable = (MIB_IFTABLE *)malloc(m_dwSize);	
        }

        return (ERROR_SUCCESS == GetIfTable(m_pIfTable, &m_dwSize, FALSE));
    }

    void CCJGW_AdapterCommonInfo::GetNetWorkTrafficInfoToDescription(const std::string& strAdapterDescription,S_NETWORK_TRAFFIC_INFO& sNetWorkTrafficInfo)
    {
        unsigned int index = -1;
        DWORD dwInPkts = 0,dwOutPkts = 0;
        sNetWorkTrafficInfo.mfRxDropRate = 0.00;
        sNetWorkTrafficInfo.mfTxDropRate = 0.00;
        sNetWorkTrafficInfo.mulInOctets = 0;
        sNetWorkTrafficInfo.mulOutOctets = 0;
        sNetWorkTrafficInfo.mulPkts = 0;
        sNetWorkTrafficInfo.mstrMac = "";
        sNetWorkTrafficInfo.mbConnectNet = false;

        if (!GetNetIfTableInfo()) return ;
        //! 查找网卡信息
        for (unsigned int i = 0; i < m_pIfTable->dwNumEntries; i++)
        {
            std::string descr = (const char*)m_pIfTable->table[i].bDescr;
            if (NULL == descr.compare(strAdapterDescription))
            {
                index = i;
                break;
            }
        }
        //! 当前网卡掉线
        if (-1 == index)
        {
            mulLastReceiver = 0;
            mulLastSent = 0;
            mulLastPps = 0;
            return ;
        }

        PMIB_IFROW pMibTmp = &m_pIfTable->table[index];
        //! 当前网络未连接
        sNetWorkTrafficInfo.mbConnectNet = (IF_OPER_STATUS_OPERATIONAL == pMibTmp->dwOperStatus);
        if (!sNetWorkTrafficInfo.mbConnectNet)
        {
            mulLastReceiver = 0;
            mulLastSent = 0;
            mulLastPps = 0;
            return ;
        }

        dwInPkts = pMibTmp->dwInUcastPkts + pMibTmp->dwInNUcastPkts;
        if (0 != dwInPkts)
            sNetWorkTrafficInfo.mfRxDropRate = (float)pMibTmp->dwInDiscards / (float)dwInPkts;

        dwOutPkts = pMibTmp->dwOutNUcastPkts + pMibTmp->dwOutUcastPkts;
        if (0 != dwOutPkts)
            sNetWorkTrafficInfo.mfTxDropRate = (float)pMibTmp->dwOutDiscards / (float)dwOutPkts;

        mulReceiver = pMibTmp->dwInOctets;
        mulSent = pMibTmp->dwOutOctets;
        mulPps = pMibTmp->dwInUcastPkts + pMibTmp->dwOutUcastPkts;

        //如果发送和接收的字节数为0或上次发送和接收的字节数为0或当前连接已改变时，网速无效
        if ((0 == mulReceiver == 0 && 0 == mulSent) || (0 == mulLastReceiver && /*0 ==*/ mulLastSent))
        {
            mulSent = 0;
            mulReceiver = 0;
            mulPps = 0;
            //             mulLastReceiver = 0;
            //             mulLastSent = 0;
        }
        else
        {
            sNetWorkTrafficInfo.mulInOctets = static_cast<unsigned int>(mulReceiver - mulLastReceiver);
            sNetWorkTrafficInfo.mulOutOctets = static_cast<unsigned int>(mulSent - mulLastSent);
            sNetWorkTrafficInfo.mulPkts = static_cast<unsigned int>(mulPps - mulLastPps); 
        }

        //! 如果上次接收发送为0 则表示开始重新计数
        if (0 == mulLastSent && 0 == mulLastReceiver && 0 == mulLastPps)
        {
            sNetWorkTrafficInfo.mulInOctets = 0;
            sNetWorkTrafficInfo.mulOutOctets = 0;
            sNetWorkTrafficInfo.mulPkts = 0;
        }

        mulLastReceiver = mulReceiver;
        mulLastSent = mulSent;
        mulLastPps = mulPps;

        //如果大于1GB/s，说明可能产生了异常，网速无效
        if (mulReceiver > 1073741824) mulReceiver = 0;
        if (mulSent > 1073741824) mulSent = 0;

        JGW_FormatString(sNetWorkTrafficInfo.mstrMac,"%02x:%02x:%02x:%02x:%02x:%02x",pMibTmp->bPhysAddr[0],pMibTmp->bPhysAddr[1],pMibTmp->bPhysAddr[2],pMibTmp->bPhysAddr[3],pMibTmp->bPhysAddr[4],pMibTmp->bPhysAddr[5]);
    }
}
