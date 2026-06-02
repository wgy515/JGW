#pragma once
#include <JGW_TestSuiteManagePlugin/CJGW_TestSuiteManageImpl.h>
#include "CJGW_TestSuiteInterface.h"
namespace JGW
{
    class TSE_SequenceManager:public CTSE_SequenceManageImpl
    {
    public:
        TSE_SequenceManager(bool bMultiSuite = false,const int nIndex = 0);
        ~TSE_SequenceManager(void);
    private:
        //! 
        virtual bool InitTestSuiteManageInterface(HWND hMain,LPCTSTR strConfigFilePath);
        //! strConfigFileBuf <?xml version="1.0" encoding="UTF-8" standalone="yes" ?><Test></Test>
        virtual bool InitTestSuiteManageInterfaceToXmlBuf(HWND hMain,std::string& strConfigFileBuf);
        //! 执行所有测试项目根据接口序列类指针
        virtual bool ExecuteAllSuiteTest(bool bErrorStop = true);
        //! 执行单个测试 根据测试ID 以及 接口序列类指针
        virtual bool ExecuteSingleSuiteTestToTestID(int nIndex);
        //! 执行当前序列销毁动作
        virtual bool ExecuteSuiteTestFinalize(void);
        //! 注册系统log事件句柄函数
        virtual void Register_System_Event_Handler_Fn(jgw_suite_test_log_func evtcb);
        //! 设置环境变量( 所有suite dll共享一份变量 )
        virtual void SetSuiteEnvironmentVar(const wchar_t* envirname,const wchar_t* envirvalue);
        //! 获取序列环境变量
        virtual std::wstring GetSuiteEnvironmentVar(const wchar_t* envirname);
        //! 获取suite测试数量
        virtual size_t GetSuiteTestCounts(void);
        //! 更新环境变量参数配置
        virtual void UpdateEnviromentParam(void);
    private:
        CCJGW_TestSuiteInterface mTestSuiteInterface;
    };
}


