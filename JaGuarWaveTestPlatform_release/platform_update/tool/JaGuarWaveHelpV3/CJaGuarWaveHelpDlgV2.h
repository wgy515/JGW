#pragma once
#include "resource.h"
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGW_UpdateServiceThread.h"
#include "CJGW_SoftwareDownloadThreadV2.h"
#include "CJGW_UploadThread.h"
#include <JaGuarWaveHelpV2/CJaGuarWaveHelpV2Define.h>
namespace JGW
{
    class CCJaGuarWaveHelpDlgV2 : public CCAsyncMsgDialog
    {
    public:
        CCJaGuarWaveHelpDlgV2(void);
        ~CCJaGuarWaveHelpDlgV2(void);
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
        //! jgw_mes_config.data
        void LoadMesConfigData();
    private:
        bool mbShow;
        CRichEditUI* mpLogRickEdt;

        CCJGW_UpdateServiceThread* mpHelpServiceThread;
        CCJGW_SoftwareDownloadThreadV2* mpSoftwareDownloadThread;
        //CCJGW_SoftwareUpdateThread* mpSoftwareUpdateThread;
        CCJGW_UploadThread* mpUploadThread;

        S_JGW_HELP_V2_CONFIG msHelpV2Config;
        std::map<CDuiString,bool> m_MenuCheckInfo;
        CDuiTrayIcon mcTrayIco;
    };
}


