#pragma once
#include <JGW_PluginViewLib/CJGW_CommanTestPlnView.h>
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>

namespace JGW
{
    class CCTSE_UiPluginView : public CCJGW_CommanTestPlnView
    {
    public:
        CCTSE_UiPluginView(void);
        ~CCTSE_UiPluginView(void);
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
        //std::wstring GetSingleTestTime(double dTimer);
    private:
        //! 
        void OnTSETimer(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnTimerUpdateLogInfo();
    private:
        bool LoadTSEUISkinXml(const std::wstring& strModuleFolderName,const std::string& strUISkinXmlFileName);
        void UpdateTSEUI();
        void ClearTSEUI();
    private:
        //CListTestCtrlUI* mpDoMainList;
        CCMessageThread* mpTestThread;
        CRichEditUI* mpThroughtputTestEdt;
        CHorizontalLayoutUI* mpUITestContent;
        CControlUI*	mpSubContainer;
        CCJGW_CHiPerfTimer	m_cHiperTime;
        S_TSE_TEST_CONFIG_PARAM msTestConfigParam;
        S_UI_CONFIG_INFO msUIConfigInfo;
    };
}