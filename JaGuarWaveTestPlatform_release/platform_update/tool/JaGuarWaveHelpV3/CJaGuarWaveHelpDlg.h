#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_SoftwareDownloadThread.h"
#include "CJGW_UploadThread.h"
#include "CJGW_SoftwareUpdateThread.h"
namespace JGW
{
    class CCJaGuarWaveHelpDlg : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarWaveHelpDlg(void);
        ~CCJaGuarWaveHelpDlg(void);
    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJaGuarWaveHelpDlg);
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
        //!
        void LoadHelpConfig();
    private:
        bool mbShow;
        CRichEditUI* mpLogRickEdt;
        CCJGW_UpdateServiceThread* mpHelpServiceThread;
        CCJGW_SoftwareDownloadThread* mpSoftwareDownloadThread;
        //! CCJGW_SoftwareUpdateThread* mpSoftwareUpdateThread;
        CCJGW_UploadThread* mpUploadThread;
        S_JGW_HELP_CONFIG msHelpConfig;
        
        std::map<CDuiString,bool> m_MenuCheckInfo;
        CDuiTrayIcon mcTrayIco;
    };
}