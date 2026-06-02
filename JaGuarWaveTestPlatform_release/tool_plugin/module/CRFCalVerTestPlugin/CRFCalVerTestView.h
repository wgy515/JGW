#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include "CRFCalVerTestThread.h"
#include "CRFCalVerTest_Define.h"
namespace JGW
{
    class CCRFCalVerTestView:public CCJGW_CommanTestPlnView
    {
    public:
        CCRFCalVerTestView(void);
        ~CCRFCalVerTestView(void);
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
    private://!domainlist
        size_t m_nListItemHeight;
        CListTestCtrlUI* mpDoMainList;
        CCRFCalVerTestThread* mpTestThread;
        CCJGW_CHiPerfTimer	m_cHiperTime;
        S_RFCALVER_TEST_PARAM msRFCalVerTestParam;   
    };

}

