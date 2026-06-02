#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_NetCardMonitorThread.h"
#include "CJGW_DonglesTRxSectorThread.h"
#include "CJGW_DonglesMonitor_Define.h"
#include <fstream>
#include <map>

namespace JGW
{
    class CCJGW_DonglesStatusDlg:public CCAsyncMsgDialog
    {
    public:
        CCJGW_DonglesStatusDlg(std::wstring& strNetCardInfo);
        ~CCJGW_DonglesStatusDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGW_DonglesStatusDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);

        void OnRspTestInfoMsg(const wchar_t* strParam);

        void OnResDonglesInfoMsg();

        void OnMenuElementClick(TEventUI* pTEventUI);

        void OnShowTrayMenuWnd(TEventUI* pTEventUI);
    private:
        void GetBpsString(const ULONGLONG& uBps,std::wstring& strBps);

        void CreateLogFile();
    private:
        bool mbColseWindows;
        CLabelUI* mpBpsLab;
        CLabelUI* mpPpsCountLab;
        CLabelUI* mpSerialNumberLab;
        CLabelUI* mpMacAddressLab;
        CLabelUI* mpFirmwareVersionLab;
        CLabelUI* mpUsbVersionLab;

        S_DONGLES_MONITOR_PARAM msDonglesMonitorParam;
        CCJGW_NetCardMonitorThread mcNetCardMonitorThread;
        CCJGW_DonglesTRxSectorThread mcDonglesTRxSectorThread;
        std::wstring mstrNetCardInfo;
        //! std::wofstream mFileStream;
        std::map<CDuiString,bool> m_MenuCheckInfo;
        CDuiTrayIcon mcTrayIco;
    };
}