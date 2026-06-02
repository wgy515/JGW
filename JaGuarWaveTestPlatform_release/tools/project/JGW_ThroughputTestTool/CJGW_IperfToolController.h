#pragma once
#include <UiLib/CAsyncMsgDialog.h>

namespace JGW
{
    class CCJGW_IperfToolDlg;
    class CCJGW_IperfToolController
    {
    public:
        CCJGW_IperfToolController(CCJGW_IperfToolDlg* pView);
        ~CCJGW_IperfToolController(void);
    public:
        //! 响应WINDOWS 消息
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 
        void OnViewNotify(TNotifyUI& msg);
    private:
        //! 
        void OnAppendRichEditTestMsg(const wchar_t* strMsg);
        //! 响应Setting菜单栏点击事件
        void OnSettingMenuBtnClick(TNotifyUI& msg);
        //! 响应Config菜单栏点击事件
        void OnConfigMenuBtnClick(TNotifyUI& msg);
        //! 
        void OnEnableTruntableCheck(TNotifyUI& msg);
        //! 
        void OnEnableAttenuatorCheck(TNotifyUI& msg);
        //! 响应菜单栏菜单点击事件
        void OnMenuElementClick(TEventUI* pTEventUI);
        //!
        void OnTurntableSettingMenuElementClick();
        //! 更新BPS数据信息
        void OnUpdateBpsData(WPARAM& wParam,LPARAM lParam);
        //! 清空BPS数据信息
        void OnClearBpsData();
        
    private:
        void OnTimerUpdateLogInfo();
    private:
        bool OpenTurnTableComPort();
    private:
        CCJGW_IperfToolDlg* mpView;
        //! wchar_t szReadBuf[1024];
    };
}