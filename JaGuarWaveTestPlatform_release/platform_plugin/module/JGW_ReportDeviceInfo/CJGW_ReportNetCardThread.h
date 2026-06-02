#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include <JGW_ReportDeviceInfo/CJGW_ReportDeviceInfoDefine.h>
#include <vector>
#include <iphlpapi.h>
#include "CJGW_NetCardSortConfig.h"
namespace JGW
{
    class CCJGW_ReportNetCardThread : public CCMessageThread
    {
    public:
        CCJGW_ReportNetCardThread(void);
        ~CCJGW_ReportNetCardThread(void);
    public:
        //! 
        void SetIntervalTimeMS(int nIntervalTimeMS = 200);
        //! 
        bool RegNetCardDeviceMsg(HWND hWindow,const std::wstring& strIPAddressSegment  = L"192.168.1.");
        // 
        bool RegNetCardDeviceThreadMsg(HANDLE hThread,const std::wstring& strIPAddressSegment  = L"192.168.1.");
        //! 
        void RemoveNetCardDeviceMsg(HWND hWindow);
        //! 
        void ClearNetCardSortConfig();
    private:
        virtual void RunThread();
    private:
        //! 获取网络适配器信息
        bool GetAdapterInfo();
        //!
        bool InitNetCardParamToAdapterInfo();
        //! 
        bool GetIfTableInfo();
        //!
        PS_NETCARD_INFO GetNetCardInfoToDescription(const std::wstring& strDescription);
        //!
        bool IsPostNetCardInfo(const std::wstring& strDescription);
        //!
        bool CheckIPAddressRangeToAddress(const std::wstring& strAddress);
        //!
        void SendNetCardChangeMsgToHwnd(PS_NETCARD_INFO psNetCardInfo);
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        //! net ptr
        size_t mnIpAdapterInfoMemSize;
        PIP_ADAPTER_INFO mpIpAdapterInfo;

        size_t mnIfTableMemSize;
        MIB_IFTABLE* mpIfTable;

        std::vector<PS_NETCARD_INFO> mvpsNetCardInfo;
        std::vector<PS_REG_NETCARD_MSG> mpvsRegNetCardMsg;
        CCJGW_CriticalSectionLock mCriticalSectionLock;
        CCJGW_NetCardSortConfig mcNetCardSortConfig;
    };

}

