#pragma once
#include "CJGW_AsyncWndMessageHelp.h"

#include <string>
#include "PrsVendorRoamingDataDefine.h"
#include <vector>
#include <map>

namespace JGW
{
    typedef struct  
    {
        bool mbIsManual;
        int mnRssi;
        __int64 mdw64TimeStamp;
        std::string mstrMac;
    }S_RSSI_DATA;

    typedef struct  
    {
        bool mbIsManual;
        int mnMcs;
        __int64 mdw64TimeStamp;
        std::string mstrMac;
    }S_MCS_DATA;

    typedef struct  
    {
        bool mbIsManual;
        int mnTP;
        __int64 mdw64TimeStamp;
        std::string mstrMac;
    }S_TX_TP_DATA;

    typedef struct
    {
        bool mbIsManual;
        int mnStatus;
        __int64 mdw64TimeStamp;
        std::string mstrSTAMac;
        std::string mstrAPMac;
    }S_CONN_DATA;

	class CCJGW_PrsVendorRoamingDataHandle
	{
	public:
		CCJGW_PrsVendorRoamingDataHandle(void);
		~CCJGW_PrsVendorRoamingDataHandle(void);
	public:
        void SetAsyncWndMessageHelp(std::shared_ptr<CCJGW_AsyncWndMessageHelp> ptrAsyncWndMessageHelp);
        //!
        void OnHandlePrsVendorRoamingMessage(const std::string& strRoamingMessage);
        //! 
        void HandleNotificationHelper(int nClassID,int nServiceID,const std::string& strData);
		//! DRIVER_CLASS_ID
		void OnHandleDriverClass(int nServiceID,const std::string& strData);
		//! ROAMING_CLASS_ID
		void OnHandleRoamingClass(int nServiceID,const std::string& strData);
		//!  DEVICE_HOST_CLASS_ID
		void OnHandleDeviceHostClass(int nServiceID,const std::string& strData);
	private:
		/*######################### Handle Notification Functions #########################*/
		//! D:\Development\peraso\14.1.390\src\utilities\PrsVendorLib\common\include\prs_device_host_interface.h
		//! BEAM_FORMING_SERVICE => handle_beam_forming
		void OnHandleBeamFormingService(const std::string& strData);
		//! LINK_STATUS_SERVICE => handle_link_status
		void OnHandleLinkStatusService(const std::string& strData);
		//! NETWORK_DISCOVERY_REQUEST_SERVICE => handle_scan_request
		void OnHandleScanRequestService(const std::string& strData);
		//! NETWORK_DISCOVERY_RESULTS_SERVICE => handle_scan_results
		void OnHandleScanResultsService(const std::string& strData);
		//! NETWORK_DISCOVERY_COMPLETE_SERVICE => handle_scan_complete_request
		void OnHandleScanCompleteRequestService(const std::string& strData);
		//! NETWORK_CONNECT_REQUEST_SERVICE => handle_connect_request
		void OnHandleConnectRequestService(const std::string& strData);
		//! SCAN_SUMMARY_SERVICE => handle_scan_summary 总结
		void OnHandleScanSummaryService(const std::string& strData);
		//! PERF_STATS_SERVICE => handle_perf_stats
		void OnHandlePerfStatsService(const std::string& strData);
		//! STATION_COMP_SERVICE => handle_station_comp
		void OnHandleStationCompService(const std::string& strData);
		//! ROUTE_CHANGE_SERVICE => handle_route_change
		void OnHandleRouteChangeService(const std::string& strData);
		//! CONNECTION_CHANGE_SERVICE => handle_connection_change
		void OnHanleConnetionChangeService(const std::string& strData);
		//! THROUGHPUT_SERVICE => handle_total_throughput
		void OnHandleTotalThroughputService(const std::string& strData);

    private:
        void add_rssi(__int64 dw64TimeStamp,int nRssi,const std::string& strMac,bool bIsManual = false);
        void add_tx_tp(__int64 dw64TimeStamp,int tp,const std::string& strMac,bool bIsManual = false);
        void add_tp(__int64 dw64TimeStamp,int tp,const std::string& strMac,bool bIsManual = false);
        void add_conn_status(__int64 dw64TimeStamp,int status,const std::string& strSTAMac,const std::string& strAPMac,bool bIsManual = false);
        void add_scan_connect(__int64 dw64TimeStamp,int type,const std::string& strSTAMac,const std::string& strAPMac,bool bIsManual = false);
        void add_mcs(__int64 dw64TimeStamp,int nmcs,const std::string& strMac,bool bIsManual = false);
        void append_sta_data(__int64 dw64TimeStamp,int data,const std::string& strAPMac);

