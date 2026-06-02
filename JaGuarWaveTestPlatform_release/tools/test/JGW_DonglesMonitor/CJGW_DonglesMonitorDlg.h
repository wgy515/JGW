#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_NetCardMonitorThread.h"
#include "CJGW_DonglesTRxSectorThread.h"
#include "CJGW_DonglesMonitor_Define.h"
#include "CJGW_IfTableTrafficMonitor.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <fstream>
#include <map>


namespace JGW
{
    class CCJGW_DonglesMonitorDlg:public CCAsyncMsgDialog
    {
    public:
        CCJGW_DonglesMonitorDlg(std::string& strNetCardInfo);
        ~CCJGW_DonglesMonitorDlg(void);
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
        void LoadMonitorConfig();
    private:
        void OnUpdateBpsDataInfo();
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);

        void OnRspTestInfoMsg(const wchar_t* strParam);

        void OnRspTRxSectorMsg(const wchar_t* strParam);

        void OnResDonglesInfoMsg();

        void OnResMcsInfoMsg(int mcs);

        void OnResRssiInfoMsg(int rssi_type,int nRssi);

        void OnMenuElementClick(TEventUI* pTEventUI);

        void OnShowTrayMenuWnd(TEventUI* pTEventUI);

        void OnRspAppendRichedt(const wchar_t* strParam);

        void OnRspRichEditReturnKey();
    private:
        void GetBpsString(const ULONGLONG& uBps,std::wstring& strBps);

        void CreateLogFile();
    private:
        bool mbColseWindows;
        CLabelUI* mpBpsLab;
        CLabelUI* mpPpsCountLab;
        CLabelUI* mpTxSectorLab;
        CLabelUI* mpRxSectorLab;
        CLabelUI* mpSerialNumberLab;
        CLabelUI* mpMacAddressLab;
        CLabelUI* mpFirmwareVersionLab;
        CLabelUI* mpUsbVersionLab;

        CLabelUI* mpRssiLab;
        CLabelUI* mpLocalRssiLab;
        CLabelUI* mpMcsLab;
        CLabelUI* mpBestRxSectorLab;
        CLabelUI* mpBestTxSectorLab;
        CButtonUI* mpEnableTxBeamBtn;
        CButtonUI* mpDisableTxBeamBtn;
        CButtonUI* mpEnableRxBeamBtn;
        CButtonUI* mpDisableRxBeamBtn;
        CButtonUI* mpAdvanceMCSBtn;

        CEditUI* mpTxSectorEdt;
        CButtonUI* mpWriteTxSectorBtn;
        CEditUI* mpRxSectorEdt;
        CButtonUI* mpWriteRxSectorBtn;

        CRichEditUI* mpWReadRichEdt;

        CTabLayoutUI* mpTabMain;

        ULONGLONG muMcsStatistics[13];

        std::string mstrNetCardInfo;
        std::wstring mstrTxSector;
        std::wstring mstrRxSector;

        std::wstring mstrCurrentTxSector;
        std::wstring mstrCurrentRxSector;
        std::wofstream mFileStream;

        S_DONGLES_MONITOR_PARAM msDonglesMonitorParam;
        CCJGW_IfTableTrafficMonitor mIfTableTrafficMonitor;
        jgw_timer mtm;
        CCJGW_DonglesTRxSectorThread mcDonglesTRxSectorThread;
        std::map<CDuiString,bool> m_MenuCheckInfo;
        CDuiTrayIcon mcTrayIco;
    };
}