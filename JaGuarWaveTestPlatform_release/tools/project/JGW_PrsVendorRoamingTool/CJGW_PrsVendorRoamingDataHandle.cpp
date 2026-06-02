#include "StdAfx.h"
#include "CJGW_PrsVendorRoamingDataHandle.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <regex>

#include "..\..\..\platform_include/JGW_MSG_ID_Define.h"

namespace JGW
{
	CCJGW_PrsVendorRoamingDataHandle::CCJGW_PrsVendorRoamingDataHandle(void)
	{
		memset(&stats,0x00,sizeof(S_NotifStats));
	}


	CCJGW_PrsVendorRoamingDataHandle::~CCJGW_PrsVendorRoamingDataHandle(void)
	{
	}

    void CCJGW_PrsVendorRoamingDataHandle::SetAsyncWndMessageHelp(std::shared_ptr<CCJGW_AsyncWndMessageHelp> ptrAsyncWndMessageHelp)
    {
        mptrAsyncWndMessageHelp = ptrAsyncWndMessageHelp;
    }

    void CCJGW_PrsVendorRoamingDataHandle::OnHandlePrsVendorRoamingMessage(const std::string& strRoamingMessage)
    {
        //! "classId": 13, "serviceId": 4, 
        if (0 == JGW_StrComparenoCaseStr(CONNECT_MSG,strRoamingMessage.c_str()) || 0 == JGW_StrComparenoCaseStr(DISCONNECT_MSG,strRoamingMessage.c_str()))
        {
            return;
        }
        int nClassID = 0,nServiceID = 0;
        //! "{field}": (-?\d+)
        const char* pClassID = JGW_StrCaseStr(strRoamingMessage.c_str(),"\"classId\":");
        if (NULL == pClassID)
        {
            return ;
        }
        nClassID = atoi(pClassID + strlen("\"classId\":"));

        const char* pServiceID = JGW_StrCaseStr(strRoamingMessage.c_str(),"\"serviceId\":");
        if (NULL == pServiceID)
        {
            return ;
        }
        nServiceID = atoi(pServiceID + strlen("\"serviceId\":"));

        try
        {
            HandleNotificationHelper(nClassID,nServiceID,strRoamingMessage.c_str());
        }
        catch (std::string& strError)
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : %s",JGW_A2W(strError).c_str());
        }
        catch (...)
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : unknown exception");
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::HandleNotificationHelper(int nClassID,int nServiceID,const std::string& strData)
    {
        if (DEVICE_HOST_CLASS_ID == nClassID)
        {
            OnHandleDeviceHostClass(nServiceID,strData);
        }
        else if (DRIVER_CLASS_ID == nClassID)
        {
            OnHandleDriverClass(nServiceID,strData);
        }
        else if (ROAMING_CLASS_ID == nClassID)
        {
            OnHandleRoamingClass(nServiceID,strData);
        }
        else
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : Unknown classId %d",nClassID);
        }
    }

	void CCJGW_PrsVendorRoamingDataHandle::OnHandleDriverClass(int nServiceID,const std::string& strData)
	{
		switch (nServiceID)
		{
		case LINK_STATUS_SERVICE:
			OnHandleLinkStatusService(strData);
			//handle_link_status(msg)
			stats.link_status += 1;
			break;
		case PERF_STATS_SERVICE:
			OnHandlePerfStatsService(strData);
			//! handle_perf_stats(msg)
			stats.perf_stat += 1;
			break;
		case PROBE_NETWORK_DISCOVERY_SERVICE:
			stats.probes += 1;
			break;
		case NETWORK_DISCOVERY_REQUEST_SERVICE:
			OnHandleScanRequestService(strData);
			//! handle_scan_request(msg)
			stats.scan_requests += 1;
			break;
		case NETWORK_CONNECT_REQUEST_SERVICE:
			OnHandleConnectRequestService(strData);
			//! handle_connect_request(msg)
			stats.connect_requests += 1;
			break;
		case NETWORK_DISCOVERY_COMPLETE_SERVICE:
			OnHandleScanCompleteRequestService(strData);
			//! handle_scan_complete_request(msg)
			stats.scan_complete += 1;
			break;
		case NETWORK_DISCOVERY_RESULTS_SERVICE:
			//! handle_scan_results(msg)
			OnHandleScanResultsService(strData);
			stats.scan_results += 1;
			break;
		default:
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : Unknown serviceId %d",nServiceID);
			break;
		}
	}

	void CCJGW_PrsVendorRoamingDataHandle::OnHandleRoamingClass(int nServiceID,const std::string& strData)
	{
		switch (nServiceID)
		{
		case STATION_COMP_SERVICE:
			OnHandleStationCompService(strData);
			//! handle_station_comp(msg)
			stats.sta_comp += 1;
			break;
		case ROUTE_CHANGE_SERVICE:
			//! handle_route_change(msg)
			OnHandleRouteChangeService(strData);
			stats.route_change += 1;
			break;
		case CONNECTION_CHANGE_SERVICE:
			//! handle_connection_change(msg)
			OnHanleConnetionChangeService(strData);
			stats.conn_change += 1;
			break;
		case THROUGHPUT_SERVICE:
			//! handle_total_throughput(msg)
			OnHandleTotalThroughputService(strData);
			stats.total_tp += 1;
			break;
		default:
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : Unknown serviceId %d",nServiceID);
			break;
		}
	}

	void CCJGW_PrsVendorRoamingDataHandle::OnHandleDeviceHostClass(int nServiceID,const std::string& strData)
	{
		switch (nServiceID)
		{
		case BEAM_FORMING_SERVICE:
			//! handle_beam_forming(msg)
			OnHandleBeamFormingService(strData);
			stats.beam_forming += 1;
			break;
		case SCAN_SUMMARY_SERVICE:
			//! handle_scan_summary(msg)
			OnHandleScanSummaryService(strData);
			stats.scan_summary += 1;
			break;
		case STATION_STATISTICS_INDICATION:
			stats.station_statistics +=1;
			break;
		case POWER_SAVE_TRANSITION_NOTIFICATION:
			stats.power_transition +=1;
			break;
        default:
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : Unknown serviceId %d - %s",nServiceID,JGW_A2W(strData).c_str());
            break;
		}
	}
	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121213144081, "sequence": 221, "classId": 2, "serviceId": 1, "operationId": 3, "version": 7, "payload": {"fragmentId": 0, "fragmentCount": 1, "peerSTAAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "resultCode": 0, "isInitiator": 1, "timeInUs": 2147930236, "reason": 8, "rxBeam": {"antenna": 0, "sector": 12}, "txBeam": {"antenna": 0, "sector": 12}, "peerTxBeam": {"antenna": 0, "sector": 2}, "brpRxBeam": {"antenna": 0, "sector": 12}, "localSnr": 79, "remoteSnr": -32768, "localRssi": -52, "debugDetails": 0, "elevation": -40, "azimuth": 0, "beamformingType": 3, "distanceToPeerEstimateInMeters": 45, "acceptableLink": 0, "slsInformationCount": 40, "slsInformation": [{"beam": {"antenna": 0, "sector": 0}, "snr": 22, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 1}, "snr": 127, "rssi": -67, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 2}, "snr": 410, "rssi": -57, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 3}, "snr": 353, "rssi": -58, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 4}, "snr": 63, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 5}, "snr": 8, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 6}, "snr": 32, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 7}, "snr": 19, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 8}, "snr": 109, "rssi": -69, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 9}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 10}, "snr": 180, "rssi": -62, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 11}, "snr": 16, "rssi": -75, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 12}, "snr": 3, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 13}, "snr": 5, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 14}, "snr": 57, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 15}, "snr": 367, "rssi": -59, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 16}, "snr": 62, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 17}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 18}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 19}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 20}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 21}, "snr": 198, "rssi": -63, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 22}, "snr": 157, "rssi": -65, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 23}, "snr": 6, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 24}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 25}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 26}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 27}, "snr": 61, "rssi": -75, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 28}, "snr": 271, "rssi": -59, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 29}, "snr": 49, "rssi": -69, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 30}, "snr": 5, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 31}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 32}, "snr": 2, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 33}, "snr": 66, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 34}, "snr": 384, "rssi": -57, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 35}, "snr": 139, "rssi": -64, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 36}, "snr": 6, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 37}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 38}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 39}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}], "brpInformation": {"ugain": 82, "beamInformationCount": 37, "beamInformation": [{"beam": {"antenna": 0, "sector": 0}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 1}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 2}, "snr": 5, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 3}, "snr": 32, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 4}, "snr": 20, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 5}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 6}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 7}, "snr": 81, "rssi": -71, "detail": 0}, {"beam": {"antenna": 0, "sector": 8}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 9}, "snr": 93, "rssi": -67, "detail": 0}, {"beam": {"antenna": 0, "sector": 10}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 11}, "snr": 284, "rssi": -57, "detail": 0}, {"beam": {"antenna": 0, "sector": 12}, "snr": 549, "rssi": -52, "detail": 0}, {"beam": {"antenna": 0, "sector": 13}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 14}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 15}, "snr": 3, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 16}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 17}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 18}, "snr": 7, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 19}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 20}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 21}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 22}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 23}, "snr": 188, "rssi": -60, "detail": 0}, {"beam": {"antenna": 0, "sector": 24}, "snr": 390, "rssi": -55, "detail": 0}, {"beam": {"antenna": 0, "sector": 25}, "snr": 55, "rssi": -71, "detail": 0}, {"beam": {"antenna": 0, "sector": 26}, "snr": 35, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 27}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 28}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 29}, "snr": 11, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 30}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 31}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 32}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 33}, "snr": 14, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 34}, "snr": 4, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 35}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 36}, "snr": 1, "rssi": -83, "detail": 0}]}}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleBeamFormingService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! The MAC address of the station that beamforming was executed with.
		//! 执行波束形成的电台的MAC地址。对端STA地址
		//! 32, 206, 196, 2, 162, 208
		std::string strAPMacAddr = GetNotifMacAddr(strData,"peerSTAAddress");
		int nRssi = GetNotifNumericField(strData,"localRssi");
		append_sta_data(dw64TimeReceived, nRssi, strAPMacAddr);
        /*
        "rxBeam": {"antenna": 0, "sector": 11}, "txBeam": {"antenna": 0, "sector": 11}
        "peerTxBeam": {"antenna": 0, "sector": 9}, "brpRxBeam": {"antenna": 0, "sector": 11}
        localSnr": 153, "remoteSnr": -32768, "localRssi": -39
        "distanceToPeerEstimateInMeters": 5,
        */
        int nLocalSnr = GetNotifNumericField(strData,"localSnr");
        std::string strTemp = GetNotifStructArrayField(strData,"rxBeam");
        int nRxBeamSector = GetNotifNumericField(strTemp,"sector");
        strTemp = GetNotifStructArrayField(strData,"txBeam");
        int nTxBeamSector = GetNotifNumericField(strTemp,"sector");
        int nDistanceMeters = GetNotifNumericField(strData,"distanceToPeerEstimateInMeters");
        std::string strApMacAddrRaw = GetNotifRawMacAddr(strData,"peerSTAAddress");

        //mptrAsyncWndMessageHelp->SendWindowMsg(L"BeamForming(%s) : [%s],rssi : %d,snr : %d,distance : %d,tx beam : %d,rx beam : %d",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strApMacAddrRaw).c_str(),nRssi,nLocalSnr,nDistanceMeters,nTxBeamSector,nRxBeamSector);
        //! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,BEAM_FORMING_SERVICE_WPARAM,L"%I64d,%s,%d,%d,%d,%d,%d",dw64TimeReceived,JGW_A2W(strAPMacAddr).c_str(),nRssi,nLocalSnr,nDistanceMeters,nTxBeamSector,nRxBeamSector);
	}
	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121739334421, "sequence": 0, "classId": 12, "serviceId": 4, "operationId": 3, "version": 1, "payload": {"txLinkSpeed": 2444335, "rxLinkSpeed": 3857421, "txMcs": 9, "rxMcs": 1, "connectionStatus": 2, "linkQuality": 76, "peerAddress": {"bytes": [32, 206, 196, 2, 162, 199]}, "localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleLinkStatusService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! AP 地址 STA对端地址 => AP地址
		std::string strPeerAddress = GetNotifMacAddr(strData,"peerAddress");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		int nConnStatus = GetNotifNumericField(strData,"connectionStatus");
		int nRxMcs = GetNotifNumericField(strData,"rxMcs");
		add_mcs(dw64TimeReceived, nRxMcs, strSTAMacAddr);
        //OutputDebugStringA(JGW_GetFormatString("add_mcs %s %d %s ",GetTimeStringToTimestamp(dw64TimeReceived).c_str(),rx_mcs,strSTAMacAddr.c_str()).c_str());
		if (nConnStatus == DISCONNECTED_STATUS)
		{
            add_mcs(dw64TimeReceived + 1, BAD_RX_MCS, strSTAMacAddr, true);
		}
		mmapApStaAddr[strPeerAddress] = strSTAMacAddr;
		add_conn_status(dw64TimeReceived, nConnStatus, strSTAMacAddr, strPeerAddress);

#if 1
        std::string strApMacAddrRaw = GetNotifRawMacAddr(strData,"peerAddress");
        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
        int nTxMcs = GetNotifNumericField(strData,"txMcs");
        if (nConnStatus == DISCONNECTED_STATUS)
        {
            nTxMcs = BAD_RX_MCS;
            nRxMcs = BAD_RX_MCS;
        }

        //mptrAsyncWndMessageHelp->SendWindowMsg(L"LinkStatusService(%s) : STA [%s] %s AP [%s],Tx Mcs : %d,Rx Mcs : %d",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str(),nConnStatus == DISCONNECTED_STATUS ? L"Disconnect" : L"Connected",JGW_A2W(strApMacAddrRaw).c_str(),nTxMcs,nRxMcs);
        //! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,LINK_STATUS_SERVICE_WPARAM,L"%I64d,%s,%d,%s,%d,%d",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str(),nConnStatus,JGW_A2W(strPeerAddress).c_str(),nTxMcs,nRxMcs);
#endif
	}
	//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121214036565, "sequence": 0, "classId": 12, "serviceId": 6, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "scanType": 0, "airTimeMinTU": 200, "airTimeMaxTU": 500, "ssidListCount": 1, "channelListSize": 6}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleScanRequestService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		//! scan type
		int nScanType = GetNotifNumericField(strData,"scanType");
		//! 最小调度时间粒度 Airtime调度通过统计用户实际占用无线信道时间，动态调整数据传输顺序，确保高速与低速用户公平共享信道资源
		int nAirTimeMinTU = GetNotifNumericField(strData,"airTimeMinTU");
		int	nAirTimeMaxTU = GetNotifNumericField(strData,"airTimeMaxTU");
        int nSsidListCount = 0;
        try
        {
            nSsidListCount = GetNotifNumericField(strData,"ssidListCount");
        }
        catch (std::string& strError)
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : %s",JGW_A2W(strError).c_str());
        }
        catch (...)
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Error : unknown exception");
        }
		add_scan_connect(dw64TimeReceived, SCAN_COMMAND, strSTAMacAddr,"");

        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"ScanRequest(%s) : [%s] scan",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str());
        //! NETWORK_DISCOVERY_REQUEST_SERVICE_WPARAM
        //! Time,STA MAC
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,NETWORK_DISCOVERY_REQUEST_SERVICE_WPARAM,L"%I64d,%s",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str());
	}
	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121216294671, "sequence": 0, "classId": 12, "serviceId": 9, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "apsListCount": 0, "apsList": [{"apAddress": {"bytes": [32, 206, 196, 2, 164, 202]}, "rssi": -38}, {"apAddress": {"bytes": [24, 214, 28, 160, 141, 170]}, "rssi": -128}, {"apAddress": {"bytes": [24, 209, 108, 160, 148, 146]}, "rssi": -128}, {"apAddress": {"bytes": [124, 108, 240, 159, 36, 15]}, "rssi": -58}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "rssi": -128}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 216]}, "rssi": -53}]}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleScanResultsService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		//! 扫描到的AP地址集合
		std::vector<std::string> vstrApAddrList = GetAllNotifMacAddr(strData,"apAddress");
        std::vector<int> vRssi = GetAllNotifNumericField(strData,"rssi");
        _ASSERT(vstrApAddrList.size() == vRssi.size());
        //! ['20:ce:c4:02:a4:cc', '18:d6:1c:a0:8d:aa', '20:ce:c4:02:a4:ca', '18:d1:6c:a0:94:92', '7c:6c:f0:9f:24:0f', '20:ce:c4:02:a2:d0', '20:ce:c4:02:a2:d8']
        std::string ap_addr_list = ConvertListString(vstrApAddrList);
		add_scan_connect(dw64TimeReceived, SCAN_COMPLETE,  strSTAMacAddr,ap_addr_list);

        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"ScanResults(%s) : [%s] - AP: %s scan complete",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str(),JGW_A2W(ap_addr_list).c_str());

        //! Time ,STA MAC,AP MAC LIST,RSSI LIST
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,NETWORK_DISCOVERY_RESULTS_SERVICE_WPARAM,L"%I64d,%s,%s,%s",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str(),JGW_A2W(ConvertListStringLParam(vstrApAddrList)).c_str(),JGW_A2W(ConvertListIntLParam(vRssi)).c_str());
	}

    std::string CCJGW_PrsVendorRoamingDataHandle::ConvertListStringLParam(std::vector<std::string>& vstrValue)
    {
        if (vstrValue.empty()) return "";

        std::string strTemp = JGW_GetFormatString("%s",vstrValue[0].c_str());
        for (size_t nIndex = 1;nIndex < vstrValue.size();nIndex++)
        {
            strTemp += JGW_GetFormatString("**%s",vstrValue[nIndex].c_str());
        }
        return strTemp;
    }

    std::string CCJGW_PrsVendorRoamingDataHandle::ConvertListIntLParam(std::vector<int>& vstrValue)
    {
        if (vstrValue.empty()) return "";

        std::string strTemp = JGW_GetFormatString("%d",vstrValue[0]);
        for (size_t nIndex = 1;nIndex < vstrValue.size();nIndex++)
        {
            strTemp += JGW_GetFormatString("**%d",vstrValue[nIndex]);
        }
        return strTemp;
    }

    std::string CCJGW_PrsVendorRoamingDataHandle::ConvertListString(std::vector<std::string>& vstrValue)
    {
        if (vstrValue.empty()) return "";

        std::string strTemp ("[");
        strTemp += JGW_GetFormatString("'%s'",vstrValue[0].c_str());
        for (size_t nIndex = 1;nIndex < vstrValue.size();nIndex++)
        {
            strTemp += JGW_GetFormatString(", '%s'",vstrValue[nIndex].c_str());
        }
        strTemp += "]";
        return strTemp;
    }
	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121216284453, "sequence": 0, "classId": 12, "serviceId": 8, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "status": 0}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleScanCompleteRequestService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		int	nStatus = GetNotifNumericField(strData,"status");

		add_scan_connect(dw64TimeReceived, SCAN_COMPLETE, strSTAMacAddr,"");

        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"ScanCompleteRequest(%s) : [%s] scan complete",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str());
        //! Time,STA MAC
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,NETWORK_DISCOVERY_COMPLETE_SERVICE_WPARAM,L"%I64d,%s",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str());
	}
	//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121215649292, "sequence": 0, "classId": 12, "serviceId": 7, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "bssid": {"bytes": [32, 206, 196, 2, 162, 216]}, "eAKMSuiteSelector": 0, "auth_type": 1, "hiddenNetwork": 0, "ssidCount": 5, "ssid": ["h", "v", "r", "_", "1"]}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleConnectRequestService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		//! 连接的AP地址
		std::string strbssid = GetNotifMacAddr(strData,"bssid");
		int nSsidListCount = GetNotifNumericField(strData,"ssidCount");
		int nAuth_type = GetNotifNumericField(strData,"auth_type");

        //mptrAsyncWndMessageHelp->SendWindowMsg(L"[WARNING] add_scan_connect to bssid: [%s]",JGW_A2W(strbssid).c_str());
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"[WARNING] add_scan_connect to sta address: [%s]",JGW_A2W(strSTAMacAddr).c_str());
	
		add_scan_connect(dw64TimeReceived, JOIN_COMMAND,  strSTAMacAddr,strbssid);

        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
        std::string strbssidRaw = GetNotifRawMacAddr(strData,"bssid");
        std::string strSsid = GetNotifStringArrayField(strData,"ssid");
