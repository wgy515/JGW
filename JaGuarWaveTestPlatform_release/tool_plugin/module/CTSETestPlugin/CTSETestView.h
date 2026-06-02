#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <vector>
#include "CTSETest_Define.h"

namespace JGW
{
    class CCTSETestView : public CCJGW_CommanTestPlnView
    {
    public:
        CCTSETestView(void);
        ~CCTSETestView(void);
    private:
        //! 响应TEST LIST消息
        void OnTestListMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应TSE初始化失败消息
        void OnTSEInitFail(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应TSE LIST 测试值显示
        void OnTSEShowTestContent(const WPARAM& wParam,const LPARAM& lParam);
        //! 响应点击config按钮消息
        void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnTSETestIndexMsg(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnPaintList(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnTSEIndexTestComplete(const WPARAM& wParam,const LPARAM& lParam);

        bool OnMsgBtnClick( TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam );
        //! 
        void OnTSEResetComplete(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 窗体关闭消息
        void CloseSubWndPlugin();
    private://! CCJGW_TestStatusListenerImp
        bool OnIdelTestStatus();
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
    private:
        std::wstring GetSingleTestTime(double dTimer);
        //! 重置List
        void InitStartTest();
        //! 
        void EnableAllButton(bool bEnable = false);
    private:
        bool mbFristTestProject;
        size_t mnTestIndex;
        size_t mnMaxTestIndex;
        int mnErrorTestIndex;
        CCJGW_CHiPerfTimer m_cHiperTime;
        CListTestCtrlUI* mpDoMainList;
        CCMessageThread* mpTestThread;
        S_TSE_TEST_CONFIG_PARAM msTSETestConfigParam;
        std::vector<CButtonUI*> mvTestContentButton;
    };
}