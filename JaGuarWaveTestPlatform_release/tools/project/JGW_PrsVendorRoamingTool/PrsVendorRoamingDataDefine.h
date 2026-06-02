#pragma once
#include <string>
#include <vector>
#include <map>
//! TCP => UDP => python3 roaming_graph.py
//! 接收缓冲区大小
#define TCP_RECEIVE_BUFFER_MAX_LENGTH (1024*10)
//! PRS VENDOR APP 转发SOCKET端口
#define PRS_VENDOR_APP_TCP_SOCKET_PORT 9010
//! PRS ROAMING GRAPH脚本接收信息的UDP SOCKET端口
#define PRS_ROAMING_GRAPH_UDP_SOCKET_PORT 9009
//! CEF3 Data UDP Port
#define CEF3_DATA_UDP_PORT 9002


//! python3 roaming_graph.py定义的最大接收缓冲区大小
#define PYTHON_ROAMING_GRAPH_PY_RECV_BUFFER_MAX_LENGTH (1024)

//! 每次定时器间隔增加多少次RICHEDIT MSG
#define MAX_APPEND_RICHEDIT_MSG 10
//! 更新prs roaming info的UI时间间隔 MS
#define UPDATE_PRS_ROAMING_INFO_UI_FREQ 50

//! Time,throughput rate(Mbits/sec),time delay(ms)
//! 每个一秒更新一次IPERF以及PING时延等信息
#define UPDATE_IPERF_PING_PRS_INFO_UI_FREQ 1000

//! 吞吐率
#define WM_THROUGHPUT_RATE_MSG WM_TEST_THREAD_TSE_INDEX_MSG_RUN
//! 时延
#define WM_TIME_DELAY_MSG WM_HWND_TSE_INDEX_MSG_RUN_COMPLETE

//! prs vendor roming data handle msg
#define WM_PRS_VENDOR_DATA_HANDLE_MSG WM_HWND_UPDATE_RSSI_INFO
//! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
#define BEAM_FORMING_SERVICE_WPARAM 1
//! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
#define LINK_STATUS_SERVICE_WPARAM 2
//! Time,STA MAC ===== SCAN
#define NETWORK_DISCOVERY_REQUEST_SERVICE_WPARAM 3
//! Time ,STA MAC,AP MAC LIST,RSSI LIST ==== scan complete
#define NETWORK_DISCOVERY_RESULTS_SERVICE_WPARAM 4
//! Time,STA MAC ==== scan complete
#define NETWORK_DISCOVERY_COMPLETE_SERVICE_WPARAM 5
//! Time,STA MAC,AP MAC,SSID ==== Join
#define NETWORK_CONNECT_REQUEST_SERVICE_WPARAM 6
//! Time,AP MAC,RSSI,SNR,DISTANCE
#define SCAN_SUMMARY_SERVICE_WPARAM 7
//! Time,STA MAC,is Tx,ThroughputMbps
#define PERF_STATS_SERVICE_WPARAM 8


/*************************************************/
#define DEVICE_HOST_CLASS_ID 2


