#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <vector>
#include "CSTSETestDefine.h"

namespace JGW
{
    class CCSTSETestPluginDlg : public CCJGW_CommanTestPlnView
    {
    public:
        CCSTSETestPluginDlg(void);
        ~CCSTSETestPluginDlg(void);
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
        virtual void CloseSubWndPlugin();
    private:
        //! 响应TEST LIST消息
        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应TSE初始化失败消息
        void OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应TSE LIST 测试值显示
        void OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnAppendRichEditTest(const WPARAM& wParam,const LPARAM& lParam);
        //!
        std::wstring GetSingleTestTime(double dTimer);
    private:
        //! 
        void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
    private:
        CListTestCtrlUI* mpDoMainList;
        CCMessageThread* mpTestThread;
        CRichEditUI* mpThroughtputTestEdt;
        CCJGW_CHiPerfTimer	m_cHiperTime;
        S_CSTSE_TEST_CONFIG msCSTSETestConfig;
    };
}


