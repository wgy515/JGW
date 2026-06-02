#pragma once
#include <UiLib/CAsyncMsgDialog.h>
#include <JaGuarWaveTestPlatform/JGWDefine.h>
#include <JGW_TestRecord/CJGW_TestRecordImpl.h>
#include "../../../platform_include/CJGWUiLibViewResolverImpl.h"
#include <map>
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include "TSEMultiSplitTestPluginDefine.h"
#include <JGW_FoundationFunc/CJGW_HiPerfTimer.h>
#include <vector>
#include <sstream>

namespace JGW
{
    class CCTSETestPluginUI;
    enum E_TEST_STATUS_TYPE
    {
        E_TEST_IDEL,
        E_TEST_BUSY,
        E_TEST_PASS,
        E_TEST_FAIL
    };

    class CCTSETestPluginWnd : public CCAsyncMsgDialog,public CCJGWUiLibViewResolverImpl
    {
    public:
        CCTSETestPluginWnd(void);
        ~CCTSETestPluginWnd(void);
    public:
        void Init(CCTSETestPluginUI* pTSETestPluginUI);
        void SetModuleFolderSuitName(const std::wstring& strModuleFolder,const std::wstring& strSulteName);
    protected:
        //! 创建窗口事件
        void OnInitWindow();
        //! 获取窗口类名
        UILIB_GetWindowClassName(CCTSETestPluginWnd);
        //! 窗口控件事件通知
        void OnNotify(TNotifyUI& msg);
        //! 响应窗口销毁信息
        void OnCloseWindow();
        //! 初始化注册消息(当返回false，卸载当前加载的子窗体)
        bool InitSubWndPlugin( LPS_VIEW_RESOLVER_IMPL_PARAM psSubWndImplParam );
        //! 响应通知事件消息
        void OnNotifySubWndPlugin( LPVOID pMsg );
    private:
        void OnFinalMessage(HWND hWnd);
        void OnTestStartClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        bool OnRspTestStatusChange(E_TEST_STATUS_TYPE eTestStatusType);
        //! return false 不继续下发， true 继续下发消息 默认返回true
        bool OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam);
        //! 
        void ExecuteFIFOEventMessage();
        //! 
        void OnRspTimeMsg(const WPARAM& wParam,const LPARAM& lParam); 
        //!
        void OnTestPassMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void OnTestFailMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void OnConfigClickMsg(const WPARAM& wParam,const LPARAM& lParam);
        //! 
        void OnShowLogLayoutClickMsg(const WPARAM& wParam,const LPARAM& lParam);
		
		LRESULT OnResponceInputCommonWindowMsg( WPARAM& wParam,LPARAM& lParam );

		LRESULT HandleInterceptCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    private:
        void OnUpdateTestTime();
        void OnClearRecordData();
        void OnTestIDELStatus();
        void OnTestBusyStatus();
        void OnTestFailStatus();
        void OnTestPassStatus();
        void UpdateTestRecordView();

    /*******************************************************************************************/
    private://! CCJGW_TestStatusListenerImp
        bool OnBusyTestStatus();
        bool OnPassTestStatus();
        bool OnFailTestStatus();
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
        void OnTSETimer(const WPARAM& wParam,const LPARAM& lParam);
        //!
        void OnTimerUpdateLogInfo();
        //! 
        void OnTSEAdapterLog(const WPARAM& wParam,const LPARAM& lParam);
    private:
        //! 保存测试LOG
        void OnSaveRichEditTestLog(const WPARAM& wParam,const LPARAM& lParam);


    private:
        bool m_bInit;
        CCTSETestPluginUI* mpTSETestPluginUI;
        CHorizontalLayoutUI* mpLogHorizontalLayout;
    private:
        CButtonUI*      mpStartBtn;
        CButtonUI*      mpConfigBtn;
        COptionUI*      mpClearTestRecOption;
        COptionUI*      mpLogOption;
        CButtonUI*      mpTestStatusBtn;
        CCheckBoxUI*      mpSaveTestLog;
        size_t          mdwTestTime;
        CButtonUI*      mpTestTimeBtn;
		CRichEditUI* mpLogTabedt;

        CTextUI*        mpPassTextUi;
        CTextUI*        mpFailTextUi;
        CTextUI*        mpCountTextUi;
        CTextUI*        mpPassRateTextUi;
        CHorizontalLayoutUI* mpTestInfoVerticalLayout;
        CListTestCtrlUI* mpDoMainList;
        CRichEditUI* mpThroughtputTestEdt;

        size_t          mnTestPassCount;
        size_t          mnTestCounts;
        std::wstring mstrModuleFolder;
        std::wstring mstrSuiteName;
        CCJGW_TestRecordImpl* mpTestRecordImpl;
        _map_reg_msg mMapRegMsg;
    private:
        bool                mbIsSaveTestLog;
        CCMessageThread* mpTestThread;
        CCJGW_CHiPerfTimer	m_cHiperTime;
        std::wstring mstrPrevTestProjectName;
        std::wstring        mstrSaveDownloadLogFolder;
        std::wostringstream mstrTestLogStream;
        //! 
        //! wchar_t szReadBuf[READ_BUFFER_MAX_LEN];
        //! 保存ThroughputTest Log
        //std::wstring mstrSaveOutPutLog;
        //!
        S_MultiSplitTestConfig msMultiSplitTestConfig;
    };


}
