#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include "CJGW_StationMonitorDefine.h"
#include "CJGW_ReportStationStatusThread.h"
#include "CJGW_StationServiceThread.h"
namespace JGW
{
    class CCJGW_StationMonitorMainDlg : public CCAsyncMsgDialog
    {
    public:
        CCJGW_StationMonitorMainDlg(void);
        ~CCJGW_StationMonitorMainDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_DonglesMonitorDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 
        bool OnInitUIControlClass();
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 更新UI显示
        void OnUpdateUIDisplay();
        //! 
        void GetBpsString(const ULONGLONG& uBps,std::string& strBps);
        //!
        void OnTimerToID(int nIDEvent);
    private:
        CLabelUI* mpUploadSpeedLab; //! 上传速度
        CLabelUI* mpDownloadSpeepLab; //！下载速度
        CLabelUI* mpTxSectorLab;
        CLabelUI* mpRxSectorLab;
        CLabelUI* mpTxMcsLab;
        CLabelUI* mpRxMcsLab;
        CLabelUI* mpIPAddressLab;
        CLabelUI* mpMcsAddressLab;
        CLabelUI* mpRssiLab;
        CLabelUI* mpTxDropRateLab;
        CLabelUI* mpRxDropRateLab;
        CCJGW_ReportStationStatusThread* mpReportStationStatusThread;
        CCJGW_StationServiceThread* mpStationServiceThread;

        std::string mstrStationServerUrl;
        jgw_timer mtm;
        S_STATION_MONITOR_UI_INFO msStationMonitorUiInfo;
        CCJGW_AdapterCommonInfo mAdapterCommonInfo;   
    };
}