//! handle_beam_forming => timeReceived peerSTAAddress localRssi
//! common\include\prs_device_host_interface.h SPrsBeamFormingIndication_V7
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121213144081, "sequence": 221, "classId": 2, "serviceId": 1, "operationId": 3, "version": 7, "payload": {"fragmentId": 0, "fragmentCount": 1, "peerSTAAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "resultCode": 0, "isInitiator": 1, "timeInUs": 2147930236, "reason": 8, "rxBeam": {"antenna": 0, "sector": 12}, "txBeam": {"antenna": 0, "sector": 12}, "peerTxBeam": {"antenna": 0, "sector": 2}, "brpRxBeam": {"antenna": 0, "sector": 12}, "localSnr": 79, "remoteSnr": -32768, "localRssi": -52, "debugDetails": 0, "elevation": -40, "azimuth": 0, "beamformingType": 3, "distanceToPeerEstimateInMeters": 45, "acceptableLink": 0, "slsInformationCount": 40, "slsInformation": [{"beam": {"antenna": 0, "sector": 0}, "snr": 22, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 1}, "snr": 127, "rssi": -67, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 2}, "snr": 410, "rssi": -57, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 3}, "snr": 353, "rssi": -58, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 4}, "snr": 63, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 5}, "snr": 8, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 6}, "snr": 32, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 7}, "snr": 19, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 8}, "snr": 109, "rssi": -69, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 9}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 10}, "snr": 180, "rssi": -62, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 11}, "snr": 16, "rssi": -75, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 12}, "snr": 3, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 13}, "snr": 5, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 14}, "snr": 57, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 15}, "snr": 367, "rssi": -59, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 16}, "snr": 62, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 17}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 18}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 19}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 20}, "snr": 1, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 21}, "snr": 198, "rssi": -63, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 22}, "snr": 157, "rssi": -65, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 23}, "snr": 6, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 24}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 25}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 26}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 27}, "snr": 61, "rssi": -75, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 28}, "snr": 271, "rssi": -59, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 29}, "snr": 49, "rssi": -69, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 30}, "snr": 5, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 31}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 32}, "snr": 2, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 33}, "snr": 66, "rssi": -71, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 34}, "snr": 384, "rssi": -57, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 35}, "snr": 139, "rssi": -64, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 36}, "snr": 6, "rssi": -81, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 37}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 38}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}, {"beam": {"antenna": 0, "sector": 39}, "snr": 0, "rssi": -87, "ugain": 89, "detail": 0}], "brpInformation": {"ugain": 82, "beamInformationCount": 37, "beamInformation": [{"beam": {"antenna": 0, "sector": 0}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 1}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 2}, "snr": 5, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 3}, "snr": 32, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 4}, "snr": 20, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 5}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 6}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 7}, "snr": 81, "rssi": -71, "detail": 0}, {"beam": {"antenna": 0, "sector": 8}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 9}, "snr": 93, "rssi": -67, "detail": 0}, {"beam": {"antenna": 0, "sector": 10}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 11}, "snr": 284, "rssi": -57, "detail": 0}, {"beam": {"antenna": 0, "sector": 12}, "snr": 549, "rssi": -52, "detail": 0}, {"beam": {"antenna": 0, "sector": 13}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 14}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 15}, "snr": 3, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 16}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 17}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 18}, "snr": 7, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 19}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 20}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 21}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 22}, "snr": 1, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 23}, "snr": 188, "rssi": -60, "detail": 0}, {"beam": {"antenna": 0, "sector": 24}, "snr": 390, "rssi": -55, "detail": 0}, {"beam": {"antenna": 0, "sector": 25}, "snr": 55, "rssi": -71, "detail": 0}, {"beam": {"antenna": 0, "sector": 26}, "snr": 35, "rssi": -77, "detail": 0}, {"beam": {"antenna": 0, "sector": 27}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 28}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 29}, "snr": 11, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 30}, "snr": 2, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 31}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 32}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 33}, "snr": 14, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 34}, "snr": 4, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 35}, "snr": 0, "rssi": -83, "detail": 0}, {"beam": {"antenna": 0, "sector": 36}, "snr": 1, "rssi": -83, "detail": 0}]}}}
#define BEAM_FORMING_SERVICE 1
//! handle_scan_summary => timeReceived apAddress rssi
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121213051980, "sequence": 219, "classId": 2, "serviceId": 25, "operationId": 3, "version": 1, "payload": {"probedApsListCount": 6, "probedApsList": [{"apAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "rssi": -53, "snr": 10, "distance": 45, "acceptableLink": 1}, {"apAddress": {"bytes": [24, 214, 28, 160, 141, 170]}, "rssi": -65, "snr": 5, "distance": 37, "acceptableLink": 0}, {"apAddress": {"bytes": [32, 206, 196, 2, 164, 202]}, "rssi": -45, "snr": 15, "distance": 9, "acceptableLink": 1}, {"apAddress": {"bytes": [32, 206, 196, 2, 164, 204]}, "rssi": -70, "snr": -3, "distance": 43, "acceptableLink": 0}, {"apAddress": {"bytes": [24, 209, 108, 160, 148, 146]}, "rssi": -53, "snr": 9, "distance": 44, "acceptableLink": 0}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 216]}, "rssi": -63, "snr": 10, "distance": 9, "acceptableLink": 1}]}
#define SCAN_SUMMARY_SERVICE 25
//! stats.station_statistics +=1 STA统计
//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121213509014, "sequence": 230, "classId": 2, "serviceId": 3, "operationId": 3, "version": 7, "payload": }
#define STATION_STATISTICS_INDICATION 3
//! stats.power_transition +=1
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121739272666, "sequence": 201, "classId": 2, "serviceId": 26, "operationId": 3, "version": 0, "payload": {"newState": 1, "isSelf": 1, "peerAddress": {"bytes": [0, 0, 0, 0, 0, 0]}}}
#define POWER_SAVE_TRANSITION_NOTIFICATION 26


/*************************************************/
#define DRIVER_CLASS_ID 12
//! handle_link_status => timeReceived peerAddress localAddress connectionStatus rxMcs
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121739334421, "sequence": 0, "classId": 12, "serviceId": 4, "operationId": 3, "version": 1, "payload": {"txLinkSpeed": 2444335, "rxLinkSpeed": 3857421, "txMcs": 9, "rxMcs": 1, "connectionStatus": 2, "linkQuality": 76, "peerAddress": {"bytes": [32, 206, 196, 2, 162, 199]}, "localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}}}
#define LINK_STATUS_SERVICE 4
//! handle_perf_stats => timeReceived localAddress perfStatType
/*
if perf_stat_type == 0:
# print(f"{Colors.DEBUG}[DEBUG] perf_stat_type RX : {perf_stat_type}{Colors.END}")
throughput = get_notif_numeric_field(msg, "rxIndicatedMbitPerSec")
add_tp(time_received, throughput, sta_mac_addr)
elif perf_stat_type == 1: 
txthroughput = get_notif_numeric_field(msg, "txTransferredMbitPerSec")
# print(f"{Colors.DEBUG}[DEBUG] perf_stat_type : {perf_stat_type} throughput { throughput} {Colors.END}")
add_tx_tp(time_received, txthroughput, sta_mac_addr)
*/
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121212142036, "sequence": 0, "classId": 12, "serviceId": 3, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "perfStatType": 1, "lastTimeStampMs": 1743118797890, "rxPayloadReceivedMbitPerSec": 0, "rxMsduProcessedMbitPerSec": 0, "rxIndicatedMbitPerSec": 0, "txTransferredMbitPerSec": 0}}
//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121212151784, "sequence": 0, "classId": 12, "serviceId": 3, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "perfStatType": 0, "lastTimeStampMs": 1743118798829, "rxPayloadReceivedMbitPerSec": 0, "rxMsduProcessedMbitPerSec": 0, "rxIndicatedMbitPerSec": 0, "txTransferredMbitPerSec": 0}}
#define PERF_STATS_SERVICE 3
//! stats.probes += 1
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121212203522, "sequence": 0, "classId": 12, "serviceId": 5, "operationId": 3, "version": 1, "payload": {"bssid": {"bytes": [24, 209, 108, 160, 148, 146]}, "bCashedEntry": 0, "bRsnaActivated": 0, "rssi ": 18446744073709551563, "channel": 3, "linkQuality": 70, "beaconInterval": 100, "tsf": 3, "ssid": ["G", "1"]}}
#define PROBE_NETWORK_DISCOVERY_SERVICE  5
//! handle_scan_request => timeReceived localAddress scanType airTimeMinTU airTimeMaxTU ssidListCount
//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121214036565, "sequence": 0, "classId": 12, "serviceId": 6, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "scanType": 0, "airTimeMinTU": 200, "airTimeMaxTU": 500, "ssidListCount": 1, "channelListSize": 6}}
#define NETWORK_DISCOVERY_REQUEST_SERVICE 6
//! thandle_connect_request => imeReceived localAddress bssid ssidCount auth_type
//! ,{"type": "DeviceHostInterfaceV2", "timeReceived": 1747121215649292, "sequence": 0, "classId": 12, "serviceId": 7, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 242]}, "bssid": {"bytes": [32, 206, 196, 2, 162, 216]}, "eAKMSuiteSelector": 0, "auth_type": 1, "hiddenNetwork": 0, "ssidCount": 5, "ssid": ["h", "v", "r", "_", "1"]}}
#define NETWORK_CONNECT_REQUEST_SERVICE 7
//! handle_scan_complete_request => timeReceived localAddress status
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121216284453, "sequence": 0, "classId": 12, "serviceId": 8, "operationId": 3, "version": 0, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "status": 0}}
#define NETWORK_DISCOVERY_COMPLETE_SERVICE 8
//! handle_scan_results => timeReceived localAddress apAddress
//! {"type": "DeviceHostInterfaceV2", "timeReceived": 1747121216294671, "sequence": 0, "classId": 12, "serviceId": 9, "operationId": 3, "version": 1, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "apsListCount": 0, "apsList": [{"apAddress": {"bytes": [32, 206, 196, 2, 164, 202]}, "rssi": -38}, {"apAddress": {"bytes": [24, 214, 28, 160, 141, 170]}, "rssi": -128}, {"apAddress": {"bytes": [24, 209, 108, 160, 148, 146]}, "rssi": -128}, {"apAddress": {"bytes": [124, 108, 240, 159, 36, 15]}, "rssi": -58}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 208]}, "rssi": -128}, {"apAddress": {"bytes": [32, 206, 196, 2, 162, 216]}, "rssi": -53}]}}
#define NETWORK_DISCOVERY_RESULTS_SERVICE 9


