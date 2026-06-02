#pragma once
#include "CTSE_SequenceTest_Define.h"
#include <TSE_SequenceManagerPlugin/TSE_SequenceManageInterface.h>
#include <TSE_SequenceManagerPlugin/TSE_SequencePocoClassLoader.hpp>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>
#include <TSE_SequenceManagerPlugin/TSE_LogListener.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>

namespace JGW
{
    class CTSE_TestSequenceExec : public CTSE_LogListener,public CTSE_TestStatusListener
    {
    public:
        CTSE_TestSequenceExec(void);
        virtual ~CTSE_TestSequenceExec(void);
    public:
        //! 初始化
        bool InitSequenceTest(HWND hMainWnd,const std::string& strConfigXmlPath);
        //! 执行序列测试
        bool ExecSequenceTest();
        //!
        bool IsInitTSEConfigOk();
    protected:
        void ExitTSE();
    protected: //! CTSE_LogListener
        virtual void OnDebugLog(const wchar_t* strDebugLog);

        virtual void OnInfoLog(const wchar_t* strInfoLog);

        virtual void OnErrorLog(const wchar_t* strErrorLog);

        virtual void OnUserDefinedLog(int id,const wchar_t* strCustomLog);
        //!
        virtual void OnTestIndexChange(size_t index,int subIndex);
    protected: //! CTSE_TestStatusListener
        virtual void OnTestStatusChange(const wchar_t* strTestName,E_TEST_STATUS eTestStatus);
    private:
        //! 加载TSE_SequenceManagerPlugin DLL
        virtual bool LoadTestSuiteManageModule(const std::string& strConfigXmlPath);
		//! 获取CTSE_SequenceManageExport 类实例
		virtual bool GetTestSuiteManageClassPtr();
        //! 初始化suite.xml配置文件
        virtual bool InitTestSuiteManageConfig(HWND hMainWnd,const std::string& strConfigXmlPath);
    protected:
		bool mbMultiTSETest;
        bool mbExecAutoTSETest;
        bool mbInitTSEConfig;
		bool mbIsLuaTSEConfig;
		unsigned long mnTokenID;
        CTSE_SequenceManageImpl* mpSequenceManageImpl;
        CTSE_SequenceManageInterface* mptrTestSuiteManageImpl;
        CTSE_SequencePocoClassLoader<CTSE_SequenceManageInterface> mTestSuitePocoClassLoader;
    };
}