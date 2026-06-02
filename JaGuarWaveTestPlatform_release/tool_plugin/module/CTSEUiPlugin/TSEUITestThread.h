#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>

namespace JGW
{
    class CTSEUITestThread : public CCMessageThread,private CTSE_TestSequenceExec
    {
    public:
        CTSEUITestThread(void);
        ~CTSEUITestThread(void);
    private: //! CTSE_TestSequenceExec
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void OnInfoLog(const wchar_t* strInfoLog);
        void OnErrorLog(const wchar_t* strErrorLog);
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        void OnDebugLog(const wchar_t* strDebugLog);
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
        //! TSE执行测试序列消息响应
        void OnExitThread(WPARAM wParam,LPARAM lParam);
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool mbRunThreaded;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        PS_TSE_TEST_CONFIG_PARAM mpsTSETTestConfigParam;
    };

}