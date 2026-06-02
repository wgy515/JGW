#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <CSTSE_FoundationLib/CSTSE_ServerBase.h>
#include "CSTSETestDefine.h"
namespace JGW
{
    class CCSTSETestThread : public CCMessageThread,public CTSE_LogListener,public CTSE_TestStatusListener
    {
    public:
        CCSTSETestThread(void);
        ~CCSTSETestThread(void);
    private: //! CTSE_LogListener
        //! 调试信息 -- 主要用来调试使用 不会在界面上显示
        void OnDebugLog(const wchar_t* strDebugLog);
        //! 测试信息 -- 显示测试相关信息  在界面上显示
        void OnInfoLog(const wchar_t* strInfoLog);
        //! 错误信息 -- 显示错误的相关内容
        void OnErrorLog(const wchar_t* strErrorLog);
        //! 自定义LOG信息
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
    protected: //! CTSE_TestStatusListener
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
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
    private:
        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:   
        bool mbExecAutoTSETest;
        bool mbInitTSEConfig;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        PS_CSTSE_TEST_CONFIG mpsCSTSETestConfig;
        CCSTSE_ServerBase mServerBase;
    };
}


