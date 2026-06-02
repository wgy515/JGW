#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>

namespace JGW
{
    class CCJGW_AutomationPluginThread : public CCMessageThread,private CTSE_TestSequenceExec
    {
    public:
        CCJGW_AutomationPluginThread(void);
        ~CCJGW_AutomationPluginThread(void);
    private: //! CTSE_TestSequenceExec
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void OnInfoLog(const wchar_t* strInfoLog);
        void OnErrorLog(const wchar_t* strErrorLog);
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        void OnDebugLog(const wchar_t* strDebugLog);
        void OnTestIndexChange(size_t index,int subIndex);
    private:
        void SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB);
    private:
        //! 初始化TSE消息响应
        void OnInitWnd( WPARAM wParam,LPARAM lParam );
        //! TSE配置消息响应
        void OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam);
        //! TSE执行测试序列消息响应
        void OnRunThread(WPARAM wParam,LPARAM lParam);
        //! TSE自动测试消息响应
        void OnAutoTSETest(WPARAM wParam,LPARAM lParam);
        //! 执行TSE单个测试项目
        void OnRunTSEIndexThread(WPARAM wParam,LPARAM lParam);
        //! 
        void OnTSEResetMsg(WPARAM wParam,LPARAM lParam);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        PS_TSE_TEST_CONFIG_PARAM mpsTSETTestConfigParam;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
    };
}