/************************************/
#define ROAMING_CLASS_ID 13
//! handle_station_comp => timeSent peerStaMacAddr rssi
//! {"type": "RoamingInterfaceV0", "timeSent": 1747121217229681, "classId": 13, "serviceId": 1, "payload": {"peerStaCount": 3, "comparedPeerStas": [{"peerStaMacAddr": {"bytes": [161, 13, 246, 0, 164, 202]}, "rssi": -38, "snr": 151, "distance": 5, "acceptableLink": 1}, {"peerStaMacAddr": {"bytes": [161, 13, 242, 0, 162, 208]}, "rssi": -128, "snr": 56, "distance": 46, "acceptableLink": 0}, {"peerStaMacAddr": {"bytes": [161, 13, 242, 0, 162, 216]}, "rssi": -128, "snr": -32768, "distance": 30000, "acceptableLink": 0}]}}
#define STATION_COMP_SERVICE 1
//! handle_route_change => timeSent vifMacAddr rssi acceptableLink
//! ,{"type": "RoamingInterfaceV0", "timeSent": 1747121217229774, "classId": 13, "serviceId": 2, "payload": {"vifMacAddr": {"bytes": [254, 214, 209, 16, 144, 225]}, "rssi": -38, "snr": 151, "acceptableLink": 1, "distance": 5}}
#define ROUTE_CHANGE_SERVICE 2
//! handle_connection_change => localAddress vifAddress
//! ,{"type": "RoamingInterfaceV0", "timeSent": 1747121219327206, "classId": 13, "serviceId": 3, "payload": {"localAddress": {"bytes": [24, 214, 28, 161, 13, 246]}, "peerAddress": {"bytes": [161, 13, 246, 0, 164, 202]}, "vifAddress": {"bytes": [254, 214, 209, 16, 144, 225]}}}
#define CONNECTION_CHANGE_SERVICE 3
//! handle_total_throughput => timeSent throughput
//! {"type": "RoamingInterfaceV0", "timeSent": 1747121219768542, "classId": 13, "serviceId": 4, "payload": {"throughput": 0}}
#define THROUGHPUT_SERVICE 4


