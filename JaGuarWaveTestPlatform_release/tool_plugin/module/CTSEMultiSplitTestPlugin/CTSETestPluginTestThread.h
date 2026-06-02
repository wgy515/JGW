#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <TSE_SequenceTestLib/TSE_TestSequenceExec.h>
#include "TSEMultiSplitTestPluginDefine.h"
#include <sstream>
namespace JGW
{
    class CCTSETestPluginTestThread : public CCMessageThread,private CTSE_TestSequenceExec
    {
    public:
        CCTSETestPluginTestThread(HWND hMainWnd);
        ~CCTSETestPluginTestThread(void);
    private: 
		//! CTSE_TestSequenceExec -- 响应状态变化
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void OnInfoLog(const wchar_t* strInfoLog);
        void OnErrorLog(const wchar_t* strErrorLog);
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        void OnDebugLog(const wchar_t* strDebugLog);
    public:
		std::wstring GetErrorMessage();
		std::wstring GetDebufMessage();
#if 1
        void UpdateCopyDataMessage(const std::string& strMessage);
#else
        void UpdateQIAWindowHandle(int hanlde);
        void UpdateCheckQIAIsRunningFlag(int flag);
        void UpdateQiaTestRessult(bool result);
        void AddHtmlTestLogging(const std::wstring& strHtmlFilePath);
#endif
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
		HWND mhMainWnd;
        std::wstring mstrErrorMsg;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
		std::wostringstream mstrErrorTestLogStream;
		std::wostringstream mstrDebugTestLogStream;
        PS_MultiSplitTestConfig mpsMultiSplitTestConfig;
    };
}


