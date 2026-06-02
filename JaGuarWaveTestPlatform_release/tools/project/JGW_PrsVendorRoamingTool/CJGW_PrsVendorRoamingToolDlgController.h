#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "PrsVendorRoamingDataDefine.h"
#include <fstream>
namespace JGW
{
    class CCJGW_PrsVendorRoamingToolDlg;
    class CCJGW_PrsRoamingTestThread;

	typedef struct  
	{
		int mnPrsRoamingInfoIndex;
		CEditUI* mpConnectApMacEdit;
		CRichEditUI* mpStaPrsRoamingInfoRichEdit;
		CEditUI* mpStaMacEdit;
		__int64 mnLastUpdateTimeStampMSec;//! 精确到毫米的时间戳
        __int64 mnConnectedApMacTimeStampMSec; //! 连接到AP的时间戳
		std::wstring mstrConnectedApMac;
		std::wstring mstrStaMac;
		S_STA_AP_TestInfo msStaApTestInfo;
		std::vector<std::wstring> mvstrPrsRoamingInfo;
	}S_STA_UI_INFO;

	typedef struct  
	{
		int mnPrsRoamingInfoIndex;
		__int64 mnLastUpdateTimeStampMSec;//! 精确到毫米的时间戳
		std::wstring mstrConnectedStaMac;
		std::vector<std::wstring> mvstrPrsRoamingInfo;
	}S_AP_UI_INFO;

    class CCJGW_PrsVendorRoamingToolDlgController
    {
    public:
        CCJGW_PrsVendorRoamingToolDlgController(CCJGW_PrsVendorRoamingToolDlg* pView);
        ~CCJGW_PrsVendorRoamingToolDlgController(void);
    public:
        //! 响应WINDOWS 消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 响应DUILIB事件通知
        void OnViewNotify(TNotifyUI& msg);
        //! 正在关闭当前窗口
        void OnCloseWindow();
        //! 开始初始化请求
        bool OnInit();
        //!
        const S_TestParams& GetTestParams() {return msTestParams;}
    private:
        //! 点击更新配置按钮
        void OnClickUpdateConfigOptionOption();
        //! 点击开始按钮
        void OnClickStartBtn();
        //! 点击停止测试按钮
        void OnClickStopBtn();
    private:
        //! 检查IP地址有效性
        bool CheckIPAddressValid(const std::wstring& strIPAddress);
    private:
        //! UI RichEdit插件添加测试消息
        void OnAppendRichEditTestMsg(const wchar_t* strMsg);
        //! UI 定时器更新消息，每隔一秒更新一次
        void OnUpdatePrsRoamingInfoTimer();
        //! 测试完成消息
        void OnTestCompletedMsg();
        //! 更新网卡信息
        void UpdateNetCardInfoComBo();
        //! 保存配置
        void SaveConfig();
        //! 更新测试结果
        //void WriteIperfPingPrsInfo();
        //!
        void OnHandlePrsVendorRoamingMsg(int nServiceID,const wchar_t* strMsg);
    private:
        //! 响应更新吞吐率信息
        void OnThroughputRateMsg(const wchar_t* strMsg);
        //! 响应时延消息
        void OnTimeDelayMsg(const wchar_t* strMsg);
        //! 写入测试日志
        //void WriteTestLog(const std::wstring& strTestLog);
        //! 写入测试日志
        void WriteRichEditTestLog(const std::wstring& strTestLog);
		//! 写入PRS ROAMING Log
		void WritePrsRoamingLog(const std::wstring& strPrsRoamingLogPath,const std::wstring& strPrsRoamingLog);
		//! 写入PRS ROAMING Log
		void WritePrsRoamingLog(S_STA_AP_TestInfo& sStaApTestInfo);
        //! 响应当前PRS连接状态变化，每次连接状态变化的时候更新WKE HTML页面，以及更新IPERF PING等测试信息
        static void OnChangePrsRomingConnectStatus(LPVOID pContext,S_STA_AP_TestInfo* psStaApTestInfo);
        //!
        void ChangePrsRomingConnectStatus(S_STA_AP_TestInfo* psStaApTestInfo);
    private:
        //! 时间,AP MAC,RSSI,SNR,DISTANCE,TX BEAM SECTOR,RX BEAM SECTOR
        void OnBeamfromingService(const wchar_t* strMsg);
        //! Time,STA Mac,ConnectStatus,AP MAC,TX MCS,RX MCS
        void OnLinkStatusService(const wchar_t* strMsg);
        //! Time,STA MAC ===== SCAN
        void OnScanNetworkDiscoveryRequestService(const wchar_t* strMsg);
        //! Time ,STA MAC,AP MAC LIST,RSSI LIST ==== scan complete
        void OnScanCompleteNetworkDiscoveryResultsService(const wchar_t* strMsg);
        //! Time,STA MAC ==== scan complete
        void OnScanCompleteNetworkDiscoveryCompleteService(const wchar_t* strMsg);
        //! Time,STA MAC,AP MAC,SSID ==== Join
        void OnJoinNetworkConnectRequestService(const wchar_t* strMsg);
        //! Time,AP MAC,RSSI,SNR,DISTANCE
        void onScanSummaryService(const wchar_t* strMsg);
        //! Time,STA MAC,is Tx,ThroughputMbps
        void OnPerfStatsService(const wchar_t* strMsg);
    private:
		void AppendRichEditTestMsg(CRichEditUI* pRichEditUI,const wchar_t* strMsg);

