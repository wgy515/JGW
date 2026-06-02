#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "PTP4100WiFi6TestThread.h"

namespace JGW
{
    class CPTP4100WiFi6TestMainDlg : public CCAsyncMsgDialog
    {
    public:
        CPTP4100WiFi6TestMainDlg(void);
        ~CPTP4100WiFi6TestMainDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CPTP4100WiFi6TestMainDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        // DUI_MSGTYPE_CLICK
        void OnClickStartBtn();
        //
        void OnAppendRichEditTestLog(const WPARAM& wParam, const LPARAM& lParam);
    private:
        bool ValidationIPAddress(const std::wstring& strAddress);
        void LoadConfigIni();
        void SaveConfigIni();
    private:
        // TX
        CEditUI* mpTxIPQIPAddressEdt;
        CEditUI* mpTxUsbIPAddressEdt;
        //CComboUI* mpTxSerialPortCombo;
        // RX
        CEditUI* mpRxIPQIPAddressEdt;
        CEditUI* mpRxUsbIPAddressEdt;
        //CComboUI* mpRxSerialPortCombo;
        CEditUI* mpTxPowerStepEdt;
        CEditUI* mpTxPowerStartEdt;
        CEditUI* mpTxPowerEndEdt;
        CEditUI* mpTxDVG1StepEdt;
        CEditUI* mpTxDVG1StartEdt;
        CEditUI* mpTxDVG1EndEdt;
        CEditUI* mpTxDVG2StepEdt;
        CEditUI* mpTxDVG2StartEdt;
        CEditUI* mpTxDVG2EndEdt;
        CEditUI* mpRxAttenStepEdt;
        CEditUI* mpRxAttenStartEdt;
        CEditUI* mpRxAttenEndEdt;
        CEditUI* mpPacketCollectionTimeEdt;

        CButtonUI* mpStartBtn;
        CRichEditUI* mpTestLogRichEdt;
        S_TEST_PARAM msTestParam; 
        CPTP4100WiFi6TestThread* mpWiFi6TestThread;

    };
}