#define CONNECTED_STATUS 2
#define DISCONNECTED_STATUS 0
#define LINK_ACCEPTED 1

#define SCAN_COMMAND 5
#define SCAN_COMPLETE 6
#define JOIN_COMMAND 7

#define MIN_RSSI -128
//! invalid (-32768).
#define INVAILD_SNR -32768


#define DISCONNECT_VIF "fe:ff:c4:00:00:00"
#define BAD_RX_MCS 0

#define DISCONNECT_MSG "Bye from PrsRoamingApp" //# From PrsVendorSocket.cpp in roaming app
#define CONNECT_MSG "Hello from PrsRoamingApp"

//! 差不多659MS,2秒上传一次此消息
//! "classId": 2, "serviceId": 25
//! 1747121213051980 2025-05-13 15:26:53.051 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE
//! 1747121213710123 2025-05-13 15:26:53.710 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE
//! 1747121215618440 2025-05-13 15:26:55.618 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE
//! 1747121216273835 2025-05-13 15:26:56.273 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE
//! 1747121218874563 2025-05-13 15:26:58.874 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE
//! 1747121219737751 2025-05-13 15:26:59.737 DEVICE_HOST_CLASS_ID -> SCAN_SUMMARY_SERVICE

//! "classId": 2, "serviceId": 1
//! 1747121219768549 2025-05-13 15:26:59.768 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219779210 2025-05-13 15:26:59.779 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219805932 2025-05-13 15:26:59.805 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219828304 2025-05-13 15:26:59.828 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219865519 2025-05-13 15:26:59.865 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219879011 2025-05-13 15:26:59.879 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219910171 2025-05-13 15:26:59.910 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219930587 2025-05-13 15:26:59.930 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE
//! 1747121219981475 2025-05-13 15:26:59.981 DEVICE_HOST_CLASS_ID -> BEAM_FORMING_SERVICE