		void UpdateWkeHtmlYSeriesData(S_STA_AP_TestInfo& sStaApTestInfo);
#if 0
        //! append_sta_data
        int AppendSTAData(const std::wstring& strStaMac);
        int AppendApMacData(const std::wstring& strApMac);
        void AppendSTAApConnectData(const std::wstring& strStaMac,const std::wstring& strApMac);
        int GetSTAPosToApConnectData(const std::wstring& strApMac);
        void RemoveSTAApConnectData(const std::wstring& strStaMac);
		void RemoveSTAApConnectDataToAPConnect(const std::wstring& strApMac);
#endif
		S_AP_UI_INFO* AppendApMacInfo(__int64 nTimeStampMSec,const std::wstring& strApMac,const wchar_t* strFormatInfo = NULL);
		S_STA_UI_INFO* AppendStaMacInfo(__int64 nTimeStampMSec,const std::wstring& strStaMac,const wchar_t* strFormatInfo = NULL);
		void AppendConnectStaApMac(__int64 nTimeStampMSec,const std::wstring& strStaMac,const std::wstring& strApMac,const wchar_t* strFormatInfo = NULL);
		void RemoveConnectStaApMac(__int64 nTimeStampMSec,const std::wstring& strStaMac,const std::wstring& strApMac,const wchar_t* strFormatInfo = NULL);
		//S_AP_UI_INFO* GetApUiInfoToApMac(const std::wstring& strApMac);
		//S_STA_UI_INFO* GetStaUiInfoToStaMac(const std::wstring& strStaMac);
	private:
		//! 开启UI更新定时器
		void StartUpdatePrsRoamingInfoTimer();
		//! 停止UI更新定时器
		void StopUpdatePrsRoamingInfoTimer();
		//! WM_TIMER_UPDATE_TEST_TIME <=> UPDATE_IPERF_PING_PRS_INFO_UI_FREQ
		void StartUpdateIperfPingPrsInfoTimer();
		//! WM_TIMER_UPDATE_TEST_TIME <=> UPDATE_IPERF_PING_PRS_INFO_UI_FREQ
		void StopUpdateIperfPingPrsInfoTimer();
		//! WM_TIMER_UPDATE_TEST_TIME <=> UPDATE_IPERF_PING_PRS_INFO_UI_FREQ
		//void OnUpdateIperfPingPrsInfoTimer();
        //! 每次间隔1.5S更新一次HTML页面显示
        //! 每次PRS连接状态更新一次
        void OnUpdateWkeHtmlAllYSeriesDataTimer();
    private:
        //! 每次接收到时延消息Iperf、Ping、Prs Info
        //! 每次接收到吞吐率更新请求的时候更新一次,然后一秒钟之内不在更新 bIsPrsIperfUpdate 是否是吞吐率更新导致
        //! 每次PRS连接状态更新，就进行更新一次  bIsPrsConnectStatusUpdate 是否是PRS连接状态更新
        void WriteIperfPingPrsInfoTestLogToOfStream(bool bIsPrsIperfUpdate = false,bool bIsPrsConnectStatusUpdate = false);
    private:
		//! 测试日志文件路径，包含iperf ping以及prs info等信息
        //! time,throughput rate(Mbits/sec),time delay(ms),sta1,sta2
		std::wstring mstrIperfPingPrsInfoTestLogFilePath;
        std::wofstream mIperfPingPrsInfoTestLogStream;
        //! WKE HTML LOG
        std::wstring mstrWkeHtmlTestLogFilePath;
        std::wofstream mWkeHtmlTestLogFilePathStream;
		//! 是否开启更新UI视图定时器
        bool mbIsStartUpdatePrsRoamingInfoTimer;
		//! 是否更新Iperf Ping Prs info定时器
		bool mbIsStartUpdateIperfPingPrsInfoTimer;
		//! PrsVendorRoamingTool.ini
		std::wstring mstrConfigFilePath;
		//! 编辑控件log文件路径
        std::wstring mstrRichEditTestMsgLogFilePath;
        std::wofstream mRichEditTestMsgLogStream;
        S_LogInfo msLogInfo;
        S_TestParams msTestParams;
        CCJGW_PrsVendorRoamingToolDlg* mpView;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        CCJGW_PrsRoamingTestThread* mpPrsRoamingTestThread;
	private:
		std::map<std::wstring,S_STA_UI_INFO> mmapStaMacPrsRoamingInfo;
		std::map<std::wstring,S_AP_UI_INFO> mmapApMacPrsRoamingInfo;
    };
}
