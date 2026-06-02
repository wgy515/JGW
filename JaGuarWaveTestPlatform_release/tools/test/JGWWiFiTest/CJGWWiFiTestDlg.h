#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include "CJGWWiFiTestThread.h"
#include "JGWWiFiTest_Define.h"

namespace JGW
{
    class CCJGWWiFiTestDlg:public CCAsyncMsgDialog
    {
    public:
        CCJGWWiFiTestDlg(void);
        ~CCJGWWiFiTestDlg(void);

    private:
        //! 初始化窗口
        void OnInitWindow();
        //! 
        UILIB_GetWindowClassName(CCJGWWiFiTestDlg);
        //! 通知
        void OnNotify(TNotifyUI& msg);
        //! 关闭窗口
        void OnCloseWindow();
    private:
        void StartWiFiTest();

        void OnWiFiTestResult(bool bResult);
        //! 更新测试信息
        void OnUpdateTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 更新测试时间
        void OnUpdateTestTimeMsg(const WPARAM& wParam,const LPARAM& lParam );
        //! 响应事件消息 默认返回TRUE
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
    private:
        void OnSaveTest();
    private:
        CListTestCtrlUI*		m_pListCtrlUi;
        CButtonUI*              m_pStartBtnUI;
        CButtonUI*              m_pTestStatusBtnUI;
        CButtonUI*              m_pTestTimeBtn;
        COptionUI*              m_pClearTestRecOption;
        CTextUI*                m_pTestInfoText;
        DWORD                   m_nListItemHeight;
        DWORD                   m_dwTime;
        S_WLAN_CONFIG       m_sWiFiTestConfig;
        CCJGWWiFiTestThread     m_cJGWWiFiTestThread;
    };
}