#if 0
		JGW_ReplaceStringA(strSsid,"\", \"","");
		JGW_ReplaceStringA(strSsid,"\"","");
#else
		std::vector<std::string> vstrTemp;
		JGW_ParserStrA(strSsid.c_str(),", ",vstrTemp);
		strSsid = "";
		for (std::vector<std::string>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
		{
			JGW_EraseLastAndFristTwoChars(it[0],'\"','\"');
			strSsid += it[0];
		}
#endif
		//JGW_ReplaceStringA(strSsid,L",",L"");
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"ConnectRequest(%s) : STA [%s] - AP [%s] ssid:%s Join",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str(),JGW_A2W(strbssidRaw).c_str(),JGW_A2W(strSsid).c_str());

        //! Time,STA MAC,AP MAC,SSID
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,NETWORK_CONNECT_REQUEST_SERVICE_WPARAM,L"%I64d,%s,%s,%s",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str(),JGW_A2W(strbssid).c_str(),JGW_A2W(strSsid).c_str());
	}
	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121213051980, "sequence": 219, "classId": 2, "serviceId": 25, "operationId": 3, "version": 1, "payload": {"probedApsListCount": 6, "probedApsList": [{"apAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "rssi": -53, "snr": 10, "distance": 45, "acceptableLink": 1}, {"apAddress": {"bytes": [24, 214, 28, 160, 141, 170]}, "rssi": -65, "snr": 5, "distance": 37, "acceptableLink": 0}, {"apAddress": {"bytes": [32, 206, 196, 2, 164, 202]}, "rssi": -45, "snr": 15, "distance": 9, "acceptableLink": 1}, {"apAddress": {"bytes": [32, 206, 196, 2, 164, 204]}, "rssi": -70, "snr": -3, "distance": 43, "acceptableLink": 0}, {"apAddress": {"bytes": [24, 209, 108, 160, 148, 146]}, "rssi": -53, "snr": 9, "distance": 44, "acceptableLink": 0}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 216]}, "rssi": -63, "snr": 10, "distance": 9, "acceptableLink": 1}]}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleScanSummaryService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! 扫描到的AP地址集合
		std::vector<std::string> vstrApAddrList = GetAllNotifMacAddr(strData,"apAddress");
		//! 扫描到的AP对应RSSI集合
		std::vector<int> vnRssi = GetAllNotifNumericField(strData,"rssi");
		_ASSERT(vstrApAddrList.size() == vnRssi.size());

        for (size_t nIndex = 0;nIndex < vstrApAddrList.size();nIndex++)
        {
            append_sta_data(dw64TimeReceived, vnRssi[nIndex], vstrApAddrList[nIndex]);
        }

        std::vector<int> vnSnr = GetAllNotifNumericField(strData,"snr");
        _ASSERT(vstrApAddrList.size() == vnSnr.size());
        std::vector<int> vnDistance = GetAllNotifNumericField(strData,"distance");
        _ASSERT(vstrApAddrList.size() == vnDistance.size());

        std::vector<std::string> vstrApAddrListRaw = GetAllNotifMacAddrRaw(strData,"apAddress");
        _ASSERT(vstrApAddrListRaw.size() == vnDistance.size());
        for (size_t nIndex = 0;nIndex < vstrApAddrList.size();nIndex++)
        {
            //mptrAsyncWndMessageHelp->SendWindowMsg(L"ScanSummary(%s) : AP [%s] rssi: %d, snr: %d, distance: %d",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(vstrApAddrListRaw[nIndex]).c_str(),vnRssi[nIndex],vnSnr[nIndex],vnDistance[nIndex]);
            //! Time,AP MAC,RSSI,SNR,DISTANCE
            mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,SCAN_SUMMARY_SERVICE_WPARAM,L"%I64d,%s,%d,%d,%d",dw64TimeReceived,JGW_A2W(vstrApAddrList[nIndex]).c_str(),vnRssi[nIndex],vnSnr[nIndex],vnDistance[nIndex]);
        }
		/*
		time_received = get_time(msg, "timeReceived")
		ap_addr_list = get_all_notif_mac_addr(msg, "apAddress")
		rssi_list = get_all_notif_numeric_field(msg, "rssi")
		assert len(ap_addr_list) == len(rssi_list)
		for idx, ap in enumerate(ap_addr_list):
		append_sta_data(time_received, rssi_list[idx], ap)
		*/
	}

	//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121212142036, "sequence": 0, "classId": 12, "serviceId": 3, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "perfStatType": 1, "lastTimeStampMs": 1743118797890, "rxPayloadReceivedMbitPerSec": 0, "rxMsduProcessedMbitPerSec": 0, "rxIndicatedMbitPerSec": 0, "txTransferredMbitPerSec": 0}}
	//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121212151784, "sequence": 0, "classId": 12, "serviceId": 3, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "perfStatType": 0, "lastTimeStampMs": 1743118798829, "rxPayloadReceivedMbitPerSec": 0, "rxMsduProcessedMbitPerSec": 0, "rxIndicatedMbitPerSec": 0, "txTransferredMbitPerSec": 0}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandlePerfStatsService(const std::string& strData)
	{
		__int64 dw64TimeReceived = GetTimeMilliSeconds(strData,"timeReceived");
		//! STA 地址
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		//! 0 : Rx performance of the Peraso dongle 1 : Tx performance of the Peraso dongle
		int nPerfStatType = GetNotifNumericField(strData,"perfStatType");

		int nThroughputMbps = 0;

        std::string strSTAMacAddrRaw = GetNotifRawMacAddr(strData,"localAddress");
		if ((int)ePrsCore_PerfStatType_Rx == nPerfStatType)
		{
			nThroughputMbps = GetNotifNumericField(strData,"rxIndicatedMbitPerSec");
			add_tp(dw64TimeReceived, nThroughputMbps, strSTAMacAddr);
		}
		else if ((int)ePrsCore_PerfStatType_Tx == nPerfStatType)
		{
			nThroughputMbps = GetNotifNumericField(strData,"txTransferredMbitPerSec");
			add_tx_tp(dw64TimeReceived, nThroughputMbps, strSTAMacAddr);
		}
        //mptrAsyncWndMessageHelp->SendWindowMsg(L"PerfStats(%s) : [%s] %s Throughput Mbps : %d",GetTimeStringToTimestampW(dw64TimeReceived).c_str(),JGW_A2W(strSTAMacAddrRaw).c_str(),ePrsCore_PerfStatType_Rx == nPerfStatType ? L"RX" : L"TX",nThroughputMbps);

        //! Time,STA MAC,is Tx,ThroughputMbps
        mptrAsyncWndMessageHelp->PutAsyncWParamStringMessage(WM_PRS_VENDOR_DATA_HANDLE_MSG,PERF_STATS_SERVICE_WPARAM,L"%I64d,%s,%d,%d",dw64TimeReceived,JGW_A2W(strSTAMacAddr).c_str(),nPerfStatType,nThroughputMbps);

		/*
		time_received = get_time(msg, "timeReceived")

		sta_mac_addr = get_notif_mac_addr(msg, "localAddress")
		perf_stat_type = get_notif_numeric_field(msg, "perfStatType")
		if perf_stat_type == 0:
		# print(f"{Colors.DEBUG}[DEBUG] perf_stat_type RX : {perf_stat_type}{Colors.END}")
		throughput = get_notif_numeric_field(msg, "rxIndicatedMbitPerSec")
		add_tp(time_received, throughput, sta_mac_addr)
		elif perf_stat_type == 1: 
		txthroughput = get_notif_numeric_field(msg, "txTransferredMbitPerSec")
		# print(f"{Colors.DEBUG}[DEBUG] perf_stat_type : {perf_stat_type} throughput { throughput} {Colors.END}")
		add_tx_tp(time_received, txthroughput, sta_mac_addr)
		*/
	}

	//! {"type": "RoamingInterfaceV0", "timeSent": 1747121217229681, "classId": 13, "serviceId": 1, "payload": {"peerStaCount": 3, "comparedPeerStas": [{"peerStaMacAddr": {"bytes": [161, 13, 246, 0, 164, 202]}, "rssi": -38, "snr": 151, "distance": 5, "acceptableLink": 1}, {"peerStaMacAddr": {"bytes": [161, 13, 242, 0, 162, 208]}, "rssi": -128, "snr": 56, "distance": 46, "acceptableLink": 0}, {"peerStaMacAddr": {"bytes": [161, 13, 242, 0, 162, 216]}, "rssi": -128, "snr": -32768, "distance": 30000, "acceptableLink": 0}]}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleStationCompService(const std::string& strData)
	{
		__int64 dw64TimeSend = GetTimeMilliSeconds(strData,"timeSent");
		//! 扫描到的AP地址集合
		std::vector<std::string> vstrApAddrList = GetAllNotifMacAddr(strData,"peerStaMacAddr");
		//! 扫描到的AP对应RSSI集合
		std::vector<int> vnRssi = GetAllNotifNumericField(strData,"rssi");
        //! 
		_ASSERT(vstrApAddrList.size() == vnRssi.size());
        //! 
        for (size_t nIndex = 0;nIndex < vstrApAddrList.size();nIndex++)
        {
            append_sta_data(dw64TimeSend, vnRssi[nIndex], vstrApAddrList[nIndex]);
        }


        std::vector<int> vnSnr = GetAllNotifNumericField(strData,"snr");
        _ASSERT(vstrApAddrList.size() == vnSnr.size());
        std::vector<int> vnDistance = GetAllNotifNumericField(strData,"distance");
        _ASSERT(vstrApAddrList.size() == vnDistance.size());

        std::vector<std::string> vstrApAddrListRaw = GetAllNotifMacAddrRaw(strData,"peerStaMacAddr");
        _ASSERT(vstrApAddrListRaw.size() == vnDistance.size());
        for (size_t nIndex = 0;nIndex < vstrApAddrList.size();nIndex++)
        {
            //mptrAsyncWndMessageHelp->SendWindowMsg(L"RoamingStationComp(%s) : AP [%s] rssi: %d, snr: %d, distance: %d",GetTimeStringToTimestampW(dw64TimeSend).c_str(),JGW_A2W(vstrApAddrListRaw[nIndex]).c_str(),vnRssi[nIndex],vnSnr[nIndex],vnDistance[nIndex]);
        }

		//time_sent = get_time(msg, "timeSent")
		//ap_addr_list = get_all_notif_mac_addr(msg, "peerStaMacAddr")
		//rssi_list = get_all_notif_numeric_field(msg, "rssi")
		//assert len(ap_addr_list) == len(rssi_list)
		//for idx, ap in enumerate(ap_addr_list):
		//	append_sta_data(time_sent, rssi_list[idx], ap)
	}
	//! {"type": "RoamingInterfaceV0", "timeSent": 1747121217229774, "classId": 13, "serviceId": 2, "payload": {"vifMacAddr": {"bytes": [254, 214, 209, 16, 144, 225]}, "rssi": -38, "snr": 151, "acceptableLink": 1, "distance": 5}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleRouteChangeService(const std::string& strData)
	{
		__int64 dw64TimeSent = GetTimeMilliSeconds(strData,"timeSent");
		//! 网桥的虚拟网卡，一个11AD模块物理网卡可以对应8个虚拟网卡， FE:D6:D1:10:90:E1后三位表示连接的AP或者STA的后三位MAC
		//! 虚拟网卡可以匹配到真实物理IP => handle_connection_change
		std::string strVifMacAddr = GetNotifMacAddr(strData, "vifMacAddr");
		//! 布尔指示符，根据漫游（连接时扫描）指标配置的限制，当前是否认为这是一个可接受的链接。当固件不支持连接时扫描时，此字段将始终显示为true。
		//! Boolean indicator whether this is currently considered an acceptable link according to the roaming (Scan While Connected) metrics configured limits. When Scan While Connected is not supported by the firmware then this field will always indicate true.
		int nAcceptableLink = GetNotifNumericField(strData,"acceptableLink");
		//! trip  漫游虚拟网卡
		int nRssi = GetNotifNumericField(strData,"rssi");
		add_rssi(dw64TimeSent, nRssi, "trip");
		//! 如果未连接并且RSS = -128
		if (nRssi == MIN_RSSI && 0 == strVifMacAddr.compare(DISCONNECT_VIF))
		{
			//! routeManually insert data for other graphs when there is no route
			//! 当没有路由时，手动为其他图插入数据
            mstrRouteStation = "";
            for (std::map<std::string,std::vector<S_CONN_DATA>>::iterator it = mmapStationMacConnData.begin();it != mmapStationMacConnData.end();++it)
            {
                if (it->second[it->second.size() - 1].mnStatus == DISCONNECTED_STATUS)
                {
                    add_conn_status(dw64TimeSent,DISCONNECTED_STATUS,it->first,"",true);
                }
                std::map<std::string,std::vector<S_MCS_DATA>>::iterator itMcsData = mmapStationMacMcsData.find(it->first);
                if (itMcsData == mmapStationMacMcsData.end() || itMcsData->second[itMcsData->second.size() - 1].mnMcs == BAD_RX_MCS)
                {
                    add_mcs(dw64TimeSent,BAD_RX_MCS,it->first,true);
                }
                std::map<std::string,std::vector<S_RSSI_DATA>>::iterator itRssiData = mmapStationMacRssiData.find(it->first);
                if (itRssiData == mmapStationMacRssiData.end() || itRssiData->second[itRssiData->second.size() - 1].mnRssi == MIN_RSSI)
                {
                    add_rssi(dw64TimeSent,MIN_RSSI,it->first,true);
                }
                std::map<std::string,std::vector<S_TX_TP_DATA>>::iterator itTpData = mmapStationMacRxTPData.find(it->first);
                if (itTpData == mmapStationMacRxTPData.end() || itTpData->second[itTpData->second.size() - 1].mnTP < TP_DROP_THRESHOLD || dw64TimeSent - itTpData->second[itTpData->second.size() - 1].mdw64TimeStamp >= GRAPH_UPDATE_FREQ)
                {
                    add_tp(dw64TimeSent,0,it->first,true);
                }
                std::map<std::string,std::vector<S_TX_TP_DATA>>::iterator itTxTpData = mmapStationMacTxTPData.find(it->first);
                if (itTxTpData == mmapStationMacTxTPData.end() || itTxTpData->second[itTxTpData->second.size() - 1].mnTP < TP_DROP_THRESHOLD || dw64TimeSent - itTxTpData->second[itTxTpData->second.size() - 1].mdw64TimeStamp >= GRAPH_UPDATE_FREQ)
                {
                    add_tx_tp(dw64TimeSent,0,it->first,true);
                }
            }
		}
		//! 虚拟网卡地址在虚拟STA地址中并且处理链接请求状态
		std::map<std::string,std::string>::iterator it = mmapVIFSTAAddr.find(strVifMacAddr);
		if (mmapVIFSTAAddr.end() != it && LINK_ACCEPTED == nAcceptableLink)
		{
			mstrRouteStation = it->second;
			add_rssi(dw64TimeSent, nRssi, mstrRouteStation);
		}
		else
		{
			//! Reset route station to avoid incorrect data, but could miss some data
			//! 重置路由站，避免数据错误，但可能会丢失一些数据
			mstrRouteStation = "";
			if (0 != strVifMacAddr.compare(DISCONNECT_VIF))
				mptrAsyncWndMessageHelp->SendWindowMsg(L"[ERROR] Could not find station associated with VIF: [%s]. Could miss data.",JGW_A2W(strVifMacAddr).c_str());
		}
	}
	//! ,{"type": "RoamingInterfaceV0", "timeSent": 1747121215133676, "classId": 13, "serviceId": 3, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "peerAddress": {"bytes": [161, 13, 246, 0, 164, 202]}, "vifAddress": {"bytes": [254, 214, 209, 16, 144, 225]}}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHanleConnetionChangeService(const std::string& strData)
	{
		std::string strSTAMacAddr = GetNotifMacAddr(strData,"localAddress");
		std::string strVIFMacAddr = GetNotifMacAddr(strData,"vifAddress");
		if (0 != strVIFMacAddr.compare(DISCONNECT_VIF))
			mmapVIFSTAAddr[strVIFMacAddr] = strSTAMacAddr;
	}

	//! {"type": "RoamingInterfaceV0", "timeSent": 1747121219768542, "classId": 13, "serviceId": 4, "payload": {"throughput": 0}}
	void CCJGW_PrsVendorRoamingDataHandle::OnHandleTotalThroughputService(const std::string& strData)
	{
		__int64 dw64TimeSent = GetTimeMilliSeconds(strData,"timeSent");
		int mnThroughputMbps = GetNotifNumericField(strData,"throughput");
		add_tp(dw64TimeSent, mnThroughputMbps, "trip");
	}


    /*######################### Add Data Functions #########################*/
    void CCJGW_PrsVendorRoamingDataHandle::add_rssi(__int64 dw64TimeStamp,int nRssi,const std::string& strMac,bool bIsManual /* = false */)
    {
        //! 在当前RSSI数据集合中未找到当前MAC对应的RSSI数据
        //! 只添加最新数据
        std::map<std::string,std::vector<S_RSSI_DATA>>::iterator it = mmapStationMacRssiData.find(strMac);
        if (mmapStationMacRssiData.end() == it)
        {
            S_RSSI_DATA sNewData = {bIsManual,nRssi,dw64TimeStamp,strMac};
            mmapStationMacRssiData[strMac].push_back(sNewData);
        }
        else if (dw64TimeStamp > it->second[it->second.size() -1].mdw64TimeStamp)
        {
            std::vector<S_RSSI_DATA> vsRssiData;
            // # Manually insert a rssi data to avoid connecting diagonal lines
            //# isManual = false 以及group != 'trip' 以及RSSI != MIN_RSSI以及最后一行RSSI == MIN_RSSI
            if (!bIsManual && (0 != strMac.compare("trip")) && nRssi != MIN_RSSI && it->second[it->second.size() - 1].mnRssi == MIN_RSSI)
            {
                S_RSSI_DATA sNewData1 = {true,MIN_RSSI,dw64TimeStamp - MANUAL_INSERT_DP_INTERVAL,strMac};
                vsRssiData.push_back(sNewData1);
                S_RSSI_DATA sNewData2 = {bIsManual,nRssi,dw64TimeStamp,strMac};
                vsRssiData.push_back(sNewData2);
            }
            else
            {
                S_RSSI_DATA sTempData = {bIsManual,nRssi,dw64TimeStamp,strMac};
                vsRssiData.push_back(sTempData);
            }
            for (std::vector<S_RSSI_DATA>::iterator it = vsRssiData.begin();it != vsRssiData.end();++it)
            {
                mmapStationMacRssiData[it->mstrMac].push_back(it[0]);
            }   
        }
        else
        {
            //mptrAsyncWndMessageHelp->SendWindowMsg(L"[WARNING] Ignoring out of date notification for rssi data");
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::add_tx_tp(__int64 dw64TimeStamp,int tp,const std::string& strMac,bool bIsManual /* = false */)
    {
        __int64 last_time = 0;
        std::map<std::string,std::vector<S_TX_TP_DATA>>::iterator it = mmapStationMacTxTPData.find(strMac);
        if (it != mmapStationMacTxTPData.end())
        {
            last_time = it->second[it->second.size() - 1].mdw64TimeStamp;
        }

        if (it == mmapStationMacTxTPData.end())
        {
            S_TX_TP_DATA sNewData = {bIsManual,tp,dw64TimeStamp,strMac};
            mmapStationMacTxTPData[strMac].push_back(sNewData);
        }
        else if (dw64TimeStamp > last_time)
        {
            std::vector<S_TX_TP_DATA> vsTxTPData;
            // # Insert datapoint to reflect actual situation of dropping throughput
            //! 插入数据点以反映吞吐量下降的实际情况
            if (tp < TP_DROP_THRESHOLD && it->second[it->second.size() - 1].mnTP > TP_DROP_THRESHOLD)
            {
                S_TX_TP_DATA sNewData1 = {true,tp,dw64TimeStamp + MANUAL_INSERT_DP_INTERVAL,strMac};
                vsTxTPData.push_back(sNewData1);
                S_TX_TP_DATA sNewData2 = {bIsManual,tp,dw64TimeStamp,strMac};
                vsTxTPData.push_back(sNewData2);
            }
            else
            {
                S_TX_TP_DATA sNewData2 = {bIsManual,tp,dw64TimeStamp,strMac};
                vsTxTPData.push_back(sNewData2);
            }
            for (std::vector<S_TX_TP_DATA>::iterator it = vsTxTPData.begin();it != vsTxTPData.end();++it)
            {
                mmapStationMacTxTPData[it->mstrMac].push_back(it[0]);
            }   
        }
        else
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Ignoring out of date notification for throughput data");
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::add_tp(__int64 dw64TimeStamp,int tp,const std::string& strMac,bool bIsManual /* = false */)
    {
        __int64 last_time = 0;
        std::map<std::string,std::vector<S_TX_TP_DATA>>::iterator it = mmapStationMacRxTPData.find(strMac);
        if (it != mmapStationMacRxTPData.end())
        {
            last_time = it->second[it->second.size() - 1].mdw64TimeStamp;
        }
        
        if (it == mmapStationMacRxTPData.end())
        {
            S_TX_TP_DATA sNewData = {bIsManual,tp,dw64TimeStamp,strMac};
            mmapStationMacRxTPData[strMac].push_back(sNewData);
        }
        else if (dw64TimeStamp > last_time)
        {
            std::vector<S_TX_TP_DATA> vsTxTPData;
            // # Insert datapoint to reflect actual situation of dropping throughput
            //! 插入数据点以反映吞吐量下降的实际情况
            if (tp < TP_DROP_THRESHOLD && it->second[it->second.size() - 1].mnTP > TP_DROP_THRESHOLD)
            {
                S_TX_TP_DATA sNewData1 = {true,tp,dw64TimeStamp + MANUAL_INSERT_DP_INTERVAL,strMac};
                vsTxTPData.push_back(sNewData1);
                S_TX_TP_DATA sNewData2 = {bIsManual,tp,dw64TimeStamp,strMac};
                vsTxTPData.push_back(sNewData2);
            }
            else
            {
                S_TX_TP_DATA sNewData2 = {bIsManual,tp,dw64TimeStamp,strMac};
                vsTxTPData.push_back(sNewData2);
            }
            for (std::vector<S_TX_TP_DATA>::iterator it = vsTxTPData.begin();it != vsTxTPData.end();++it)
            {
                mmapStationMacRxTPData[it->mstrMac].push_back(it[0]);
            }   
        }
        else
        {
            mptrAsyncWndMessageHelp->SendWindowMsg(L"Ignoring out of date notification for throughput data");
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::add_conn_status(__int64 dw64TimeStamp,int status,const std::string& strSTAMac,const std::string& strAPMac,bool bIsManual /* = false */)
    {
        std::vector<std::string>::iterator itSTAMac = std::find_if(mvGroupColors.begin(),mvGroupColors.end(),[&](std::string sta_mac){ return 0 == sta_mac.compare(strSTAMac);});
        if (itSTAMac == mvGroupColors.end())
        {
            mvGroupColors.push_back(strSTAMac);
        }

        std::map<std::string,std::string>::iterator itConnPair = mmapConnPair.find(strSTAMac);
        if (itConnPair != mmapConnPair.end() && 0 != strAPMac.compare("") && 0 != itConnPair->second.compare(strAPMac))
        {
            mmapConnPair[strSTAMac] = strAPMac;
        }
        // # Add an offset to make the graph look cleaner 添加偏移量使图形看起来更干净
        if (0 == strSTAMac.compare(mvGroupColors[0]) && status == CONNECTED_STATUS)
        {
            status += 2;
        }
        std::vector<S_CONN_DATA> vsConnData;
        S_CONN_DATA sConnData = {bIsManual,status,dw64TimeStamp,strSTAMac,strAPMac};
        vsConnData.push_back(sConnData);
        //! mmapStationMacConnData
        //! Help fill additional data for disconnected stations only if current station is active
        //! 仅当当前站点处于活动状态时，帮助填充断开的站点的附加数据
        if (!bIsManual && status != DISCONNECTED_STATUS)
        {
            for (std::map<std::string,std::vector<S_CONN_DATA>>::iterator it = mmapStationMacConnData.begin();it != mmapStationMacConnData.end();++it)
            {
                if (0 != strSTAMac.compare(it->first) && it->second[it->second.size() - 1].mnStatus < CONNECTED_STATUS)
                {
                    S_CONN_DATA sTempConnData = {true,DISCONNECTED_STATUS,dw64TimeStamp,strSTAMac,""};
                    vsConnData.push_back(sTempConnData);
                }
            }
        }

        for (std::vector<S_CONN_DATA>::iterator it = vsConnData.begin();it != vsConnData.end();++it)
        {
            mmapStationMacConnData[it->mstrSTAMac].push_back(it[0]);
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::add_scan_connect(__int64 dw64TimeStamp,int type,const std::string& strSTAMac,const std::string& strAPMac,bool bIsManual /* = false */)
    {
        S_CONN_DATA sConnData = {bIsManual,type,dw64TimeStamp,strSTAMac,strAPMac};
        mmapStationMacConnData[sConnData.mstrSTAMac].push_back(sConnData);

        if (type == SCAN_COMPLETE)
        {
            //mptrAsyncWndMessageHelp->SendWindowMsg(L"[WARNING] scan complete: [%s]",JGW_A2W(strSTAMac).c_str());
        }
    }

    void CCJGW_PrsVendorRoamingDataHandle::add_mcs(__int64 dw64TimeStamp,int nmcs,const std::string& strMac,bool bIsManual /* = false */)
    {
        std::vector<S_MCS_DATA> vsMcsData;
        S_MCS_DATA sMcsData = {bIsManual,nmcs,dw64TimeStamp,strMac};
        vsMcsData.push_back(sMcsData);
        //! 仅当当前站点处于活动状态时，帮助填充断开的站点的附加数据
        if (!bIsManual && nmcs != BAD_RX_MCS)
        {
            
            //! 遍历MAC对应MCS数据信息
            for (std::map<std::string,std::vector<S_MCS_DATA>>::iterator it = mmapStationMacMcsData.begin();it != mmapStationMacMcsData.end();++it)
            {
                //! 检查MAC数据是否一致以及最后一个MCS是否是BAD_RX_MCS
                if (0 != it->first.compare(strMac) && it->second.size() >= 1 && it->second[it->second.size() - 1].mnMcs == BAD_RX_MCS)
                {
                    S_MCS_DATA sTempMcsData = {true,BAD_RX_MCS,dw64TimeStamp,it->first};
                    vsMcsData.push_back(sTempMcsData);
                }
            } 
        }
        for (std::vector<S_MCS_DATA>::iterator it = vsMcsData.begin();it != vsMcsData.end();++it)
        {
            mmapStationMacMcsData[it->mstrMac].push_back(it[0]);
        }   
    }

    void CCJGW_PrsVendorRoamingDataHandle::append_sta_data(__int64 dw64TimeStamp,int data,const std::string& strAPMac)
    {
        if (mmapApStaAddr.end() != mmapApStaAddr.find(strAPMac))
        {
            std::string strConnectedSta = mmapApStaAddr[strAPMac];
            add_rssi(dw64TimeStamp,data,strConnectedSta);
            if (0 == mstrRouteStation.compare(strConnectedSta))
            {
                add_rssi(dw64TimeStamp,data,"trip");
            }
        }
        else
        {
            //mptrAsyncWndMessageHelp->SendWindowMsg(L"[WARNING] No station is known to be connected to AP: [%s]",JGW_A2W(strAPMac).c_str());
        }
    }














#if 0
	SYSTEMTIME CCJGW_PrsVendorRoamingDataHandle::GetSystemTimeToTimestamp(__int64 dw64TimeStamp)
	{
		time_t t = (dw64TimeStamp / 1000000);
		SYSTEMTIME st = JGW_TimeT2SystemTime(t);
		st.wMilliseconds = (WORD)((dw64TimeStamp % 1000000) / 1000.0);
		return st;
	}

	std::string CCJGW_PrsVendorRoamingDataHandle::GetTimeStringToTimestamp(__int64 dw64TimeStamp,const std::string& strFormat /* = "%04d-%02d-%02d %02d:%02d:%02d.%03d" */)
	{
		SYSTEMTIME st = GetSystemTimeToTimestamp(dw64TimeStamp);
		return JGW_GetFormatString(strFormat.c_str(),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	}

    std::wstring CCJGW_PrsVendorRoamingDataHandle::GetTimeStringToTimestampW(__int64 dw64TimeStamp,const std::wstring& strFormat /* = L"%04d-%02d-%02d %02d:%02d:%02d.%03d" */)
    {
        SYSTEMTIME st = GetSystemTimeToTimestamp(dw64TimeStamp);
        return JGW_GetFormatWString(strFormat.c_str(),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
    }
#endif

    __int64 CCJGW_PrsVendorRoamingDataHandle::GetTimeMilliSeconds(const std::string& strNotif,const std::string& strField)
    {
        __int64 dw64TimeStamp = GetNotifNumeric64Field(strNotif,strField);
        if (dw64TimeStamp > raw_max_time_stamp)
        {
            return dw64TimeStamp / raw_msec_base_time;
        }
        return dw64TimeStamp / roaming_msec_base_time;
    }

	__int64 CCJGW_PrsVendorRoamingDataHandle::GetNotifNumeric64Field(const std::string& strNotif,const std::string& strField)
	{
		std::regex regex_patterns(JGW_GetFormatString("\"%s\": (-?\\d+)",strField.c_str()));

		std::smatch cm;
		std::regex_search(strNotif,cm, regex_patterns);
		if (0 == cm.size()) throw JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
		return _atoi64(((std::string)cm[1]).c_str());
	}
    //! 获取指定字段的字符串类型数据
    std::string CCJGW_PrsVendorRoamingDataHandle::GetNotifStringArrayField(const std::string& strNotif,const std::string& strField)
    {
        //! "\"%s\": \\[([\\d\\s,]+)\\]
        std::string strValue = JGW_GetFormatString("\"%s\": \\[([^\\[\\]]+)\\]",strField.c_str());
        std::regex regex_patterns(strValue);

        std::smatch cm;
        std::regex_search(strNotif,cm, regex_patterns);
        if (0 == cm.size()) throw JGW::JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
        return (std::string)cm[1];
    }
    //! 获取指定字段的字符串类型数据
    std::string CCJGW_PrsVendorRoamingDataHandle::GetNotifStructArrayField(const std::string& strNotif,const std::string& strField)
    {
        std::string strValue = JGW::JGW_GetFormatString("\"%s\": \\{([^\\{\\}]+)\\}",strField.c_str());
        //std::string strValue = JGW::JGW_GetFormatString("\"%s\": \\[\\{\"([^@]+)\\}\\],",strField.c_str());
        std::regex regex_patterns(strValue);

        std::smatch cm;
        std::regex_search(strNotif,cm, regex_patterns);
        if (0 == cm.size()) throw JGW::JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
        return (std::string)cm[1];
    }

	//! 获取指定字段的数值型数据
	int CCJGW_PrsVendorRoamingDataHandle::GetNotifNumericField(const std::string& strNotif,const std::string& strField)
	{
		std::regex regex_patterns(JGW_GetFormatString("\"%s\": (-?\\d+)",strField.c_str()));

		std::smatch cm;
		std::regex_search(strNotif,cm, regex_patterns);
		if (0 == cm.size()) throw JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
		return atoi(((std::string)cm[1]).c_str());
	}
	//! 获取指定字段所有的数值型数据
	std::vector<int> CCJGW_PrsVendorRoamingDataHandle::GetAllNotifNumericField(const std::string& strNotif,const std::string& strField)
	{
		std::regex regex_patterns(JGW_GetFormatString("\"%s\": (-?\\d+)",strField.c_str()));
		std::sregex_iterator next(strNotif.begin(), strNotif.end(), regex_patterns);
		std::sregex_iterator end;
		std::vector<std::string> matches;

		while (next != end) 
		{
			std::smatch match = *next;
			matches.push_back(match[1]);
			//matches.push_back(next->str());
			++next;
		}
		if (0 == matches.size()) throw JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());

		std::vector<int> vTemp;
		for (size_t nIndex = 0;nIndex < matches.size();nIndex++)
		{
			vTemp.push_back(atoi(matches[nIndex].c_str()));
		}
		return vTemp;
	}
	//! 将十进制MAC地址转化为16进制MAC地址  24, 214, 28, 161, 13, 246 => 18:d6:1c:a1:0d:f6
	std::string CCJGW_PrsVendorRoamingDataHandle::MacAddrDecToHexString(const std::string& strDecStr)
	{
		std::vector<std::string> vstrTemp;
		JGW_ParserStrA(strDecStr.c_str(),", ",vstrTemp);
		std::string strMac;
		for (std::vector<std::string>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
		{
			if (strMac.empty())
			{
				strMac = JGW_GetFormatString("%02x",atoi(it->c_str()));
			}
			else
			{
				strMac += JGW_GetFormatString(":%02x",atoi(it->c_str()));
			}
		}
		return strMac;
	}
    //! 获取指定字段的原始MAC号段类型数据24, 214, 28, 161, 13, 246
    std::string CCJGW_PrsVendorRoamingDataHandle::GetNotifRawMacAddr(const std::string& strNotif,const std::string& strField)
    {
        std::string strValue = JGW_GetFormatString("\"%s\": \\{\"bytes\": \\[([\\d\\s,]+)\\]\\}",strField.c_str());
        std::regex regex_patterns(strValue);

        std::smatch cm;
        std::regex_search(strNotif,cm, regex_patterns);
        if (0 == cm.size()) throw JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
        return cm[1];
    }
	//! 获取指定字段的MAC号段类型数据24, 214, 28, 161, 13, 246
	std::string CCJGW_PrsVendorRoamingDataHandle::GetNotifMacAddr(const std::string& strNotif,const std::string& strField)
	{
		std::string strValue = JGW_GetFormatString("\"%s\": \\{\"bytes\": \\[([\\d\\s,]+)\\]\\}",strField.c_str());
		std::regex regex_patterns(strValue);

		std::smatch cm;
		std::regex_search(strNotif,cm, regex_patterns);
		if (0 == cm.size()) throw JGW_GetFormatString("[ERROR] \"%s -------------------- %s\" field not found",strNotif.c_str(),strField.c_str());
		return MacAddrDecToHexString(cm[1]);
	}

    //! 获取指定字段的所有MAC号段类型数据并转换为18:d6:1c:a1:0d:f6格式
    std::vector<std::string> CCJGW_PrsVendorRoamingDataHandle::GetAllNotifMacAddrRaw(const std::string& strNotif,const std::string& strField)
    {
        std::string strValue = JGW_GetFormatString("\"%s\": \\{\"bytes\": \\[([\\d\\s,]+)\\]\\}",strField.c_str());
        std::regex regex_patterns(strValue);
        std::sregex_iterator next(strNotif.begin(), strNotif.end(), regex_patterns);
        std::sregex_iterator end;
        std::vector<std::string> matches;

        while (next != end) 
        {
            std::smatch match = *next;
            matches.push_back(match[1]);
            //matches.push_back(next->str());
            ++next;
        }
        if (0 == matches.size()) throw JGW_GetFormatString("[ERROR] \"%s\" field not found",strField.c_str());

        std::vector<std::string> vTemp;
        for (size_t nIndex = 0;nIndex < matches.size();nIndex++)
        {
            vTemp.push_back(matches[nIndex]);
        }
        return vTemp;
    }
	//! 获取指定字段的所有MAC号段类型数据并转换为18:d6:1c:a1:0d:f6格式
	std::vector<std::string> CCJGW_PrsVendorRoamingDataHandle::GetAllNotifMacAddr(const std::string& strNotif,const std::string& strField)
	{
		std::string strValue = JGW_GetFormatString("\"%s\": \\{\"bytes\": \\[([\\d\\s,]+)\\]\\}",strField.c_str());
		std::regex regex_patterns(strValue);
		std::sregex_iterator next(strNotif.begin(), strNotif.end(), regex_patterns);
		std::sregex_iterator end;
		std::vector<std::string> matches;

		while (next != end) 
		{
			std::smatch match = *next;
			matches.push_back(match[1]);
			//matches.push_back(next->str());
			++next;
		}
		if (0 == matches.size()) throw JGW_GetFormatString("[ERROR] \"%s\" field not found",strField.c_str());

		std::vector<std::string> vTemp;
		for (size_t nIndex = 0;nIndex < matches.size();nIndex++)
		{
			vTemp.push_back(MacAddrDecToHexString(matches[nIndex]));
		}
		return vTemp;
	}

	int CCJGW_PrsVendorRoamingDataHandle::GetSignalQuality(int nRssi)
	{
		if (nRssi > -52)
		{
			return 4;
		}
		else if (nRssi > -60)
		{
			return 3;
		}
		else if (nRssi > -63)
		{
			return 2;
		}
		else if (nRssi > -68)
		{
			return 1;
		}
		return 0;
	}

}
