#include "StdAfx.h"
#include "CJGW_ReportNetCardThread.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_ThreadFunc.h>
namespace JGW
{
    CCJGW_ReportNetCardThread::CCJGW_ReportNetCardThread(void)
    {
        m_nThreadSleepTimeInterval = (200);
        mnIpAdapterInfoMemSize = sizeof(IP_ADAPTER_INFO);
        mpIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,mnIpAdapterInfoMemSize);
        mnIfTableMemSize = sizeof(MIB_IFTABLE);
        mpIfTable = (MIB_IFTABLE*)calloc(1,mnIfTableMemSize);
    }


    CCJGW_ReportNetCardThread::~CCJGW_ReportNetCardThread(void)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        for (size_t i = 0;i < mpvsRegNetCardMsg.size();i ++)
        {
            delete mpvsRegNetCardMsg[i];
            mpvsRegNetCardMsg[i] = NULL;
        }
        mpvsRegNetCardMsg.clear();
        for (size_t i = 0;i < mvpsNetCardInfo.size();i++)
        {
            delete mvpsNetCardInfo[i];
            mvpsNetCardInfo[i] = NULL;
        }
        mvpsNetCardInfo.clear();
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_ReportNetCardThread, CCMessageThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_ReportNetCardThread::SetIntervalTimeMS(int nIntervalTimeMS /* = 200 */)
    {
        m_nThreadSleepTimeInterval = nIntervalTimeMS;
        mcNetCardSortConfig.LoadNetCardSortConfig(JGW_GetExecConfigFilePath(_T(NETCARD_SORT_CONFIG)));
    }

    void CCJGW_ReportNetCardThread::ClearNetCardSortConfig()
    {
        mcNetCardSortConfig.ClearNetCardSortConfig();
    }

    bool CCJGW_ReportNetCardThread::RegNetCardDeviceThreadMsg(HANDLE hThread,const std::wstring& strIPAddressSegment /* = L"192.168.1." */)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        PS_REG_NETCARD_MSG psRegNetCardMsg = new S_REG_NETCARD_MSG;
        psRegNetCardMsg->mhThreadID = GetThreadId(hThread);
        psRegNetCardMsg->mbIsWindwHwnd = false;
        psRegNetCardMsg->mstrIPAddressSegment = strIPAddressSegment;
        if (JGW_ThreadIsRunning(hThread))
        {
            Log4WD_F(L"Reg NetCard Device Thread Msg (%d-%s)",hThread,strIPAddressSegment.c_str());
            mpvsRegNetCardMsg.push_back(psRegNetCardMsg);
            return true;
        }
        Log4WI_F(L"Reg NetCard Device Msg Fail(%s)",strIPAddressSegment.c_str());
        return false;
    }

    bool CCJGW_ReportNetCardThread::RegNetCardDeviceMsg(HWND hWindow,const std::wstring& strIPAddressSegment /* = L"192.168.1." */)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        PS_REG_NETCARD_MSG psRegNetCardMsg = new S_REG_NETCARD_MSG;
        psRegNetCardMsg->mhWindow = hWindow;
        psRegNetCardMsg->mbIsWindwHwnd = true;
        psRegNetCardMsg->mstrIPAddressSegment = strIPAddressSegment;
        if (IsWindow(hWindow))
        {
            Log4WD_F(L"Reg NetCard Device Msg (%d-%s)",hWindow,strIPAddressSegment.c_str());
            mpvsRegNetCardMsg.push_back(psRegNetCardMsg);
            return true;
        }
        Log4WI_F(L"Reg NetCard Device Msg Fail(%s)",strIPAddressSegment.c_str());
        return false;
    }

    void CCJGW_ReportNetCardThread::RemoveNetCardDeviceMsg(HWND hWindow)
    {
        Log4WI_F(L"Remove NetCard Device Msg Window:%d",hWindow);
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        std::vector<PS_REG_NETCARD_MSG>::iterator it = mpvsRegNetCardMsg.begin();
        while (it != mpvsRegNetCardMsg.end())
        {
            if (it[0]->mhWindow == hWindow)
            {
                PS_REG_NETCARD_MSG psRegNetCardMsg = it[0];
                it = mpvsRegNetCardMsg.erase(it);
                delete psRegNetCardMsg;
            }
            else
            {
                ++it;
            }
        }
    }

    PS_NETCARD_INFO CCJGW_ReportNetCardThread::GetNetCardInfoToDescription(const std::wstring& strDescription)
    {
        //int nEmptyIndex = -1;
        for (size_t i = 0;i < mvpsNetCardInfo.size();i++)
        {
            //! 检查当前描述信息是否已更新
            if (0 == JGW_WStrComparenoCaseWStr(mvpsNetCardInfo[i]->mstrDescription.c_str(),strDescription.c_str()))
            {
                ////! 当前网卡还未解析完成，请等待当前网卡消息解析完成
                //if (mvpsNetCardInfo[i]->mReferenceCount.GetReferenceCount() > 0)
                //{
                //    return NULL;
                //}
                return mvpsNetCardInfo[i];
            }
            //if (mvpsNetCardInfo[i]->mReferenceCount.GetReferenceCount() <= 0)
            //{
            //    nEmptyIndex = i;
            //}
        }
        //! 查找到未处理过的网卡消息
        //if (nEmptyIndex >= 0)
        //{
        //    mvpsNetCardInfo[nEmptyIndex]->mstrDescription = strDescription;
        //    return mvpsNetCardInfo[nEmptyIndex];
        //}

        //if (mvpsNetCardInfo.size() >= MAX_NETCARD_DEFAULT_COUNT)
        //{
        //    mvpsNetCardInfo.erase(mvpsNetCardInfo.begin());
        //}

        PS_NETCARD_INFO psNetCardInfo = new S_NETCARD_INFO;
        psNetCardInfo->meDeviceType = E_NETCARD_TYPE;
        psNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_CONNECT;
        psNetCardInfo->mnSortIndex = 0;
        psNetCardInfo->mstrDescription = strDescription;  
        mvpsNetCardInfo.push_back(psNetCardInfo);
        Log4WD_F(L"GetNetCardInfoToDescription %d - %s",mvpsNetCardInfo.size(),strDescription.c_str());
        return psNetCardInfo;
    }

    void CCJGW_ReportNetCardThread::RunThread()
    {
        if (!GetAdapterInfo() || !GetIfTableInfo()) return ;

        if (!InitNetCardParamToAdapterInfo()) return ;
    }

    bool CCJGW_ReportNetCardThread::CheckIPAddressRangeToAddress(const std::wstring& strAddress)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        for (size_t i = 0;i < mpvsRegNetCardMsg.size();i ++)
        {
            if (std::wstring::npos != strAddress.find(mpvsRegNetCardMsg[i]->mstrIPAddressSegment))
            {
                return true;
            }
        }
        return false;
    }

    void CCJGW_ReportNetCardThread::SendNetCardChangeMsgToHwnd(PS_NETCARD_INFO psNetCardInfo)
    {
        CCJGW_CriticalSectionAutoLock criticalSectionAutoLock(mCriticalSectionLock);
        for (size_t i = 0;i < mpvsRegNetCardMsg.size();i ++)
        {
            if (std::wstring::npos != psNetCardInfo->mstrIPAddr.find(mpvsRegNetCardMsg[i]->mstrIPAddressSegment) && IsWindow(mpvsRegNetCardMsg[i]->mhWindow))
            {
                //psNetCardInfo->mReferenceCount.AddReferenceCount();
                Log4WI_F(L"ReportNetCardThread PostMessage %d - %s",mpvsRegNetCardMsg[i]->mhWindow,psNetCardInfo->mstrDescription.c_str());
                if (mpvsRegNetCardMsg[i]->mbIsWindwHwnd)
                {
                    if (FALSE == PostMessage(mpvsRegNetCardMsg[i]->mhWindow,WM_HWND_NETCARD_CHANGE_MSG,WPARAM(psNetCardInfo),NULL))
                    {
                        Log4WE_F(L"ReportNetCardThread PostMessage Fail %d - %s",mpvsRegNetCardMsg[i]->mhWindow,psNetCardInfo->mstrDescription.c_str());
                        //psNetCardInfo->mReferenceCount.DecReferenceCount();
                    }
                }
                else
                {
                    if (FALSE == JGW_PostThreadMsg(mpvsRegNetCardMsg[i]->mhThreadID,WM_HWND_NETCARD_CHANGE_MSG,WPARAM(psNetCardInfo),NULL))
                    {
                        Log4WE_F(L"ReportNetCardThread PostMessage Fail %d - %s",mpvsRegNetCardMsg[i]->mhThreadID,psNetCardInfo->mstrDescription.c_str());
                        //psNetCardInfo->mReferenceCount.DecReferenceCount();
                    }
                }           
            }
        }
    }

    bool CCJGW_ReportNetCardThread::IsPostNetCardInfo(const std::wstring& strDescription)
    {
        for (size_t i = 0;i < mvpsNetCardInfo.size();i++)
        {
            //! 检查当前描述信息是否已更新
            if (0 == JGW_WStrComparenoCaseWStr(mvpsNetCardInfo[i]->mstrDescription.c_str(),strDescription.c_str()))
            {
                return true;
            }
        }
        return false;
    }

    bool CCJGW_ReportNetCardThread::InitNetCardParamToAdapterInfo()
    {
#if 1
        PIP_ADAPTER_INFO pTempIpAdapterInfo = mpIpAdapterInfo;
        std::wstring strTempAddr,strTempDescription,strAdapterDescription ;
        while (pTempIpAdapterInfo)
        {
            //strTempAddr = JGW_A2W_A(pTempIpAdapterInfo->IpAddressList.IpAddress.String);
            strAdapterDescription = JGW_A2W_A(pTempIpAdapterInfo->Description);
            //! 读取静态IP配置以及动态IP配置
            strTempAddr = JGW_GetIPV4AddrToAdpaterName(JGW_A2W_A(pTempIpAdapterInfo->AdapterName).c_str());

            if (!CheckIPAddressRangeToAddress(strTempAddr) && !IsPostNetCardInfo(strAdapterDescription))
            {
                pTempIpAdapterInfo = pTempIpAdapterInfo->Next;
                continue;
            }
            for (unsigned int i = 0; i < mpIfTable->dwNumEntries; i++)
            {
                strTempDescription = JGW_A2W_A((const char*)mpIfTable->table[i].bDescr);
                //! 判断描述符是否一致
                if (0 != strTempDescription.compare(strAdapterDescription))
                {
                    continue;
                }
                PS_NETCARD_INFO psTempNetCardInfo = GetNetCardInfoToDescription(strAdapterDescription);
                //! 为空则表示当前网卡信息正在被刷新，请等待再次处理
                if (NULL == psTempNetCardInfo)
                {
                    Log4WW_F(L"Get NetCard Info Fail!!!(%s-%s)",strTempAddr.c_str(),strTempDescription.c_str());
                    continue;
                }
                psTempNetCardInfo->mstrIPAddr = strTempAddr;
                //! 判断当前适配器网络是否处于连接状态
                if (IF_OPER_STATUS_OPERATIONAL != mpIfTable->table[i].dwOperStatus)
                {
                    Log4WD_F(L"NetCard DisConnect(%s-%s)",psTempNetCardInfo->mstrIPAddr.c_str(),strTempDescription.c_str());
                    //! 连接状态不一致时发送网卡变更消息
                    if (psTempNetCardInfo->meNetCardPhysicalState == E_NETCARD_PHYSICAL_CONNECT)
                    {
                        psTempNetCardInfo->mstrDescription = strTempDescription;
                        psTempNetCardInfo->meDeviceType = E_NETCARD_TYPE;
                        psTempNetCardInfo->mnSortIndex = mcNetCardSortConfig.GetSortIndexToNetCardDescription(strTempDescription);
                        psTempNetCardInfo->mstrNetGUID = JGW_A2W_A(pTempIpAdapterInfo->AdapterName);
                        psTempNetCardInfo->mstrAdapterName = JGW_GetNetcardDeviceNameToAdapterName(psTempNetCardInfo->mstrNetGUID.c_str());
                        psTempNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_DISCONNECT;
                        SendNetCardChangeMsgToHwnd(psTempNetCardInfo);
                    }
                }
                else //! 适配器硬件已经连接
                {
                    Log4WD_F(L"NetCard Connect(%s-%s)",psTempNetCardInfo->mstrIPAddr.c_str(),strTempDescription.c_str());
                    //! 连接状态不一致时发送网卡变更消息
                    if (psTempNetCardInfo->meNetCardPhysicalState == E_NETCARD_PHYSICAL_DISCONNECT)
                    {
                        psTempNetCardInfo->mstrDescription = strTempDescription;
                        psTempNetCardInfo->meDeviceType = E_NETCARD_TYPE;
                        psTempNetCardInfo->mnSortIndex = mcNetCardSortConfig.GetSortIndexToNetCardDescription(strTempDescription);
                        psTempNetCardInfo->mstrNetGUID = JGW_A2W_A(pTempIpAdapterInfo->AdapterName);
                        psTempNetCardInfo->mstrAdapterName = JGW_GetNetcardDeviceNameToAdapterName(psTempNetCardInfo->mstrNetGUID.c_str());
                        psTempNetCardInfo->meNetCardPhysicalState = E_NETCARD_PHYSICAL_CONNECT;
                        SendNetCardChangeMsgToHwnd(psTempNetCardInfo);
                    }
                }
            }
            pTempIpAdapterInfo = pTempIpAdapterInfo->Next;
        }
#else
        PIP_ADAPTER_INFO pTempIpAdapterInfo = mpIpAdapterInfo;
        std::wstring strTempAddr,strTempDescription;

        while (pTempIpAdapterInfo)
        {
            strTempAddr = JGW_A2W_A(pTempIpAdapterInfo->IpAddressList.IpAddress.String);
            if (-1 == strTempAddr.find(msAPSMultiUpgradeConfig.mstrHostIPSeg)) { pTempIpAdapterInfo = pTempIpAdapterInfo->Next;continue;};

            strTempDescription = JGW_A2W_A(pTempIpAdapterInfo->Description);
            LPS_NET_CARD_PARAM psNetCardParam = GetNetCardParamStruct(strTempDescription);
            if (!psNetCardParam /*|| !psNetCardParam->mstrDescription.empty()*/) { pTempIpAdapterInfo = pTempIpAdapterInfo->Next;continue;};

            if (0 != psNetCardParam->mstrDescription.compare(strTempDescription))
            {
                psNetCardParam->mstrDescription = strTempDescription;
                mLanSortConfig.SaveLanSortConfig(psNetCardParam);
            }
            psNetCardParam->mstrAdapterName = JGW_A2W_A(pTempIpAdapterInfo->AdapterName);
            psNetCardParam->mstrNetName = JGW_GetNetcardDeviceNameToAdapterName(psNetCardParam->mstrAdapterName.c_str());
            psNetCardParam->mstrIPAddr = strTempAddr;
            if (psNetCardParam->mstrDescription.empty()) PDLOG4WW_F(L"AdapterInfo Description: %s,AdapterName: %s,IPAddr:%s",strTempDescription.c_str(),psNetCardParam->mstrAdapterName.c_str(),psNetCardParam->mstrIPAddr.c_str()); 
            psNetCardParam->mstrDescription = strTempDescription;
            pTempIpAdapterInfo = pTempIpAdapterInfo->Next;
        }

        for (unsigned int i = 0; i < mpIfTable->dwNumEntries; i++)
        {
            strTempDescription = JGW_A2W_A((const char*)mpIfTable->table[i].bDescr);
            for (int index = 0;index < DOWNLOAD_THREAD_MAX;index ++)
            {
                if (!msNetCardParam[index].mstrDescription.empty() && 0 == strTempDescription.compare(msNetCardParam[index].mstrDescription) && E_BUSY_DOWNLOAD != msNetCardParam[index].meDownloadStatus)
                {
                    //! 判断当前适配器网络是否处于连接状态
                    if (IF_OPER_STATUS_OPERATIONAL != mpIfTable->table[i].dwOperStatus)
                    {
                        if (ENETCARD_DISCONNCT_STATUS != msNetCardParam[index].meNetCardStatus) PDLOG4WW_F(L"AdapterInfo Description: %s,NetCardStatu: %s",strTempDescription.c_str(),L"DisConnect");
                        msNetCardParam[index].meNetCardStatus = ENETCARD_DISCONNCT_STATUS;
                        //! 记录第一次下载完成后的测试状态
                        if (msNetCardParam[index].mbIsFinishedDownloading)
                        {
                            msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus = ENETCARD_DISCONNCT_STATUS;
                            msNetCardParam[index].mbIsFinishedDownloading = false;
                        }
                        if (msNetCardParam[index].meDownloadStatus != E_IDLE_DOWNLOAD && msNetCardParam[index].meDownloadStatus != E_BUSY_DOWNLOAD && ENETCARD_CONNECT_STATUS == msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus)
                        {
                            if (msNetCardParam[index].meDownloadStatus == msNetCardParam[index].mePrevDownloadStatus)
                                msNetCardParam[index].meDownloadStatus = E_IDLE_DOWNLOAD;         
                        }
                        break;
                    }
                    else
                    {
                        //! 记录第一次下载完成后的测试状态
                        if (msNetCardParam[index].mbIsFinishedDownloading)
                        {
                            msNetCardParam[index].mePrvFinishedDownloadingNetCardStatus = ENETCARD_CONNECT_STATUS;
                            msNetCardParam[index].mbIsFinishedDownloading = false;
                        }
                        if (msNetCardParam[index].meDownloadStatus == E_IDLE_DOWNLOAD)
                        {
                            if (msNetCardParam[index].meDownloadStatus == msNetCardParam[index].mePrevDownloadStatus)
                                msNetCardParam[index].meDownloadStatus = E_INST_DOWNLOAD;
                        }
                    }
                    //! 更新状态的时候上报一次log
                    if (ENETCARD_CONNECT_STATUS != msNetCardParam[index].meNetCardStatus) PDLOG4WW_F(L"AdapterInfo Description: %s,NetCardStatu: %s",strTempDescription.c_str(),L"Connect");

                    //! if (msNetCardParam[index].mbNetCardStatusUpdateFlag) break;
                    if (ENETCARD_CONNECT_STATUS != msNetCardParam[index].meNetCardStatus) msNetCardParam[index].mbNetCardStatusUpdateFlag = true;
                    //! msNetCardParam[index].meNetCardStatus = ENETCARD_CONNECT_STATUS;

                    break;
                }
            }
        }
#endif
        return true;
    }

    bool CCJGW_ReportNetCardThread::GetAdapterInfo()
    {
        unsigned long stSize = mnIpAdapterInfoMemSize;		
        //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
        int nRel = GetAdaptersInfo(mpIpAdapterInfo, &stSize);	

        //! 如果函数返回的是ERROR_BUFFER_OVERFLOW
        //! 则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //! 这也是说明为什么stSize既是一个输入量也是一个输出量
        if (ERROR_BUFFER_OVERFLOW == nRel)
        {
            free(mpIpAdapterInfo);	//释放原来的内存空间
            //! 重新申请内存空间用来存储所有网卡信息
            mpIpAdapterInfo = (PIP_ADAPTER_INFO)calloc(1,stSize);
            mnIpAdapterInfoMemSize = stSize;
            //! 再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
            nRel = GetAdaptersInfo(mpIpAdapterInfo, &stSize);
        }

        return (ERROR_SUCCESS == nRel);
    }

    bool CCJGW_ReportNetCardThread::GetIfTableInfo()
    {
        unsigned long dwSize = mnIfTableMemSize;
        DWORD dwRel = GetIfTable(mpIfTable, &dwSize, FALSE);
        if (ERROR_INSUFFICIENT_BUFFER == dwRel)	
        {
            free(mpIfTable);
            mpIfTable = (MIB_IFTABLE *)calloc(1,dwSize);	
            dwRel = GetIfTable(mpIfTable, &dwSize, FALSE);
        }
        return (ERROR_SUCCESS == dwRel);
    }
}
