#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <UiLib/CAsyncMsgDialog.h>
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include "CRFCalVerTest_Define.h"
/*#include "CTestLogRecord.h"*/
namespace JGW
{
    class CCRFCalVerTestThread:public CCMessageThread,public CTSE_LogListener,public CTSE_TestStatusListener
    {
    public:
        CCRFCalVerTestThread(void);
        virtual ~CCRFCalVerTestThread(void);
    private:
        void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
        void OnInfoLog(const wchar_t* strInfoLog);
        void OnErrorLog(const wchar_t* strErrorLog);
        void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        void OnDebugLog(const wchar_t* strDebugLog);
    public:
        void SendListMsgToWnd(const wchar_t* strMsg,DWORD dwRGB);
    private:
        void OnInitWnd( WPARAM wParam,LPARAM lParam );
        bool GetTestSuiteManageClassPtr();
        bool LoadTestSuiteManageModule();
        void OnRunThread( WPARAM wParam,LPARAM lParam );
		void OnAutoTSETest(WPARAM wParam,LPARAM lParam);
		void OnExecuteConfigViewTest(WPARAM wParam,LPARAM lParam);

        DECLARE_MYTHREAD_MESSAGE_MAP()
    private:
        bool InitTestSuiteManageConfig();

        bool OnStartSuiteTest(); 
    private:
        bool mbInitPreTSETest;
		bool mbInitTSEConfig;
        CCJGW_AsyncWndMessage*  mpAsyncWndMessage;
        PS_RFCALVER_TEST_PARAM mpsRFCalVerTestParam;
        CTSE_SequenceManageImpl* mpSequenceManageImpl;
        CTSE_SequenceManageInterface* mptrTestSuiteManageImpl;
        /*CCTestLogRecord mTestLogRecord;*/
        CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
    };
}