//! 1747121213051980 +1000 增加一毫秒 => 1747121213052980
//! 1747121213 增加一秒 => 1747121214 
//! 1000000 为一秒
//! prs_vendor_app 脚本上传的时间比PrsRoamingApp时间多一位
//#define milliseconds_timestamep (10000)
//! PrsRoamingApp 脚本上传的时间毫米
#define prs_roaming_app_milliseconds_timestamep (1000)

#define raw_max_time_stamp 10000000000000000
#define raw_sec_base_time 10000000
#define roaming_sec_base_time 1000000
#define raw_msec_base_time 10000
#define roaming_msec_base_time 1000

//! 手动插入数据点的时间间隔
#define MANUAL_INSERT_DP_INTERVAL (3 ) //! # Time interval of manually inserted datapoints
//! 1.5 秒
#define GRAPH_UPDATE_FREQ (1500)
#define GRAPH_UPDATE_FREQ_TIMER_MSEC (1500)

#define TP_DROP_THRESHOLD 100



#define IPQ_LOG_FOLDER "/root"
#define IPQ_PROC_KMSG_COMMAND_FORMAT "cat /proc/kmsg > %s &\n"
#define IPQ_LOGREAD_COMMAND_FORMAT "logread -f > %s &\n"
#define IPQ_GET_CAT_PROC_KMSG_PID "ps |grep cat |grep /proc/kmsg\n"
#define IPQ_GET_LOGREAD_PID "ps |grep logread |grep \" logread -f\"\n"
//! 检查11AD是否已经连接
#define WM_11AD_CONNECT_STATUS_MSG WM_TEST_THREAD_RESET_TSE