	private:
        __int64 GetTimeMilliSeconds(const std::string& strNotif,const std::string& strField);
		//1747121220269487 => 2025-05-15 16:32:22.763
		//std::string GetTimeStringToTimestamp(__int64 dw64TimeStamp,const std::string& strFormat = "%04d-%02d-%02d %02d:%02d:%02d.%03d");
        //std::wstring GetTimeStringToTimestampW(__int64 dw64TimeStamp,const std::wstring& strFormat = L"%04d-%02d-%02d %02d:%02d:%02d.%03d");

		//SYSTEMTIME GetSystemTimeToTimestamp(__int64 dw64TimeStamp);
		//! 获取指定字段的数值型64位数据
		__int64 GetNotifNumeric64Field(const std::string& strNotif,const std::string& strField);
		//! 获取指定字段的数值型数据
		int GetNotifNumericField(const std::string& strNotif,const std::string& strField);
        //! 获取指定字段的结构体数据
        std::string GetNotifStructArrayField(const std::string& strNotif,const std::string& strField);
        //! 获取指定字段的字符串数组类型数据[]
        std::string GetNotifStringArrayField(const std::string& strNotif,const std::string& strField);

		//! 获取指定字段所有的数值型数据
		std::vector<int> GetAllNotifNumericField(const std::string& strNotif,const std::string& strField);
		//! 将十进制MAC地址转化为16进制MAC地址  24, 214, 28, 161, 13, 246 => 18:d6:1c:a1:0d:f6
		std::string MacAddrDecToHexString(const std::string& strDecStr);
		//! 获取指定字段的MAC号段类型数据24, 214, 28, 161, 13, 246 => 18:d6:1c:a1:0d:f6
		std::string GetNotifMacAddr(const std::string& strNotif,const std::string& strField);
        //! 获取指定字段的原始MAC号段类型数据24, 214, 28, 161, 13, 246
        std::string GetNotifRawMacAddr(const std::string& strNotif,const std::string& strField);
		//! 获取指定字段的所有MAC号段类型数据并转换为18:d6:1c:a1:0d:f6格式
		std::vector<std::string> GetAllNotifMacAddr(const std::string& strNotif,const std::string& strField);
        //! 获取指定字段的所有原始MAC号段类型数据24, 214, 28, 161, 13, 246
        std::vector<std::string> GetAllNotifMacAddrRaw(const std::string& strNotif,const std::string& strField);
        //! ['20:ce:c4:02:a4:cc', '18:d6:1c:a0:8d:aa', '20:ce:c4:02:a4:ca', '18:d1:6c:a0:94:92', '7c:6c:f0:9f:24:0f', '20:ce:c4:02:a2:d0', '20:ce:c4:02:a2:d8']
        std::string ConvertListString(std::vector<std::string>& vstrValue);

        std::string ConvertListStringLParam(std::vector<std::string>& vstrValue);
        std::string ConvertListIntLParam(std::vector<int>& vstrValue);
		//! RSSI转换为信号质量
		int GetSignalQuality(int nRssi);
	private:
        std::shared_ptr<CCJGW_AsyncWndMessageHelp> mptrAsyncWndMessageHelp;
		std::string mstrRouteStation;
		S_NotifStats stats;
		std::map<std::string,std::string> mmapVIFSTAAddr;
		std::map<std::string,std::string> mmapApStaAddr;
        //! STA - AP
        std::map<std::string,std::string> mmapConnPair;
        //!
        std::vector<std::string> mvGroupColors;
    private:
        //! 
        std::map<std::string,std::vector<S_RSSI_DATA>> mmapStationMacRssiData;
        //! S_TX_TP_DATA
        std::map<std::string,std::vector<S_TX_TP_DATA>> mmapStationMacTxTPData;
        //! S_TX_TP_DATA
        std::map<std::string,std::vector<S_TX_TP_DATA>> mmapStationMacRxTPData;
        //! S_CONN_DATA
        std::map<std::string,std::vector<S_CONN_DATA>> mmapStationMacConnData;
        //! station mac 唯一键值
        std::map<std::string,std::vector<S_MCS_DATA>> mmapStationMacMcsData;
	};

}

