#pragma once

#include <JaGuarWaveHelpV3/CJaGuarWaveHelpV3Define.h>
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_SoftwareDownloadThreadV2.h"
#include "CJGW_FTPUploadingTestLogsThread.h"
#include <map>

namespace JGW
{
    class CCJaGuarWaveHelpDlgV3 : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarWaveHelpDlgV3(S_HELP_V3_CONFIG& sHelpV3Config);
        ~CCJaGuarWaveHelpDlgV3(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJaGuarWaveHelpDlgV3);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //!
        void OnMenuElementClick(TEventUI* pTEventUI);
        //!
        void OnShowTrayMenuWnd(TEventUI* pTEventUI);
        //!
        void OnAppendRichEditTest(WPARAM wParam,LPARAM lParam);
        //!
        void OnTimerMsg(int id);
    private:
        bool mbShow;
        CRichEditUI* mpLogRickEdt;

        CCJGW_UpdateServiceThread* mpHelpServiceThread;
        CCJGW_SoftwareDownloadThreadV2* mpSoftwareDownloadThread;
        CCJGW_FTPUploadingTestLogsThread* mpFTPUploadingTestLogsThread;

        S_HELP_V3_CONFIG& msHelpV3Config;
        std::map<CDuiString,bool> m_MenuCheckInfo;
        CDuiTrayIcon mcTrayIco;
    };
}