//! rssi、throughput、snr、mcs、distance、associated time、sector
//! associated time 接入时间，与各个AP接入时间记录
namespace JGW
{
#if 0
	//! time,rssi,group,isManual
	//! 2025-05-15 16:32:22.924176,-40,trip,False
	typedef struct
	{
		//! 2025-05-15 16:32:22.924176
		std::string mstrTime;
		//! 
		int mnRssi;
		//! 18:d6:1c:a1:0d:f6
		std::string mstrGroup;
		//! isManual ? "False" : "True"
		std::string mstrManual;
	}S_RSSI_DATA;
	//! time,throughput,group,isManual
	//! 2025-05-15 16:32:22.762731,0,trip,False
	//! 2025-05-15 16:32:49.618907,0,18:d6:1c:a1:0d:f6,False
	typedef struct  
	{
		//! 2025-05-15 16:32:22.924176
		std::string mstrTime;
		int mnThroughputMbps;
		//! 18:d6:1c:a1:0d:f6
		std::string mstrGroup;
		//! isManual ? "False" : "True"
		std::string mstrManual;
	}S_Throughput_Data;
	//! time,status,group,peer,isManual
	//! 2025-05-15 16:32:22.838943,6,18:d6:1c:a1:0d:f6,,False
	//! 2025-05-15 16:32:22.849187,6,18:d6:1c:a1:0d:f6,"['18:d1:6c:a0:94:92', '20:ce:c4:02:a4:ca', '18:d6:1c:a0:8d:aa', '20:ce:c4:02:a2:d8', '7c:6c:f0:9f:24:0f']",False
	typedef struct  
	{
		//! 2025-05-15 16:32:22.924176
		std::string mstrTime;
		/*
		#define CONNECTED_STATUS 2
		#define DISCONNECTED_STATUS 0
		#define LINK_ACCEPTED 1

		#define SCAN_COMMAND 5
		#define SCAN_COMPLETE 6
		#define JOIN_COMMAND 7
		*/
		int mnStatus;
		//! 18:d6:1c:a1:0d:f6
		std::string mstrGroup;
		//! ['18:d1:6c:a0:94:92', '20:ce:c4:02:a4:ca', '18:d6:1c:a0:8d:aa', '20:ce:c4:02:a2:d8', '7c:6c:f0:9f:24:0f']
		std::string mstrPeerGroup;
		//! isManual ? "False" : "True"
		std::string mstrManual;
	}S_Connect_Status_Data;
	//! time,mcs,group,isManual
	//! 2025-05-15 16:32:23.360845,9,18:d6:1c:a1:0d:f6,False
	typedef struct
	{
		//! 2025-05-15 16:32:22.924176
		std::string mstrTime;
		//! 
		int mnMcs;
		//! 18:d6:1c:a1:0d:f6
		std::string mstrGroup;
		//! isManual ? "False" : "True"
		std::string mstrManual;
	}S_MCS_Data;
	//! time,txthroughput,group,isManual
	//! 2025-05-15 16:32:42.861120,0,18:d6:1c:a1:0d:f6,False
	typedef struct  
	{
		//! 2025-05-15 16:32:22.924176
		std::string mstrTime;
		//! 
		int mnTxThroughputMbps;
		//! 18:d6:1c:a1:0d:f6
		std::string mstrGroup;
		//! isManual ? "False" : "True"
		std::string mstrManual;
	}S_TxThroughput_Data;

#endif
	typedef struct
	{
		int perf_stat;
		int beam_forming ;
		int scan_summary ;
		int link_status ;
		int route_change ;
		int sta_comp;
		int conn_change ;
		int total_tp ;
		int probes ;
		int scan_requests ;
		int connect_requests ;
		int scan_complete ;
		int scan_results ;
		int power_transition ;
		int station_statistics;
	} S_NotifStats;

	typedef enum _SPrsCore_PerfStatType
	{
		/// @brief Rx performance of the Peraso dongle
		ePrsCore_PerfStatType_Rx = 0,
		/// @brief Tx performance of the Peraso dongle
		ePrsCore_PerfStatType_Tx = 1,
	} SPrsCore_PerfStatType;



    typedef enum 
    {
        E_SERVICE_TYPE = 0,
        E_CLIENT_TYPE
    }E_SOCKET_TERIMALTYPE;

    typedef enum
    {
        E_TCP_TYPE = 0,
        E_UDP_TYPE,
        E_PING_TYPE
    }E_PROTOCOL_TYPE;

    typedef struct  
    {
        bool mbIsAndCommandOk;
        int mnErrorRetry;
        std::string mstrRead;
        std::string mstrCommand;
        std::string mstrCommandOk;
        std::vector<std::string> mvstrCommandOk;
    }S_SSH_COMMAND;

