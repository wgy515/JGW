#pragma once
//! 上报当前设备状态信息线程
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <JGW_WindowsFuncPlugin/CJGW_AdapterCommonInfo.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "json/CJGW_RepStatusJson.h"
#include "CJGW_StationMonitorDefine.h"

namespace JGW
{
    class CCJGW_ReportStationStatusThread : public CCMessageThread
    {
    public:
        CCJGW_ReportStationStatusThread(std::string& strStationServerAddress);
        ~CCJGW_ReportStationStatusThread(void);

    private:
        void OnReportStationStatus(WPARAM wParam,LPARAM lParam);
    private:
        //!
        void InitStationMonitorUIInfo();
        //!
        bool OpenPerasoComPort();
        //! 检查Peraso网卡设备是否已经连接
        bool CheckPerasoWiFiIsConnected();
        //! 获取peraso网卡连接的IP地址，并获取网关地址
        bool GetPerasoWiFiIPAddress();
        //! 检查网卡与网关的连接是否正常  ping peraso 网关
        bool CheckPingPerasoWiFiGateway();
        //! 获取dongles 状态信息
        bool GetPerasoStatusInfo();
        //!
        bool ReportStationDeviceInfo();

    protected:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbGateWayServer; //! 是否是网关服务器，默认为true
        int mnWEBRequestErrorCount;
        PS_STATION_MONITOR_UI_INFO mpsStationMonitorUiInfo;
        CCJGW_AsyncWndMessage* mpAsyncWndMessage;
        std::string mstrContentType;
        std::string& mstrStationServerAddress;
        
        CCJGW_CSerialComPort mCSerialComPort;
        CCJGW_DonglesComPort mDonglesComPort;
        CCJGW_RepStatusJson mRepStatusJson;
        CCJGW_AdapterCommonInfo mAdapterCommonInfo;
    };
}