    typedef struct
    {
        //! 是否是prs_vendor_app消息
        //! true : TCP MSG 默认9010
        //! false : UDP MSG 默认9009
        bool mbIsPrsVendorAppMsg;
        //! roaming_graph 接收到消息后是否转发，如果为true 则不会接收UDP MSG
        bool mbIsSendRoamingGraphMsg;
        //bool mbTestDelayTime;//! 是否测试时延,true : Iperf测试过程中测试时延 ,false : Iperf测试过程中不测试时延
        bool mbIsUseCellphoneIP;
        bool mbIsSaveIPQLog;
        E_SOCKET_TERIMALTYPE meSocketTerimalType;
        E_PROTOCOL_TYPE meProtocolType;
        size_t mnTestTimes;
        size_t mnThreadCount;
        size_t mnTestIndex;
        size_t mnSettingMaxListCount;
        size_t mnCheckSignalTimeIntervalMS;
        size_t mnTimeStamp;
        //! iw wlan0 station dump 检查11AD连接命令
        std::string mstrCheck11ADConnectCommand;
        //! 11AD连接成功的查找字符串
        std::string mstr11ADConnectedFindString;
        std::wstring mstrIperfIPAddr;
        std::string mstrIperfIPAddrA;
        std::wstring mstrSSHIPAddr;
        std::wstring mstrPhoneIPAddr;
        std::wstring mstrExtraParameter;
        std::wstring mstrIperfCommand;
        std::wstring mstrIperfTestLogPath;
        std::string mstrUserNameSSH;
        std::string mstrPasswordSSH;
        std::wstring mstrHtmlFile;
        std::wstring mstrIperfRelativePath;
        std::string mstrSignalConfig;
        std::string mstrMcsConfig;
        //! LOG_PREFIX=20KM_30KM;
        std::wstring mstrLogPrefix;   
        std::wstring mstrUploadPhoneSocketMessage;

        std::wstring mstrLogDebugFilePath;
        //!
        std::wstring mstrLogFolder;

        std::vector<S_SSH_COMMAND> mvInitSSHCommand;
        std::vector<S_SSH_COMMAND> mvExitSSHCommand;
    }S_TestParams;



    //! {"latitude":22.580678878200533,"longitude":113.94094368440628,"speed":0.0}
    typedef struct  
    {
        //! time,connect,throughput rate(Mbits/sec),time delay(ms),Latitude,Longitude,speed(km/h)
        size_t muUnixTestLogTimeStamp;
        std::wstring mstrThroughputRate;
        std::wstring mstrPingTimeDelay;
    } S_LogInfo;

    //! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
    typedef struct  
    {
        int mnRssi;
        int mnSnr;
        int mnDistance;
        int mnTxBeamSector;
        int mnRxBeamSector;
		__int64 mdw64TimeStampMSec;
        std::wstring mstrApMac;
        //std::wstring mstrTime;
    }S_BeamformingService_Info;

    typedef enum
    {
        //! 初始化态
        E_UNKNOW_PRS_ROAMING_CONNECT = -1,
        //! 断开连接
        E_DISCONNECTED_STATUS = 0,
        //! 连接状态，STA1连接
        E_CONNECTED_STATUS = 2,
        //! 连接状态，STA2连接
        E_CONNECTED_STATUS_2 = 4,
        //! 扫描状态
        E_SCAN_COMMAND = 5,
        //! 扫描完成
        E_SCAN_COMPLETE = 6,
        //! 加入连接状态
        E_JOIN_COMMAND
    }E_PRS_ROAMING_CONNECT_STATUS;


    //! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
    typedef struct  
    {
        E_PRS_ROAMING_CONNECT_STATUS mePrsRoamingConnectStatus;
        int mnTxMcs;
        int mnRxMcs;
        __int64 mdw64TimeStampMSec;
        std::wstring mstrAPMac;
        std::wstring mstrSTAMac;
    }S_LinkStatusService_Info;


	//! Time ,STA MAC,AP MAC LIST,RSSI LIST ==== scan complete
	typedef struct  
	{
		__int64 mdw64TimeStampMSec;
		std::wstring mstrSTAMac;
		std::vector<int> mvnRssi;
		std::vector<std::wstring> mvstrApMac;
	}S_ScanCompleteNetworkDiscoveryResults_Info;

	//! Time,STA MAC,AP MAC,SSID ==== Join
	typedef struct  
	{
		__int64 mdw64TimeStampMSec;
		std::wstring mstrSSID;
		std::wstring mstrAPMac;
		std::wstring mstrSTAMac;
	}S_JoinNetworkConnectRequest_Info;

	//! Time,AP MAC,RSSI,SNR,DISTANCE
	typedef struct
	{
		int mnRssi;
		int mnSnr;
		int mnDistance;
		__int64 mdw64TimeStampMSec;
		std::wstring mstrAPMac;
	}S_ScanSummary_Info;

	//! Time,STA MAC,is Tx,ThroughputMbps
	typedef struct  
	{
		bool mbIsTx;
		int mnThroughputMbps;
		__int64 mdw64TimeStampMSec;
		std::wstring mstrSTAMac;
	}S_PerfStats_Info;

    
    struct S_STA_AP_TestInfo;
    typedef void (*OnChangePrsRomingConnectStatusFunc)(LPVOID pContext,S_STA_AP_TestInfo* psStaApTestInfo);

	struct S_STA_AP_TestInfo
	{
        //E_PRS_ROAMING_CONNECT_STATUS mePrsRomingConnectStatus;
		//E_PRS_ROAMING_CONNECT_STATUS mePrsRomingConnectStatus;
        E_PRS_ROAMING_CONNECT_STATUS mePrivarePrsRomingConnectStatus;
		int mnRssi;
		int mnSnr;
		int mnDistance;
		int mnTxMcs;
		int mnRxMcs;
		int mnTxBeamSector;
		int mnRxBeamSector;
		int mnTxThroughputMbps;
		int mnRxThroughputMbps;
        LPVOID mpContext;
        OnChangePrsRomingConnectStatusFunc mpChangePrsRomingConnectStatus;
        //! 接入时间
		__int64 mdw64TimeStampMSec;
		std::wstring mstrApMac;
		std::wstring mstrStaMac;
		std::wstring mstrSsid;
		std::wstring mstrStaMacFilePath;
        std::wstring mstrPrevUpdateWkeHtmlData;
        std::wstring mstrAssociatedTimeSec;

        void SetPrsRomingConnectStatus(E_PRS_ROAMING_CONNECT_STATUS ePrsRomingConnectStatus)
        {
            if (mePrivarePrsRomingConnectStatus != E_UNKNOW_PRS_ROAMING_CONNECT && mePrivarePrsRomingConnectStatus != ePrsRomingConnectStatus)
            {
                mePrivarePrsRomingConnectStatus = ePrsRomingConnectStatus;
                if (NULL != mpChangePrsRomingConnectStatus) mpChangePrsRomingConnectStatus(mpContext,this);
            }
            else 
            {
                mePrivarePrsRomingConnectStatus = ePrsRomingConnectStatus;
            }
        }

        E_PRS_ROAMING_CONNECT_STATUS GetPrsRomingConnectStatus()
        {
            return mePrivarePrsRomingConnectStatus;
        }
	};
}

#if 1
/*
rssi_df = pd.DataFrame(columns=['time', 'rssi', 'group', 'isManual'])
Trip Link Quality
default_graph_layout('Trip Link Quality', 'Time Stamp', 'Link Quality (rssi)', RSSI_RANGE, RSSI_GRAPH_Y_TICKVAL, RSSI_GRAPH_Y_TICKTXT)
handle_beam_forming
handle_scan_summary
handle_station_comp

tp_df = pd.DataFrame(columns=['time', 'throughput', 'group', 'isManual'])
default_graph_layout('Trip Rx Throughput', 'Time Stamp', 'RX Throughput (Mbps)', TP_RANGE, TP_GRAPH_Y_TICKVAL, TP_GRAPH_Y_TICKTXT)
handle_total_throughput
handle_perf_stats
handle_route_change

conn_df = pd.DataFrame(columns=['time', 'status', 'group', 'peer', 'isManual'])
default_graph_layout('Connection Status', 'Time Stamp', 'Connection Status', CONN_RANGE, CONN_GRAPH_Y_TICKVAL, CONN_GRAPH_Y_TICKTXT)
add_conn_status
add_scan_connect
handle_link_status
handle_route_change

handle_scan_request
handle_scan_results
handle_scan_complete_request
handle_connect_request


mcs_df = pd.DataFrame(columns=['time', 'mcs', 'group', 'isManual'])
default_graph_layout('RX MCS', 'Time Stamp', 'RX MCS', MCS_RANGE, MCS_GRAPH_Y_TICKVAL, MCS_GRAPH_Y_TICKTXT)
handle_route_change
handle_link_status

tp_tx_df = pd.DataFrame(columns=['time', 'txthroughput', 'group', 'isManual'])
default_graph_layout('Trip Tx Throughput', 'Time Stamp', ' TX Throughput (Mbps)', TP_RANGE,  TP_GRAPH_Y_TICKVAL,  TP_GRAPH_Y_TICKTXT)
handle_perf_stats
handle_route_change

*/
#endif